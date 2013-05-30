#include <stdarg.h>

#define MAX_ARRAY_DIM 8
typedef struct {
	ElemType *base;
	int dim;
	int *bounds;
	int *constants;
} Array;
