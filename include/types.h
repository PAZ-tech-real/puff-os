/*
 * Basic type definitions for Raspberry Pi 5 OS
 */

#ifndef __TYPES_H__
#define __TYPES_H__

/* Integer types */
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;

/* Pointer and size types */
typedef unsigned long       usize;
typedef signed long         ssize;
typedef unsigned long       uintptr_t;
typedef unsigned long long  uint64_t;

/* Boolean */
typedef int bool;
#define true  1
#define false 0

/* NULL pointer */
#define NULL ((void *)0)

/* Architecture-specific */
#define PAGE_SIZE       4096
#define CACHE_LINE_SIZE 64

#endif /* __TYPES_H__ */
