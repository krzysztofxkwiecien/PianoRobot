#include <SD.h>

#define CS_PIN 10

File textFile;
byte song_length{};
byte song_melodic_lines{};
char *file_name = "music.txt";
byte**score = nullptr;

byte convertNotes(String note);
void setup();
void loop();
void add_to_array(byte music_line, String actual_note, byte pos);
byte convertNotes(String note);
byte** getScore(){ return score; };

void setup() {
  Serial.begin(115200);
  if (!SD.begin(CS_PIN)) {
    Serial.println("Card initialization failed!");
    while (true);
  }

  File textFile = SD.open(file_name);   
  if (textFile) {                       // read basic parameters 
    for(byte i=0; i<2; i++){
      String line = "";
      while (textFile.available()) {
        char sign = textFile.read();
        if(sign == '\n') break;
        line.concat(sign);
      }
      if(i==0) song_melodic_lines = line.toInt();
      else song_length = line.toInt();
    }
  }

  score = new byte*[song_melodic_lines]; // create a music score container
  for(byte i = 0; i < song_melodic_lines; ++i)
    score[i] = new byte[song_length]; // fill a music score container

  
  if (textFile) {   // read score parameters (musi być spacja na końcu linii melodycznej w pliku)
    byte pos=0;                
    String token = "";
    byte melodic_line = 0;

    while (textFile.available()) {
      char sign = textFile.read();
      if(sign == '\n') {
        melodic_line++;
        pos = 0;
        Serial.println();
      }
      else if(sign == ' ' && token != "") {
        Serial.print(token + " ");
        add_to_array(melodic_line, token, pos);
        token = "";
        pos += 1;
      }
      else if(sign != ' ') token.concat(sign);
    }
    textFile.close();
  }
  Serial.println();

  byte** score3 = getScore();
  
  for(byte i=0; i<song_melodic_lines; i++){
  for(byte j=0; j<song_length; j++){
    Serial.print(score3[i][j]);
    Serial.print(" ");
  }
  Serial.println();
  }
}

void add_to_array(byte music_line, String actual_note, byte pos){
  if(actual_note.indexOf('+') >= 0) { 
    // trzymaj ostatni dzwiek
    score[music_line][pos] = 0;
  }
  else if(actual_note.indexOf("_") >= 0){
    // Pozycja neutralna
    score[music_line][pos] = 1;
  }
  else{
    // zagraj dźwięk
    score[music_line][pos] = convertNotes(actual_note);
  }
}

byte convertNotes(String note){
  if(note == "c1") return 2;
  else if(note == "d1") return 3;
  else if(note == "e1") return 4;
  else if(note == "f1") return 5;
  else if(note == "g1") return 6;
  else if(note == "a1") return 7;
  else if(note == "h1") return 8;
  else if(note == "c2") return 9;
  else if(note == "d2") return 10;
  else if(note == "e2") return 11;
  else if(note == "f2") return 12;
  else if(note == "g2") return 13;
  else if(note == "a2") return 14;
  else if(note == "h2") return 15;
  else if(note == "c3") return 16;
  else if(note == "d3") return 17;
  else if(note == "e3") return 18;
  else if(note == "f3") return 19;
  else if(note == "g3") return 20;
  else if(note == "a3") return 21;
  else if(note == "h3") return 22;
  else if(note == "c4") return 23;
  else if(note == "d4") return 24;
  else if(note == "e4") return 25;
  else if(note == "f4") return 26;
  else if(note == "g4") return 27;
  else if(note == "a4") return 28;
  else if(note == "h4") return 29;
  else if(note == "c5") return 30;
  else if(note == "d5") return 31;
  else if(note == "e5") return 32;
  else if(note == "f5") return 33;
  else if(note == "g5") return 34;
  else if(note == "a5") return 35;
  else if(note == "h5") return 36;
  else if(note == "c6") return 37;
  else return 0;
}


void loop() {
  // nothing happens after setup finishes.
}
