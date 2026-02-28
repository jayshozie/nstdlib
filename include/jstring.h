#if !defined(__JSTR_H)
# define __JSTR_H
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

#include <stddef.h>


/*
 * ==========================================================================
 */

/*
 *              1 .   R A W   M E M O R Y   O P E R A T I O N S
 */
/*
 * 1. Raw Memory Operations (Byte-level)
 *     memcpy ✔️
 *     memmove ✔️
 *     memset ✔️
 *     memcmp ✔️
 */
/*
 * The memcpy() function copies n bytes from memory area src to memory area
 * dest. The memory areas must not overlap. Use memmove(3) if the memory areas
 * do overlap.
 */
extern void *jmemcpy(void *restrict dest, const void *restrict src, size_t n);

/*
 * The  memmove() function copies n bytes from memory area src to memory area
 * dest. The memory areas may overlap: copying takes place as though the bytes
 * in src are first copied into a temporary array that does not overlap src or
 * dest,  and  the bytes are then copied from the temporary array to dest.
 */
extern void *jmemmove(void *dest, const void *src, size_t n);

/*
 * The memset() function fills the first n bytes of the memory area pointed to
 * by s with the constant byte c.
 */
extern void *jmemset(void *s, int c, size_t n);

/*
 * The  memcmp()  function  compares  the  first  n  bytes (each interpreted as
 * unsigned char) of the memory areas s1 and s2.
 */
extern int jmemcmp(const void *s1, const void *s2, size_t n);

/*
 * ==========================================================================
 */

/*
 *             2 .   C O R E   S T R I N G   O P E R A T I O N S
 */
/*
 * 2. Core String Operations (Null-terminated)
 *     strlen ✔️
 *     strcpy ✔️
 *     strncpy ✔️
 *     strcmp ✔️
 *     strncmp ✔️
 */
/* Here "character" means "byte"; these functions do not work with wide or
 * multi‐byte characters. */
/*
 * The  strlen()  function calculates the length of the string pointed to by s,
 * excluding the terminating null byte ('\0').
 */
extern size_t jstrlen(const char *s);

/*
 * stpcpy()
 * strcpy()
 *      These  functions  copy the string pointed to by src, into a string at
 *      the buffer pointed to by dst.  The programmer is responsible for
 *      allocating a destination buffer large enough, that is, strlen(src) + 1.
 *      For the  difference between the two functions, see RETURN VALUE.
 */
extern char *jstpcpy(char *restrict dst, const char *restrict src);
extern char *jstrcpy(char *restrict dst, const char *restrict src);
/*
 * strcat()
 *      This  function  catenates  the string pointed to by src, after the
 *      string pointed to by dst (overwriting its terminating null byte).  The
 *      programmer is responsible for allocating a destination buffer large
 *      enough, that is, strlen(dst) + strlen(src) + 1.
 */
extern char *jstrcat(char *restrict dst, const char *restrict src);

/*
 * These functions copy non-null bytes from the string pointed to by src  into
 * the array pointed to by dst. If the source has too few non-null bytes to fill
 * the destination, the functions pad the destination with trailing null bytes.
 * If the destination buffer, limited by its size, isn't large enough to hold
 * the copy, the resulting character sequence is truncated. For the difference
 * between the two functions, see RETURN VALUE.
 */
extern char *jstrncpy(char *restrict dst, const char *restrict src, size_t n);
extern char *jstpncpy(char *restrict dst, const char *restrict src, size_t n);

/*
 * The strcmp() function compares the two strings s1 and s2. The locale is not
 * taken into account (for a locale-aware comparison, see strcoll(3)). The
 * comparison is done using unsigned characters.
 *
 * strcmp() returns an integer indicating the result of the comparison, as
 * follows:
 *     •  0, if the s1 and s2 are equal;
 *     •  a negative value if s1 is less than s2;
 *     •  a positive value if s1 is greater than s2.
 */
extern int jstrcmp(const char *s1, const char *s2);

/*
 * The strncmp() function is similar, except it compares only the first (at
 * most) n bytes of s1 and s2.
 */
extern int jstrncmp(const char s1[], const char s2[], size_t n);

/*
 * ==========================================================================
 */

/*
 *          3 .   A D V A N C E D   S T R I N G   P A R S I N G
 */
/*
 * 3. Advanced String Parsing
 *     strchr ✔️
 *     strrchr ✔️
 *     strsep ✔️
 *     strdup ✔️
*/
/* Here "character" means "byte"; these functions do not work with wide or
 * multi‐byte characters. */
/*
 * The strchr() function returns a pointer to the first occurrence of the
 * character c in the string s.
 */
extern char *jstrchr(const char *s, int c);
/*
 * The strrchr() function returns a pointer to the last occurrence of the
 * characterc in the string s.
 */
extern char *jstrrchr(const char *s, int c);
/*
 * The  strchrnul()  function  is like strchr() except that if c is not found
 * in s, then it returns a pointer to the null byte at the end of s, rather than
 * NULL.
 */
extern char *jstrchrnul(const char *s, int c);

/*
 * If  *stringp  is NULL, the strsep() function returns NULL and does nothing
 * else. Otherwise, this function finds the first token in the string  *stringp
 * that is delimited  by one of the bytes in the string delim. This token is
 * terminated by overwriting the delimiter with a null byte ('\0'), and *stringp
 * is  updated  to point  past the token.  In case no delimiter was found, the
 * token is taken to be the entire string *stringp, and *stringp is made NULL.
 */
extern char *jstrsep(char **restrict stringp, const char *restrict delim);

/*
 * The  strdup() function returns a pointer to a new string which is a duplicate
 * of the string s. Memory for the new string is obtained with malloc(3), and
 * can be freed with free(3).
 */
extern char *jstrdup(const char *s);

/*
 * The  strndup()  function is similar, but copies at most n bytes. If s is
 * longer than n, only n bytes are copied, and a terminating null byte ('\0') is
 * added.
 */
extern char *jstrndup(const char *s, size_t n);

/*
 * strdupa() and strndupa() are similar, but use alloca(3) to allocate the
 * buffer. These two has to be macros due to them not having a separate stack.
 */
// extern char *jstrdupa(const char *s);
// extern char *jstrdupa(size_t n; const char *s[n], size_t n);

/*
 * ==========================================================================
 */

#endif /* __JSTR_H */
