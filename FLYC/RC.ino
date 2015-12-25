void nrfSet() {
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  
  radio.startListening();
  Serial.println("nrf start");
}
void getExp() {
  radio.read(&recBuf, sizeof(recBuf));
    
  Serial.println(recBuf);
}

