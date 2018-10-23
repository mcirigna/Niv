#ifndef EDITOR_H
#define EDITOR_H

#include "Document.h"
#include "View.h"

typedef struct Editor {
  Document doc;
  View view;
  char *filename;
  int logfile;
} Editor;

void initEditor(Editor *E);

void freeEditor(Editor *E);

int loadFileToEditor(Editor *E, char *filename);

int loadEditorDimensions(Editor *E);

void commandEditor(Editor *E, int command);

void updateEditor(Editor *E);

#endif
