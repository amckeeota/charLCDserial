//
//***SHIFT REG LAYOUT***
//  Bit 5  D7
//  Bit 4  D6
//  Bit 3  D5
//  Bit 2  D4
//  Bit 1  RW
//  Bit 0  RS
//
//
//


#define enbl 13
#define srl 11
#define clk 8
#define LCDpow 9

char m[16];

String line1,line2;

void clock(){
  delayMicroseconds(10);
  digitalWrite(clk,HIGH);
  delayMicroseconds(10);
  digitalWrite(clk,LOW);
  delayMicroseconds(10);
}

void enable(){
  delayMicroseconds(10);
  digitalWrite(enbl,HIGH);
  delayMicroseconds(10);
  digitalWrite(enbl,LOW);
  delayMicroseconds(10);
}


void sendMessage(unsigned char msg){
  for(int i=0;i<6;i++){
    if(msg&0b100000)
      digitalWrite(srl,HIGH);
    else
      digitalWrite(srl,LOW);
    
    clock();
    msg=msg<<1;
  }
  enable();
}

void clearSR(){
  digitalWrite(srl,LOW);
  for(int i=0;i<6;i++){
    clock();
  }
}

void setDDRAM(int aDDy){
  sendMessage(((aDDy>>2)&0b111100)+0b100000);
  sendMessage((aDDy<<2)&0b111100);
}

    
void displayString(int lineNum,char *msg)
{
  if(lineNum==1){
    int j=0;
    //go to beginning of line and fill with spaces. Then return to beginning of line.
    setDDRAM(0);
    for(int i=0;i<16;i++){
      sendMessage(0b001001);
      sendMessage(0b000001);
    }
    setDDRAM(0);
    while(msg[j]!='\0'){
      sendMessage(((msg[j]>>2)&0b111100)|1);
      sendMessage(((msg[j]<<2)&0b111100)|1);
      j++;
    }
  }
  else{
    int j=0;
    setDDRAM(64);
    for(int i=0;i<16;i++){
      sendMessage(0b001001);
      sendMessage(0b000001);
    }
    setDDRAM(64);
    while(msg[j]!='\0'){
      sendMessage(((msg[j]>>2)&0b111100)|1);
      sendMessage(((msg[j]<<2)&0b111100)|1);
      j++;
    }
  }
}

void drawChar(int charIndex, char *drawing){

  //format the character index by choosing the correct row and
  //adding a bit in position D6 to signal CG RAM address info
  char formattedIndex = charIndex * 8 + (1<<6);
  
  //send start address for CG RAM characters
  sendMessage((formattedIndex>>2) & 0b111100);
  sendMessage((formattedIndex<<2) & 0b111100);

  //start drawing character
  for(int i=0;i<8;i++){
    sendMessage(((drawing[i]>>2) & 0b111100) + 1);
    sendMessage(((drawing[i]<<2) & 0b111100) + 1);
  }

//    sendMessage(0b101);
//    sendMessage(0b101);
  setDDRAM(0);
}

void showPercent(int p,char *l){
  l[15]='%';
  int ones;
  ones=p%10;
  l[14]=ones+48;
  l[13]=p/10+48;
  
}
  
void setup() {
  pinMode(enbl, OUTPUT);
  pinMode(srl, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(LCDpow,OUTPUT);
  
  
  digitalWrite(enbl, LOW);
  digitalWrite(srl,LOW);
  digitalWrite(clk,LOW);
  digitalWrite(LCDpow,LOW);
  delay(1000);
  digitalWrite(LCDpow,HIGH);
  delay(1000);
  clearSR();
  
  //set to 4 bit mode
  sendMessage(0b001000);
  //set to 8x2 display && 5x8 char size
  sendMessage(0b001000);
  sendMessage(0b100000);
  
  //Turn on display and blink it!
  sendMessage(0b000000);
  sendMessage(0b111100);
  
  //Set display to move forward
  sendMessage(0b000000);
  sendMessage(0b011000);  
  
  char test[8]={
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000};

  char test1[8]={
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000};


  char test2[8]={
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100};

  char test3[8]={
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110};
  
  
  char test4[8]={
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111};
  
  drawChar(0,test);
  drawChar(1,test1);
  drawChar(2,test2);
  drawChar(3,test3);
  drawChar(4,test4);
  
  
  //type name
  char amot[16]="String 1";
  displayString(1,amot);
  
  //turn off cursor
  sendMessage(0);
  sendMessage(0b110000);
}

void loop() {
  int percent=0;
  for(int k=0;k<10;k++){
    m[k]=0;
    displayString(2,m);
    percent+=2;
    showPercent(percent,m);
    delay(100);
    m[k]=1;
    displayString(2,m);
    percent+=2;
    showPercent(percent,m);
    delay(100);
    m[k]=2;
    displayString(2,m);
    percent+=2;
    showPercent(percent,m);
    delay(100);
    m[k]=3;
    displayString(2,m);
    percent+=2;
    showPercent(percent,m);
    delay(100);
    m[k]=4;
    displayString(2,m);
    percent+=2;
    showPercent(percent,m);
    delay(100);
    m[k]=0xff;
  }
  for(int k=0;k<16;k++){
    m[k]=' ';
  }

}
