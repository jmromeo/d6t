/**
 * @file d6t.c
 *
 * @brief Contains functions to access Omron D6T thermal imaging sensors.
 */


#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <dirent.h>
#include <string.h>

#include "d6t.h"
#include "i2c-reg.h"


#define D6T_8L_09_SETUP_MSG_SIZE 4
#define D6T_8L_09_SETUP_NUM_MSGS 5
static uint8_t d6t_8l_09_setupbuf[D6T_8L_09_SETUP_NUM_MSGS][D6T_8L_09_SETUP_MSG_SIZE] =
{
    {0x02, 0x00, 0x01, 0xEE},
    {0x05, 0x90, 0x3A, 0xB8},
    {0x03, 0x00, 0x03, 0x8B},
    {0x03, 0x00, 0x07, 0x97},
    {0x02, 0x00, 0x00, 0xE9}
};


static int i2c_open(d6t_devh_t *d6t)
{
    DIR     *dirp;
    struct  dirent *dp;
    char    i2c_devname[] = "i2c-";
    char    devpath[10];

    // opening /dev directory so we can get a list of i2c device files
    dirp = opendir("/dev/");
    if (dirp == NULL) {
        return -1;
    }

    // looping through files in /dev directory and checking if they are
    // an i2c device. if we find an i2c device we will attempt a read to
    // see if a d6t device exists on the bus, otherwise we will continue
    // checking
    while ((dp = readdir(dirp)) != NULL)
    {
        if (strncmp(dp->d_name, i2c_devname, strlen(i2c_devname)) == 0)
        {
            strcpy(devpath, "/dev/");
            strcat(devpath, dp->d_name);
            if ((d6t->fd = open(devpath, O_RDWR)) >= 0)
            {
                if (d6t_read(d6t) != -1)
                {
                    closedir(dirp);
                    return 0;
                }
            }
        }
    }

    closedir(dirp);
    return -1;
}

/**
 * @brief Opens I2C bus with D6T device and fills in D6T device handle.
 *
 * @param d6t           D6T device handle, containing buffer and file
 *                      descriptor needed to use d6t read function.
 * @param sensor        Sensor type. See sensor_t enum for more details.
 * @param i2c_devname   I2C device file name. If NULL, will search all I2C
 *                      adapters for D6T device.
 *
 * Example usage:
 * @code
 *
 * d6t_devh_t d6t;
 *
 * if (d6t_open(&d6t, D6T_44L_06, NULL) < 0)
 * {
 *     exit(-1);
 * }
 *
 * while(1)
 * {
 *     d6t_read(&d6t);
 *     printBuf(d6t.rdbuf);
 *     usleep(250000);
 * }
 *
 * d6t_close(d6t);
 *
 * @endcode
 */
int d6t_open(d6t_devh_t *d6t, sensor_t sensor, char *i2c_devname)
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
            d6t->bufsize = 19;
            d6t->setupbuf = &d6t_8l_09_setupbuf[0][0];
            d6t->setup_nummsgs = D6T_8L_09_SETUP_NUM_MSGS;
            d6t->setup_msgsize = D6T_8L_09_SETUP_MSG_SIZE;
            break;

        default:
            printf("Device not supported\n");
            return -1;
    }
    d6t->rdbuf = malloc(d6t->bufsize);

    // if an i2c device name is provided, we will just attempt to open that
    // device, otherwiese we will check all i2c-dev<num> devices for a d6t
    // device
    if (i2c_devname == NULL)
    {
        if (i2c_open(d6t) == -1)
        {
            printf("Unable to find d6t device on any i2c bus\n");
            d6t_close(d6t);
            return -1;
        }
    }
    else
    {
        if ((d6t->fd = open(i2c_devname, O_RDWR)) < 0)
        {
            perror("Unable to open i2c control file");
            d6t_close(d6t);
            return -1;
        }

        // If we're able to successfully open the i2c bus, let's check bus
        // to see if a d6t sensor is present. We will do this by sending a
        // read command to the sensor and see if we get an ack.
        if (d6t_read(d6t) == -1)
        {
            printf("No d6t device on bus\n");
            d6t_close(d6t);
            return -1;
        }
    }

    return 0;
}


/**
 * @brief Closes I2C device handle and cleans up D6T read buffer.
 *
 * @param d6t   D6T device handle, containing buffer and file descriptor
 *              needed to use d6t read function.
 *
 * Example usage:
 * @code
 *
 * d6t_devh_t d6t;
 *
 * if (d6t_open(&d6t, D6T_44L_06, NULL) < 0)
 * {
 *     exit(-1);
 * }
 *
 * while(1)
 * {
 *     d6t_read(&d6t);
 *     printBuf(d6t.rdbuf);
 *     usleep(250000);
 * }
 *
 * d6t_close(d6t);
 *
 * @endcode
 */
void d6t_close(d6t_devh_t *d6t)
{
    if (d6t->rdbuf != NULL) {
        free(d6t->rdbuf);
    }
    if (d6t->fd > 0) {
        close(d6t->fd);
    }
}

/**
 * @brief Reads from D6T device and fills in d6t read buffer.
 *
 * @param d6t   D6T device handle, containing buffer and file descriptor
 *              needed to use d6t read function.
 *
 * Example usage:
 * @code
 *
 * d6t_devh_t d6t;
 *
 * if (d6t_open(&d6t, D6T_44L_06, NULL) < 0)
 * {
 *     exit(-1);
 * }
 *
 * while(1)
 * {
 *     d6t_read(&d6t);
 *     printBuf(d6t.rdbuf);
 *     usleep(250000);
 * }
 *
 * d6t_close(d6t);
 *
 * @endcode
 */
int d6t_read(d6t_devh_t *d6t)
{
    // performing special pre-setup before read for some sensors
    switch (d6t->sensor)
    {
        // special setup for D6T_8L_09
        case D6T_8L_09:
            for (int i = 0; i < d6t->setup_nummsgs; i++)
            {
                i2c_write(d6t->fd,
                          D6T_ADDR,
                          &d6t->setupbuf[i*d6t->setup_msgsize],
                          d6t->setup_msgsize);
            }
            break;

        default:
            break;
    }

    // reading and returning sensor data
    return i2c_read_reg(d6t->fd, D6T_ADDR, D6T_RD_CMD, d6t->rdbuf, d6t->bufsize);
}



