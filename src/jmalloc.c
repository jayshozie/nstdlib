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

#include "jmalloc.h"
#if defined(DEBUG_JMALLOC) || defined(DEBUG_JFREE) || defined(DEBUG_JREALLOC)
#include <stdio.h>
#endif

/* head of the linked list */
header *head;

/*
 * The only place where sbrk() is actually called.
 */
header *upbrk(size_t size)
{
	header *p;
	size_t aligned;
	if (size <= BLOCK_LIT) {
		if ((p = sbrk(BLOCK_LIT)) != (void *)-1)
			p->b.size = BLOCK_LIT;
		else
			p = NULL;
	} else if (BLOCK_LIT < size && size <= BLOCK_MID) {
		if ((p = sbrk(BLOCK_MID)) != (void *)-1)
			p->b.size = BLOCK_MID;
		else
			p = NULL;
	} else if (BLOCK_MID < size && size <= BLOCK_BIG) {
		if ((p = sbrk(BLOCK_BIG)) != (void *)-1)
			p->b.size = BLOCK_BIG;
		else
			p = NULL;
	} else {
		aligned = (size / sizeof(header) + 1) * sizeof(header);
		if ((p = sbrk(aligned)) != (void *)-1)
			p->b.size = aligned;
		else
			p = NULL;
	}
	if (p) {
		p->b.is_free = true;
		p->b.prev = NULL;
		p->b.next = NULL;
	}
	return p;
}

void *jmalloc(size_t size)
{
	header *p;
	header *n;
	size_t total;

	if (size == 0)
		return NULL;

	/* total includes the size of the header */
	total = size + sizeof(header);
	total = (total + (sizeof(header) - 1)) & ~(sizeof(header) - 1);

	if (total < size) {
#ifdef DEBUG_JMALLOC
		fprintf(stderr, "[DEBUG] integer overflow. size : %d\n", size);
#endif
		return NULL;
	}

	/* list doesn't exist yet */
	if (head == NULL)
		head = upbrk(BLOCK_LIT);
#ifdef DEBUG_JMALLOC
	fprintf(stderr, "[DEBUG] requested size: %d\n", (int)size);
	fprintf(stderr, "[DEBUG] total_size: %d\n", (int)total);
#endif
	p = head;
	while (p != NULL) {
#ifdef DEBUG_JMALLOC
		fprintf(stderr, "[DEBUG] p->b.size : %d\n", (int)p->b.size);
#endif
		if (p->b.is_free && p->b.size >= total) {
			/* claim the block. there is enough space */
			if (p->b.size - total >= sizeof(header)) { /* trim */
				/* there is enough space for a second block */
				n = (header *)((void *)p + total);
				n->b.is_free = true;
				n->b.size = p->b.size - total;

				p->b.is_free = false;
				p->b.size = total;

				/* doubly linked lists are confusing af */
				n->b.prev = p;
				n->b.next = p->b.next;
				p->b.next = n;
				if (n->b.next)
					n->b.next->b.prev = n;
			} else {
				/*
				 * either there isn't enough space for a second
				 * block, or we have the perfect case
				 * (size == total)
				 */
				p->b.is_free = false;
			}
			break;
		} else if (p->b.next == NULL) {
			/* we don't have a free block */
			n = upbrk(total);
			p->b.next = n;
			n->b.prev = p;
			p = n;
			p->b.is_free = false;
			break;
		}
		p = p->b.next;
	}

#ifdef DEBUG_JMALLOC
	fprintf(stderr, "[DEBUG] p->b.is_free : %b\n", p->b.is_free);
	fprintf(stderr, "[DEBUG] p->b.size    : %d\n", (int)p->b.size);
	fprintf(stderr, "[DEBUG] p->b.next     : %p\n", p->b.next);
#endif

	return (void *)(++p);
}

void jfree(void *__attribute__((nullable)) p)
{
	header *prev = NULL;
	header *dead = NULL;
	header *next = NULL;
	if (p)
		dead = (header *)p - 1;
	else
		return;

	/* find prev */
	if (dead->b.prev)
		prev = dead->b.prev;

	/* find next */
	if (dead->b.next)
		next = dead->b.next;

	/* there is a free block after dead */
	if (next && next->b.is_free) {
		dead->b.size += next->b.size;
		dead->b.next = next->b.next;
		if (dead->b.next)
			dead->b.next->b.prev = dead;
		next = NULL;
	}

	/* there is a free block before dead */
	if (prev && prev->b.is_free) {
		prev->b.size += dead->b.size;
		prev->b.next = dead->b.next;
		if (prev->b.next)
			prev->b.next->b.prev = prev;
		dead = prev;
		prev = NULL;
	}

	dead->b.is_free = true;
	return;
}
