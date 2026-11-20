#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <iostream>
#include "page_table.h"

const int OK = 0;
const int OUT_OF_RANGE = 1;
const int PAGE_FAULT = 2;
const int INVALID_INPUT = 3;

int translate(int LA, int P, const PageTable& table, int &PA, std::ostream &out);

#endif

