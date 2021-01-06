/* 
* MQTT.cpp
*
* Created: 26/12/2020 01:17:14 p. m.
* Author: Omar Valdivia
*/


#include "MQTT.h"

int MQTT::pwm_value = 0;
const char* MQTT::ARDUINO_FAN_SUB = "Arduinofan";
const char* MQTT::ARDUINO_LAMP_SUB = "ArduinoLamp";
const char* MQTT::ARDUINO_COUNTER_SUB = "ArduinoCounter";
const char* MQTT::PWM_STATUS_TOPIC = "pwm_status";
const char* MQTT::SCROLLTEXT_STATUS_TOPIC = "msg_status";
const char* MQTT::LAMP_STATUC_TOPIC = "lamp_status";
const char* MQTT::TEMPERATURE_SENSOR_TOPIC = "ArduinoTemperatureSensor";
const char* MQTT::ARDUINO_LOCK_DOOR = "ArduinoLockDoor";
const char* MQTT::ARDUINO_LOCK_PASSWORD = "1234567890";
MQTT::MQTT()
{
	InitWiFi();
	
	espClient = new WiFiEspClient();

	client = new PubSubClient(*espClient);
	
	client->setSocketTimeout(3);

	client->setServer(server, 1883);


	status = WiFi.status();
	if(status != WL_CONNECTED) {
		
		reconnectWifi();
	}

	if(!client->connected() ) {
		reconnectClient();
	}
	client->setCallback(MQTT::callback);
	lastSend = 0;
	
} 
void MQTT::sendTemperature(){
	float temperature = readTemperature();
	
	Serial.print("Temperature = ");
	Serial.println(readTemperature());
	

	String payload = "Temperature = " + String(temperature);

	char attributes[payload.length() + 1];
	payload.toCharArray( attributes, payload.length() + 1);
	
	client->publish( TEMPERATURE_SENSOR_TOPIC, attributes );
}
float MQTT::readTemperature(){
	float lecture = analogRead(A0);
	float Vout =(float) (lecture * 5.f) / 1023.f;
	float TC = 0.02f;
	float V0 = 0.40f;
	float TA = (Vout - V0) / TC;
	
	return TA;
	
}
void MQTT::update(){
		
		if(millis() - lastSend > 5000 || id_msg > 0) {
			id_msg++;
			
			if(id_msg == 1)
				sendScrollTextStatus();
			
			if(id_msg == 2){
				sendPwmStatus();
				
			}
			if(id_msg == 3){
				sendLampStatus();
				
			}
			if(id_msg == 4){
				sendTemperature();
				id_msg = 0;
			}
			
			delay(10);
			lastSend = millis();
		}
	
	if(espClient->available()){
		client->loop();
		espClient->flush();
	}
	
	
	if(millis() - loopRefresh > 2000){
	
		status = WiFi.status();
		if(status != WL_CONNECTED) {
			
			reconnectWifi();
		}
		
		reconnectClient();
		loopRefresh = millis();
	}

}
void MQTT::sendLampStatus(){
	Serial.print("LampStatus = ");
	Serial.println(DDRA & (1 << PA0));
	

	String payload = "LampStatus = " + String(PORTA & (1 << PA0) ? "True" : "False");

	char attributes[payload.length() + 1];
	payload.toCharArray( attributes, payload.length() + 1);
	
	client->publish( LAMP_STATUC_TOPIC, attributes );
	
	
}
void MQTT::sendPwmStatus(){
	
	Serial.print("pwmStatus = ");
	Serial.println(MQTT::pwm_value);
	

	String payload = "PWM = " + String(MQTT::pwm_value);

	char attributes[payload.length() + 1];
	payload.toCharArray( attributes, payload.length() + 1);
	
	client->publish( PWM_STATUS_TOPIC, attributes );

	
}
void MQTT::sendScrollTextStatus(){
	Serial.print("TextStatus = ");
	Serial.println(ScrollText::msgToShow +" "+ String(ScrollText::enable));

	String payload = "ScrollText = " + ScrollText::msgToShow + " " + String(ScrollText::enable ? "True" : "False");

	char attributes[payload.length() + 1];
	payload.toCharArray( attributes, payload.length() + 1);
	
	client->publish( SCROLLTEXT_STATUS_TOPIC, attributes );
	
	
}
void MQTT::InitWiFi(){
	
	Serial1.begin(19200);
	WiFi.init(&Serial1);
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println("El modulo WiFi no esta presente");
		while (true);
	}
	reconnectWifi();
}
void MQTT::reconnectWifi(){
	Serial.println("Iniciar conección a la red WIFI");
	while(status != WL_CONNECTED) {
		Serial.print("Intentando conectarse a WPA SSID: ");
		Serial.println(WIFI_AP);
		
		status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
		delay(500);
	}
	Serial.println("Conectado a la red WIFI");
	
}
void MQTT::reconnectClient(){
	
	
	while(!client->connected()) {
		Serial.print("Conectando a: ");
		Serial.println(server);
				
		String clientId = "ESP8266Client-Arduino";

		if(client->connect(clientId.c_str())) {
			Serial.println("[DONE]");
			client->subscribe(ARDUINO_FAN_SUB, 1);
			client->subscribe(ScrollText::SCROLL_TEXT_SUB, 1);
			client->subscribe(ARDUINO_LAMP_SUB, 1);
			client->subscribe(ARDUINO_COUNTER_SUB, 1);
			client->subscribe(ARDUINO_LOCK_DOOR, 1);
			}
		else{
			Serial.print( "[FAILED] [ rc = " );
			Serial.print( client->state() );
			Serial.println( " : retrying in 5 seconds]" );
			delay( 1000 );

		}
		
	}
}
void MQTT::callback(char* topic, byte* payload, unsigned int length){
	String msg;
	
	msg.reserve(length);
	
	if(Serial.availableForWrite()){
		Serial.print("Message arrived [");
		Serial.print(topic);
		Serial.print("] ");
		for (int i=0;i<length;i++) {
			msg += (char)payload[i];
		}
		Serial.println(msg);
	}

	if(strcmp(topic, MQTT::ARDUINO_FAN_SUB) == 0){//Si el topico es para el ventilador
		MQTT::pwm_value = msg.toInt();
		analogWrite(PWM_MOTOR, MQTT::pwm_value);
	}
	if(strcmp(topic, ScrollText::SCROLL_TEXT_SUB) == 0){//Si el topico es para mostrar texto
		
		if(msg.indexOf("True") > 0){
			msg.replace("True", "");
			ScrollText::enable = true;
		}
		else{
			msg.replace("False", "");
			ScrollText::enable = false;
		}
		msg.trim();
		ScrollText::msgToShow = msg;
		ScrollText::msg_aux = msg;
		ScrollText::msg_aux += ' ';
	}
	if(strcmp(topic, MQTT::ARDUINO_LAMP_SUB) == 0){
		if(msg.equals("False")){
			PORTA |= 1 << PA0;
		}
		else{
			PORTA &= ~(1 << PA0);
		}
	}
	if(strcmp(topic, MQTT::ARDUINO_COUNTER_SUB) == 0){
		if(msg.equals("True")){
			PORTA |= 1 << PA1;
			delay(1);
			PORTA &= ~(1 << PA1);
	
		}
		else{

			PORTA |= 1 << PA2;
			delay(1);
			PORTA &= ~(1 << PA2);
		}
	}
	
	if(strcmp(topic, ARDUINO_LOCK_DOOR) == 0){
		msg.trim();
		if(msg.equals(ARDUINO_LOCK_PASSWORD))
		{
			PORTA |= 1 << PA3;
			PORTA &= ~(1 << PA4);
		}
		else{
			PORTA |= 1 << PA4;
			PORTA &= ~(1 << PA3);
		}
	}
	
	

	
	
}
// default destructor
MQTT::~MQTT()
{
	delete server;
	delete espClient;
	delete client;
} //~MQTT
