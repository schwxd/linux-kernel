#include <stdio.h>
 

int buf[5] = {3};

int main()
{
    short int x=0x1122;
    char *p = &x;
    unsigned int tmp;
    tmp = 0xffffffffff;
    printf("tmp=%u\n", tmp);
 
    if ( *p == 0x11)
        printf("big-endian\n");
    else
        printf("little-endian\n");

    return 0;
}
