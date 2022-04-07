#include <SD.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver();

/*****************
  # DEV OPTIONS
*****************/
#define DEV_MODE 0 // Set 0 to disable all dev options
#define CALIBRATION_MODE 0 // Hold all servos in the MIDDLE position (for mounting)
#define STEP_MODE 0 // Step though the song with button presses instead of auto-playing

#define STEP_BUTTON_PIN -1
//////////////////

#define NOTE_CLEAR 255
#define NOTE_HOLD 254

// Servo config
#define OSC_FREQ 27000000
#define PWM_FREQ 50
#define LEFT  200
#define MIDDLE  325
#define RIGHT  450
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

  readFromSD();

  for (byte note = 0; note < SERVO_COUNT * 2; note++) {
    noteToServoID[note] = (byte)(note / 2);
    noteToServoPosition[note] = (note % 2 == 0) ? LEFT : RIGHT;
  }

  pwmDriver.begin();
  pwmDriver.setOscillatorFrequency(OSC_FREQ);
  pwmDriver.setPWMFreq(PWM_FREQ);

  delay(1000);
}


void loop() {

  if (DEV_MODE && CALIBRATION_MODE) {
    for (int i = 0; i < SERVO_COUNT; i++) {
      turnServo(i, MIDDLE);
    }
  }

  playScore(currentNote);

  if (DEV_MODE && STEP_MODE) {
    while (!digitalRead(STEP_BUTTON_PIN)) {}
  }

  if (++currentNote == songLength)
    currentNote = 0;
}

void playScore(short index) {
  for (byte note; note < songLines; note++) {
    if (note == NOTE_CLEAR)
      clearNote(note);
    if (note != NOTE_HOLD)
      playNote(note);
  }
  delay(noteLength);
}

void clearNote(byte note) {
  turnServo(noteToServoID[note], MIDDLE);
}

void playNote(byte note) {
  turnServo(noteToServoID[note], noteToServoPosition[note]);
}

void turnServo(byte servoID, short position) {
  pwmDriver.setPWM(servoID, 0, position);
}
