#include <iostream>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "RPi_Sensor.h"
#include "RPi_BNO055.h"
#include "utility/imumaths.h"

/* This driver reads raw data from the BNO055

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055();

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
int main(void)
{
    if (stdio_init_all() == false)  // pico-sdkの初期化
	{
		std::cout <<"Initialisation error of the GPIO \n Closing program..."<< std::endl;
		return -1;
	}
	
	// I2Cをセットアップ
	// bno._HandleBNO=i2cOpen(bno._i2cChannel,BNO055_ADDRESS_A,0);
	i2c_init(I2cChannel, I2cBaudRate);
	gpio_set_function(SclPinGpio, GPIO_FUNC_I2C);
	gpio_set_function(SdaPinGpio, GPIO_FUNC_I2C);
	
	std::cout << "Orientation Sensor Raw Data Test" << std::endl;

	/* Initialise the sensor */
	if(!bno.begin())
	{
		/* There was a problem detecting the BNO055 ... check your connections */
		std::cout << "Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!" << std::endl;
		while(1);
	}

	sleep_ms(1);

	/* Display the current temperature */
	int8_t temp = bno.getTemp();
	std::cout << "Current Temperature: "<< (int)temp << " C" << std::endl;


	bno.setExtCrystalUse(true);

	std::cout << "Calibration status values: 0=uncalibrated, 3=fully calibrated"<<std::endl;

	while (1)
	{
		// Possible vector values can be:
		// - VECTOR_ACCELEROMETER - m/s^2
		// - VECTOR_MAGNETOMETER  - uT
		// - VECTOR_GYROSCOPE     - rad/s
		// - VECTOR_EULER         - degrees
		// - VECTOR_LINEARACCEL   - m/s^2
		// - VECTOR_GRAVITY       - m/s^2
		imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

		/* Display the floating point data */
		std::cout << "X: " << euler.x() <<  " Y: " << euler.y() << " Z: "
		<< euler.z() << "\t\t";

		/*
		// Quaternion data
		imu::Quaternion quat = bno.getQuat();
		printf("qW: ");
		printf(quat.w(), 4);
		printf(" qX: ");
		printf(quat.y(), 4);
		printf(" qY: ");
		printf(quat.x(), 4);
		printf(" qZ: ");
		printf(quat.z(), 4);
		printf("\t\t");
		*/

		/* Display calibration status for each sensor. */
		uint8_t system, gyro, accel, mag = 0;
		bno.getCalibration(&system, &gyro, &accel, &mag);
		std::cout<< "CALIBRATIO: Sys=" << (int)system << " Gyro=" << (int) gyro
		<< " Accel=" << (int) accel << " Mag=" << (int)mag << std::endl;

		sleep_ms(BNO055_SAMPLERATE_DELAY_MS);
	}

}
