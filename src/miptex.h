#ifndef PNG2QUAKE_MIPTEX_H
#define PNG2QUAKE_MIPTEX_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

struct miptex_s {
  char name[16];
  unsigned int width;
  unsigned int height;
  unsigned int miplevel[4];
  unsigned char data[];
};

struct miptex_s* miptex_new(unsigned int width, unsigned int height);
void miptex_delete(struct miptex_s* miptex);

unsigned char* miptex_get_mipmap(struct miptex_s* miptex, size_t miplevel);

size_t miptex_size(struct miptex_s* miptex);

#ifdef __cplusplus
}
#endif
#endif // PNG2QUAKE_MIPTEX_H
