#ifndef PNG2QUAKE_MIPTEX_H
#define PNG2QUAKE_MIPTEX_H

#include <stdlib.h>

struct miptex_s {
  char name[16];
  unsigned int width;
  unsigned int height;
  unsigned int miplevel[4];
  unsigned char data[];
};

size_t miptex_miplevel_offset(unsigned int width, unsigned int height, unsigned char miplevel);

size_t miptex_size(unsigned int width, unsigned int height);

#endif // PNG2QUAKE_MIPTEX_H
