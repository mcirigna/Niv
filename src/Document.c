#include "Document.h"
#include "Textline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***** private *****/

int validLineIndex(Document *D, int idx) {
  return (idx < 0 || idx >= D->lineCount) ? 0 : 1;
}

void appendLine(Document *D, char *text, int length) {
  D->lines = realloc(D->lines, sizeof(Textline) * (D->lineCount + 1));

  if (length < 0) length = 0;
  
  int newline = D->lineCount;
  initTextline(&D->lines[newline]);

  D->lines[newline].length = length;
  D->lines[newline].text = malloc(length + 1);
  if (length > 0 && text != NULL) {
    memcpy(D->lines[newline].text, text, length);
  }
  D->lines[newline].text[length] = '\0';

  D->lineCount++;
}

void removeAllLines(Document *D) {
  freeTextlines(D->lines, D->lineCount);
}

/***** public *****/

void initDocument(Document *D) {
  D->lines = NULL;
  D->lineCount = 0;
  D->x = 0;
  D->y = 0;
}

void freeDocument(Document *D) {
  removeAllLines(D);
}

void insertLineToDocument(Document *D, int lineIdx, char *text, int length) {
  if (validLineIndex(D, lineIdx) == 0 && lineIdx != D->lineCount) return;

  Textline *originalLines = D->lines;
  int originalLineCount = D->lineCount;

  D->lines = NULL;  
  D->lineCount = 0;

  int i;
  for (i = 0; i < lineIdx; i++) {
    appendLine(D, originalLines[i].text, originalLines[i].length);
  }
  
  appendLine(D, text, length);
  
  for (; i < originalLineCount; i++) {
    appendLine(D, originalLines[i].text, originalLines[i].length);
  }

  freeTextlines(originalLines, originalLineCount);
}

void removeLineFromDocument(Document *D, int lineToRemove) {
  if (validLineIndex(D, lineToRemove) == 0) return;

  Textline *originalLines = D->lines;
  int originalLineCount = D->lineCount;
  D->lines = NULL;
  D->lineCount = 0;

  int l;
  for(l = 0; l < originalLineCount; l++) {
    if (l != lineToRemove) {
      appendLine(D, originalLines[l].text, originalLines[l].length);
    }
  }
  freeTextlines(originalLines, originalLineCount);
}

void insertToDocumentLine(Document *D, int lineIdx, int idx, char *text, int length) {
  if (validLineIndex(D, lineIdx) == 0) return;
  Textline *line = &D->lines[lineIdx];
  insertToTextline(line, idx, text, length);
}

void removeFromDocumentLine(Document *D, int lineIdx, int idx, int count) {
  if (validLineIndex(D, lineIdx) == 0) return;
  Textline *line = &D->lines[lineIdx];
  removeFromTextline(line, idx, count);
}

void setDocumentX(Document *D, int newX) {

  Textline *line = &D->lines[D->y];
  
  if (validTextlineIndex(line, newX) || newX == line->length) {
    D->x = newX;
  } else if (newX > line->length) { /* move down a row */
    int below = D->y + 1;
    if (validLineIndex(D, below)) {
      D->y = below;
      D->x = 0;
    }
  } else if (newX < 0) { /* move up a row */
    int above = D->y - 1;
    if (validLineIndex(D, above)) {
      D->x = D->lines[above].length;
      D->y = above;
    }
  }
}

void setDocumentY(Document *D, int newY) {
  if (validLineIndex(D, newY)) {
    D->y = newY;
    Textline *line = &D->lines[D->y];
    if (D->x > line->length) D->x = line->length;
  }
}

int loadFileToDocument(Document *D, char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) return -1;

  char *line = NULL;
  size_t cap = 0;
  ssize_t len = 0;
  while ((len = getline(&line, &cap, file)) > 0) {
    while(len > 0 && (line[len - 1] == '\r' || line[len - 1] == '\n')) len--;
    appendLine(D, line, len);
  }

  if (D->lineCount == 0) {
    appendLine(D, NULL, 0);
  }

  free(line);
  fclose(file);
  return 0;
}
