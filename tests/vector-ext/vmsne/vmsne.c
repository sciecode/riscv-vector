#include <stddef.h>
#include <stdio.h>

void vmsne(const int*x, const int*y, int*z, size_t n );

int main() {

    static const int inp0[] = { 0x1, 0x2, 0x3, 0x4 };
    static const int inp1[] = { 0x1, 0x2, 0x4, 0x3 };

    int out[ sizeof inp0 ];

    vmsne( inp0, inp1, out, sizeof inp0 / 4 );
    
    return 0;
}