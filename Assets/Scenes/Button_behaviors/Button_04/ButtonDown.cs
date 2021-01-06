using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using uPLibrary.Networking.M2Mqtt.Messages;

public class ButtonDown : MonoBehaviour
{
    public Animator animator;
    private string _topicName = "ArduinoCounter";
    public DashBoards dashBoards;
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnMouseDown()
    {
        animator.Play("ButtonDown");
        dashBoards.Client.Publish(_topicName, System.Text.Encoding.UTF8.GetBytes("False"),
            MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
    }
}
