#ifndef CONTROLLER_H
#define CONTROLLER_H


class PIDController {
  public:
    float kp = 0;
    float ki = 0;
    float kd = 0;
    float totalError = 0;
    float previousError = 0;

    // Initialize the controller with kp, ki, and kd
    PIDController(float kp, float ki, float kd) {
      this->kp = kp;
      this->ki = ki;
      this->kd = kd;
    }

    // Reset the controller at the beginning of a motion
    void reset() {
      this->totalError = 0;
      this->previousError = 0;
    }

    // Calculate a command based on error and time since last calculation
    float calculate(float error, float time) {       
      this->totalError += error * time;

      // Main PID formula
      float command = (kp * error) + (ki * totalError) + (kd * (error - previousError) / time);

      this->previousError = error;

      return command;
    }
};


#endif