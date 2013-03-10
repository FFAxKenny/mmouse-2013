Motor Test Bench - Team Manpapua
===============================

This test bench is for testing driver motors. 

Introduction
------------
    Inputs:
    1 - toggle switch
    2 - push-button switch 
    3 - push-button switch

    Outputs:
    1 - motor step output
    2 - motor step output
    3 - change direction LED indicator

    Usage: 
    To increase the step rate of a motor, change the toggle switch to the OFF position. This
    will change the mode to increasing mode. Then push the pushbutton, and the step rate will 
    *increase* by a small amount. Keep adjusting as necessary.

    To decrease the step rate of a motor, change the toggle swtich to the ON position. Then push 
    the pushbutton for the motor.


Detailed
--------
    Behavior:

    (Toggle Switch) - Changes the step rate change sign
    |
    | 0 - increase step rate
    | 1 - decrease step rate
    |
    
    (Pushbutton) - Activates the change in step rate
    |
    | 0 - nothing happens
    | 1 - increases step rate by shortening time interval
    | ** This change will only happen once-- the PIC will check for a state change

    (Change Direction LED) - Tells what change direction mode PIC is currently in
    |
    | ON - increase step rate
    | OFF - decrease step rate
    |


                        
