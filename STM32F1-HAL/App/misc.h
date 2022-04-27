#ifndef __MISC_H__
#define __MISC_H__
#include <stdio.h>

#ifndef SIZE_ARR
#define SIZE_ARR(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

#ifndef UNUSED
#define UNUSED(X) (void)X
#endif

#define ENUM_TO_STR(e) (#e)

#endif
