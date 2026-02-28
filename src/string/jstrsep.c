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

char *jstrsep(char **restrict stringp, const char *restrict delim)
{
        char *ret;
        char *c;

        if (!stringp || !*stringp)
                return NULL;

        ret = *stringp;
        c = *stringp;
        while (*c) {
                if (jstrchr(delim, *c)) {
                        *c = '\0';
                        *stringp = c + 1;
                        return ret;
                }
                c++;
        }
        *stringp = NULL;
        return ret;
}
