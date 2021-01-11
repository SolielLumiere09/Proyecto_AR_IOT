/* 
* MQTT.h
*
* Created: 26/12/2020 01:17:14 p. m.
* Author: Omar Valdivia
*/


#ifndef __MQTT_H__
#define __MQTT_H__

#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
#include <string.h>
#include <ScrollText.h>

//Conexión a la red wifi: nombre de la red y contraseña
#define WIFI_AP "INFINITUM82A1_2.4"
#define WIFI_PASSWORD "TEFEHdG0ut"

#define PWM_MOTOR 2

//TOPICS



class MQTT
{

private:
	char *server = "192.168.1.65";
	WiFiEspClient *espClient;
	PubSubClient *client;
	unsigned long lastSend = 0;
	unsigned long loopRefresh = 0;
	int status = WL_IDLE_STATUS;
	uint8_t id_msg = 0;
	uint8_t MAX_TOPICS = 3;
	
	
	//keep validating pwm_value
	static int pwm_value; //valor actual del pwm
	static const char* PWM_STATUS_TOPIC; //topico para mandar el estado del pwm
	static const char* SCROLLTEXT_STATUS_TOPIC;//topico para mandar el estado del scrolltext
	static const char* LAMP_STATUC_TOPIC;//topico para mandar el estado de la lampara
	static const char* TEMPERATURE_SENSOR_TOPIC;//topico para mandar el sensor
	//subscription
	static const char* ARDUINO_FAN_SUB;
	static const char* ARDUINO_LAMP_SUB;
	static const char* ARDUINO_COUNTER_SUB;
	static const char* ARDUINO_LOCK_DOOR;
	static const char* ARDUINO_LOCK_PASSWORD;
	static const char* ARDUINO_LUMINISCENSE;

//functions
public:
	static boolean DOOR_STATUS;
	MQTT();
	void sendPwmStatus();
	void sendScrollTextStatus();
	void sendLampStatus();
	void InitWiFi();
	void reconnectWifi();
	void reconnectClient();
	void sendTemperature();
	void sendLuminiscense();
	float readTemperature();
	float readLuminiscense();
	static void callback(char* topic, byte* payload, unsigned int length);
	void update();

	~MQTT();

private:
	MQTT( const MQTT &c );
	MQTT& operator=( const MQTT &c );

}; //MQTT

#endif //__MQTT_H__
