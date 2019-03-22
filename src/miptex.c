#include "miptex.h"

#include <math.h>
#include <stdlib.h>

static size_t miptex_calculate_mipmap_offset(unsigned int width, unsigned int height, size_t miplevel) {
  size_t size = 40;
  unsigned long area = width * height;

  for (size_t i = 0; i < miplevel; ++i) {
    size += area / pow(4, i);
  }

  return size;
}

static size_t miptex_calculate_size(unsigned int width, unsigned int height) {
  return miptex_calculate_mipmap_offset(width, height, 4);
}

struct miptex_s* miptex_new(unsigned int width, unsigned int height) {
  struct miptex_s* miptex;

  miptex = malloc(miptex_calculate_size(width, height));
  miptex->width = width;
  miptex->height = height;
  for (size_t i = 0; i < 4; ++i) {
    miptex->miplevel[i] = miptex_calculate_mipmap_offset(width, height, i);
  }

  return miptex;
}

void miptex_delete(struct miptex_s* miptex) {
  free(miptex);
}

unsigned char* miptex_get_mipmap(struct miptex_s* miptex, size_t miplevel) {
  if (miplevel < 4) {
    return (unsigned char*)miptex + miptex->miplevel[miplevel];
  }

  return NULL;
}

size_t miptex_size(struct miptex_s* miptex) {
  return miptex_calculate_size(miptex->width, miptex->height);
}
