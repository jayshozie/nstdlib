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

#include "jmm.h"
#include "jstring.h"
#include <sys/mman.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#if defined(DEBUG_JMALLOC) || defined(DEBUG_JFREE) || defined(DEBUG_JREALLOC)
#include <stdio.h>
#endif

/* idempotent */
#if !defined(JMM_ALIGN)
#define JMM_ALIGN(x) (x + (sizeof(header) - 1)) & ~(sizeof(header) - 1)
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
			p->size = BLOCK_LIT;
		else
			p = NULL;
	} else if (BLOCK_LIT < size && size <= BLOCK_MID) {
		if ((p = sbrk(BLOCK_MID)) != (void *)-1)
			p->size = BLOCK_MID;
		else
			p = NULL;
	} else if (BLOCK_MID < size && size <= BLOCK_BIG) {
		if ((p = sbrk(BLOCK_BIG)) != (void *)-1)
			p->size = BLOCK_BIG;
		else
			p = NULL;
	} else {
		aligned = (size / sizeof(header) + 1) * sizeof(header);
		if ((p = sbrk(aligned)) != (void *)-1)
			p->size = aligned;
		else
			p = NULL;
	}
	if (p) {
		p->is_free = true;
		p->prev = NULL;
		p->next = NULL;
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
	total = JMM_ALIGN(total);

	if (total < size) {
#ifdef DEBUG_JMALLOC
		fprintf(stderr, "[DEBUG] integer overflow. size : %d\n",
			(int)size);
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
		fprintf(stderr, "[DEBUG] p->size : %d\n", (int)p->size);
#endif
		if (p->is_free && p->size >= total) {
			/* claim the block. there is enough space */
			if (p->size - total >= sizeof(header)) { /* trim */
				/* there is enough space for a second block */
				n = (header *)((void *)p + total);
				n->is_free = true;
				n->size = p->size - total;

				p->is_free = false;
				p->size = total;

				n->prev = p;
				n->next = p->next;
				p->next = n;
				if (n->next)
					n->next->prev = n;
			} else {
				/*
				 * either there isn't enough space for a second
				 * block, or we have the perfect case
				 * (size == total)
				 */
				p->is_free = false;
			}
			break;
		} else if (p->next == NULL) {
			/* we don't have a free block */
			n = upbrk(total);
			p->next = n;
			n->prev = p;
			p = n;
			p->is_free = false;
			break;
		}
		p = p->next;
	}

#ifdef DEBUG_JMALLOC
	fprintf(stderr, "[DEBUG] p->is_free : %b\n", p->is_free);
	fprintf(stderr, "[DEBUG] p->size    : %d\n", (int)p->size);
	fprintf(stderr, "[DEBUG] p->next    : %p\n", p->next);
#endif

	return (void *)(++p);
}

void jfree(void *__jnullable p)
{
	header *prev = NULL;
	header *dead = NULL;
	header *next = NULL;
	if (p)
		dead = (header *)p - 1;
	else
		return;

	/* find prev */
	if (dead->prev)
		prev = dead->prev;

	/* find next */
	if (dead->next)
		next = dead->next;

	/* there is a free block after dead */
	if (next && next->is_free) {
		dead->size += next->size;
		dead->next = next->next;
		if (dead->next)
			dead->next->prev = dead;
		next = NULL;
	}

	/* there is a free block before dead */
	if (prev && prev->is_free) {
		prev->size += dead->size;
		prev->next = dead->next;
		if (prev->next)
			prev->next->prev = prev;
		dead = prev;
		prev = NULL;
	}

	dead->is_free = true;
	return;
}

void *jrealloc(void *__jnullable p, size_t size)
{
	size_t left = 0;
	size_t need = 0;
	header *curr = NULL;
	header *next = NULL;

	if (!p)
		return jmalloc(size);
	if (size == 0) {
		jfree(p);
		return NULL;
	}

	curr = (header *)p - 1;
	need = sizeof(header) + size;
	need = JMM_ALIGN(need);
	if (need <= curr->size) {
		/*
                 *        P A T H   1 :   S H R I N K
                 */
		left = curr->size - need;
		left = JMM_ALIGN(left);
		if (left >= sizeof(header)) {
			/* new block */
			next = (header *)((void *)curr + need);
			next->is_free = true;
			curr->size = need;
			next->size = left;
			next->prev = curr;
			next->next = curr->next;
			curr->next = next;
			if (next->next)
				next->next->prev = next;
			return p;
		} else {
			/* no new block, accept fragmentation */
			return p;
		}
	} else if (curr->next && curr->next->is_free &&
		   curr->size + curr->next->size >= need) {
		/*
                 *        P A T H   2 :   E X P A N D
                 */
		left = (curr->size + curr->next->size) - need;
		if (left >= sizeof(header)) {
			/* new block */
			next = (header *)((void *)curr + need);
			next->is_free = true;
			curr->size = need;
			next->size = left;
			next->prev = curr;
			next->next = curr->next->next;
			curr->next = next;
			if (next->next)
				next->next->prev = next;
			return p;
		} else {
			/* no new block, accept fragmentation */
			curr->size += curr->next->size;
			curr->next = curr->next->next;
			if (curr->next)
				curr->next->prev = curr;
			return p;
		}
	} else {
		/*
                 *        P A T H   3 :   R E L O C A T E
                 */
		if ((next = jmalloc(size)) == NULL) {
			/* jmalloc failed, return NULL */
			return NULL;
		} else {
			/* copy old data */
			jmemcpy(next, p, curr->size - sizeof(header));
			jfree(p);
			return next;
		}
	}
}
