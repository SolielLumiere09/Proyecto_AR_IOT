using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlueButton : MonoBehaviour
{
    public Transform indicator;
    
    private Animator _animator;

    private const float _UP_INDICATOR = -13f;
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
        _animator.Play("BlueButtonPressed");
        float y = indicator.localRotation.eulerAngles.y;

        if (y > 270f)
            y -= 360f;
        
        if(y > -70f)
            indicator.localRotation *= Quaternion.Euler(0, _UP_INDICATOR, 0);


    }
}
