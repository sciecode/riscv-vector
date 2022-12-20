# uint16_t blurSIMD(const uint16_t *mat, size_t width, size_t n )
# a0 = *mat, a1 = width, a2 = n;
#

.text                               # Start text section
.align 2                            # align 4 byte instructions by 2**2 bytes
.global blurSIMD                    # define global function symbol

blurSIMD:

    mv t1, a2                       # Copy kernel size
    slli a1, a1, 1                  # Width * 2 = 16b elements

    vsetvli t0, a2, e16, m4, ta,ma  # Set vector length to 16b SEW
    vmv.s.x v0, zero                # Set running sum to 0


loop:

    vle16.v v4, (a0)                # Get first vector

    vredsum.vs v0, v4, v0           # vd[0] = sum( vs1[0], vs2[*] )

    addi t1, t1, -1                 # Decrement kernel height iteration
    add a0, a0, a1                  # Bump pointer

    bnez t1, loop                   # Loop back

    vmv.x.s a0, v0                  # Return total sum from convolution

    ret                             # Finished
