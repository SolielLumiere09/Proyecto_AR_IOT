using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using uPLibrary.Networking.M2Mqtt.Messages;

public class ScrollText : MonoBehaviour
{
    public DashBoards dashBoards;
    private string topicName = "ScrollText";

    private string inputString = "MSG False";
    public InputField TextToScroll;
    public Toggle Status;
    
    // Start is called before the first frame update
    void Start()
    {
       
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void OnInputText()
    {
        inputString = TextToScroll.text + " " + Status.isOn;
        Debug.Log("msg = " + inputString);
        
        
        dashBoards.Client.Publish(topicName, System.Text.Encoding.UTF8.GetBytes(inputString),
            MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
    }

    public void check_msg_status(string msg)
    {

        if (!inputString.Equals(msg))
        {
            dashBoards.Client.Publish(topicName, System.Text.Encoding.UTF8.GetBytes(inputString),
                MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
        }
    }
}
