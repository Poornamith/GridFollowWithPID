#include "lineFollow.h"

void setup() {
	//setting up the sensors
	/*for (int i = 0; i < 5; i++) {
	pinMode(lsPin[i], INPUT);
	}*/
	pinMode(swPin, INPUT);
	pinMode(speakerPin, OUTPUT);

	pinMode(inA, OUTPUT);
	pinMode(inB, OUTPUT);
	//pinMode(enL, OUTPUT);

	pinMode(inC, OUTPUT);
	pinMode(inD, OUTPUT);
	//pinMode(enR, OUTPUT);

	//sw pin enable  Pull ups
	digitalWrite(swPin, HIGH);

	Serial.begin(9600);
	Serial.println("OK");

	analogWrite(enL, 220);

	//intensityCalc();
}

void loop() {
	
	//motorTest();

	//wait till the button is pressed
	while(checkSwitch());
	delay(1000);
	intensityCalc();

	while (checkSwitch());
	sensorTest();

	while(checkSwitch());
	delay(1000);
	//follow the line
	while (linefollow());
	
}

void motorTest() {

	//ROBOT MOTORS DIRECTION TEST CODE
	speakerTone(512, 1000);

	forward(speedMax, speedMax);
	delay(1000);
	stopBot();
	delay(1000);
	//RIGHT
	forward(speedMax, 0);
	delay(1000);
	stopBot();
	delay(1000);
	//LEFT
	forward(0, speedMax);
	delay(1000);
	stopBot();
	delay(1000);

	forward(speedMin, speedMin);
	delay(2000);
	stopBot();
	delay(1000);
}

void sensorTest() {

	readSensors();

	for (short i = 0; i < 6; i++) {
		Serial.print(lsData[i]);
		Serial.print(" ");
	}
	Serial.println(" ");
	delay(500);
}