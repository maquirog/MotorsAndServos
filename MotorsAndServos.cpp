

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
extern "C" {
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
}

#include "Intel_Edison_BT_SPP.hpp"

using namespace std;

#define LEFT_UP_DOWN		(buf[0]=='0') && (buf[1]=='0')
#define LEFT_FOLD_UNFOLD	(buf[0]=='0') && (buf[1]=='1')
#define MOVE_LEFT			(buf[0]=='0') && (buf[1]=='2')
#define HEAD_LEFT_RIGHT		(buf[0]=='0') && (buf[1]=='4')
#define MOVE_FORWARD		(buf[0]=='0') && (buf[1]=='5')
#define MOVE_STOP			(buf[0]=='0') && (buf[1]=='6')
#define MOVE_BACKWARD		(buf[0]=='0') && (buf[1]=='7')
#define RIGHT_UP_DOWN		(buf[0]=='0') && (buf[1]=='8')
#define RIGHT_FOLD_UNFOLD	(buf[0]=='0') && (buf[1]=='9')
#define MOVE_RIGHT			(buf[0]=='1') && (buf[1]=='0')
#define RESET_ALL			(buf[0]=='1') && (buf[1]=='1')

#define RIGHT_ELBOW			0
#define RIGHT_ARM_FRONT		1
#define RIGHT_ARM_BACK		2
#define HEAD				3
#define LEFT_ELBOW			4
#define LEFT_ARM_FRONT		5
#define LEFT_ARM_BACK		6



void leftUpDown(pca9685 pwm, tb6612 motors){
	static bool up = 0;
	motors.standby(true);
	if(up){
		cout << "leftUp" << endl;
		pwm.setChlDuty(LEFT_ARM_FRONT,11.5);
		pwm.setChlDuty(LEFT_ARM_BACK,11.5);
		up = 0;
	}else{
		cout << "leftDown" << endl;
		pwm.setChlDuty(LEFT_ARM_FRONT,5.5);
		pwm.setChlDuty(LEFT_ARM_BACK,5.5);
		up = 1;
	}
}

void leftFoldUnfold(pca9685 pwm, tb6612 motors){
	static bool fold = 0;
	motors.standby(true);
	if(fold){
		cout << "leftFold" << endl;
		pwm.setChlDuty(LEFT_ELBOW,11);
		fold = 0;
	}else{
		cout << "leftUnfold" << endl;
		pwm.setChlDuty(LEFT_ELBOW,5);
		fold = 1;
	}
}

void rightUpDown(pca9685 pwm, tb6612 motors){
	static bool up = 0;
	motors.standby(true);
	if(up){
		cout << "rightUp" << endl;
		pwm.setChlDuty(RIGHT_ARM_FRONT,6);
		pwm.setChlDuty(RIGHT_ARM_BACK,6);
		up = 0;
	}
	else{
		cout << "rightDown" << endl;
		pwm.setChlDuty(RIGHT_ARM_FRONT,12);
		pwm.setChlDuty(RIGHT_ARM_BACK,12);
		up = 1;
	}
}

void rightFoldUnfold(pca9685 pwm, tb6612 motors){
	static bool fold = 0;
	motors.standby(true);
	if(fold){
		cout << "rightFold" << endl;
		pwm.setChlDuty(RIGHT_ELBOW,6);
		fold = 0;
	}else{
		cout << "rightUnfold" << endl;
		pwm.setChlDuty(RIGHT_ELBOW,13);
		fold = 1;
	}
}

void headLeftRight(pca9685 pwm, tb6612 motors){
	static bool left = 0;
	motors.standby(true);
	if(left){
		cout << "headLeft" << endl;
		pwm.setChlDuty(HEAD,14);
		left = 0;
	}else{
		cout << "headRight" << endl;
		pwm.setChlDuty(HEAD,4);
		left = 1;
	}
}

void servosStop(pca9685 pwm){
	cout << "servosStop" << endl;
	pwm.setChlStop(LEFT_ARM_FRONT, 0);
	pwm.setChlStop(LEFT_ARM_BACK, 0);
	pwm.setChlStop(LEFT_ELBOW, 0);
	pwm.setChlStop(RIGHT_ARM_FRONT, 0);
	pwm.setChlStop(RIGHT_ARM_BACK, 0);
	pwm.setChlStop(RIGHT_ELBOW, 0);
	pwm.setChlStop(HEAD, 0);
}

void moveForward(pca9685 pwm, tb6612 motors){
	cout << "moveForward" << endl;
	servosStop(pwm);
	motors.standby(false);
	motors.diffDrive(0.7,0.7);
}

void moveBackward(pca9685 pwm, tb6612 motors){
	cout << "moveBackward" << endl;
	servosStop(pwm);
	motors.standby(false);
	motors.diffDrive(-0.7,-0.7);
}

void moveStop(tb6612 motors){
	cout << "moveStop" << endl;
	motors.standby(true);
}

void moveLeft(pca9685 pwm, tb6612 motors){
	cout << "moveLeft" << endl;
	servosStop(pwm);
	motors.standby(false);
	motors.diffDrive(-0.7,0.7);
}

void moveRight(pca9685 pwm, tb6612 motors){
	cout << "moveRight" << endl;
	servosStop(pwm);
	motors.standby(false);
	motors.diffDrive(0.7,-0.7);
}

void resetAll(pca9685 pwm,tb6612 motors){
	cout << "resetAll" << endl;
	servosStop(pwm);
	moveStop(motors);
//	pwm.setChlDuty(LEFT_ARM_FRONT,5.5);
//	pwm.setChlDuty(LEFT_ARM_BACK,5.5);
//	pwm.setChlDuty(LEFT_ELBOW,5);
//	pwm.setChlDuty(RIGHT_ARM_FRONT,12);
//	pwm.setChlDuty(RIGHT_ARM_BACK,12);
//	pwm.setChlDuty(RIGHT_ELBOW,13);
}

void moveSomething(char *buf, pca9685 pwm, tb6612 motors){
	if(LEFT_UP_DOWN){
		leftUpDown(pwm, motors);
	}else if(LEFT_FOLD_UNFOLD){
		leftFoldUnfold(pwm, motors);
	}else if(HEAD_LEFT_RIGHT){
		headLeftRight(pwm, motors);
	}else if(MOVE_FORWARD){
		moveForward(pwm, motors);
	}else if(MOVE_LEFT){
		moveLeft(pwm, motors);
	}else if(MOVE_STOP){
		moveStop(motors);
	}else if(MOVE_RIGHT){
		moveRight(pwm, motors);
	}else if(MOVE_BACKWARD){
		moveBackward(pwm, motors);
	}else if(RIGHT_UP_DOWN){
		rightUpDown(pwm, motors);
	}else if(RIGHT_FOLD_UNFOLD){
		rightFoldUnfold(pwm, motors);
	}else if(RESET_ALL){
		resetAll(pwm,motors);
	}else{
		cout << "Nothing" << endl;
	}
}

int main()
{

	Intel_Edison_BT_SPP spp = Intel_Edison_BT_SPP();
	tb6612 motors;

	mraa::I2c* pwm_i2c;
	pwm_i2c = new mraa::I2c(1); // Tell the I2c object which bus it's on.

	pca9685 pwm(pwm_i2c, 0x40); // 0x40 is the default address for the PCA9685.

	motors.standby(false);
	pwm.enableServoMode();
	cout << "1" << endl;
	spp.open();		// Open BT SPP

	cout << "Here" << endl;
	for (;;) {
		ssize_t size = spp.read();
		cout << size << endl;
		if (size > 0 && size < 32)
		{
			cout << "2" << endl;
			char * buf = spp.getBuf();
			cout << buf[0] << ", " << buf[1] << ", "  << buf[2] << endl;
			moveSomething(buf,pwm,motors);

		}
	}


	return MRAA_SUCCESS;
}






