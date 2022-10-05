#ifndef ELLIE_COMMON_H
#define ELLIE_COMMON_H

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t bool32;
typedef float f32;
typedef double f64;
typedef i8 b8;

#define internal static
#define global static
#define local_persist static
#define false 0
#define true 1

#ifdef _WIN32
#define ELLIE_WINDOWS
#include <Windows.h>
#elif defined(__linux__)
#define ELLIE_LINUX
#endif

#endif
