#include "utils.h"

#include <math.h>
#include <float.h>

int double_compare(const double* lhs, const double* rhs)
{
    double diff = *rhs - *lhs;
    if (fabs(diff) <= DBL_EPSILON) {
        return 0;
    }
    else if (*lhs < *rhs) {
        return -1;
    }
    else {
        return 1;
    }
}
