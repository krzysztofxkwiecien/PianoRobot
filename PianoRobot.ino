#include <SD.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define CS_PIN 10

File textFile;
byte song_length;
byte song_melodic_lines;
String file_name = "music.txt";
byte** score = nullptr;

#define SERVO_FREQ 50
#define SERVO_COUNT 2

// Servo positions
#define LEFT  200
#define MIDDLE  325
#define RIGHT  450

// Config
#define NOTES_COUNT 4

// Mapping
#define G_NOTE 0
#define A_NOTE 1
#define B_NOTE 2
#define C_NOTE 3

byte noteToServoID[NOTES_COUNT];
short noteToServoPosition[NOTES_COUNT];

// Song
short noteLength = 150;
short currentNote = 0;

void setup() {

  sd_setup();

  for(byte note = 0; note < NOTES_COUNT; note++){
    noteToServoID[note] = (byte)(note / 2);
    noteToServoPosition[note] = (note % 2 == 0) ? LEFT : RIGHT;
  }
  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  delay(2000);
}


void loop() {
//  for(int i = 0; i < SERVO_COUNT; i++){
//    turnServo(i, MIDDLE);
//  }

  playScore(currentNote);

  if(++currentNote == song_length)
    currentNote = 0;
}

void playScore(short index){
  for(byte note; note < song_melodic_lines; note++){
    if(note == 1)
      clearNote(note);
    if(note != 0)
      playNote(note);
  }
  delay(noteLength);
}

void clearNote(byte note){
  turnServo(noteToServoID[note], MIDDLE);
}

void playNote(byte note){
  turnServo(noteToServoID[note], noteToServoPosition[note]);
}

void turnServo(byte servoID, short position){
  pwm.setPWM(servoID, 0, position);
}
