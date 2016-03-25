#include <Arduino.h>

//Motors
const short enL = 5;
const short inA = 8;
const short inB = 9;
const short enR = 6;
const short inC = 10;
const short inD = 11;

short speedL = 0, speedR = 0;

short speedMax = 250;
short speedMin = 180;

//switch
const short swPin = 2;
//speaker
const short speakerPin = 3;

//sensors
bool lsData[6] = { false };
const short lsPin[6] = { 0, 1, 2, 3, 4, 5 };
short tempCol = 0;
short thresholdVal = 0;
short blackCol = 0;
short whiteCol = 0;

//PID variables
int preError = 0, error = 0;
short Kp = 25, Ki = 1, Kd = 2;
float P, I, D, correction;

//function prototypes
bool checkSwitch();
void intensityCalc();
void readSensors();
void forward(short speed_L, short speed_R);
void reverse();
void stopBot();
void left();
void right();
void speakerTone(short toneF, short delayTone);

bool checkSwitch() {
	//end loop if the switch is pressed
	if (digitalRead(swPin) == LOW) {
		digitalWrite(swPin, HIGH);
		return false;
	}
	else
		return true;
}

void intensityCalc() {

	//make max: for the condition below ( to get the lowest white intensity )
	whiteCol = 1023;

	//grab the intensity values

	for (short i = 0; i < 6; i++) {
		tempCol = analogRead(lsPin[i]);

		if (blackCol <= tempCol) {
			//blackCol = blackCol + lsPin[i];
			blackCol = tempCol;
		}

		if (whiteCol >= tempCol) {
			whiteCol = tempCol;
		}
		delay(5);
	}

	analogWrite(enL, speedMin);
	digitalWrite(inA, HIGH);
	digitalWrite(inB, LOW);
	analogWrite(enR, speedMin);
	digitalWrite(inC, LOW);
	digitalWrite(inD, HIGH);
	delay(300);
	stopBot();

	//grab the intensity values
	for (short i = 0; i < 6; i++) {
		tempCol = analogRead(lsPin[i]);

		if (whiteCol >= tempCol) {
			whiteCol = whiteCol + tempCol;
		}
		delay(5);
	}

	analogWrite(enL, speedMin);
	digitalWrite(inA, LOW);
	digitalWrite(inB, HIGH);
	analogWrite(enR, speedMin);
	digitalWrite(inC, HIGH);
	digitalWrite(inD, LOW);
	delay(600);
	stopBot();

	//grab the intensity values
	for (short i = 0; i < 6; i++) {
		tempCol = analogRead(lsPin[i]);

		if (whiteCol >= tempCol) {
			whiteCol = whiteCol + tempCol;
		}
		delay(5);
	}

	analogWrite(enL, speedMin);
	digitalWrite(inA, HIGH);
	digitalWrite(inB, LOW);
	analogWrite(enR, speedMin);
	digitalWrite(inC, LOW);
	digitalWrite(inD, HIGH);
	delay(320);
	stopBot();

	whiteCol = (whiteCol / 13);

	thresholdVal = (whiteCol + blackCol) / 2;


	Serial.print("Black: ");
	Serial.println(blackCol);
	Serial.print("White: ");
	Serial.println(whiteCol);
	Serial.print("Threshold: ");
	Serial.println(thresholdVal);

	return;
}

void readSensors() {

	//save the previous error
	preError = error;

	//read the sensor value and save as Digital
	for (int i = 0; i < 5; i++) {
		//clear the values
		lsData[i] = false;
		tempCol = analogRead(lsPin[i]);

		if (tempCol < thresholdVal)
			lsData[i] = true;

		delay(5);

		//calculate error
		error = error + (lsData[i] * (i + 1));
	}

	error = error / (lsData[0] + lsData[1] + lsData[2] + lsData[3] + lsData[4] + lsData[5]);
	//error = error - 4.5;

	return;
}

bool linefollow() {
	/*
	* 000000
	* 100001 - stop
	* 001100 - fw
	* 100000 - l 90
	* 011000 - l
	* 000110 - r
	* 000001 - r 90
	*/

	readSensors();

	if (lsData[0] && lsData[5]) {
		//stop Bot
		stopBot();
		return false;
	}

	/*if (lsData[0]) {
		speakerTone(512, 100);
	}

	if (lsData[5]) {
		speakerTone(512, 100);
	}*/


	/*P = error * Kp;

	I = I + error;
	I = I * Ki;

	D = error - preError;

	correction = P + I + D;

	speedL = speedMin + correction;
	speedR = speedMin - correction;

	if (speedR > speedMax)
		speedR = speedMax;

	if (speedL > speedMax)
		speedL = speedMax;

	forward(speedL, speedR);*/

	return true;
}

void speakerTone(short toneF, short delayTone) {
	tone(speakerPin, toneF);
	delay(delayTone);
	noTone(speakerPin);
}


//movements handler functions
void forward(short speed_L, short speed_R) {
	analogWrite(enL, speed_L);
	digitalWrite(inA, HIGH);
	digitalWrite(inB, LOW);
	analogWrite(enR, speed_R);
	digitalWrite(inC, HIGH);
	digitalWrite(inD, LOW);
}
void reverse() {
	//analogWrite(enL, 250);
	digitalWrite(inA, LOW);
	digitalWrite(inB, HIGH);
	//analogWrite(enR, 250);
	digitalWrite(inC, LOW);
	digitalWrite(inD, HIGH);
}
void stopBot() {
	//analogWrite(enL, 250);
	digitalWrite(inA, LOW);
	digitalWrite(inB, LOW);
	//analogWrite(enR, 250);
	digitalWrite(inC, LOW);
	digitalWrite(inD, LOW);

	speakerTone(800, 500);
}
void left() {
	//analogWrite(enL, 250);
	digitalWrite(inA, LOW);
	digitalWrite(inB, LOW);
	//analogWrite(enR, 250);
	digitalWrite(inC, HIGH);
	digitalWrite(inD, LOW);
}
void right() {
	//analogWrite(enL, 250);
	digitalWrite(inA, HIGH);
	digitalWrite(inB, LOW);
	//analogWrite(enR, 250);
	digitalWrite(inC, LOW);
	digitalWrite(inD, LOW);
}


