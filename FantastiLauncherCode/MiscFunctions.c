int step = 0;
float averageEncoderValueAbsolute = 0;
float rightEncoderValueAbsolute = 0;
float leftEncoderValueAbsolute = 0;
float averageEncoderOffset = 0;
float rightEncoderOffset = 0;
float leftEncoderOffset = 0;

// Function that initilizes sensor values and timers before the main loop starts to run
void init() {
	resetMotorEncoder(leftDrive); // I2C 1
	resetMotorEncoder(rightDrive); // I2C 2
	resetMotorEncoder(clawArm); // I2C 3
	//resetMotorEncoder(clawExtender); // I2C 4
	resetMotorEncoder(launchArm); // I2C 5
	//resetMotorEncoder(clawMotor); // I2C 6
	resetMotorEncoder(clawWrist); // I2C 7
	resetMotorEncoder(loadMotor); // I2C 8
	clearTimer(T1);
	clearTimer(T2);
}

// Variables that need to update continuosly
void loopCode()
{
	// Record the cumulative values of each sensor
	rightEncoderValueAbsolute = getMotorEncoder(rightDrive);
	leftEncoderValueAbsolute = getMotorEncoder(leftDrive);

	// Record the average cumulative value of the two sensors
	averageEncoderValueAbsolute = ((rightEncoderValueAbsolute + leftEncoderValueAbsolute) / 2);

	// Record the relative values of each sensor
	rightEncoderValueRelative = rightEncoderValueAbsolute - rightEncoderOffset;
	leftEncoderValueRelative = leftEncoderValueAbsolute - leftEncoderOffset;

	// Record the average relative value of the two sensors
	averageEncoderValueRelative = averageEncoderValueAbsolute - averageEncoderOffset;
}

// Function that clears some variables after each step in the switch case statement of the main loop
void clearStep()
{
	clearTimer(T2);
	move(0, 0);
	moveClaw(0);
	moveClawArm(0);
	moveClawExtender(0);
	moveClawWrist(0);
	moveIntake(0);
	moveLaunchArm(0);
	moveLoad(0);
	averageEncoderOffset = averageEncoderValueAbsolute;
	rightEncoderOffset = rightEncoderValueAbsolute;
	leftEncoderOffset = leftEncoderValueAbsolute;
	leftEncoderReference = 0;
	rightEncoderReference = 0;
	encoderReferenceFlag = false;
	step++;
}
