#pragma once

#include <stdlib.h>

size_t
FloatCastToIntHashFunction (const float value,
                            const size_t key_num);

size_t
FloatBitExpressionHashFunction (const float value,
                                const size_t key_num);

size_t
FloatGetMantissaHashFunction (const float value,
                              const size_t key_num);

size_t
FloatGetExponentHashFunction (const float value,
                              const size_t key_num);

size_t
FloatMultiplyHashFunction (const float value,
                           const size_t key_num);

size_t