#include "miptex.h"

#include <math.h>
#include <stdlib.h>

size_t miptex_miplevel_offset(unsigned int width, unsigned int height, unsigned char miplevel) {
  size_t offset = 40;
  for (size_t i = 0; i < miplevel; ++i) {
    offset += width * height / pow(4, i);
  }
  return offset;
}

size_t miptex_size(unsigned int width, unsigned int height) {
  return miptex_miplevel_offset(width, height, 4);
}
