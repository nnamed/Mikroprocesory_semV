#include <idDHT11.h>
#include <VirtualWire.h>

#define led_pin 11
#define transmit_pin 12
#define idDHT11pin 2
#define idDHT11intNumber 0

void dht11_wrapper();
idDHT11 DHT11(idDHT11pin, idDHT11intNumber, dht11_wrapper);

String toSend = "";

void setup() {
  Serial.begin(9600);
  vw_setup(2000);
  vw_set_tx_pin(transmit_pin);
}

void dht11_wrapper()
{
  DHT11.isrCallback();
}



void loop() {
  DHT11.acquire();
  while (DHT11.acquiring());
  int result = DHT11.getStatus();
  Serial.println(DHT11.getStatus());
  switch (result) {
    case IDDHTLIB_OK:
    
      break;
  }
  int temp = DHT11.getCelsius();
  int hum = DHT11.getHumidity();

  toSend = String(temp, DEC) + "$" + String(hum, DEC) + "$";
  char msg[15]; //char array to copy the String into

  toSend.toCharArray(msg, toSend.length() + 1); //Copy the string (+1 is to hold the terminating null char)
  Serial.println(msg);

  digitalWrite(led_pin, HIGH);
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx();
  digitalWrite(led_pin, LOW);

  delay(500);
}

