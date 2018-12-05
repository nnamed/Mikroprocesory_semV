#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <VirtualWire.h>
#include <DS3231.h>

#define LCD_ADDR 0x27

LiquidCrystal_PCF8574 lcd(LCD_ADDR);

DS3231 clock;

const int led_pin = 13;
const int transmit_pin = 12;
const int receive_pin = 11;
const int transmit_en_pin = 3;
int przycisk = 7;
int zmienna = 0;
void setup()
{

    Wire.begin();
    lcd.begin(16, 2);
    lcd.setBacklight(true);

    lcd.print("Witaj!");
    delay(3000);
    lcd.clear();
    
    delay(1000);
    

    // Initialise the IO and ISR
    
    vw_set_rx_pin(receive_pin);
    vw_set_ptt_pin(transmit_en_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	 // Bits per sec

    vw_rx_start();       // Start the receiver PLL running
Serial.begin(9600);
Serial.println("Setup");
    pinMode(led_pin, OUTPUT);
    pinMode(7, INPUT_PULLUP);

   
}

void loop()
{
  
  if(digitalRead(7) == LOW && zmienna == 0){
    
    zmienna = 1;
    delay(300);
  }
  else if(digitalRead(7) == LOW && zmienna == 1){
  
    zmienna = 0;
    delay(300);
  }

  switch(zmienna){
    
    case 0:
    lcd.clear();
    displayDateTime();
    break;

    case 1:
    lcd.clear();
    delay(1000);
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
  int i;

        digitalWrite(led_pin, HIGH); // Flash a light to show received good message
  // Message with a good checksum received, dump it.
  
  
  for (i = 0; i < buflen; i++)
  { 
    //lcd.clear();
    
      lcd.print("Jasn.");
      lcd.setCursor(6,0);
      double jasn= (buf[i]);
      lcd.print(jasn);
      lcd.print('%');
      
    lcd.setCursor(0,1);
    lcd.print("Wil.Pow.");
    break;
  }
        digitalWrite(led_pin, LOW);
    }
    
    
  } 
  
  delay(1000);
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
	int i;

        digitalWrite(led_pin, HIGH); // Flash a light to show received good message
	// Message with a good checksum received, dump it.
	Serial.print("Got: ");
	
	for (i = 0; i < buflen; i++)
	{
	    Serial.print(buf[i], DEC);
	    Serial.print(' ');
	}
	Serial.println();
        digitalWrite(led_pin, LOW);
    }
}

void displayDateTime(){
  lcd.clear();
  lcd.print(getDateString());
  lcd.setCursor(0, 1);
  lcd.print(getTimeString());

}

String getDateString() {
  bool century;

  String day = toStringWithLeadingZeros(clock.getDate());
  String month = toStringWithLeadingZeros(clock.getMonth(century));
  String year = String(clock.getYear() + 2000);

  return day + "." + month + "." + year + "r.";
}

String getTimeString(){
  bool h12, pm;

  String hour = toStringWithLeadingZeros(clock.getHour(h12, pm));
  String minute = toStringWithLeadingZeros(clock.getMinute());
  String second = toStringWithLeadingZeros(clock.getSecond());

  return hour + ":" + minute + ":" + second;
}

String toStringWithLeadingZeros(byte number){
  String result = String(number);
  if(number<10){
    result = "0" + result;
  }
  return result;
  }


