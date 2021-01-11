/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
#include <MQTT.h>
#include <ScrollText.h>

#include <util/delay.h>
MQTT *mqtt;
ScrollText *scrollText;




void setup() {
	//log para el ESP8266
	Serial.begin(19200);
	DDRA = 1 << PA0 | 1 << PA1 | 1 << PA2 | 1 << PA3 | 1 << PA4 | 1 << PA6;
	PORTA |= (1 << PA5);
	mqtt = new MQTT();
	scrollText = new ScrollText(51, 53, 52, 4);
	
	delay(1000);
}
void loop() {
	mqtt->update();
	scrollText->update();
	
	
	if(!(PINA & (1 << PA5))){
		PORTA &= ~(1 << PA4) & ~(1 << PA3) & ~(1 << PA6);
	}
	

}

