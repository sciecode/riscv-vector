#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h> 

int kernel_size = 3;
char filename[] = "baboon.pgm";

char inputs[] = "inputs/";
char outputs[] = "outputs/";
char path[] = "tests/vector-ext/sharpen/";

int16_t sharpenSIMD(const int16_t *mat, const int16_t *mask, size_t width, size_t n );

int clamp( int val ) {
  return val < 0 ? 0 : ( val > 255 ? 255 : val );
}

void padding( int kn, int16_t *mat, int width, int height ) {

  int w = width + 2*kn;
  int h = height + 2*kn;

  for ( int k = 0; k < kn; k++ ) {
    // padding row
    for ( int i = 0; i < width; i++ ) {
      mat[ k*w + (i+kn) ] = mat[ kn*w + (i+kn) ];
      mat[ (h-1-k)*w + (i+kn) ] = mat[ (h-1-kn) * w + (i+kn) ];
    }
  }

  for ( int k = 0; k < kn; k++ ) {
    // padding column
    for ( int j = 0; j < h; j++ ) {
      mat[ j * w + kn-1-k ] = mat[ j * w + kn-k ];
      mat[ j * w + w-kn+k ] = mat[ j * w + (w-1)-kn+k ];
    }
  }

}

void sharpen( int kn, int16_t *mat, int width, int height ) {

  int w = width + 2*kn;
  int h = height + 2*kn;

  int sq = kernel_size * kernel_size;
  int r, c;

  char filepath[200] = "";
  strcat(filepath,path);
  strcat(filepath,outputs);
  strcat(filepath,filename);

  // sharpen
  static const int16_t mask[] = { 
     0, -1,  0,
    -1,  5, -1,
     0, -1,  0
  };

  // // edge detection
  // static const int16_t mask[] = { 
  //    1,  0,  -1,
  //    0,  0,  0,
  //   -1,  0,  1
  // };

  // write output
  FILE *fw = fopen(filepath, "w");
  fprintf(fw, "P2 %d %d 255\n", width, height);

  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      int res = clamp( sharpenSIMD( &mat[ j * w + i ], mask, w, kernel_size ) );
      fprintf(fw, "%4d", res );
    }
    fprintf(fw, "\n");
  }

  fclose(fw);

}

int main() {

  FILE *fr;
  char version[3];
  uint32_t width, height, depth, data, kn = kernel_size / 2;

  char filepath[200] = "";
  strcat(filepath,path);
  strcat(filepath,inputs);
  strcat(filepath,filename);

  // read header
  fr = fopen(filepath, "r");
  fscanf(fr, "%s %d %d %d\n", &version [0], &width, &height, &depth);

  // alloc data
  int w = width + 2*kn;
  int h = height + 2*kn;
  int16_t *mat = malloc( w * h * sizeof(int16_t) );

  // read data
  for ( int j = 0; j < height; j++) {
    for ( int i = 0; i < width; i++ ) {
      fscanf(fr, "%d", &data);
      mat[ (j+kn) * w + (i+kn) ] = data;
    }
  }

  fclose(fr);

  // insert padding
  padding( kn, mat, width, height );

  // sharpen
  sharpen( kn, mat, width, height );

  return 0;

}