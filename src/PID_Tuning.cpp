#include <PID_Tuning.h>
#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_tockn.h>

MPU6050 mpu6050(Wire);

PID::PID(float kp, float ki, float kd) {

    _elapsedTime;
    _timePrev;
    _time;

    _PID;
    _pwm1;
    _pwm2;
    _error;
    _previousError;
    _pid_p = 0;
    _pid_i = 0;
    _pid_d = 0;

    _kp = kp;
    _ki = ki;
    _kd = kd;

    _throttle1 = 1430;
    _throttle2 = 1345;
    _speedMin = 1325;
    _speedMax = 1500;
    _desiredAngle = 0;
    _LED_PIN = 13;

    _angle;
    
}

void PID::begin() {

    Serial.begin(9600);
    Wire.begin();

    mpu6050.begin();
    mpu6050.setGyroOffsets(0.0, -1.0, 0.0);
    // mpu6050.calcGyroOffsets(true);

    _time = millis();

}

void PID::tune(int desiredAngle) {

    _desiredAngle = desiredAngle;

    _timePrev = _time;
    _time = millis();
    _elapsedTime = (_time - _timePrev) / 1000;

    mpu6050.update();

    _angle = mpu6050.getAngleY();

    _error = _angle - _desiredAngle;

    _pid_p = _kp * _error;
    if(_error < 5 && _error > -5) {
        _pid_i = _pid_i + (_ki * _error);  
    }
    _pid_d = _kd * ((_error - _previousError) / _elapsedTime);

    _PID = _pid_p + _pid_i + _pid_d;

    if(_PID < -1000) {
        _PID = -1000;
    }
    if(_PID > 1000) {
        _PID = 1000;
    }

    _pwm1 = _throttle1 + _PID;
    _pwm2 = _throttle2 - _PID;

    if(_pwm1 < _speedMin) {
        _pwm1 = _speedMin;
    }
    if(_pwm1 > _speedMax) {
        _pwm1 = _speedMax;
    }

    if(_pwm2 < _speedMin) {
        _pwm2 = _speedMin;
    }
    if(_pwm2 > _speedMax) {
        _pwm2 = _speedMax;
    }

    if (_angle > (desiredAngle - 1) && _angle < (desiredAngle + 1)) {
        _pwm1 = _pwm2 + 82;
        // Serial.println("Stable");
    }

    _previousError = _error;

}