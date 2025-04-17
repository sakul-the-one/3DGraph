#include "GUInput.h"
//#include <ti/getcsc.h>
#include <ti/getkey.h>
#include <fileioc.h>
#include <ti/tokens.h>

equ_t * equ;

float  startInputFloat(Vector2 position) 
{
    equ = ti_MallocEqu(15*sizeof(OS_TOK_RIGHT));
    os_GetKey();
    return 5;
}