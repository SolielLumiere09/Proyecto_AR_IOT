using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TemperatureController : MonoBehaviour
{
    public Text temperatureText;
    private volatile Boolean _changeRequest = false;
    private volatile float _temperature = 0;

    public Sprite sunSprite;

    public Sprite iceSprite;

    public Image imageToShow;
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (_changeRequest)
        {
            temperatureText.text = "Temperatura = " + _temperature + " *C";
            _changeRequest = false;

            if (_temperature < 15f)
            {
                imageToShow.sprite = iceSprite;
            }
            else
            {
                imageToShow.sprite = sunSprite;
            }
        }
    }


    public void RequestUpdateText(float temperature)
    {
        _temperature = temperature;
        _changeRequest = true;

    }
}
