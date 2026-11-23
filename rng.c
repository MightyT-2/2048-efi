#include "rng.h"

#define MODULUS     0x80000000
#define MULTIPLIER  1103515245
#define INCREMENTOR 12345

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
