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

char *jstpncpy(char *restrict dst, const char *restrict src, size_t n)
{
	size_t bytes = 0;
        char *anchor;

        while (bytes < n) {
                if (!*src) {
                        break;
                } else {
                        *dst++ = *src++;
                        bytes++;
                }
        }
        anchor = dst;
        while (bytes < n) {
                *dst++ = '\0';
                bytes++;
        }

        return anchor;
}
