#pragma once

#include <stdlib.h>

size_t
StrlenHashFunction (const char* const str,
                    const size_t key_num);

size_t
AsciiSumHashFunction (const char* const str,
                      const size_t key_num);

size_t
PolynomialHashFunction (const char* const str,
                        const size_t key_num);

size_t
Crc32HashFunction (const char* const str,
                   const size_t key_num);