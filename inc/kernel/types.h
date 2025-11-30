// kernel/types.h
#ifndef __fulmonyx_16_types_h__
#define __fulmonyx_16_types_h__ 1

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef uint32_t size_t;
typedef int32_t ssize_t;
typedef uint16_t uintptr_t;  // 16位指针类型

#define NULL ((void*)0)

#endif