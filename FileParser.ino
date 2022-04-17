#define CS_PIN 10

File textFile;
String fileName = "music.txt";

/*****************
  # FILE STRUCTURE
//////////////////
1. No. of melodic lines
2. No. of notes in a line
3. BPM of the song
Each next line represents a melodic line, which consists of
notes separated by space. Also, + means holding the previous note
and _ means playing no note. Example:
2
12
120
a2  a2  _   _  c2  +   +   a2  c2  +   _   _
d4  c4  a4  +  _   d4  c4  a4  +   _   d4  a4
*****************/

void readFromSD() {

  //Serial.println("Begin parsing! **********");

  if (!SD.begin(CS_PIN)) {
    //Serial.println("initialization failed!");
    while (true);
  }

  textFile = SD.open(fileName);

  readSongParameters();
  initializeArray();
  readSong();

  //Serial.println("Done parsing! **********");

  //Serial.println(songLength);
  //Serial.println(songLines);
  //Serial.println(noteLength);
  for(int m = 0; m < songLines; m++){
    for(int n = 0; n < songLength; n++){
      //Serial.print(score[m][n]);
      //Serial.print(" ");
    }
    //Serial.print("\n");
  }

  textFile.close();
}

void readSongParameters() {
  for (byte i = 0; i < 3; i++) {
    String line = "";
    while (textFile.available()) {
      char c = textFile.read();
      if (c == '\n')
        break;
      line.concat(c);
    }
    if (i == 0)
      songLines = line.toInt();
    else if (i == 1)
      songLength = line.toInt();
    else
      noteLength = floor((double)1000 * (double)60 / (double)line.toInt());
  }
}

void initializeArray(){
  score = new byte*[songLines];
  for (byte i = 0; i < songLines; i++)
    score[i] = new byte[songLength];
}

void readSong() {
  byte pos = 0;
  String token = "";
  byte melodicLine = 0;

  while (textFile.available()) {
    char c = textFile.read();
    if (c == '\n') {
      melodicLine++;
      token = "";
      pos = 0;
    }
    else if (c == ' ' && token != "") {
      addToArray(melodicLine, token, pos);
      token = "";
      pos += 1;
    }
    else if (c != ' ')
      token.concat(c);
  }
}

void addToArray(byte melodicLine, String actuaNote, byte pos) {
  if (actuaNote.indexOf('+') >= 0)
    score[melodicLine][pos] = NOTE_HOLD;
  else if (actuaNote.indexOf("_") >= 0)
    score[melodicLine][pos] = NOTE_NONE;
  else
    score[melodicLine][pos] = convertNote(actuaNote);
}

byte convertNote(String note) {
  if (note == "e1") return 0;
  else if (note == "f1") return 1;
  else if (note == "g1") return 2;
  else if (note == "a1") return 3;
  else if (note == "b1") return 4;
  else if (note == "c2") return 5;
  else if (note == "d2") return 6;
  else if (note == "e2") return 7;
  else if (note == "f2") return 8;
  else if (note == "g2") return 9;
  else if (note == "a2") return 10;
  else if (note == "b2") return 11;
  else if (note == "c3") return 12;
  else if (note == "d3") return 13;
  else if (note == "e3") return 14;
  else if (note == "f3") return 15;
  else if (note == "g3") return 16;
  else if (note == "a3") return 17;
  else if (note == "b3") return 18;
  else if (note == "c4") return 19;
  else if (note == "d4") return 20;
  else if (note == "e4") return 21;
  else if (note == "f4") return 22;
  else if (note == "g4") return 23;
  else if (note == "a4") return 24;
  else if (note == "b4") return 25;
  else if (note == "c5") return 26;
  else if (note == "d5") return 27;
  else if (note == "e5") return 28;
  else if (note == "f5") return 29;
  else if (note == "g5") return 30;
  else if (note == "a5") return 31;
}
