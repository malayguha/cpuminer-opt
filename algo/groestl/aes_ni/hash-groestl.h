/* hash.h     Aug 2011
 *
 * Groestl implementation for different versions.
 * Author: Krystian Matusiewicz, Günther A. Roland, Martin Schläffer
 *
 * This code is placed in the public domain
 */

#ifndef __hash_h
#define __hash_h

#include <stdio.h>
#if defined(_WIN64) || defined(__WINDOWS__)
#include <windows.h>
#endif
#include <stdlib.h>

#define LENGTH (512)

#include "brg_endian.h"
#define NEED_UINT_64T
#include "brg_types.h"

/* some sizes (number of bytes) */
#define ROWS (8)
#define LENGTHFIELDLEN (ROWS)
#define COLS512 (8)
#define COLS1024 (16)
#define SIZE512 ((ROWS)*(COLS512))
#define SIZE1024 ((ROWS)*(COLS1024))
#define ROUNDS512 (10)
#define ROUNDS1024 (14)

#if LENGTH<=256
#define COLS (COLS512)
#define SIZE (SIZE512)
#define ROUNDS (ROUNDS512)
#else
#define COLS (COLS1024)
#define SIZE (SIZE1024)
#define ROUNDS (ROUNDS1024)
#endif

#define ROTL64(a,n) ((((a)<<(n))|((a)>>(64-(n))))&li_64(ffffffffffffffff))

#if (PLATFORM_BYTE_ORDER == IS_BIG_ENDIAN)
#define EXT_BYTE(var,n) ((u8)((u64)(var) >> (8*(7-(n)))))
#define U64BIG(a) (a)
#endif /* IS_BIG_ENDIAN */

#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#define EXT_BYTE(var,n) ((u8)((u64)(var) >> (8*n)))
#define U64BIG(a) \
  ((ROTL64(a, 8) & li_64(000000FF000000FF)) | \
   (ROTL64(a,24) & li_64(0000FF000000FF00)) | \
   (ROTL64(a,40) & li_64(00FF000000FF0000)) | \
   (ROTL64(a,56) & li_64(FF000000FF000000)))
#endif /* IS_LITTLE_ENDIAN */

typedef unsigned char BitSequence_gr;
typedef unsigned long long DataLength_gr;
typedef enum { SUCCESS_GR = 0, FAIL_GR = 1, BAD_HASHBITLEN_GR = 2} HashReturn_gr;

// Use area128 overlay for buffer to facilitate fast copying

typedef struct {
  __attribute__ ((aligned (32))) u64 chaining[SIZE/8];        // actual state
  __attribute__ ((aligned (32))) BitSequence_gr buffer[SIZE]; // data buffer
  u64 block_counter;        /* message block counter */
  int hashlen;              // bytes
  int buf_ptr;              /* data buffer pointer */
} hashState_groestl;

//HashReturn_gr init_groestl( hashState_groestl* );

HashReturn_gr init_groestl( hashState_groestl*, int );

HashReturn_gr reinit_groestl( hashState_groestl* );

HashReturn_gr update_groestl( hashState_groestl*, const BitSequence_gr*,
                              DataLength_gr );

HashReturn_gr final_groestl( hashState_groestl*, BitSequence_gr* );

HashReturn_gr hash_groestl( int, const BitSequence_gr*, DataLength_gr,
                            BitSequence_gr* );

HashReturn_gr update_and_final_groestl( hashState_groestl*,
                       BitSequence_gr*, const BitSequence_gr*, DataLength_gr );

#endif /* __hash_h */
