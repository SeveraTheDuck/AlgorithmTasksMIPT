#pragma once

#include <stdlib.h>

size_t
UnsignedModuleValueHashFunction (const unsigned value,
                                 const size_t key_num);

size_t
UnsignedBitExpressionHashFunction (const unsigned value,
                                   const size_t key_num);

size_t
UnsignedMultiplicationHashFunction (const unsigned value,
                                    const size_t key_num);