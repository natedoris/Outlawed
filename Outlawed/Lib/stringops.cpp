#include "../pch.h"
#include "stringops.h"
#include <assert.h>

namespace stringops
{
    void slicestr(const char* src, char* dst, int start_point, int end_point)
    {
        size_t count = 0;
        size_t src_len = strlen(src);
        for (size_t i = start_point; i < src_len; i++)
        {
            dst[count] = src[i];
            count++;
            if (i == end_point) break;
        }
    }

    int revstrcmp(const char* string1, const char* string2)
    {
        size_t len_str1 = strlen(string1);
        size_t len_str2 = strlen(string2);
        assert(len_str1 < len_str2 && "param 1 must be >= to param 2");
        int counter = 0;

        for (int i = len_str1; i >= 0; i--)
        {
            if (string1[i] == string2[len_str2--])
            {
                counter++;
            }
            else
            {
                break;
            }
        }

        if (counter - 1 == len_str1) {
            return 0;
        }
        else {
            return 1;
        }
    }
}