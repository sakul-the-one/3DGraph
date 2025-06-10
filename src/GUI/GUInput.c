#include "GUInput.h"
//#include <ti/getcsc.h>
#include <ti/getkey.h>
#include <fileioc.h>
#include <ti/tokens.h>
#include <ti/screen.h>

equ_t * equ;

float  startInputFloat(Vector2 position, char * Prompt) 
{
    gfx_End();
    size_t buffersize = 15*sizeof(OS_TOK_RIGHT);
    equ = ti_MallocEqu(buffersize);
    //os_GetKey();  
    os_GetTokenInput(Prompt,&equ->data,buffersize);
    float result;
    os_Eval(equ->data,equ->len);
    real_t temp;
    os_GetRealVar(OS_VAR_ANS, &temp);
    result = os_RealToFloat(&temp);
    gfx_Begin();
    return result;
}