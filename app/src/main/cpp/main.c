#include <stdio.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

/*
** the bitwise version :
** we apply n - 1 mask to n, and then check that is equal to 0
** it will be true for all numbers that are power of 2.
** Lastly we make sure that n is superior to 0.
*/
static inline int is_power_of_2(uint32_t n) {
    return (n > 0 && !(n & (n - 1)));
}

static inline uint32_t round_to_pow2(uint32_t x) {
    if (x == 0 || is_power_of_2(x)) {
        return x;
    }
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

#pragma clang diagnostic pop

int main() {
    printf("\n======= Now Run Main ==========\n");

    printf("\nround_to_pow2(128) => %d\n", round_to_pow2(128));
    printf("\nround_to_pow2(511) => %d\n", round_to_pow2(511));
    printf("\nround_to_pow2(1023) => %d\n", round_to_pow2(1023));
    printf("\nround_to_pow2(4096) => %d\n", round_to_pow2(4096));
    printf("\nround_to_pow2(65535) => %d\n", round_to_pow2(65535));
    printf("\nround_to_pow2(1280000) => %d\n", round_to_pow2(1280000));

    printf("\n======= Run Main Finished ==========\n");
    return 0;
}
