using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using uPLibrary.Networking.M2Mqtt.Messages;

public class LampController : MonoBehaviour
{

    public DashBoards dashBoards;
    private Boolean _lampStatus = false; //false->up, true->down
    private Animator _animator;
    private String _topicName = "ArduinoLamp";
    
    // Start is called before the first frame update
    void Start()
    {
        _animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }


    private void OnMouseDown()
    {
        if (_lampStatus)
        {
            _animator.Play("leverUp");
            _lampStatus = false;
        }
        else
        {
          
            _animator.Play("leverDown");
            _lampStatus = true;
        }
        
        dashBoards.Client.Publish(_topicName, System.Text.Encoding.UTF8.GetBytes((!_lampStatus).ToString()),
            MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
    }

    public void checkLampStatus(Boolean status)
    {
        if (_lampStatus != status)
        {
            Debug.Log("Adjusting");
            dashBoards.Client.Publish(_topicName, System.Text.Encoding.UTF8.GetBytes((!_lampStatus).ToString()),
                MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
            
        }
        
    }
}
