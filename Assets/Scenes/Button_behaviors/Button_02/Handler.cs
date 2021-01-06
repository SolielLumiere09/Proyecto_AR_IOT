using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using uPLibrary.Networking.M2Mqtt.Messages;

public class Handler : MonoBehaviour
{
    private Animator _animator;
    private Boolean _handleState = false; //false = down, true = up
    public Transform indicatorTransform;
    public DashBoards dashBoards;
    
    private volatile int PWM_value = 0;
    private String topicName = "Arduinofan";
    
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
        if (_handleState)
        {
            _animator.Play("HandleDown");
            _handleState = false;
        }
        else
        {
            _animator.Play("HandleUp");
            _handleState = true;
        }

        float y = indicatorTransform.localRotation.eulerAngles.y;
        if (y > 270f)
            y -= 360f;

        PWM_value = (int) indicatorToPWM(y);
        dashBoards.Client.Publish(topicName, System.Text.Encoding.UTF8.GetBytes(""+ PWM_value),
            MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
    }

    public void check_pwm_status(int pwm_value)
    {
        if (PWM_value != pwm_value)
        {
          
            dashBoards.Client.Publish(topicName, System.Text.Encoding.UTF8.GetBytes(""+ PWM_value),
                MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);

            Debug.Log("Adjusting");
        }
        
        
    }


    private float indicatorToPWM(float x)
    {

        float m = (-85f / 52f);
        float b = (1615f / 13f);

        return m * x + b;
    }
}
