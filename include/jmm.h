#if !defined(__JMM_H)
# define __JMM_H
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

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>


#if defined(__has_feature) && __has_feature(nullability)
#define __jnullable _Nullable
#else
#define __jnullable
#endif

#define BLOCK_LIT 4096 /* always request 4096 bytes at least */
#define BLOCK_MID 131072 /* 128 kibibytes */
#define BLOCK_BIG 1048576 /* 1 mebibyte */

typedef struct header header;
struct __attribute__((aligned(16))) header {
	size_t size; /* size of this block */
	bool is_free; /* whether the block is free or not */
	header *prev; /* the prev block if on free list */
	header *next; /* the next block if on free list */
};

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
extern void jfree(void *__jnullable p);

/*
 * Reallocates the space used on the heap by `p` to size `size`.
 */
extern void *jrealloc(void *__jnullable p, size_t size);
#endif /* __JMM_H */
