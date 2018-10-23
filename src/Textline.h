#ifndef TEXTLINE_H
#define TEXTLINE_H

/*** TextLine ***/
typedef struct Textline {
  char *text;
  int length;
} Textline;

void initTextline(Textline *line);

void freeTextline(Textline *line);

void freeTextlines(Textline *lines, int lineCount);

void appendToTextline(Textline *line, const char *text, int length);

void insertToTextline(Textline *line, int idx, const char *text, int length);

void removeFromTextline(Textline *line, int idx, int count);

void newTextline(Textline *line, const char *text, int length);

/* 1 if valid, 0 otherwise */
int validTextlineIndex(Textline *line, int idx);

#endif
