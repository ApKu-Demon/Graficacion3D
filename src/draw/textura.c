#include "textura.h"

void swapUV(TexturaUV *tuv1, TexturaUV *tuv2)
{
    TexturaUV temp = *tuv1; 
    
    *tuv1 = *tuv2; 
    *tuv2 = temp;
}


