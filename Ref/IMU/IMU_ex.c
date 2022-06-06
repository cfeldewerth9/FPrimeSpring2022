/*
 * This file is an MPU6050 demonstration.
 * https://openest.io/en/2020/01/21/mpu6050-accelerometer-on-raspberry-pi/
 * Copyright (c) 2020 Julien Grossholtz - https://openest.io.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This updated version of the source code is for a demo with the LSM6DSOX
 * sensor for basic acceleration and gyroscope data processing. 
 */

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

#include "LSM6DSOX.h"

#define LSM6DSOX_I2C_ADDR 0x6A

int file = -1;

volatile sig_atomic_t stop;

void inthand(int signum) {
    stop = 1;
}

/*
 * This function taken from source code
 */
void i2c_write(__u8 reg_address, __u8 val) {
	char buf[2];
	if(file < 0) {
		printf("Error, i2c bus is not available\n");
		exit(1);
	}

	buf[0] = reg_address;
	buf[1] = val;

	if (write(file, buf, 2) != 2) {
		printf("Error, unable to write to i2c device\n");
		exit(1);
	}

}

/*
 * This function taken from source code
 */
char i2c_read(uint8_t reg_address) {
	char buf[1];
	if(file < 0) {
		printf("Error, i2c bus is not available\n");
		exit(1);
	}

	buf[0] = reg_address;

	if (write(file, buf, 1) != 1) {
		printf("Error, unable to write to i2c device\n");
		exit(1);
	}


	if (read(file, buf, 1) != 1) {
		printf("Error, unable to read from i2c device\n");
		exit(1);
	}

	return buf[0];

}

/*
 * This function taken from source code
 */
uint16_t merge_bytes( uint8_t LSB, uint8_t MSB) {
	return  (uint16_t) ((( LSB & 0xFF) << 8) | MSB);
}

/*
 * This function taken from source code
 */
int16_t two_complement_to_int( uint8_t LSB, uint8_t MSB) {
	int16_t signed_int = 0;
	uint16_t word;

	word = merge_bytes(LSB, MSB);

	if((word & 0x8000) == 0x8000) { // negative number
		signed_int = (int16_t) -(~word);
	} else {
		signed_int = (int16_t) (word & 0x7fff);
	}

	return signed_int;
}

int main(int argc, char *argv[]) {
	
    signal(SIGINT, inthand);

    int adapter_nr = 1; /* probably dynamically determined */
	char bus_filename[250];
    int data;

	char accel_x_h, accel_x_l;
    char accel_y_h, accel_y_l;
    char accel_z_h, accel_z_l;
    char temp_h, temp_l;

	// uint16_t fifo_len = 0;
	int16_t x_accel = 0, y_accel = 0, z_accel = 0, temp = 0;
	float x_accel_g, y_accel_g, z_accel_g, temp_f;

	snprintf(bus_filename, 250, "/dev/i2c-1", adapter_nr);

	file = open(bus_filename, O_RDWR);
	if (file < 0) {
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}

	if (ioctl(file, I2C_SLAVE, LSM6DSOX_I2C_ADDR) < 0) {
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}

    /*
     * To turn on the accelerometer and gather acceleration data through 
     * the primary I²C / MIPI I3CSM / SPI interface, it is necessary to 
     * select one of the operating modes through the CTRL1_XL register.
     * The following general-purpose sequence can be used to configure the accelerometer:
     */
    i2c_write(INT1_CTRL, 0x02);
    i2c_write(CTRL1_XL, 0x60);

    /* 
     * To turn on the gyroscope and gather angular rate data through the 
     * primary I²C / MIPI I3CSM / SPI interface, it is necessary to select 
     * one of the operating modes through CTRL2_G.
     * The following general-purpose sequence can be used to configure the gyroscope:
     */
    i2c_write(INT1_CTRL, 0x02);
    i2c_write(CTRL2_G, 0x60);

    /* 
     * It is strongly recommended to set the BDU (Block Data Update) bit to 1 
     * in the CTRL3_C register. This feature avoids reading values (most 
     * significant and least significant parts of output data) related to different
     * samples. In particular, when the BDU is activated, the data registers 
     * related to each channel always contain the most recent output data produced 
     * by the device.
     */
    i2c_write(CTRL3_C, 0x40);

    stop = 0;

    while (!stop) {
        /*
         * The device is provided with a STATUS_REG register which should be 
         * polled to check when a new set of data is available. The XLDA bit is 
         * set to 1 when a new set of data is available at the accelerometer output; 
         * the GDA bit is set to 1 when a new set of data is available at the 
         * gyroscope output.
         * For the accelerometer (the gyroscope is similar), the read of the output 
         * registers should be performed as follows:
         */
        data = i2c_read(STATUS_REG);

        if (data != 0) {
            accel_x_l = i2c_read(OUTX_L_A);
            accel_x_h = i2c_read(OUTX_H_A);
            accel_y_l = i2c_read(OUTY_L_A);
            accel_y_h = i2c_read(OUTY_H_A);
            accel_z_l = i2c_read(OUTZ_L_A);
            accel_z_h = i2c_read(OUTZ_H_A);
            temp_l = i2c_read(OUT_TEMP_L);
            temp_h = i2c_read(OUT_TEMP_H);

            x_accel= two_complement_to_int(accel_x_h,accel_x_l);
			x_accel_g = ((float) x_accel)/16384;

			y_accel= two_complement_to_int(accel_y_h,accel_y_l);
			y_accel_g = ((float) y_accel)/16384;

			z_accel= two_complement_to_int(accel_z_h,accel_z_l);
			z_accel_g = ((float) z_accel)/16384;

			temp = two_complement_to_int(temp_h, temp_l);
			temp_f = (float)temp/340 + 36.53;

            printf("x_accel %.3fg	y_accel %.3fg	z_accel %.3fg	temp=%.1fc         \r", 
                x_accel_g, y_accel_g, z_accel_g, temp_f);
        }

    }
  
    printf("System exiting...");
    system("pause");

    return 0;
}

    /*
     * Left overs from source code
     */
	// i2c_write(REG_PWR_MGMT_1, 0x01);
	// i2c_write(REG_ACCEL_CONFIG, 0x00);
	// i2c_write(REG_SMPRT_DIV, 0x07);
	// i2c_write(REG_CONFIG, 0x00);
	// i2c_write(REG_FIFO_EN, 0x88);
	// i2c_write(REG_USER_CTRL, 0x44);

	// while(fifo_len != 1024) {
	// 	accel_x_h = i2c_read(REG_FIFO_COUNT_L);
	// 	accel_x_l = i2c_read(REG_FIFO_COUNT_H);
	// 	fifo_len = merge_bytes(accel_x_h,accel_x_l);

	// 	if(fifo_len == 1024) {
	// 		printf("fifo overflow !\n");
	// 		i2c_write(REG_USER_CTRL, 0x44);
	// 		continue;
	// 	}

	// 	if(fifo_len >= 8) {
	// 		accel_x_h = i2c_read(REG_FIFO);
	// 		accel_x_l = i2c_read(REG_FIFO);
	// 		accel_y_h = i2c_read(REG_FIFO);
	// 		accel_y_l = i2c_read(REG_FIFO);
	// 		accel_z_h = i2c_read(REG_FIFO);
	// 		accel_z_l = i2c_read(REG_FIFO);
	// 		temp_h = i2c_read(REG_FIFO);
	// 		temp_l= i2c_read(REG_FIFO);

	// 		x_accel= two_complement_to_int(accel_x_h,accel_x_l);
	// 		x_accel_g = ((float) x_accel)/16384;

	// 		y_accel= two_complement_to_int(accel_y_h,accel_y_l);
	// 		y_accel_g = ((float) y_accel)/16384;

	// 		z_accel= two_complement_to_int(accel_z_h,accel_z_l);
	// 		z_accel_g = ((float) z_accel)/16384;

	// 		temp = two_complement_to_int(temp_h, temp_l);
	// 		temp_f = (float)temp/340 + 36.53; // calculated as described in the MPU60%) register map document

	// 		printf("x_accel %.3fg	y_accel %.3fg	z_accel %.3fg	temp=%.1fc         \r", x_accel_g, y_accel_g, z_accel_g, temp_f);
	// 	} else {
	// 		usleep(10000);
	// 	}

	// }

	// return 0;
