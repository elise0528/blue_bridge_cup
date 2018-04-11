#include <stdio.h>

int main(int argc, char const *argv[])
{
    int a = 10;
    int *p
    p = &a;
    printf("%d", *p);
    return 0;
}