#include "Textline.h"
#include <stdio.h>     /* NULL */
#include <stdlib.h>    /* free */
#include <string.h>    /* memcpy */

int validTextlineIndex(Textline *line, int idx) {
  return (idx < 0 || idx >= line->length) ? 0 : 1;
}

void initTextline(Textline *line) {
  line->text = NULL;
  line->length = 0;
}

void freeTextline(Textline *line) {
  free(line->text);
}

void freeTextlines(Textline *lines, int lineCount) {
  int i;
  for (i = 0; i < lineCount; i++) {
    freeTextline(&lines[i]);
  }
  free(lines);
  lines = NULL;
}

void insertToTextline(Textline *line, int idx, const char *text, int length) {
  if (text == NULL || length == 0) return;
  if (idx < 0 || idx > line->length) return;

  int newlength = line->length + length;
  char *newtext = malloc(newlength + 1);

  int i;
  for (i = 0; i < idx; i++) {
    newtext[i] = line->text[i];
  }

  int j;
  for (j = 0; j < length; j++, i++) {
    newtext[i] = text[j];
  }

  int k;
  for (k = idx; k < line->length; k++, i++) {
    newtext[i] = line->text[k];
  }
  newtext[newlength] = '\0';

  free(line->text);
  line->text = newtext;
  line->length = newlength;
}

void appendToTextline(Textline *line, const char *text, int length) {
  if (line == NULL || length <= 0) return;

  int prevlength = line->length;
  int newlength = prevlength + length;
  line->text = realloc(line->text, newlength + 1);
  memcpy(&line->text[prevlength], text, length);
  line->text[newlength] = '\0';
  line->length = newlength;
}

void removeFromTextline(Textline *line, int idx, int count) {
  if (idx < 0 || idx >= line->length) return;

  while (idx + count > line->length) count--;
  
  char *newtext;
  int newlength = line->length - count;
  newtext = malloc(newlength + 1);

  int i;
  for (i = 0; i < idx; i++) {
    newtext[i] = line->text[i];
  }

  int j;
  for (j = idx + count; j < line->length; j++, i++) {
    newtext[i] = line->text[j];
  }
  newtext[i] = '\0';
  
  free(line->text);
  line->text = newtext;
  line->length = newlength;
}

void newTextline(Textline *line, const char *text, int length) {
  freeTextline(line);
  line->length = length;
  line->text = malloc(length + 1);
  memcpy(line->text, text, length);
  line->text[length] = '\0';
}
