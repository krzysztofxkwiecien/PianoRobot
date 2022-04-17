#include <SPI.h>
#include <SD.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver();

/*****************
  # DEV OPTIONS
*****************/
#define DEV_MODE 0 // Set 0 to disable all dev options
#define CALIBRATION_MODE 1 // Hold all servos in the MIDDLE position (for mounting)
#define STEP_MODE 0 // Step though the song with button presses instead of auto-playing

#define STEP_BUTTON_PIN 8
//////////////////

#define NOTE_NONE 255
#define NOTE_HOLD 254

// Servo config
#define OSC_FREQ 27000000
#define PWM_FREQ 50
#define LEFT 450
#define MIDDLE 325
#define RIGHT 200
#define SERVO_COUNT 16

byte noteToServoID[SERVO_COUNT * 2];
short noteToServoPosition[SERVO_COUNT * 2];

// Song
byte songLength;
byte songLines;
byte** score = nullptr;
short noteLength;

// Iterator
short currentNote = 0;

void setup() {

  //Serial.begin(115200);
  //Serial.println("Start");

  readFromSD();

  for (byte note = 0; note < SERVO_COUNT * 2; note++) {
    noteToServoID[note] = (byte)(note / 2);
    noteToServoPosition[note] = (note % 2 == 0) ? LEFT : RIGHT;
  }

  pwmDriver.begin();
  pwmDriver.setOscillatorFrequency(OSC_FREQ);
  pwmDriver.setPWMFreq(PWM_FREQ);
  for (byte i = 0; i < SERVO_COUNT; i++) {
    turnServo(i, MIDDLE);
    delay(100);
  }

  //Serial.println("Setup complete!");

  delay(1000);
}


void loop() {

  //Serial.println("Loop --------------");

  if (DEV_MODE && CALIBRATION_MODE) {
    for (byte i = 0; i < SERVO_COUNT; i++) {
      turnServo(i, MIDDLE);
      delay(100);
    }
    delay(1000);
    return;
  }

  playScore(currentNote);

  if (DEV_MODE && STEP_MODE) {
    while (!digitalRead(STEP_BUTTON_PIN)) {
      delay(10);
    }
  }

  if (++currentNote == songLength)
    currentNote = 0;
}

void playScore(short index) {


  for (byte line = 0; line < songLines; line++) {
    if (!(score[line][index] == NOTE_HOLD)) {
      byte clearIndex;
      if (index != 0)
        clearIndex = index - 1;
      else
        clearIndex = songLength - 1;
      while (score[line][clearIndex] == NOTE_HOLD) {
        clearIndex--;
      }
      //Serial.print("Clear note ");
      //Serial.println(score[line][clearIndex]);
      clearNote(score[line][clearIndex]);
    }
  }
  delay(noteLength);


  //Serial.println("Cleared");
  for (byte line = 0; line < songLines; line++) {
    //Serial.print("Time for note ");
    //Serial.println(score[line][index]);
    if (!(score[line][index] == NOTE_HOLD))
      playNote(score[line][index]);
  }
  delay(noteLength);
}

void clearNote(byte note) {
  if (note != NOTE_NONE)
    turnServo(noteToServoID[note], MIDDLE);
}

void playNote(byte note) {
  if (note != NOTE_NONE)
    turnServo(noteToServoID[note], noteToServoPosition[note]);
}

void turnServo(byte servoID, short position) {
  //Serial.print("Turn servo ");
  //Serial.print(servoID);
  //Serial.print(" to ");
  //Serial.println(position);
  pwmDriver.setPWM(servoID, 0, position);
}
