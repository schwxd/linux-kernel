#define ElemType int
#include "array.h"

Status InitArray(Array *A, int dim, ...);

Status DestroyArray(Array *A);

Status Value(Array A, ElemType *e, ...);

Status Assign(Array *A, ElemType e, ...);


Status InitArray(Array *A, int dim, ...)
{
	va_list ap;
	int n, i = 0, total = 1;

	if (dim < 1 || dim > MAX_ARRAY_DIM)
		return ERROR;

	A->dim = dim;
	A->bounds = (int *)malloc(dim * sizeof(int));
	if (!A->bounds)
		return ERROR;

	va_start(ap, dim);
	for (i = 0; i < dim; i++) {
		n = va_arg(ap, int);
		A->bounds[i] = n;
		total *= n;
	}
	va_end(ap);

	A->base = (ElemType *)malloc(total * sizeof(ElemType));
	if (!A->base)
		return ERROR;

	A->constants = (int *)malloc(dim * sizeof(int));
	if (!A->constants)
		return ERROR;
	A->constants[dim-1] = 1;
	for (i = dim -2; i >= 0; i--)
		A->constants[i] = A->bounds[i+1] * A->constants[i+1];

	return OK;
}

Status DestroyArray(Array *A)
{
	if (A->base) {
		free(A->base);
		A->base = NULL;
	}
	if (A->bounds) {
		free(A->bounds);
		A->bounds = NULL;
	}
	if (A->constants) {
		free(A->constants);
		A->constants = NULL;
	}
	return OK;	
}

Status Locate(Array A, va_list ap, int *off)
{
	int i, n;
	*off = 0;

	for (i = 0; i < A.dim; i++) {
		n = va_arg(ap, int);
		if (n < 0 || n >= MAX_ARRAY_DIM)
			return ERROR;
		*off += A.constants[i] * n;
	}
	return OK;
}

Status Value(Array A, ElemType *e, ...)
{
	va_list ap;
	int off;

	va_start(ap, e);
	if (Locate(A, ap, &off))
		*e = *(A.base + off);
	va_end(ap);
	return OK;
}

Status Assign(Array *A, ElemType e, ...)
{
	va_list ap;
	int off;

	va_start(ap, e);
	if (Locate(*A, ap, &off))
		*(A->base + off) = e;
	va_end(ap);
	return OK;
}

