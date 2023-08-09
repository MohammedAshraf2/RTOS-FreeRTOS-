#ifndef STDTYPES_H_
#define STDTYPES_H_

#define F_CPU	(16000000UL)
#include <util/delay.h>

typedef unsigned char	u8;
typedef signed   char	s8;
typedef unsigned int	u16;
typedef signed   int	s16;
typedef unsigned long	u32;
typedef signed   long	s32;
typedef signed long long s64;

typedef float			f32;

/*@brief: return type for any function in project, contain three constants
		OK if function didn't face errors, NOK if three was error and NULL if function got
		Null pointer in its arguments.
*/


typedef enum
{
	TRUE,
	FASLE	
}bool_type;

#define ZERO_INIT	((u8)0)
#define NULL_PTR	((void*)0)
#define MAX_U32		((u32)4294967295)
#define MAX_S32		((s32)2147483647)
#define MAX_U16		((u16)65535)
#define MAX_S16		((s16)32767)
#define MAX_u8		((u8)255)
#define MAX_S8		((s8)127)
#define MIN_S32		((s32)-2147483648)
#define MIN_S16		((s16)-32768)
#define MIN_S8		((s8)-128)

#endif /* STDTYPES_H_ */