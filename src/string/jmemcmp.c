/* nstdlib - C standard library implementation done as a study exercise.
Copyright (C) 2026  Emir Baha Yıldırım

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "jstring.h"

int jmemcmp(const void *s1, const void *s2, size_t n)
{
        const unsigned char *t1 = (const unsigned char *)s1;
        const unsigned char *t2 = (const unsigned char *)s2;
        for (size_t i = 0; i < n; i++) {
                if (t1[i] != t2[i])
                        return t1[i] - t2[i];
        }
        return 0;
}
