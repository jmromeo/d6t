#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "d6t.h"
#include "i2c-reg.h"


int d6t_open(d6t_devh_t *d6t, sensor_t sensor)
{
    d6t->sensor = sensor;
    d6t->rdbuf  = NULL;
    d6t->fd     = -1;

    // Setting up buffer to store sensor data. Size of buffer is
    // determined by the sensor type.
    switch (sensor)
    {
        case D6T_44L_06:
            d6t->bufsize = 35;
            break;

        case D6T_8L_06:
            d6t->bufsize = 19;
            break;

        case D6T_1A_01:
        case D6T_1A_02:
            d6t->bufsize = 5;
            break;

        case D6T_8L_09:
        default:
            printf("Device not supported\n");
            return -1;
    }
    d6t->rdbuf = malloc(d6t->bufsize);


    /// @todo add ability to loop through all i2c-dev files present
    // Attempting to open i2c bus.
    if ((d6t->fd = open("/dev/i2c-1", O_RDWR)) < 0)
    {
        perror("Unable to open i2c control file");
        d6t_close(d6t);
        return -1;
    }

    // If we're able to successfully open the i2c bus, let's check bus
    // to see if a d6t sensor is present. We will do this by sending a
    // read command to the sensor and see if we get an ack.
    if (i2c_read_reg(d6t->fd, D6T_ADDR, D6T_RD_CMD, d6t->rdbuf, d6t->bufsize) == -1)
    {
        printf("No d6t device on bus\n");
        d6t_close(d6t);
        return -1;
    }

    return 0;
}


void d6t_close(d6t_devh_t *d6t)
{
    if (d6t->rdbuf != NULL) {
        free(d6t->rdbuf);
    }
    if (d6t->fd > 0) {
        close(d6t->fd);
    }
}


void d6t_read(d6t_devh_t *d6t)
{
    i2c_read_reg(d6t->fd, D6T_ADDR, D6T_RD_CMD, d6t->rdbuf, d6t->bufsize);
}



