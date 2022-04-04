#include <SD.h>

#define CS_PIN 10

File textFile;
struct SONG{
  byte length{};
  byte melodic_lines{};
};
String file_name = "wokwi.txt";
SONG song;
byte convertNotes(String note);

byte**score = nullptr;
byte** getScore(){
  return score;
};

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
      if(i==0) song.melodic_lines = line.toInt();
      else song.length = line.toInt();
    }
  }

  score = new byte*[song.melodic_lines]; // create a music score container
  for(byte i = 0; i < song.melodic_lines; ++i)
    score[i] = new byte[2*song.length]; // create a music score container

  String tokens[song.melodic_lines][song.length];

  if (textFile) {                   // read score parameters (musi być spacja na końcu linii melodycznej w pliku)
    String line = "";
    byte melodic_line = 0;
    byte pos = 0;
    byte to_much_space = 0;
    while (textFile.available()) {
      char sign = textFile.read();
      if(sign == '\n') {
        melodic_line++;
        pos = 0;
      }
      else if(sign == ' ' && line != "") {
        tokens[melodic_line][pos] = line;
        pos += 1;
        line = "";
      }
      else if(sign != ' ') line.concat(sign);
      
    }
    textFile.close();
  }

  for(byte music_line = 0; music_line < song.melodic_lines; music_line++){
    byte token_pos = 0;
  for(byte pos = 0; pos < 2*song.length; pos+=1){
    String predecessor = "";
    String actual_note = "";
    String successor = "";

    if(token_pos > 0) predecessor = tokens[music_line][token_pos-1];
    actual_note = tokens[music_line][token_pos];
    if(token_pos + 1 < song.length) successor = tokens[music_line][token_pos+1];

    if(actual_note.indexOf('+') >= 0) { // SERVO - do nothing, keep the note played (write double 0 to score array)
      if(successor.indexOf('+') == -1){
        // 1. trzymaj ostatni dzwiek           -  0 = "KeepLast - do nothing"
        // 2. wroc do pozycji neutralnej       -  1 = "BackToNetural"
        score[music_line][pos] = 0;
        score[music_line][pos+1] = 1;
        pos += 1;
      }
      else{
        // 1. trzymaj ostatni dzwiek
        score[music_line][pos] = 0;
      }
    }
    else if(actual_note.indexOf("_") >= 0){
      //1. Pozycja neutralna
      score[music_line][pos] = 1;
    }
    else{
      if(successor.indexOf('+') >= 0){
        // 1. zagraj dźwięk
        // 2. zostaw
        score[music_line][pos] = convertNotes(actual_note);
        score[music_line][pos+1] = 0;
        pos += 1;
      }
      else{
        // 1. zagraj dźwięk
        score[music_line][pos] = convertNotes(actual_note);
      }
    }
    if(token_pos < song.length-1)token_pos += 1;
    else break;
  }
  }

  for(byte i=0; i<song.melodic_lines; i++){
  for(byte j=0; j<song.length; j++)
    Serial.print(tokens[i][j] + " ");
    Serial.println();
  }

  byte** score3 = getScore();
  
  for(byte i=0; i<song.melodic_lines; i++){
  for(byte j=0; j<2*song.length; j++){
    Serial.print(score3[i][j]);
    Serial.print(" ");
  }
  Serial.println();
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
