#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "d6t.h"


void printBuf(uint8_t *buf)
{
    int i = 0;
    float temp;

    temp = ((256*buf[1]) + buf[0]) / 10.0;

    printf("\n\nTemperature: %f Degrees Celsius\n", temp);

    for (i = 2; i < 34; i = i + 2)
    {
        if (((i - 2) % 8) == 0)
        {
            printf("\n");
        }
        temp = (256 * buf[i+1] + buf[i]) / 10.0;
        printf("%3.1f ", temp);
    }
}

int main()
{
    d6t_devh_t d6t;

    printf("Opening device\n");
    if (d6t_open(&d6t, D6T_44L_06, "/dev/i2c-1") < 0)
    {
        exit(-1);
    }


    while(1)
    {
        d6t_read(&d6t);
        printBuf(d6t.rdbuf);
        usleep(250000);
    }

    return 0;
}
