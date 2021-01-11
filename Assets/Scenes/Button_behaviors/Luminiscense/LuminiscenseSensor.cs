using System;
using UnityEngine;
using UnityEngine.UI;

public class LuminiscenseSensor : MonoBehaviour
{
    public DashBoards DashBoards;
    private volatile Boolean changeRequest = false;
    private volatile float luminescense = 0;
    private string _topicName = "ArduinoLuminSensor";

    public Sprite morning, afternoon, night;

    public Text text;

    private Image _image;
    // Start is called before the first frame update
    void Start()
    {
        _image = GetComponent<Image>();
    }

    // Update is called once per frame
    void Update()
    {
        if (changeRequest)
        {
            text.text = "luminescence = " + luminescense;
            changeRequest = false;

            if (luminescense <= 30f)
            {
                _image.sprite = night;
                text.color = Color.white;
                
            }
            else if (luminescense <= 120f)
            {
                _image.sprite = morning;
                text.color = Color.black;
            }
            else if (luminescense <= 200f)
            {
                _image.sprite = afternoon;
                text.color = Color.black;
            }


        }
        
    }

    public void SetLuminiscense(float value)
    {
        luminescense = value;
        changeRequest = true;
    }
    
}
