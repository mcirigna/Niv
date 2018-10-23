#ifndef VIEW_H
#define VIEW_H

#include "Document.h"
#include "Textline.h"

typedef struct View {
  Textline display;
  int rowCount, colCount;
  int rowOffset, colOffset;
  int x, y;
} View;

void initView(View *V);

void freeView(View *V);

/* 0 if success, -1 otherwise */
int loadViewDimensions(View *V);

#endif /* VIEW_H */
