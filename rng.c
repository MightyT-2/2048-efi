#include "rng.h"

#define MODULUS     0x80000000
#define MULTIPLIER  0x41C64E6D
#define INCREMENTOR 0x3039

int seed = 1;

void seed_rng(int value)
{
    seed = value % MODULUS;

    return;
}

int fetch_rng()
{
    seed = (MULTIPLIER * seed + INCREMENTOR) % MODULUS;
    return seed;
}
