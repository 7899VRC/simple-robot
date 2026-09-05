#ifndef PARAMS_H
#define PARAMS_H


class RobotParams {
  public:
    // Assign hardware interfaces for the robot
    std::vector<motor> LEFT_MOTORS = {
      motor(PORT12, ratio18_1, false),
    };

    std::vector<motor> RIGHT_MOTORS = {
      motor(PORT19, ratio18_1, true),
    };

    inertial IMU = inertial(PORT11);
    distance FRONT_DISTANCE = distance(PORT18);

    float WHEEL_DIAMETER = 2.75; // Diameter of wheels
    float EXTERNAL_RATIO = 1.0; // External gear ratio between motor and wheel
    float ENCODER_FACTOR = 1.0; // Use to adjust encoder distance measurement

    float CENTER_TO_FRONT_DISTANCE = 15.5; // Robot center to front distance sensor

    float INTERVAL_TIME = 0.05; // Time per control interval in seconds
};

#endif