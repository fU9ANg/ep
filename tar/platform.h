#ifndef BB_PLATFORM_H
#define BB_PLATFORM_H 1




#undef inline
#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 199901L
/* it's a keyword */
#endif

#ifndef __const
# define __const const
#endif

#define UNUSED_PARAM __attribute__ ((__unused__))
#define NORETURN __attribute__ ((__noreturn__))
/* "The malloc attribute is used to tell the compiler that a function
 * may be treated as if any non-NULL pointer it returns cannot alias
 * any other pointer valid when the function returns. This will often
 * improve optimization. Standard functions with this property include
 * malloc and calloc. realloc-like functions have this property as long
 * as the old pointer is never referred to (including comparing it
 * to the new pointer) after the function returns a non-NULL value."
 */
#define RETURNS_MALLOC __attribute__ ((malloc))
#define PACKED __attribute__ ((__packed__))
#define ALIGNED(m) __attribute__ ((__aligned__(m)))

# define ALWAYS_INLINE inline
# define NOINLINE
# define DEPRECATED
# define UNUSED_PARAM_RESULT

/* -fwhole-program makes all symbols local. The attribute externally_visible
 * forces a symbol global.  */
# define EXTERNALLY_VISIBLE

/* At 4.4 gcc become much more anal about this, need to use "aliased" types */
# define FIX_ALIASING

/* We use __extension__ in some places to suppress -pedantic warnings
 * about GCC extensions.  This feature didn't work properly before
 * gcc 2.8.  */

/* FAST_FUNC is a qualifier which (possibly) makes function call faster
 * and/or smaller by using modified ABI. It is usually only needed
 * on non-static, busybox internal functions. Recent versions of gcc
 * optimize statics automatically. FAST_FUNC on static is required
 * only if you need to match a function pointer's type */
# define FAST_FUNC

/* Make all declarations hidden (-fvisibility flag only affects definitions) */
/* (don't include system headers after this until corresponding pop!) */
# define PUSH_AND_SET_FUNCTION_VISIBILITY_TO_HIDDEN
# define POP_SAVED_FUNCTION_VISIBILITY

/* gcc-2.95 had no va_copy but only __va_copy. */


/* ---- Endian Detection ------------------------------------ */

#include <limits.h>
#if defined(__digital__) && defined(__unix__)
# include <sex.h>
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) \
   || defined(__APPLE__)
# include <sys/resource.h>  /* rlimit */
# include <machine/endian.h>
# define bswap_64 __bswap64
# define bswap_32 __bswap32
# define bswap_16 __bswap16
#else
# include <byteswap.h>
# include <endian.h>
#endif

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
# define BB_BIG_ENDIAN 1
# define BB_LITTLE_ENDIAN 0
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN
# define BB_BIG_ENDIAN 0
# define BB_LITTLE_ENDIAN 1
#elif defined(_BYTE_ORDER) && _BYTE_ORDER == _BIG_ENDIAN
# define BB_BIG_ENDIAN 1
# define BB_LITTLE_ENDIAN 0
#elif defined(_BYTE_ORDER) && _BYTE_ORDER == _LITTLE_ENDIAN
# define BB_BIG_ENDIAN 0
# define BB_LITTLE_ENDIAN 1
#elif defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN
# define BB_BIG_ENDIAN 1
# define BB_LITTLE_ENDIAN 0
#elif defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN
# define BB_BIG_ENDIAN 0
# define BB_LITTLE_ENDIAN 1
#elif defined(__386__)
# define BB_BIG_ENDIAN 0
# define BB_LITTLE_ENDIAN 1
#else
# error "Can't determine endianness"
#endif

#if ULONG_MAX > 0xffffffff
# define bb_bswap_64(x) bswap_64(x)
#endif

/* SWAP_LEnn means "convert CPU<->little_endian by swapping bytes" */
#if BB_BIG_ENDIAN
# define SWAP_BE16(x) (x)
# define SWAP_BE32(x) (x)
# define SWAP_BE64(x) (x)
# define SWAP_LE16(x) bswap_16(x)
# define SWAP_LE32(x) bswap_32(x)
# define SWAP_LE64(x) bb_bswap_64(x)
# define IF_BIG_ENDIAN(...) __VA_ARGS__
# define IF_LITTLE_ENDIAN(...)
#else
# define SWAP_BE16(x) bswap_16(x)
# define SWAP_BE32(x) bswap_32(x)
# define SWAP_BE64(x) bb_bswap_64(x)
# define SWAP_LE16(x) (x)
# define SWAP_LE32(x) (x)
# define SWAP_LE64(x) (x)
# define IF_BIG_ENDIAN(...)
# define IF_LITTLE_ENDIAN(...) __VA_ARGS__
#endif


/* ---- Unaligned access ------------------------------------ */

#include <stdint.h>
typedef int      bb__aliased_int      FIX_ALIASING;
typedef long     bb__aliased_long     FIX_ALIASING;
typedef uint16_t bb__aliased_uint16_t FIX_ALIASING;
typedef uint32_t bb__aliased_uint32_t FIX_ALIASING;
typedef uint64_t bb__aliased_uint64_t FIX_ALIASING;

/* NB: unaligned parameter should be a pointer, aligned one -
 * a lvalue. This makes it more likely to not swap them by mistake
 */
#if defined(i386) || defined(__x86_64__) || defined(__powerpc__)
# define move_from_unaligned_int(v, intp)  ((v) = *(bb__aliased_int*)(intp))
# define move_from_unaligned_long(v, longp) ((v) = *(bb__aliased_long*)(longp))
# define move_from_unaligned16(v, u16p) ((v) = *(bb__aliased_uint16_t*)(u16p))
# define move_from_unaligned32(v, u32p) ((v) = *(bb__aliased_uint32_t*)(u32p))
# define move_to_unaligned16(u16p, v)   (*(bb__aliased_uint16_t*)(u16p) = (v))
# define move_to_unaligned32(u32p, v)   (*(bb__aliased_uint32_t*)(u32p) = (v))
/* #elif ... - add your favorite arch today! */
#else
/* performs reasonably well (gcc usually inlines memcpy here) */
# define move_from_unaligned_int(v, intp) (memcpy(&(v), (intp), sizeof(int)))
# define move_from_unaligned_long(v, longp) (memcpy(&(v), (longp), sizeof(long)))
# define move_from_unaligned16(v, u16p) (memcpy(&(v), (u16p), 2))
# define move_from_unaligned32(v, u32p) (memcpy(&(v), (u32p), 4))
# define move_to_unaligned16(u16p, v) do { \
	uint16_t __t = (v); \
	memcpy((u16p), &__t, 2); \
} while (0)
# define move_to_unaligned32(u32p, v) do { \
	uint32_t __t = (v); \
	memcpy((u32p), &__t, 4); \
} while (0)
#endif


/* ---- Size-saving "small" ints (arch-dependent) ----------- */

#if defined(i386) || defined(__x86_64__) || defined(__mips__) || defined(__cris__)
/* add other arches which benefit from this... */
typedef signed char smallint;
typedef unsigned char smalluint;
#else
/* for arches where byte accesses generate larger code: */
typedef int smallint;
typedef unsigned smalluint;
#endif

/* ISO C Standard:  7.16  Boolean type and values  <stdbool.h> */
#if (defined __digital__ && defined __unix__)
/* old system without (proper) C99 support */
# define bool smalluint
#else
/* modern system, so use it */
# include <stdbool.h>
#endif


/*----- Kernel versioning ------------------------------------*/

#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#ifdef __UCLIBC__
# define UCLIBC_VERSION KERNEL_VERSION(__UCLIBC_MAJOR__, __UCLIBC_MINOR__, __UCLIBC_SUBLEVEL__)
#else
# define UCLIBC_VERSION 0
#endif


/* ---- Miscellaneous --------------------------------------- */

#if defined __GLIBC__ \
 || defined __UCLIBC__ \
 || defined __dietlibc__ \
 || defined __BIONIC__ \
 || defined _NEWLIB_VERSION
# include <features.h>
#endif

/* Define bb_setpgrp */
#if defined(__digital__) && defined(__unix__)
/* use legacy setpgrp(pid_t, pid_t) for now.  move to platform.c */
# define bb_setpgrp() do { pid_t __me = getpid(); setpgrp(__me, __me); } while (0)
#else
# define bb_setpgrp() setpgrp()
#endif

/* fdprintf is more readable, we used it before dprintf was standardized */
#include <unistd.h>
#define fdprintf dprintf

/* Useful for defeating gcc's alignment of "char message[]"-like data */
#if !defined(__s390__)
    /* on s390[x], non-word-aligned data accesses require larger code */
# define ALIGN1 __attribute__((aligned(1)))
# define ALIGN2 __attribute__((aligned(2)))
# define ALIGN4 __attribute__((aligned(4)))
#else
/* Arches which MUST have 2 or 4 byte alignment for everything are here */
# define ALIGN1
# define ALIGN2
# define ALIGN4
#endif

/*
 * For 0.9.29 and svn, __ARCH_USE_MMU__ indicates no-mmu reliably.
 * For earlier versions there is no reliable way to check if we are building
 * for a mmu-less system.
 */
#if ENABLE_NOMMU || \
    (defined __UCLIBC__ && \
     UCLIBC_VERSION > KERNEL_VERSION(0, 9, 28) && \
     !defined __ARCH_USE_MMU__)
# define BB_MMU 0
# define USE_FOR_NOMMU(...) __VA_ARGS__
# define USE_FOR_MMU(...)
#else
# define BB_MMU 1
# define USE_FOR_NOMMU(...)
# define USE_FOR_MMU(...) __VA_ARGS__
#endif

#if defined(__digital__) && defined(__unix__)
# include <standards.h>
# include <inttypes.h>
# define PRIu32 "u"
# if !defined ADJ_OFFSET_SINGLESHOT && defined MOD_CLKA && defined MOD_OFFSET
#  define ADJ_OFFSET_SINGLESHOT (MOD_CLKA | MOD_OFFSET)
# endif
# if !defined ADJ_FREQUENCY && defined MOD_FREQUENCY
#  define ADJ_FREQUENCY MOD_FREQUENCY
# endif
# if !defined ADJ_TIMECONST && defined MOD_TIMECONST
#  define ADJ_TIMECONST MOD_TIMECONST
# endif
# if !defined ADJ_TICK && defined MOD_CLKB
#  define ADJ_TICK MOD_CLKB
# endif
#endif

#if defined(__CYGWIN__)
# define MAXSYMLINKS SYMLOOP_MAX
#endif

#if defined(ANDROID) || defined(__ANDROID__)
# define BB_ADDITIONAL_PATH ":/system/sbin:/system/bin:/system/xbin"
# define SYS_ioprio_set __NR_ioprio_set
# define SYS_ioprio_get __NR_ioprio_get
#endif


/* ---- Who misses what? ------------------------------------ */

/* Assume all these functions and header files exist by default.
 * Platforms where it is not true will #undef them below.
 */
#define HAVE_CLEARENV 1
#define HAVE_FDATASYNC 1
#define HAVE_DPRINTF 1
#define HAVE_MEMRCHR 1
#define HAVE_MKDTEMP 1
#define HAVE_PTSNAME_R 1
#define HAVE_SETBIT 1
#define HAVE_SIGHANDLER_T 1
#define HAVE_STPCPY 1
#define HAVE_STRCASESTR 1
#define HAVE_STRCHRNUL 1
#define HAVE_STRSEP 1
#define HAVE_STRSIGNAL 1
#define HAVE_STRVERSCMP 1
#define HAVE_VASPRINTF 1
#define HAVE_UNLOCKED_STDIO 1
#define HAVE_UNLOCKED_LINE_OPS 1
#define HAVE_GETLINE 1
#define HAVE_XTABS 1
#define HAVE_MNTENT_H 1
#define HAVE_NET_ETHERNET_H 1
#define HAVE_SYS_STATFS_H 1
#define HAVE_MEMPCPY 1

#if defined(__UCLIBC__) && UCLIBC_VERSION < KERNEL_VERSION(0, 9, 32)
# undef HAVE_STRVERSCMP
#endif

#if defined(__WATCOMC__)
# undef HAVE_DPRINTF
# undef HAVE_GETLINE
# undef HAVE_MEMRCHR
# undef HAVE_MKDTEMP
# undef HAVE_SETBIT
# undef HAVE_STPCPY
# undef HAVE_STRCASESTR
# undef HAVE_STRCHRNUL
# undef HAVE_STRSEP
# undef HAVE_STRSIGNAL
# undef HAVE_STRVERSCMP
# undef HAVE_VASPRINTF
# undef HAVE_UNLOCKED_STDIO
# undef HAVE_UNLOCKED_LINE_OPS
# undef HAVE_NET_ETHERNET_H
#endif

#if defined(__CYGWIN__)
# undef HAVE_CLEARENV
# undef HAVE_FDPRINTF
# undef HAVE_MEMRCHR
# undef HAVE_PTSNAME_R
# undef HAVE_STRVERSCMP
# undef HAVE_UNLOCKED_LINE_OPS
#endif

/* These BSD-derived OSes share many similarities */
#if (defined __digital__ && defined __unix__) \
 || defined __APPLE__ \
 || defined __OpenBSD__ || defined __NetBSD__
# undef HAVE_CLEARENV
# undef HAVE_FDATASYNC
# undef HAVE_GETLINE
# undef HAVE_MNTENT_H
# undef HAVE_PTSNAME_R
# undef HAVE_SYS_STATFS_H
# undef HAVE_SIGHANDLER_T
# undef HAVE_STRVERSCMP
# undef HAVE_XTABS
# undef HAVE_DPRINTF
# undef HAVE_UNLOCKED_STDIO
# undef HAVE_UNLOCKED_LINE_OPS
#endif

#if defined(__dietlibc__)
# undef HAVE_STRCHRNUL
#endif

#if defined(__APPLE__)
# undef HAVE_STRCHRNUL
#endif

#if defined(__FreeBSD__)
# undef HAVE_CLEARENV
# undef HAVE_FDATASYNC
# undef HAVE_MNTENT_H
# undef HAVE_PTSNAME_R
# undef HAVE_SYS_STATFS_H
# undef HAVE_SIGHANDLER_T
# undef HAVE_STRVERSCMP
# undef HAVE_XTABS
# undef HAVE_UNLOCKED_LINE_OPS
# include <osreldate.h>
# if __FreeBSD_version < 1000029
#  undef HAVE_STRCHRNUL /* FreeBSD added strchrnul() between 1000028 and 1000029 */
# endif
#endif

#if defined(__NetBSD__)
# define HAVE_GETLINE 1  /* Recent NetBSD versions have getline() */
#endif

#if defined(__digital__) && defined(__unix__)
# undef HAVE_STPCPY
#endif

#if defined(ANDROID) || defined(__ANDROID__)
# undef HAVE_DPRINTF
# undef HAVE_GETLINE
# undef HAVE_STPCPY
# undef HAVE_STRCHRNUL
# undef HAVE_STRVERSCMP
# undef HAVE_UNLOCKED_LINE_OPS
# undef HAVE_NET_ETHERNET_H
# undef HAVE_MEMPCPY
# undef HAVE_SETBIT
#endif

/*
 * Now, define prototypes for all the functions defined in platform.c
 * These must come after all the HAVE_* macros are defined (or not)
 */

#ifndef HAVE_DPRINTF
extern int dprintf(int fd, const char *format, ...);
#endif

#ifndef HAVE_MEMRCHR
extern void *memrchr(const void *s, int c, size_t n) FAST_FUNC;
#endif

#ifndef HAVE_MKDTEMP
extern char *mkdtemp(char *template) FAST_FUNC;
#endif

#ifndef HAVE_SETBIT
# define setbit(a, b)  ((a)[(b) >> 3] |= 1 << ((b) & 7))
# define clrbit(a, b)  ((a)[(b) >> 3] &= ~(1 << ((b) & 7)))
#endif

#ifndef HAVE_SIGHANDLER_T
typedef void (*sighandler_t)(int);
#endif

#ifndef HAVE_STPCPY
extern char *stpcpy(char *p, const char *to_add) FAST_FUNC;
#endif

#ifndef HAVE_STRCASESTR
extern char *strcasestr(const char *s, const char *pattern) FAST_FUNC;
#endif

#ifndef HAVE_STRCHRNUL
extern char *strchrnul(const char *s, int c) FAST_FUNC;
#endif

#ifndef HAVE_STRSEP
extern char *strsep(char **stringp, const char *delim) FAST_FUNC;
#endif

#ifndef HAVE_STRSIGNAL
/* Not exactly the same: instead of "Stopped" it shows "STOP" etc */
# define strsignal(sig) get_signame(sig)
#endif

#ifndef HAVE_VASPRINTF
extern int vasprintf(char **string_ptr, const char *format, va_list p) FAST_FUNC;
#endif

#ifndef HAVE_GETLINE
# include <stdio.h> /* for FILE */
# include <sys/types.h> /* size_t */
extern ssize_t getline(char **lineptr, size_t *n, FILE *stream) FAST_FUNC;
#endif

#ifndef HAVE_MEMPCPY
# include <sys/types.h> /* size_t */
extern void *mempcpy(void *dest, const void *src, size_t n);
#endif

#endif