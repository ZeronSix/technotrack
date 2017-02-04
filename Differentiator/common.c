#include "common.h"
#include "math.h"

#define EPS 1E-09

bool CompareDouble(double a, double b)
{
    return fabs((a - b)) < EPS;
}
