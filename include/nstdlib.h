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

#include <sys/mman.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define BLOCK_LIT 4096     /* always request 4096 bytes at least */
#define BLOCK_MID 131072   /* 128 kibibytes */
#define BLOCK_BIG 1048576  /* 1 mebibyte */

typedef double Align;

typedef union header header;
union header {
	struct {
		size_t size;   /* size of this block */
                bool is_free;  /* whether the block is free or not */
		header *prev;   /* the next block if on free list */
                header *next;
	} b;
	Align x; /* forces the alignment of the blocks */
};
