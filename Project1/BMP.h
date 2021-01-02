#ifndef BMP_H
#define BMP_H

#pragma once
#include <stdio.h>

unsigned char* BmpToTexture(const char*, int*, int*);
int ReadInt(FILE*);
short ReadShort(FILE*);
#endif 
