#include <stddef.h>
#include <stdio.h>

void vadd(const int*x, const int*y, int*z, size_t n );

int main() {

    static const int inp0[] = { 0x1, 0x2, 0x3, 0x4 };
    static const int inp1[] = { 0x9, 0x7, 0x5, 0x3 };

    int out[ sizeof inp0 ];

    vadd( inp0, inp1, out, sizeof inp0 / 4 );
    
    return 0;
}