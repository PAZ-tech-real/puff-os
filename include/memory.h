/*
 * Memory Management Header
 * Virtual memory, paging, and heap allocation
 */

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "types.h"

/* Memory management structure */
typedef struct {
    u64 total;      /* Total physical memory */
    u64 available;  /* Available memory */
    u64 used;       /* Used memory */
} memory_info_t;

/**
 * memory_init - Initialize memory management
 */
void memory_init(void);

/**
 * memory_allocator_init - Initialize heap allocator
 */
void memory_allocator_init(void);

/**
 * malloc - Allocate memory from heap
 * @size: Number of bytes to allocate
 * Returns: Pointer to allocated memory or NULL
 */
void* malloc(usize size);

/**
 * free - Free allocated memory
 * @ptr: Pointer to memory to free
 */
void free(void *ptr);

/**
 * get_memory_info - Get memory statistics
 * Returns: Memory information structure
 */
memory_info_t get_memory_info(void);

/**
 * page_alloc - Allocate physical pages
 * @num_pages: Number of pages to allocate
 * Returns: Physical address of first page
 */
u64 page_alloc(u32 num_pages);

/**
 * page_free - Free physical pages
 * @phys_addr: Physical address of first page
 * @num_pages: Number of pages to free
 */
void page_free(u64 phys_addr, u32 num_pages);

#endif /* __MEMORY_H__ */
