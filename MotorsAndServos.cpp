

/****************************************************************************
 * SparkFun_PWM_Edison_Block_Example.cpp
 * Example code showing how to use the SparkFun PWM Edison Block
 * Mike Hord @ SparkFun Electronics
 * 9 June 2015
 * https://github.com/sparkfun/SparkFun_PWM_Block_for_Edison_CPP_Library
 *
 * This file is a demonstration program showing the various functions that we've
 * provided for working with the PCA9685 IC on the SparkFun PWM Edison Block.
 * It uses an RGB LED and a small servo motor to show what the library can do.
 *
 * Resources:
 * Requires Intel's MRAA framework. This can be downloaded from either the
 * GitHub site (https://github.com/intel-iot-devkit/mraa) or in pre-built form
 * from http://iotdk.intel.com/sdk/mraa-update/.
 *
 * Development environment specifics:
 * Developed in the Intel iot-ide-dk Eclipse on Win 7 (v1.0.0.201502201135)
 * Using lib-mraa v0.6.2
 * On Edison poky-linux image build ww18-15
 *
 * This code is beerware; if you see me (or any other SparkFun employee) at the
 * local, and you've found our code helpful, please buy us a round!
 * ****************************************************************************/


#include "SparkFun_TB6612_Edison.h"
#include <iomanip>
#include "mraa.hpp"
#include "SparkFun_pca9685_Edison.h"
#include <iostream>
#include <unistd.h>

using namespace std;

// These channel definitions mirror the ones used in the PWM Block hookup guide.
#define SERVO 0

int main()
{


	tb6612 motors;
	float i;
	float j;

	mraa::I2c* pwm_i2c;
	pwm_i2c = new mraa::I2c(1); // Tell the I2c object which bus it's on.

	pca9685 pwm(pwm_i2c, 0x40); // 0x40 is the default address for the PCA9685.

	motors.standby(false);
	pwm.enableServoMode();


	for (int k = 0; k < 2; k++)
	{
		for (j = 14; j >= 6; j-=0.01)
		{
		  pwm.setChlDuty(SERVO,j);
		  pwm.setChlDuty(1,j);
		  cout<<j<<endl;
		  usleep(20000);
		}
		for (j = 6; j <= 14; j+=0.01)
		{
		  pwm.setChlDuty(SERVO,j);
		  pwm.setChlDuty(1,j);
		  cout<<j<<endl;
		  usleep(2000);
		}

		for(i = 0; i < 1; i += 0.1)
		{
		  motors.diffDrive(i,i);
		  usleep(500000);
		}

		for(i = 1; i > 0; i -= 0.1)
		{
		  motors.diffDrive(i,i);
		  usleep(500000);
		}

		for(i = 0; i < 1; i += 0.1)
		{
		  motors.diffDrive(-i,-i);
		  usleep(500000);
		}

		for(i = 1; i > 0; i -= 0.1)
		{
		  motors.diffDrive(-i,-i);
		  usleep(500000);
		}

		motors.shortBrake(true, true);
	}

	motors.standby(true);

	pwm.setChlDuty(SERVO,14);
	pwm.setChlDuty(1,14);

	bool brakeA = false;
	bool brakeB = false;
	float dcA = 0;
	float dcB = 0;
	bool onStandby = false;

	onStandby = motors.getStandby();

	motors.getDiffDrive(&dcA, &dcB);

	motors.getShortBrake(&brakeA, &brakeB);

	cout<<"Motor standby status: "<< boolalpha << onStandby << endl;
	cout<<"Motor A brake status: "<< brakeA << endl;
	cout<<"Motor B brake status: "<< brakeB << endl;
	cout<<"Channel A speed: "<< fixed << setprecision(3)<<dcA<<endl;
	cout<<"Channel B speed: "<<dcB<<endl;

	cout<<"Servo demo complete!"<<endl;


	return MRAA_SUCCESS;
}






