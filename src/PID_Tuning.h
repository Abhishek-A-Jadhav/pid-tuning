#ifndef PID_Tuning_h
#define PID_Tuning_h

#include <Arduino.h>


class PID {

    private:

        float _elapsedTime;
        float _timePrev;
        float _time;

        float _PID;
        float _pwm1;
        float _pwm2;
        float _error;
        float _previousError;
        float _pid_p;
        float _pid_i;
        float _pid_d;

        double _kp;
        double _ki;
        double _kd;

        double _throttle1;
        double _throttle2;
        double _speedMin;
        double _speedMax;
        int _desiredAngle;
        int _LED_PIN;

        float _angle;


    public:

        PID(float kp, float ki, float kd);
        void begin();
        void tune(int desiredAngle);
        int getPwm1(){ return _pwm1; };
        int getPwm2(){ return _pwm2; };
        int getAngleY(){ return _angle; };

};


#endif