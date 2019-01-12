float averageEncoderValueRelative = 0;
float rightEncoderValueRelative = 0;
float leftEncoderValueRelative = 0;
float leftEncoderReference = 0;
float rightEncoderReference = 0;
bool encoderReferenceFlag = false;
const int kMaxMotorSpeed = 127;
const int kRobotLength = 46; //in centimeters

#pragma systemFile;

// This function moves the base purely based off of the two values given when the function is called
void move(int left, int right) {
	motor[leftDrive] = left;
	motor[rightDrive] = right;
}

// Positive speed closes the claw, negative speed opens it
void moveClaw(int speed) {
	motor[clawMotor] = speed;
}

// Positive speed moves the claw wrist up and negative speed moves it down
void moveClawWrist(int speed) {
	motor[clawWrist] = speed;
}

void moveClawArm(int speed) {
	motor[clawArm] = speed;
}

void moveClawExtender(int speed) {
	motor[clawExtender]	= speed;
}

// Positive speed takes in the ball and negative speed spits it out
void moveIntake(int speed) {
	motor[leftIntake] = speed;
	motor[rightIntake] = speed;
}

// Positive speed takes in the ball and negative speed spits it out
void moveLoad(int speed) {
	motor[loadMotor] = speed;
}

void moveLaunchArm(int speed) {
	motor[launchArm]	= speed;
}

// Returns 0 once the time given has passed, otherwise it returns the time left until it finishes
int wait(int msec) {
	return msec - time1[T2] < 0 ? 0 : msec - time1[T2];
}

//Function that sets a reference value for the goForward and Arc functions based on the encoder value when each function is called for the first time
void setReferences(float leftValue, float rightValue) {
	if(encoderReferenceFlag == false) {
		leftEncoderReference = leftValue;
		rightEncoderReference = rightValue;
		encoderReferenceFlag = true;
	}
}

// Moves to a target encoder value using the average left and right drive encoder values
int moveBaseTo(int leftTarget, int rightTarget, int absolute = false, int maxSpeed = kMaxMotorSpeed, int leftLogValue = 750, int rightLogValue = 750) {
	//Sets a fixed value for the smoothing function	to use
	if (absolute) {
		setReferences(leftTarget, rightTarget);
	} else {
		setReferences(leftTarget + leftEncoderValueRelative, rightTarget + rightEncoderValueRelative);
	}

	int leftDeltaPos = leftEncoderReference - leftEncoderValueRelative;
	int rightDeltaPos = rightEncoderReference - rightEncoderValueRelative;
	float leftSpeed = 0;
	float rightSpeed = 0;

	//As the robot gets close to its destination, it slows down to minimize overshoot
	leftSpeed = floor((kMaxMotorSpeed * log(abs(leftDeltaPos) + 1)) / (log(leftLogValue + 1)));
	rightSpeed = floor((kMaxMotorSpeed * log(abs(rightDeltaPos) + 1)) / (log(rightLogValue + 1)));

	//Makes sure that the speed doesn't go over the max motor speed
	if(abs(leftSpeed) > maxSpeed)
		leftSpeed = maxSpeed * (abs(leftSpeed) / leftSpeed);
	if(abs(rightSpeed) > maxSpeed)
		rightSpeed = maxSpeed * (abs(rightSpeed) / rightSpeed);

	if (leftDeltaPos < 0)
		leftSpeed *= -1;
	if (rightDeltaPos < 0)
		rightSpeed *= -1;

	move(leftSpeed, rightSpeed);

	return abs(leftDeltaPos) > abs(rightDeltaPos) ? abs(leftDeltaPos) : abs(rightDeltaPos);
}

int moveClawWristTo(int target, int logValue = 750) {
	int deltaPos = target - getMotorEncoder(clawWrist);
	float speed = floor((kMaxMotorSpeed * log(abs(deltaPos) + 1)) / (log(logValue + 1)));

	if (speed > kMaxMotorSpeed)
		speed = kMaxMotorSpeed * (abs(speed) / speed);

	if (deltaPos < 0)
		speed *= -1;

	moveClawWrist(speed);

	return abs(deltaPos);
}

int moveClawArmTo(int target, int logValue = 750) {
	int deltaPos = target - getMotorEncoder(clawArm);
	float speed = floor((kMaxMotorSpeed * log(abs(deltaPos) + 1)) / (log(logValue + 1)));

	if (speed > kMaxMotorSpeed)
		speed = kMaxMotorSpeed * (abs(speed) / speed);

	if (deltaPos < 0)
		speed *= -1;

	moveClawArm(speed);

	//writeDebugStreamLine("Speed is: %d", speed);
	writeDebugStreamLine("DeltaPos is: %d", deltaPos);

	return abs(deltaPos);
}

int moveLaunchArmTo(int target, int logValue = 750) {
	int deltaPos = target - getMotorEncoder(launchArm);
	float speed = floor((kMaxMotorSpeed * log(abs(deltaPos) + 1)) / (log(logValue + 1)));

	if (speed > kMaxMotorSpeed)
		speed = kMaxMotorSpeed * (abs(speed) / speed);

	if (deltaPos < 0)
		speed *= -1;

	moveLaunchArm(speed);

	return abs(deltaPos);
}
