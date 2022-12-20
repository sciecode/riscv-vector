#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int kernel_size = 9;
char filename[] = "baboon.pgm";

char inputs[] = "inputs/";
char outputs[] = "outputs/";
char path[] = "tests/vector-ext/blurV/";

int16_t blurSIMD(const int16_t *mat, size_t width, size_t n );

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

void blur( int kn, int16_t *mat, int width, int height ) {

  int w = width + 2*kn;
  int h = height + 2*kn;

  int sq = kernel_size * kernel_size;
  int r, c, sum;

  char filepath[200] = "";
  strcat(filepath,path);
  strcat(filepath,outputs);
  strcat(filepath,filename);

  // write output
  FILE *fw = fopen(filepath, "w");
  fprintf(fw, "P2 %d %d 255\n", width, height);

  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      int sum = blurSIMD( &mat[ j * w + i ], w, kernel_size );
      fprintf(fw, "%4d", sum / sq );
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

  // blur
  blur( kn, mat, width, height );

  return 0;

}