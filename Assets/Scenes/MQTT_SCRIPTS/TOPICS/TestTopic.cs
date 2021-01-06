using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using uPLibrary.Networking.M2Mqtt.Messages;

public class TestTopic : MonoBehaviour
{
    public DashBoards dashBoards;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void onClickButton()
    {

        dashBoards.Client.Publish("Arduino1", System.Text.Encoding.UTF8.GetBytes("Hola desde Unity"),
            MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);

    }
}
