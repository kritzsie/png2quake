#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <png.h>

#include "miptex.h"

static struct miptex_s* miptex_from_png(FILE* pngfile) {
  struct miptex_s* miptex;

  png_structp png;
  png_infop info;

  png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png == NULL) {
    fprintf(stderr, "Error: no libpng read struct\n");
    return NULL;
  }

  info = png_create_info_struct(png);
  if (info == NULL) {
    fprintf(stderr, "Error: no libpng info struct\n");
    png_destroy_read_struct(&png, NULL, NULL);
    return NULL;
  }

  png_init_io(png, pngfile);
  png_read_info(png, info);

  if (png_get_bit_depth(png, info) != 8) {
    png_destroy_read_struct(&png, &info, NULL);
    return NULL;
  }

  miptex = miptex_new(png_get_image_width(png, info), png_get_image_height(png, info));

  png_bytep* rows = malloc(sizeof(png_bytep*) * miptex->height);
  for (size_t y = 0; y < miptex->height; ++y) {
    rows[y] = miptex->data + y * miptex->width;
  }
  png_read_image(png, rows);
  free(rows);

  for (int level = 1; level < 4; ++level) {
    unsigned char* mipmap = (unsigned char*)miptex + miptex->miplevel[level];
    unsigned int ratio = pow(2, level);

    for (size_t y = 0; y < miptex->height / ratio; ++y)
    for (size_t x = 0; x < miptex->width  / ratio; ++x) {
      mipmap[x + y * miptex->width / ratio] = miptex->data[x * ratio + y * miptex->width * ratio];
    }
  }

  png_destroy_read_struct(&png, &info, NULL);

  return miptex;
}

static int png2quake(const char* filename_png) {
  FILE* pngfile;
  FILE* miptexfile;
  char texname[17];
  char filename[21];
  size_t extpos;

  struct miptex_s* miptex;

  // Open PNG file
  pngfile = fopen(filename_png, "rb");
  if (pngfile == NULL) {
    fprintf(stderr, "Error: couldn't open \"%s\" for reading\n", filename_png);
    return 1;
  }

  // Convert png to miptex in memory
  miptex = miptex_from_png(pngfile);
  if (miptex == NULL) {
    fprintf(stderr, "Error: %s is not a valid PNG file", pngfile);
    fclose(pngfile);
    return 1;
  }

  // Determine output file name
  extpos = strlen(filename_png);
  for (size_t i = extpos; i > 0; --i) {
    if (filename_png[i] == '.') {
      extpos = i > 16 ? 16 : i;
      break;
    }
  }

  strncpy(texname, filename_png, extpos);
  texname[extpos] = '\0';

  strcpy(filename, texname);
  strcpy(filename + extpos, ".lmp");

  // Write out miptex file
  miptexfile = fopen(filename, "wb");
  if (miptexfile == NULL) {
    fprintf(stderr, "Error: couldn't open \"%s\" for writing\n", filename);
    free(miptex);
    fclose(pngfile);
    return 1;
  }

  strncpy(miptex->name, texname, 16);

  fwrite(miptex, 1, miptex_size(miptex), miptexfile);

  free(miptex);
  fclose(miptexfile);
  fclose(pngfile);

  return 0;
}

int main(int argc, char** argv) {
  if (argc > 1) {
    for (size_t i = 1; i < argc; ++i) {
      png2quake(argv[i]);
    }
  }
  else {
    fprintf(stderr, "Usage: %s FILE [FILE]...\n", argv[0]);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
