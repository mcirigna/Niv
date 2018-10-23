#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Textline.h"

/*** Document ***/
typedef struct TextDocument {
  Textline *lines;
  int lineCount;
  int x, y;
} Document;

void initDocument(Document *D);

void freeDocument(Document *D);

void insertLineToDocument(Document *D, int lineIdx, char *text, int length);

void removeLineFromDocument(Document *D, int lineIdx);

void insertToDocumentLine(Document *D, int lineIdx, int idx, char *text, int length);

void removeFromDocumentLine(Document *D, int lineIdx, int idx, int count);

void setDocumentX(Document *D, int newX);

void setDocumentY(Document *D, int newY);

int loadFileToDocument(Document *D, char *filename);

#endif /* DOCUMENT_H */
