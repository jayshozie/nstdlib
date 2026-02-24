/* allocs - Custom memory allocators written as a study exercise.
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

#include "nstdlib.h"

/*
 * Helper for calling sbrk()
 */
extern header *upbrk(size_t size);

/*
 * Allocates memory of size `size`, returs the pointer
 */
extern void *jmalloc(size_t size);

/*
 * Free's the memory on the heap used by `p`. Doesn't return.
 */
extern void jfree(void *__attribute__((nullable)) p);
//
// /*
//  * Reallocates the space used on the heap by `p` to size `size`.
//  */
// extern void *realloc(void *_Nullable p, size_t size);
