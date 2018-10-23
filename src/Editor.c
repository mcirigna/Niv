#include "Editor.h"
#include "Document.h"
#include "View.h"
#include "Textline.h"
#include "Constants.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

/***** private *****/

int copyFileFromTo(char *src, char *dest) {
  FILE *srcfile, *destfile;

  if (src == NULL || dest == NULL) return -1;

  srcfile = fopen(src, "r");
  if (srcfile == NULL) return -1;

  destfile = fopen(dest, "w");
  if (destfile == NULL) return -1;

  char c = fgetc(srcfile);
  while (c != EOF) {
    fputc(c, destfile);
    c = fgetc(srcfile);
  }
  fclose(srcfile);
  fclose(destfile);
  return 0;
}

void documentToTextline(Document *D, Textline *buffer) {
  if (buffer == NULL) return;

  int i;
  for (i = 0; i < D->lineCount; i++) {
    appendToTextline(buffer, D->lines[i].text, D->lines[i].length);
    appendToTextline(buffer, "\r\n", 2);
  }
}

int saveFile(Editor* E) {

  char *original = E->filename;
  if (original == NULL) return -1;
  char backup[100];
  strcpy(backup, "#");
  strcat(backup, original);
  
  int err = copyFileFromTo(original, backup);
  if (err) return -1;
  
  Textline buffer;
  initTextline(&buffer);
  documentToTextline(&E->doc, &buffer);
  
  int ogfile = open(original, O_TRUNC | O_CREAT | O_WRONLY, 0644);
  if (ogfile < 0) return -1;

  int count = write(ogfile, buffer.text, buffer.length);
  if (count < 0) {
    copyFileFromTo(backup, original);
    freeTextline(&buffer);
    return -1;
  }
  freeTextline(&buffer);
  return 0;
}

/* private view */
void updateViewOffsets(Editor *E) {

  /* update view offsets */
  if (E->doc.x >= E->view.colOffset + E->view.colCount) {
    E->view.colOffset = E->doc.x - E->view.colCount + 1;
  }
  if (E->doc.x < E->view.colOffset) {
    E->view.colOffset = E->doc.x;
  }
  if (E->doc.y >= E->view.rowOffset + E->view.rowCount) {
    E->view.rowOffset = E->doc.y - E->view.rowCount + 1;
  }
  if (E->doc.y < E->view.rowOffset) {
    E->view.rowOffset = E->doc.y;
  }
  
  /* update view xy */
  E->view.x = E->doc.x - E->view.colOffset + 1;
  E->view.y = E->doc.y - E->view.rowOffset + 1;
}

void updateViewDisplay(Editor *E) {
  Textline buffer;
  initTextline(&buffer);
  int i;
  for (i = 0; i < E->view.rowCount; i++) {
    int lineOffset = i + E->view.rowOffset;
    if (i < E->doc.lineCount) {
      char *text = &E->doc.lines[lineOffset].text[E->view.colOffset];
      int length = E->doc.lines[lineOffset].length - E->view.colOffset;
      if (length < 0) length = 0;
      if (length > E->view.colCount) length = E->view.colCount;
      appendToTextline(&buffer, text, length);
    } else {
      appendToTextline(&buffer, "~", 1);
    }
    appendToTextline(&buffer, ERASE_RIGHT_OF_LINE,
                     strlen(ERASE_RIGHT_OF_LINE));
    if (i < E->view.rowCount - 1) appendToTextline(&buffer, "\n", 1);
  }
  newTextline(&E->view.display, buffer.text, buffer.length);
  freeTextline(&buffer);
}  

/* private commands */

void insert(Editor *E, int key) {
  int lineIdx = E->doc.y;
  int textIdx = E->doc.x;
  char c = 0x00 | key;
  insertToDocumentLine(&E->doc, lineIdx, textIdx, &c, 1);
  setDocumentX(&E->doc, E->doc.x + 1);
}

void backspace(Editor *E) {

  Textline *line = &E->doc.lines[E->doc.y];
  char *text = line->text;
  int length = line->length;
  int idx = E->doc.x - 1;
  
  if (idx == -1 && E->doc.y -1 >= 0) {
    int above = E->doc.y - 1;
    Textline *lineAbove = &E->doc.lines[above];
    int lengthAbove = lineAbove->length;

    insertToDocumentLine(&E->doc, above, lengthAbove, text, length);

    removeLineFromDocument(&E->doc, E->doc.y);

    /* correct x and y */
    setDocumentY(&E->doc, E->doc.y - 1);
    setDocumentX(&E->doc, lengthAbove);

  } else if (idx < length) {
    removeFromDocumentLine(&E->doc, E->doc.y, idx, 1);
    setDocumentX(&E->doc, E->doc.x - 1);
  }
}

void newline(Editor *E) {
  int below = E->doc.y + 1;
  insertLineToDocument(&E->doc, below, NULL, 0);

  Textline *line = &E->doc.lines[E->doc.y];
  int textIdx = E->doc.x;
  char *text = &line->text[textIdx];
  int length = line->length - textIdx;
  
  insertToDocumentLine(&E->doc, below, 0, text, length);
  removeFromDocumentLine(&E->doc, E->doc.y, textIdx, length);

  setDocumentX(&E->doc, 0);
  setDocumentY(&E->doc, E->doc.y + 1);
}

/***** public *****/

void initEditor(Editor *E) {
  initDocument(&E->doc);
  initView(&E->view);
  E->filename = NULL;
}

void freeEditor(Editor *E) {
  freeDocument(&E->doc);
  freeView(&E->view);
}

int loadFileToEditor(Editor *E, char *filename) {
  int err;
  err = loadFileToDocument(&E->doc, filename);
  return (err) ? -1 : 0;
}

int loadEditorDimensions(Editor *E) {
  int err;
  err = loadViewDimensions(&E->view);
  return (err) ? -1 : 0;
}

void commandEditor(Editor *E, int key) {
  switch (key) {
  case Ctrl('n'): setDocumentY(&E->doc, E->doc.y + 1);
    break;
  case Ctrl('p'): setDocumentY(&E->doc, E->doc.y - 1);
    break;
  case Ctrl('b'): setDocumentX(&E->doc, E->doc.x - 1);
    break;
  case Ctrl('f'): setDocumentX(&E->doc, E->doc.x + 1);
    break;
  case UP: setDocumentY(&E->doc, E->doc.y - 1);
    break;
  case DOWN: setDocumentY(&E->doc, E->doc.y + 1);
    break;
  case LEFT: setDocumentX(&E->doc, E->doc.x - 1);
    break;
  case RIGHT: setDocumentX(&E->doc, E->doc.x + 1);
    break;
  case DELETE: backspace(E);
    break;
  case CARRIAGE_RETURN: newline(E);
    break;
  case Ctrl('e'): setDocumentX(&E->doc, E->doc.lines[E->doc.y].length);
    break;
  case Ctrl('a'): setDocumentX(&E->doc, 0);
    break;
  case Ctrl('s'): saveFile(E);
    break;
  }
  if (key > 31 && key < 127) insert(E, key);
}

void updateEditor(Editor *E) {
  updateViewOffsets(E);
  updateViewDisplay(E);
}

