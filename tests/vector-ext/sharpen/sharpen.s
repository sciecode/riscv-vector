# uint16_t sharpenSIMD(const uint8_t *mat, const int8_t *mask, size_t width, size_t n )
# a0 = *mat, a1 = *mask, a2 = width, a3 = n;
#

.text                               # Start text section
.align 2                            # align 4 byte instructions by 2**2 bytes
.global sharpenSIMD                 # define global function symbol

sharpenSIMD:

    mv t1, a3                       # Copy kernel size
    slli a2, a2, 1                  # Width * 2
    slli t2, a3, 1                  # Kernelsize * 2

    vsetvli t0, a3, e16, ta,ma      # Set vector length to 16b SEW
    vmv.s.x v0, zero                # Set running sum to 0

loop:

    vle16.v v1, (a0)                # Load image line
    vle16.v v2, (a1)                # Load kernel line

    vmul.vv v1, v1, v2              # vector multiply mask * image
    vredsum.vs v0, v1, v0           # vd[0] = sum( accum, vs2[*] )

    add a0, a0, a2                  # Bump pointer
    add a1, a1, t2                  # Bump pointer

    addi t1, t1, -1                 # Decrement kernel height iteration
    bnez t1, loop                   # Loop back

    vmv.x.s a0, v0                  # Return total sum from convolution

    ret                             # Finished
