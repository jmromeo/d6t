#ifndef __D6T_H__
#define __D6T_H__

#define D6T_ADDR    0x0A
#define D6T_RD_CMD  0x4C

#include <stdint.h>

typedef enum 
{
    D6T_44L_06,
    D6T_8L_06,
    D6T_1A_01,
    D6T_1A_02,
    D6T_8L_09
} sensor_t;


typedef struct
{
    int       fd;
    sensor_t  sensor;
    uint8_t   *rdbuf;
    uint8_t   bufsize;
} d6t_devh_t;


int d6t_open(d6t_devh_t *d6t, sensor_t sensor);
void d6t_close(d6t_devh_t *d6t);
void d6t_read(d6t_devh_t *d6t);

#endif // __D6T_H__
