/*
 * Author: Caitlin Feldewerth
 * Purpose: To display acceleration, gyroscope, and temperature values on the RPi 4 terminal using the LSM6DSOX sensor.
 * 
 * Source Code: https://gist.github.com/JGrossholtz/24e427b62c2e5dbbec6546c55c8979d8
 * 
 * LSM6DSOX Datasheet: https://www.st.com/resource/en/datasheet/lsm6dsox.pdf
 * LSM6DSOX Application Note: https://www.st.com/resource/en/application_note/an5272-lsm6dsox-alwayson-3d-accelerometer-and-3d-gyroscope-stmicroelectronics.pdf
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

/*
 * This function is used to halt the program based on user input (Ctrl-C to exit).
 */
void inthand(int signum) {
    stop = 1;
}

/*
 * This function taken from source code.
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
 * This function taken from source code.
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
 * This function taken from source code.
 */
uint16_t merge_bytes( uint8_t LSB, uint8_t MSB) {
	return  (uint16_t) ((( LSB & 0xFF) << 8) | MSB);
}

/*
 * This function taken from source code.
 */
int16_t two_complement_to_int( uint8_t LSB, uint8_t MSB) {
	int16_t signed_int = 0;
	uint16_t word;

	word = merge_bytes(LSB, MSB);

	if((word & 0x8000) == 0x8000) { // negative number
		signed_int = (int16_t) - (~word);
	} else {
		signed_int = (int16_t) (word & 0x7fff);
	}

	return signed_int;
}

/*
 * This function adapted from source code. 
 */
int main(int argc, char *argv[]) {
	
    signal(SIGINT, inthand);

    int adapter_nr = 1; /* probably dynamically determined */
	char bus_filename[250];
    int data;

	char accel_x_h, accel_x_l, accel_y_h, accel_y_l, accel_z_h, accel_z_l;
	char gyro_x_h, gyro_x_l, gyro_y_h, gyro_y_l, gyro_z_h, gyro_z_l;
    char temp_h, temp_l;

	int16_t x_accel = 0, y_accel = 0, z_accel = 0, x_gyro = 0, y_gyro = 0, z_gyro = 0, temp = 0;
	float x_accel_g, y_accel_g, z_accel_g, x_gyro_g, y_gyro_g, z_gyro_g, temp_f;

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

	/*
	 * The system will continously poll the sensor for data until user exits by 
	 * pressing Ctrl-C on their keyboard.
	 */
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
			// --------------------------------
			// Reading the registers:
			// --------------------------------
            accel_x_l = i2c_read(OUTX_L_A);
            accel_x_h = i2c_read(OUTX_H_A);
            accel_y_l = i2c_read(OUTY_L_A);
            accel_y_h = i2c_read(OUTY_H_A);
            accel_z_l = i2c_read(OUTZ_L_A);
            accel_z_h = i2c_read(OUTZ_H_A);
		
			gyro_x_l = i2c_read(OUTX_L_G);
            gyro_x_h = i2c_read(OUTX_H_G);
            gyro_y_l = i2c_read(OUTY_L_G);
            gyro_y_h = i2c_read(OUTY_H_G);
            gyro_z_l = i2c_read(OUTZ_L_G);
            gyro_z_h = i2c_read(OUTZ_H_G);
			
            temp_l = i2c_read(OUT_TEMP_L);
            temp_h = i2c_read(OUT_TEMP_H);

			// --------------------------------
			// Processing the data:
			// --------------------------------
            x_accel= two_complement_to_int(accel_x_h, accel_x_l);
			x_accel_g = ((float) x_accel) / 16384;
			y_accel= two_complement_to_int(accel_y_h, accel_y_l);
			y_accel_g = ((float) y_accel) / 16384;
			z_accel= two_complement_to_int(accel_z_h, accel_z_l);
			z_accel_g = ((float) z_accel) / 16384;
			
			x_gyro= two_complement_to_int(gyro_x_h, gyro_x_l);
			x_gyro_g = ((float) x_gyro) / 16384;
			y_gyro= two_complement_to_int(gyro_y_h, gyro_y_l);
			y_gyro_g = ((float) y_gyro) / 16384;
			z_gyro= two_complement_to_int(gyro_z_h, gyro_z_l);
			z_gyro_g = ((float) z_gyro) / 16384;
			
			temp = two_complement_to_int(temp_h, temp_l);
			temp_f = (float)temp / 340 + 36.53;

			// --------------------------------
			// Printing the results:
			// --------------------------------
			printf("Acceleration:\n");
            printf("X = %.3f [g]\tY = %.3f [g]\t Z = %.3f [g]\r", x_accel_g, y_accel_g, z_accel_g); 
            printf("Gyroscope:\n");
			printf("X = %.3f [g]\tY = %.3f [g]\t Z = %.3f [g]\r", x_gyro_g, y_gyro_g, z_gyro_g);
			printf("Temperature:\n");
			printf("T = %.1f [C]\r", temp_f);
        }

    }
  
    printf("System exiting...");
    system("pause");

    return 0;
}