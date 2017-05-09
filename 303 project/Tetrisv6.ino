#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <gfxfont.h>
#include <Node.h>

Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

Node board[16][8]; // initalize node matrix

int piece1[2]; // identifies location of a moving node
int piece2[2]; // identifies location of a moving node
int piece3[2]; // identifies location of a moving node
int piece4[2]; // identifies location of a moving node
int rotation; // identifies state of rotation
int piece = 0; // identifies piece number (line=0, box=1, T=2, L=3, backwards L=4, Z=5, S=6)

int left = 0; // left button
int right = 0; // right button
int rotate = 0; // rotate button
int potVal = 1000;
int varspeed = 1200;
int score = 0;

const int leftPin = 13;
const int rightPin = 11;
const int rotatePin = 12;
const int potPin = 1;
const int musPin=9;

void setup() {
  for (int i=0; i<8; i++){
    for (int j=0; j<16; j++){
      board[j][i].setTemp(false);
      board[j][i].setPerm(false);
    }
  }

  Serial.begin(9600);
  matrix.begin(0x70); // passin the address
  randomSeed(analogRead(2));

  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(rotatePin, INPUT);
  pinMode(musPin,OUTPUT);
}

void loop() {
  // game functionality
  bool play=false;
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  for (int8_t x=7; x>=-36; x--){
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("Tetris");
    matrix.writeDisplay();
    delay(100);
    checkButtons();
    if(rotate==HIGH)
    {
      play=true;
      digitalWrite(musPin,HIGH);
      //break;
    }
   }
  while(play)
  {
  spawn_piece();
  refresh();
  delay(varspeed);
  bool go=true;
  while(go)
  {
     for(int i=0; i<10; i++){
       checkButtons();
        if(left==HIGH||right==HIGH||rotate==HIGH)
        {
          //shift in direction or rotate
         if(left==HIGH && check_move(1)==true){
            shift_left();
         }
         else if(right==HIGH && check_move(2)==true){
            shift_right();
         }
         else if(rotate==HIGH && check_rotate()==true){
          rotate_piece();
         }
          refresh();
        }
        //update varspeed based on potentiometer value
        potVal = analogRead(potPin);
        varspeed = 100+potVal;
        delay(varspeed/7);  
      }
      if(check_move(0)==true)
      {//true
        shift_down();
        refresh();
      }
      else
      {
        lock();
        go=false;
        refresh();
      }
    }
    go=true;
    score+=check_rows();
    lose();
   }
}

void checkButtons(){
  left = digitalRead(leftPin);
  right = digitalRead(rightPin);
  rotate = digitalRead(rotatePin);
}

// spawn functions
void spawn_piece() {
  piece = random(7);
  if (piece == 0){ spawn_0(); }
  else if(piece == 1) { spawn_1(); }
  else if(piece == 2) { spawn_2(); }
  else if(piece == 3) { spawn_3(); }
  else if(piece == 4) { spawn_4(); }
  else if(piece == 5) { spawn_5(); }
  else if(piece == 6) { spawn_6(); }
  rotation = 0;
}
void spawn_0() { // spawn in line rotation 0
  board[0][2].setTemp(true);
  board[0][3].setTemp(true);
  board[0][4].setTemp(true);
  board[0][5].setTemp(true);
  piece1[0]=0;
  piece1[1]=2;
  piece2[0]=0;
  piece2[1]=3;
  piece3[0]=0;
  piece3[1]=4;
  piece4[0]=0;
  piece4[1]=5;
}
void spawn_1() { // spawn in box rotation 0
  board[0][3].setTemp(true);
  board[0][4].setTemp(true);
  board[1][3].setTemp(true);
  board[1][4].setTemp(true);
  piece1[0]=0;
  piece1[1]=3;
  piece2[0]=0;
  piece2[1]=4;
  piece3[0]=1;
  piece3[1]=3;
  piece4[0]=1;
  piece4[1]=4;
}
void spawn_2() { // spawn in T rotation 0
  board[0][2].setTemp(true);
  board[0][3].setTemp(true);
  board[0][4].setTemp(true);
  board[1][3].setTemp(true);
  piece1[0]=0;
  piece1[1]=2;
  piece2[0]=0;
  piece2[1]=3;
  piece3[0]=0;
  piece3[1]=4;
  piece4[0]=1;
  piece4[1]=3;
}
void spawn_3() { // span in L rotation 0
  board[0][2].setTemp(true);
  board[0][3].setTemp(true);
  board[0][4].setTemp(true);
  board[1][2].setTemp(true);
  piece1[0]=0;
  piece1[1]=2;
  piece2[0]=0;
  piece2[1]=3;
  piece3[0]=0;
  piece3[1]=4;
  piece4[0]=1;
  piece4[1]=2;
}
void spawn_4() { // span in backwards L rotation 0
  board[0][2].setTemp(true);
  board[0][3].setTemp(true);
  board[0][4].setTemp(true);
  board[1][4].setTemp(true);
  piece1[0]=0;
  piece1[1]=2;
  piece2[0]=0;
  piece2[1]=3;
  piece3[0]=0;
  piece3[1]=4;
  piece4[0]=1;
  piece4[1]=4;
}
void spawn_5() { // spawn in Z rotation 0
  board[0][2].setTemp(true);
  board[0][3].setTemp(true);
  board[1][3].setTemp(true);
  board[1][4].setTemp(true);
  piece1[0]=0;
  piece1[1]=2;
  piece2[0]=0;
  piece2[1]=3;
  piece3[0]=1;
  piece3[1]=3;
  piece4[0]=1;
  piece4[1]=4;
}
void spawn_6() { // span in S rotation 0
  board[1][2].setTemp(true);
  board[1][3].setTemp(true);
  board[0][3].setTemp(true);
  board[0][4].setTemp(true);
  piece1[0]=1;
  piece1[1]=2;
  piece2[0]=1;
  piece2[1]=3;
  piece3[0]=0;
  piece3[1]=3;
  piece4[0]=0;
  piece4[1]=4;
}

//movement functions
bool check_move(int dir)//checks ability to be moved, 0=down 1=left 2=right returns a boolean of if the movement is possible or not
{
  if(dir==0)
  {
    if(board[piece1[0]+1][piece1[1]].getPerm()==false&&board[piece2[0]+1][piece2[1]].getPerm()==false&&board[piece3[0]+1][piece3[1]].getPerm()==false&&board[piece4[0]+1][piece4[1]].getPerm()==false&&piece1[0]<15&&piece2[0]<15&&piece3[0]<15&&piece4[0]<15)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  if(dir==1)
  {
    if(board[piece1[0]][piece1[1]-1].getPerm()==false&&board[piece2[0]][piece2[1]-1].getPerm()==false&&board[piece3[0]][piece3[1]-1].getPerm()==false&&board[piece4[0]][piece4[1]-1].getPerm()==false&&piece1[1]>0&&piece2[1]>0&&piece3[1]>0&&piece4[1]>0)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  if(dir==2)
  {
    if(board[piece1[0]][piece1[1]+1].getPerm()==false&&board[piece2[0]][piece2[1]+1].getPerm()==false&&board[piece3[0]][piece3[1]+1].getPerm()==false&&board[piece4[0]][piece4[1]+1].getPerm()==false&&piece1[1]<7&&piece2[1]<7&&piece3[1]<7&&piece4[1]<7)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

// piece number (line=0, box=1, T=2, L=3, backwards L=4, Z=5, S=6)
bool check_rotate() {
  if(piece==0){ // line
    if(rotation==0||rotation==2){ // horizontial to vertical
      if(board[piece1[0]-1][piece1[1]+2].getPerm()==false&&board[piece2[0]][piece2[1]+1].getPerm()==false&&board[piece3[0]+1][piece3[1]].getPerm()==false&&board[piece4[0]+2][piece4[1]-1].getPerm()==false&&
      piece1[0]-1>=0&&piece1[1]+2<=7&&piece2[1]+1<=7&&piece3[0]+1<=15&&piece4[0]+2<=15&&piece4[1]-1>=0){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==1||rotation==3){ // vertical to horizontial
      if(board[piece1[0]+1][piece1[1]-2].getPerm()==false&&board[piece2[0]][piece2[1]-1].getPerm()==false&&board[piece3[0]-1][piece3[1]].getPerm()==false&&board[piece4[0]-2][piece4[1]+1].getPerm()==false&&
      piece1[0]+1<=15&&piece1[1]-2>=0&&piece2[1]-1>=0&&piece3[0]-1>=0&&piece4[0]-2>=0&&piece4[1]+1<=7){
        return true;
      }
      else{
        return false;
      }
    }
  }
  else if(piece==2){ // T
    if(rotation==0) {
      if(board[piece3[0]-1][piece3[1]-1].getPerm()==false&&
      piece3[0]-1>=0&&piece3[1]-1>=0){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==1) {
      if(board[piece4[0]-1][piece4[1]+1].getPerm()==false&&
      piece4[0]-1>=0&&piece4[1]+1<=7){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==2) {
      if(board[piece1[0]+1][piece1[1]+1].getPerm()==false&&
      piece1[0]+1<=15&&piece1[1]+1<=7){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==3) {
      if(board[piece1[0]-1][piece1[1]-1].getPerm()==false&&board[piece3[0]+1][piece3[1]+1].getPerm()==false&&board[piece4[0]+1][piece4[1]-1].getPerm()==false&&
      piece1[0]-1>=0&&piece1[1]+1<=7&&piece3[0]+1<=15&&piece3[1]+1<=7&&piece4[0]+1<=15&&piece4[1]-1>=0){
        return true;
      }
      else{
        return false;
      }
    }
  }
  else if(piece==3){ // L
    if(rotation==0) {
      if(board[piece1[0]-1][piece1[1]+1].getPerm()==false&&board[piece3[0]+1][piece3[1]-1].getPerm()==false&&board[piece4[0]-2][piece4[1]].getPerm()==false&&
      piece1[0]-1>=0&&piece1[1]+1<=7&&piece3[0]+1<=15&&piece3[1]-1>=0&&piece4[0]-2>=0){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==1) {
      if(board[piece1[0]+1][piece1[1]+1].getPerm()==false&&board[piece3[0]-1][piece3[1]-1].getPerm()==false&&board[piece4[0]][piece4[1]+2].getPerm()==false&&
      piece1[0]+1<=15&&piece1[1]+1<=7&&piece3[0]-1>=0&&piece3[1]-1>=0&&piece4[1]+2<=7){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==2) {
      if(board[piece1[0]+1][piece1[1]-1].getPerm()==false&&board[piece3[0]-1][piece3[1]+1].getPerm()==false&&board[piece4[0]+2][piece4[1]].getPerm()==false&&
      piece1[0]+1<=15&&piece1[1]-1>=0&&piece3[0]-1>=0&&piece3[1]+1<=7&&piece4[0]+2<=15){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==3) {
      if(board[piece1[0]-1][piece1[1]-1].getPerm()==false&&board[piece3[0]+1][piece3[1]+1].getPerm()==false&&board[piece4[0]][piece4[1]-2].getPerm()==false&&
      piece1[0]-1>=0&&piece1[0]-1>=0&&piece3[0]+1<=15&&piece3[1]+1<=7&&piece4[1]-2>=0){
        return true;
      }
      else{
        return false;
      }
    } 
  }
  else if(piece==4){ // backwards L
    if(rotation==0) {
      if(board[piece1[0]-1][piece1[1]+1].getPerm()==false&&board[piece3[0]+1][piece3[1]-1].getPerm()==false&&board[piece4[0]][piece4[1]-2].getPerm()==false&&
      piece1[0]-1>=0&&piece1[1]+1<=7&&piece3[0]+1<=15&&piece3[1]-1>=0&&piece4[1]-2>=0){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==1) {
      if(board[piece1[0]+1][piece1[1]+1].getPerm()==false&&board[piece3[0]-1][piece3[1]-1].getPerm()==false&&board[piece4[0]-2][piece4[1]].getPerm()==false&&
      piece1[0]+1<=15&&piece1[1]+1<=7&&piece3[0]-1>=0&&piece3[1]-1>=0&&piece4[0]-2>=0){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==2) {
      if(board[piece1[0]+1][piece1[1]-1].getPerm()==false&&board[piece3[0]-1][piece3[1]+1].getPerm()==false&&board[piece4[0]][piece4[1]+2].getPerm()==false&&
      piece1[0]+1<=15&&piece1[1]-1>=0&&piece3[0]-1>=0&&piece3[1]+1<=7&&piece4[1]+2<=7){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==3) {
      if(board[piece1[0]-1][piece1[1]-1].getPerm()==false&&board[piece3[0]+1][piece3[1]+1].getPerm()==false&&board[piece4[0]+2][piece4[1]].getPerm()==false&&
      piece1[0]-1>=0&&piece1[0]-1>=0&&piece3[0]+1<=15&&piece3[1]+1<=7&&piece4[0]+2<=15){
        return true;
      }
      else{
        return false;
      }
    } 
  }
  else if(piece==5){ // Z
    if(rotation==0||rotation==2){
      if(board[piece1[0]-1][piece1[1]+2].getPerm()==false&&board[piece2[0]][piece2[1]+1].getPerm()==false&&board[piece3[0]-1][piece3[1]].getPerm()==false&&board[piece4[0]][piece4[1]-1].getPerm()==false&&
      piece1[0]-1>=0&&piece1[1]+2<=7&&piece2[1]+1<=7&&piece3[0]-1>=0&&piece4[1]-1>=0){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==1||rotation==3){
      if(board[piece1[0]+1][piece1[1]-2].getPerm()==false&&board[piece2[0]][piece2[1]-1].getPerm()==false&&board[piece3[0]+1][piece3[1]].getPerm()==false&&board[piece4[0]][piece4[1]+1].getPerm()==false&&
      piece1[0]+1<=15&&piece1[1]-2>=0&&piece2[1]-1>=0&&piece3[0]+1<=15&&piece4[1]+1<=7){
        return true;
      }
      else{
        return false;
      }
    }
  }
  else if(piece==6){ // S
    if(rotation==0||rotation==2){
      if(board[piece1[0]-2][piece1[1]].getPerm()==false&&board[piece2[0]-1][piece2[1]-1].getPerm()==false&&board[piece3[0]][piece3[1]].getPerm()==false&&board[piece4[0]+1][piece4[1]-1].getPerm()==false&&
      piece1[0]-1>=0&&piece2[0]-1>=0&&piece2[1]-2>=0&&piece3[1]+1<=7&&piece4[0]-1<=15){
        return true;
      }
      else{
        return false;
      }
    }
    else if(rotation==1||rotation==3){
      if(board[piece1[0]+2][piece1[1]].getPerm()==false&&board[piece2[0]+1][piece2[1]+1].getPerm()==false&&board[piece3[0]][piece3[1]].getPerm()==false&&board[piece4[0]-1][piece4[1]+1].getPerm()==false&&
      piece1[0]+2<=15&&piece2[0]+1<=15&&piece2[1]+1<=7&&piece4[0]-1>=0&&piece4[1]+1<=7){
        return true;
      }
      else{
        return false;
      }
    }
  }
}

// piece number (line=0, box=1, T=2, L=3, backwards L=4, Z=5, S=6)
void rotate_piece() {
  if(piece==0){ // line
    if(rotation==0||rotation==2){ // horizontial to vertical
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece2[0]][piece2[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]-1][piece1[1]+2].setTemp(true);
      board[piece2[0]][piece2[1]+1].setTemp(true);
      board[piece3[0]+1][piece3[1]].setTemp(true);
      board[piece4[0]+2][piece4[1]-1].setTemp(true);
      piece1[0] = piece1[0]-1;
      piece1[1] = piece1[1]+2;
      //piece2[0] = piece2[0];
      piece2[1] = piece2[1]+1;
      piece3[0] = piece3[0]+1;
      //piece3[1] = piece3[1];
      piece4[0] = piece4[0]+2;
      piece4[1] = piece4[1]-1;
    }
    else if(rotation==1||rotation==3){ // vertical to horizontial
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece2[0]][piece2[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]+1][piece1[1]-2].setTemp(true);
      board[piece2[0]][piece2[1]-1].setTemp(true);
      board[piece3[0]-1][piece3[1]].setTemp(true);
      board[piece4[0]-2][piece4[1]+1].setTemp(true);
      piece1[0] = piece1[0]+1;
      piece1[1] = piece1[1]-2;
      //piece2[0] = piece2[0];
      piece2[1] = piece2[1]-1;
      piece3[0] = piece3[0]-1;
      //piece3[1] = piece3[1];
      piece4[0] = piece4[0]-2;
      piece4[1] = piece4[1]+1;
    }
  }
  else if(piece==2){ // T
    if(rotation==0) {
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece3[0]-1][piece3[1]-1].setTemp(true);
      piece3[0] = piece3[0]-1;
      piece3[1] = piece3[1]-1;
    }
    else if(rotation==1) {
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece4[0]-1][piece4[1]+1].setTemp(true);
      piece4[0] = piece4[0]-1;
      piece4[1] = piece4[1]+1;
    }
    else if(rotation==2) {
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece1[0]+1][piece1[1]+1].setTemp(true);
      piece1[0] = piece1[0]+1;
      piece1[1] = piece1[1]+1;
    }
    else if(rotation==3) {  
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]-1][piece3[1]-1].setTemp(true);
      board[piece3[0]+1][piece3[1]+1].setTemp(true);
      board[piece4[0]+1][piece4[1]-1].setTemp(true);
      piece1[0] = piece1[0]-1;
      piece1[1] = piece1[1]-1;
      piece3[0] = piece3[0]+1;
      piece3[1] = piece3[1]+1;
      piece4[0] = piece4[0]+1;
      piece4[1] = piece4[1]-1;
    }
  }
  else if(piece==3){ // L
    if(rotation==0) {
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]-1][piece1[1]+1].setTemp(true);
      board[piece3[0]+1][piece3[1]-1].setTemp(true);
      board[piece4[0]-2][piece4[1]].setTemp(true);
      piece1[0] = piece1[0]-1;
      piece1[1] = piece1[1]+1;
      piece3[0] = piece3[0]+1;
      piece3[1] = piece3[1]-1;
      piece4[0] = piece4[0]-2;
      //piece4[1] = piece4[1];
    }
    else if(rotation==1) {
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]+1][piece1[1]+1].setTemp(true);
      board[piece3[0]-1][piece3[1]-1].setTemp(true);
      board[piece4[0]][piece4[1]+2].setTemp(true);
      piece1[0] = piece1[0]+1;
      piece1[1] = piece1[1]+1;
      piece3[0] = piece3[0]-1;
      piece3[1] = piece3[1]-1;
      //piece4[0] = piece4[0];
      piece4[1] = piece4[1]+2;
    }
    else if(rotation==2) {
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]+1][piece1[1]-1].setTemp(true);
      board[piece3[0]-1][piece3[1]+1].setTemp(true);
      board[piece4[0]+2][piece4[1]].setTemp(true);
      piece1[0] = piece1[0]+1;
      piece1[1] = piece1[1]-1;
      piece3[0] = piece3[0]-1;
      piece3[1] = piece3[1]+1;
      piece4[0] = piece4[0]+2;
      //piece4[1] = piece4[1];
    }
    else if(rotation==3) {
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]-1][piece1[1]-1].setTemp(true);
      board[piece3[0]+1][piece3[1]+1].setTemp(true);
      board[piece4[0]][piece4[1]-2].setTemp(true);
      piece1[0] = piece1[0]-1;
      piece1[1] = piece1[1]-1;
      piece3[0] = piece3[0]+1;
      piece3[1] = piece3[1]+1;
      //piece4[0] = piece4[0];
      piece4[1] = piece4[1]-2; 
    } 
  }
  else if(piece==4){ // backwards L
     if(rotation==0) {
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]-1][piece1[1]+1].setTemp(true);
      board[piece3[0]+1][piece3[1]-1].setTemp(true);
      board[piece4[0]][piece4[1]-2].setTemp(true);
      piece1[0] = piece1[0]-1;
      piece1[1] = piece1[1]+1;
      piece3[0] = piece3[0]+1;
      piece3[1] = piece3[1]-1;
      //piece4[0] = piece4[0];
      piece4[1] = piece4[1]-2;
    }
    else if(rotation==1) {
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]+1][piece1[1]+1].setTemp(true);
      board[piece3[0]-1][piece3[1]-1].setTemp(true);
      board[piece4[0]-2][piece4[1]].setTemp(true);
      piece1[0] = piece1[0]+1;
      piece1[1] = piece1[1]+1;
      piece3[0] = piece3[0]-1;
      piece3[1] = piece3[1]-1;
      piece4[0] = piece4[0]-2;
      //piece4[1] = piece4[1];
    }
    else if(rotation==2) {
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]+1][piece1[1]-1].setTemp(true);
      board[piece3[0]-1][piece3[1]+1].setTemp(true);
      board[piece4[0]][piece4[1]+2].setTemp(true);
      piece1[0] = piece1[0]+1;
      piece1[1] = piece1[1]-1;
      piece3[0] = piece3[0]-1;
      piece3[1] = piece3[1]+1;
      //piece4[0] = piece4[0];
      piece4[1] = piece4[1]+2;
    }
    else if(rotation==3) {
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]-1][piece1[1]-1].setTemp(true);
      board[piece3[0]+1][piece3[1]+1].setTemp(true);
      board[piece4[0]+2][piece4[1]].setTemp(true);
      piece1[0] = piece1[0]-1;
      piece1[1] = piece1[1]-1;
      piece3[0] = piece3[0]+1;
      piece3[1] = piece3[1]+1;
      piece4[0] = piece4[0]+2;
      //piece4[1] = piece4[1]; 
    } 
  }
  else if(piece==5){ // Z
    if(rotation==0||rotation==2){
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece2[0]][piece2[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]-1][piece1[1]+2].setTemp(true);
      board[piece2[0]][piece2[1]+1].setTemp(true);
      board[piece3[0]-1][piece3[1]].setTemp(true);
      board[piece4[0]][piece4[1]-1].setTemp(true);
      piece1[0] = piece1[0]-1;
      piece1[1] = piece1[1]+2;
      //piece2[0] = piece2[0];
      piece2[1] = piece2[1]+1;
      piece3[0] = piece3[0]-1;
      //piece3[1] = piece3[1];
      //piece4[0] = piece4[0];
      piece4[1] = piece4[1]-1;    
    }
    else if(rotation==1||rotation==3){
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece2[0]][piece2[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      board[piece1[0]+1][piece1[1]-2].setTemp(true);
      board[piece2[0]][piece2[1]-1].setTemp(true);
      board[piece3[0]+1][piece3[1]].setTemp(true);
      board[piece4[0]][piece4[1]+1].setTemp(true);
      piece1[0] = piece1[0]+1;
      piece1[1] = piece1[1]-2;
      //piece2[0] = piece2[0];
      piece2[1] = piece2[1]-1;
      piece3[0] = piece3[0]+1;
      //piece3[1] = piece3[1];
      //piece4[0] = piece4[0];
      piece4[1] = piece4[1]+1;
    }
  }
  else if(piece==6){ // S
    if(rotation==0||rotation==2){
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece2[0]][piece2[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      
      board[piece1[0]-2][piece1[1]].setTemp(true);
      board[piece2[0]-1][piece2[1]-1].setTemp(true);
      board[piece3[0]][piece3[1]].setTemp(true);
      board[piece4[0]+1][piece4[1]-1].setTemp(true);
      
      piece1[0] = piece1[0]-2;
      //piece1[1] = piece1[1];
      piece2[0] = piece2[0]-1;
      piece2[1] = piece2[1]-1;
      //piece3[0] = piece3[0];
      //piece3[1] = piece3[1];
      piece4[0] = piece4[0]+1;
      piece4[1] = piece4[1]-1;    
    }
    else if(rotation==1||rotation==3){
      board[piece1[0]][piece1[1]].setTemp(false);
      board[piece2[0]][piece2[1]].setTemp(false);
      board[piece3[0]][piece3[1]].setTemp(false);
      board[piece4[0]][piece4[1]].setTemp(false);
      
      board[piece1[0]+2][piece1[1]].setTemp(true);
      board[piece2[0]+1][piece2[1]+1].setTemp(true);
      board[piece3[0]][piece3[1]].setTemp(true);
      board[piece4[0]-1][piece4[1]+1].setTemp(true);
      
      piece1[0] = piece1[0]+2;
      //piece1[1] = piece1[1];
      piece2[0] = piece2[0]+1;
      piece2[1] = piece2[1]+1;
      //piece3[0] = piece3[0];
      //piece3[1] = piece3[1];
      piece4[0] = piece4[0]-1;
      piece4[1] = piece4[1]+1;
    }
  }
  if(rotation==3){
    rotation=0;
  }
  else{
    rotation = rotation+1;
  }
}

void shift_down() {
  board[piece1[0]][piece1[1]].setTemp(false);
  board[piece2[0]][piece2[1]].setTemp(false);
  board[piece3[0]][piece3[1]].setTemp(false);
  board[piece4[0]][piece4[1]].setTemp(false);
  board[piece1[0]+1][piece1[1]].setTemp(true);
  board[piece2[0]+1][piece2[1]].setTemp(true);
  board[piece3[0]+1][piece3[1]].setTemp(true);
  board[piece4[0]+1][piece4[1]].setTemp(true);
  piece1[0] = piece1[0]+1; // piece1[1] stays constant
  piece2[0] = piece2[0]+1; // piece2[1] stays constant
  piece3[0] = piece3[0]+1; // piece3[1] stays constant
  piece4[0] = piece4[0]+1; // piece4[1] stays constant
}

void shift_left() {
  board[piece1[0]][piece1[1]].setTemp(false);
  board[piece2[0]][piece2[1]].setTemp(false);
  board[piece3[0]][piece3[1]].setTemp(false);
  board[piece4[0]][piece4[1]].setTemp(false);
  board[piece1[0]][piece1[1]-1].setTemp(true); 
  board[piece2[0]][piece2[1]-1].setTemp(true);
  board[piece3[0]][piece3[1]-1].setTemp(true);
  board[piece4[0]][piece4[1]-1].setTemp(true);
  piece1[1] = piece1[1]-1; // piece1[0] stays constant
  piece2[1] = piece2[1]-1; // piece2[0] stays constant
  piece3[1] = piece3[1]-1; // piece3[0] stays constant
  piece4[1] = piece4[1]-1; // piece4[0] stays constant
}

void shift_right() {
  board[piece1[0]][piece1[1]].setTemp(false);
  board[piece2[0]][piece2[1]].setTemp(false);
  board[piece3[0]][piece3[1]].setTemp(false);
  board[piece4[0]][piece4[1]].setTemp(false);
  board[piece1[0]][piece1[1]+1].setTemp(true);
  board[piece2[0]][piece2[1]+1].setTemp(true);
  board[piece3[0]][piece3[1]+1].setTemp(true);
  board[piece4[0]][piece4[1]+1].setTemp(true);
  piece1[1] = piece1[1]+1; // piece1[0] stays constant
  piece2[1] = piece2[1]+1; // piece2[0] stays constant
  piece3[1] = piece3[1]+1; // piece3[0] stays constant
  piece4[1] = piece4[1]+1; // piece4[0] stays constant
}

int check_rows() {
  bool del=true;
  int count=0;
  for(int i=15;i>=0;i--)
  {
    for(int j=7;j>=0;j--)
    {
      if(board[i][j].getPerm()==false)
      {
        del=false;
      }
    }
    if (del==false)
    {
      del=true;
    }
    else
    {
      for(int k=i;k>0;k--)
       {
        for(int l=0;l<8;l++)
        {
          board[k][l]=board[k-1][l];
        }
      }
      for(int f=0;f<8;f++)
      {
        board[0][f].setTemp(false);
        board[0][f].setPerm(false);
      }
      count++;
      del=true;
      i++;
    }
  }
  return count;
}

void refresh()
{
  matrix.clear();
  for (int i=0; i<8; i++)
  {
    for (int j=0; j<16; j++)
    {
      if(board[j][i].getPerm()==true || board[j][i].getTemp()==true)
      {
        matrix.drawPixel(i,j,LED_ON);
      }
    }
  }
  matrix.writeDisplay();
  
}

void lose()
{
  bool lose = false;
  bool reset = false;
  for(int i=0; i<1; i++){
    for(int j=2; j<6; j++){
      if(board[i][j].getPerm()==true){
        lose = true;
      }
    }
  }
  while(lose==true){
   digitalWrite(musPin,LOW);
   matrix.setTextSize(1);
   matrix.setTextWrap(false);
   matrix.setTextColor(LED_ON);
   for (int8_t x=7; x>=-80; x--){
     matrix.clear();
     matrix.setCursor(x,0);
     String score_str= String(score);
     matrix.print("Loser Score:" + score_str);
     matrix.writeDisplay();
     delay(100);
     checkButtons();
     if(rotate==HIGH){
      lose=false;
      reset=true;
      digitalWrite(musPin,HIGH);
      break;
     }
   }
  }
  if(reset==true){
    for(int i=0; i<16; i++){
      for(int j=0; j<8; j++){
        board[i][j].setTemp(false);
        board[i][j].setPerm(false);
      }
    }
    score = 0;
    reset=false;
  }
}

void lock()
{
  board[piece1[0]][piece1[1]].setPerm(true);
  board[piece1[0]][piece1[1]].setTemp(false);
  board[piece2[0]][piece2[1]].setPerm(true);
  board[piece2[0]][piece2[1]].setTemp(false);
  board[piece3[0]][piece3[1]].setPerm(true);
  board[piece3[0]][piece3[1]].setTemp(false);
  board[piece4[0]][piece4[1]].setPerm(true);
  board[piece4[0]][piece4[1]].setTemp(false);
}

