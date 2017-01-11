#define pinCLK  7
#define pinCS   6
#define pinDIN  5

void WriteByte(char data) {
  int i;
  for(i = 8; i >= 1; i--) {
    digitalWrite(pinCLK, LOW);
    if(data&0x80) {
      digitalWrite(pinDIN, HIGH);
    }
    else {
      digitalWrite(pinDIN, LOW);
    }
    data = data<<1;
    digitalWrite(pinCLK, HIGH);
  }
}
void WriteData(char address, char dat) {
  digitalWrite(pinCS, LOW);
  WriteByte(address);
  WriteByte(dat);
  digitalWrite(pinCS, HIGH);
}
void Init(void) {
  pinMode(pinCLK, OUTPUT);
  pinMode(pinCS,  OUTPUT);
  pinMode(pinDIN, OUTPUT);
  
  WriteData(0x09, 0x00);
  WriteData(0x0a, 0x03);
  WriteData(0x0b, 0x07);
  WriteData(0x0c, 0x01);
  WriteData(0x0f, 0x00);

  for(char j = 0; j < 8; j++) {
    WriteData(j+1, 0);  // shutdown all display
  }
}

void setup(void) {
  delay(50);
  Init();
  randomSeed(analogRead(0));
}
int delay_time=1000;
int _direction=-1;
char d[8]={0x00};
void loop(void) {
  for(char j = 0; j < 8; j++) {
    WriteData(j+1, d[j]);
  }
  for(char i=0;i<8;i++) {
    d[i] = 0xff<<random(0,8);
  }
  if(delay_time==100) {
    _direction=1;
  } else if(delay_time==2000) {
    _direction=-1;
  }
  delay_time+=_direction*100;
  delay(delay_time);
}
