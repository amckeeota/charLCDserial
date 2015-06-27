#define enbl 13
#define srl 11
#define clk 8
#define LCDpow 9

String line1,line2;

void clock(){
  delay(1);
  digitalWrite(clk,HIGH);
  delay(1);
  digitalWrite(clk,LOW);
  delay(1);
}

void enable(){
  delay(1);
  digitalWrite(enbl,HIGH);
  delay(1);
  digitalWrite(enbl,LOW);
  delay(1);
}

//6 LSB are (MSB) D7,D6,D5,D4,RW,RS (LSB)
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
  
  //type name
  char amot[16]="String 1";
  displayString(1,amot);
  char illw[16]="String 2";
  displayString(2,illw);
}

void loop() {


}
