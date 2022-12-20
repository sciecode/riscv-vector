# void vor(const int*x, const int*y, int*z, size_t n )
# a0 = x, a1 = y, a2 = z, a3 = n;
#

.text                               # Start text section
.align 2                            # align 4 byte instructions by 2**2 bytes
.global vxor                        # define global function symbol

vxor:

    vsetvli t0, a3, e32, ta,ma      # Set vector length based on 32-bit vectors
    vle32.v v0, (a0)                # Get first vector

    sub a3, a3, t0                  # Decrement number done

    slli t0, t0, 2                  # Multiply number done by 4 bytes
    add a0, a0, t0                  # Bump pointer

    vle32.v v1, (a1)                # Get second vector
    add a1, a1, t0                  # Bump pointer

    vxor.vv v2, v0, v1              # vector bitwise-xor elements

    vse32.v v2, (a2)                # Store result
    add a2, a2, t0                  # Bump pointer

    bnez a3, vxor                   # Loop back

    ret                             # Finished
