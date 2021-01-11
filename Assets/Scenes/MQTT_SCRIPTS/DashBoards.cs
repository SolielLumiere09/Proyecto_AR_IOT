using System;
using System.Net;
using UnityEngine;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

public class DashBoards : MonoBehaviour
{
    public string brokerIpAddress = "127.0.0.1";
    public int brokerPort = 1883;
    private MqttClient client;
    string lastMessage;

    
    
    //subscribers
    public Handler handler;
    public ScrollText scrollText;
    public LampController lampController;
    public TemperatureController temperatureController;
    public LuminiscenseSensor luminiscenseSensorController;
    
    private String pwm_status = "pwm_status";
    private String msg_status = "msg_status";
    private string lamp_status = "lamp_status";
    private string temperature_status = "ArduinoTemperatureSensor";
    private string luminiscense_sensor = "ArduinoLuminiscense";
    
    
    void Start()
    {
        client = new MqttClient(IPAddress.Parse(brokerIpAddress), brokerPort, false, null); 
        client.MqttMsgPublishReceived += client_MqttMsgPublishReceived; 
        
        string clientId = Guid.NewGuid().ToString(); 
        client.Connect(clientId); 
        
        client.Subscribe(new string[] { pwm_status }, new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });
        client.Subscribe(new string[] { msg_status }, new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });
        client.Subscribe(new string[] { lamp_status }, new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });
        client.Subscribe(new string[] { temperature_status }, new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });
        client.Subscribe(new string[] { luminiscense_sensor }, new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });

        
    }

    void client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
    {
        Debug.Log("Received: " + System.Text.Encoding.UTF8.GetString(e.Message));
        lastMessage = System.Text.Encoding.UTF8.GetString(e.Message);


        if (lastMessage.Contains("PWM"))
        {
            lastMessage = lastMessage.Replace("PWM = ", "");
            lastMessage = lastMessage.Trim();

           
            handler.check_pwm_status(int.Parse(lastMessage));

        }

        if (lastMessage.Contains("ScrollText"))
        {
            lastMessage = lastMessage.Replace("ScrollText = ", "");
            lastMessage = lastMessage.Trim();

           
            scrollText.check_msg_status(lastMessage);
        }

        if (lastMessage.Contains("LampStatus"))
        {
            
            if(lastMessage.Contains("True"))
                lampController.checkLampStatus(true);
            else
                lampController.checkLampStatus(false);

        }

        if (lastMessage.Contains("Temperature"))
        {
            lastMessage = lastMessage.Replace("Temperature = ", "");
            lastMessage = lastMessage.Trim();
            
            temperatureController.RequestUpdateText(float.Parse(lastMessage));
        }

        if (lastMessage.Contains("luminescence"))
        {
            lastMessage = lastMessage.Replace("luminescence = ", "");
            lastMessage = lastMessage.Trim();
            Debug.Log("luminscense = " + lastMessage);
            luminiscenseSensorController.SetLuminiscense(float.Parse(lastMessage));
        }
    }

    // Update is called once per frame
    void Update()
    {

        
    }
    
    void OnApplicationQuit()
    {
        client.Disconnect();
    }


    public MqttClient Client => client;
}
