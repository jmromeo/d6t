/**
 * @file i2c-reg.c
 *
 * @brief Contains functions to access i2c registers that require
 *        write followed by repeated start reads.
 */

/*
 This software uses a BSD license.

 Copyright (c) 2010, Sean Cross / chumby industries
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.
 * Neither the name of Sean Cross / chumby industries nor the names
   of its contributors may be used to endorse or promote products
   derived from this software without specific prior written
   permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

 */

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "i2c-reg.h"


/**
 * @brief Reads register of i2c device using a write with register address
 *        as data followed by repeated start reads.
 *
 * @param fd        File descriptor of opened i2c file.
 * @param addr      Address of i2c device to perform register read on.
 * @param reg       Register address to read from i2c device.
 * @param rdbuf     Buffer to store read data. Should be at least size
 *                  of rdbytes.
 * @param rdbytes   Number of bytes to read.
 *
 *
 * Example usage:
 * @code
 *
 * #define D6T_ADDR 0x0A
 * #define D6T_CMD  0x4C
 *
 * int fd_i2c;
 * uint8_t buf;
 *
 * if ((fd_i2c = open("/dev/i2c-1", O_RDWR)) < 0) {
 *     perror("Unable to open i2c control file");
 *     exit(1);
 * }
 *
 * while(1) {
 *     i2c_read_reg(fd_i2c, D6T_ADDR, D6T_CMD, &buf, 1);
 *     printf("0x%02X\n", buf);
 *     sleep(1);
 * }
 *
 * close(fd_i2c);
 *
 * @endcode
 */
int i2c_read_reg(int fd, uint8_t addr, uint8_t reg, uint8_t *rdbuf, uint8_t rdbytes)
{
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    // writing "reg" in data portion of i2c packet, representing the address
    // of the register we would like to read from
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = 1;
    messages[0].buf   = &reg;

    // reading rdbytes bytes into rdbuf
    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len   = rdbytes;
    messages[1].buf   = rdbuf;

    // sending packet with write followed by read with repeated start to kernel
    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return -1;
    }

    return 0;
}


/**
 * @brief Writes data in wrbuf to i2c device specified by address.
 *
 * @param fd        File descriptor of opened i2c file.
 * @param addr      Address of i2c device to perform write on.
 * @param wrbuf     Buffer with data to be written. Should be at least size
 *                  of wrbytes.
 * @param wrbytes   Number of bytes to write.
 */
int i2c_write(int fd, uint8_t addr, uint8_t *wrbuf, uint8_t wrbytes)
{
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages;

    // writing "reg" in data portion of i2c packet, representing the address
    // of the register we would like to read from
    messages.addr  = addr;
    messages.flags = 0;
    messages.len   = wrbytes;
    messages.buf   = wrbuf;

    // sending i2c write command
    packets.msgs      = &messages;
    packets.nmsgs     = 1;
    if(ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return -1;
    }

    return 0;
}



