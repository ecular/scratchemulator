#include <sys/time.h>
#include <stdio.h>
void main()
{
    double irq0 = 65536.0 * 1000000 / 1193181.6;
    printf("%f\n", irq0);
}
