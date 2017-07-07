char *pc;

#define printline \
{Serial.print((int)pc, HEX); \
Serial.print("\t"); \
Serial.println((int)*pc, HEX); \
pc--;}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("ok");
  pc = SP;
  printline;
  printline;
  printline;
  printline;
}

void loop() {
  // put your main code here, to run repeatedly:
}
