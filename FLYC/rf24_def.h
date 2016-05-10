#define Angle_Max 40.0

#define P_ENABLE 0x5A
#define I_ENABLE 0x5A
#define D_ENABLE 0x5A

RF24 radio(7, 8);

char FLY_ENABLE = 0;  // Enable Fly flag

const byte rxAddr[6] = "00001";
unsigned int recBuf[32] = {0};

