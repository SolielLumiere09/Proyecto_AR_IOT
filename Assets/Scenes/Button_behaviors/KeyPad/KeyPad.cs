using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using uPLibrary.Networking.M2Mqtt.Messages;

public class KeyPad : MonoBehaviour
{
    private string _password;
    private Boolean _send = false;
    private string _topicName = "ArduinoLockDoor";
    public DashBoards dashBoards;
    public Text textPassword;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (_send)
        {
            dashBoards.Client.Publish(_topicName, System.Text.Encoding.UTF8.GetBytes(_password),
                MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
            _send = false;
           
        }
        
    }

    public void OnClickDigit(int digit)
    {
        _password += digit;
        textPassword.text = _password;
    }

    public void OnSend()
    {
        _send = true;

    }

    public void OnClear()
    {
        _password = "";
        textPassword.text = _password;
    }
}
