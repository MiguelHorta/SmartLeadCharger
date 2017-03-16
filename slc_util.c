#include "slc_util.h"
void slc_clamp(uint16_t *n, int lower, int upper )
{
    *n = *n <= lower ? lower : *n >= upper ? upper : *n;
}