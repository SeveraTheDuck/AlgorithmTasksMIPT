#ifndef MAKE_ARRAY_H
#define MAKE_ARRAY_H

#include <stdlib.h>
#include <assert.h>

void*
MakeArray (const size_t n_elements,
           const unsigned long long max_value);

#endif /* MAKE_ARRAY_H */