#ifndef ROBOT_H
#define ROBOT_H


class Robot : public RobotParams {
  public:
    // Variables to keep track of the robot's current pose
    float currentX = 0;
    float currentY = 0;
    float currentDeg = 0;

    // Variables to keep track of speed settings
    float minPercent = 5;
    float maxPercent = 30;
    float acceleration = 10;

    // Variables to keep track of PID accuracy settings
    float turnAccuracy = 1;
    float straightAccuracy = 1;

    // Controllers for PID functions
    PIDController turnController = PIDController(0, 0, 0);
    PIDController straightController = PIDController(0, 0, 0);
    PIDController angularController = PIDController(0, 0, 0);

    // This allows us to create a blank robot variable
    Robot() = default;

    // Robot constructor, robot is initialized here with starting pose
    Robot(float x, float y, float deg) {
      // Store starting pose in class variables
      this->currentX = x;
      this->currentY = y;
      this->currentDeg = deg;

      // Run IMU calibration until it is finished
      // Make sure to not touch the robot when the program starts
      this->IMU.calibrate();
      while (IMU.isCalibrating()) {
        wait(this->INTERVAL_TIME, sec);
      }
  
      // Reset all hardware interfaces when robot is initialized
      resetSensors();
    }

    void resetSensors() {
      // Set IMU to zero
      this->IMU.setHeading(0, degrees);

      // Set all left encoders to zero
      for (auto leftMotor : this->LEFT_MOTORS) {
        leftMotor.resetPosition();
      }

      // Set all right encoders to zero
      for (auto rightMotor : this->RIGHT_MOTORS) {
        rightMotor.resetPosition();
      }
    }

    //////////////////////////////////////////////////
    // Update robot settings
    //////////////////////////////////////////////////

    void setMinPercent(float percent) {
      this->minPercent = percent;
    }

    void setMaxPercent(float percent) {
      this->maxPercent = percent;
    }

    void setAcceleration(float acceleration) {
      this->acceleration = acceleration;
    }

    void setTurnAccuracy(float accuracy) {
      this->turnAccuracy = accuracy;
    }

    void setStraightAccuracy(float accuracy) {
      this->straightAccuracy = accuracy;
    }

    void setTurnPID(float kp, float ki, float kd) {
      this->turnController.kp = kp;
      this->turnController.ki = ki;
      this->turnController.kd = kd;
    }

    void setStraightPID(float kp, float ki, float kd) {
      this->straightController.kp = kp;
      this->straightController.ki = ki;
      this->straightController.kd = kd;
    }

    void setAngularPID(float kp, float ki, float kd) {
      this->angularController.kp = kp;
      this->angularController.ki = ki;
      this->angularController.kd = kd;
    }

    //////////////////////////////////////////////////
    // Speed calculation
    //////////////////////////////////////////////////

    float getAccelerationPercent(float current, float target) {
      float progress = fabs(current) / fabs(target);
      float percent =  this->acceleration * this->maxPercent * progress;
      return clamp(percent, this->minPercent, this->maxPercent);
    }

    //////////////////////////////////////////////////
    // Direct control functions
    //////////////////////////////////////////////////

    void leftDrive(float percent) {
      percent = clamp(percent, -100, 100);

      // Spin all left motors at the same speed
      for (auto leftMotor : this->LEFT_MOTORS) {
        leftMotor.spin(forward, percent, pct);
      }
    }

    void rightDrive(float percent) {
      percent = clamp(percent, -100, 100);

      // Spin all right motors at the same speed
      for (auto rightMotor : this->RIGHT_MOTORS) {
        rightMotor.spin(forward, percent, pct);
      }
    }

    void drive(float leftPercent, float rightPercent) {
      leftDrive(leftPercent);
      rightDrive(rightPercent);
    }

    void leftBrake() {
      // Stop all left motors
      for (auto leftMotor : this->LEFT_MOTORS) {
        leftMotor.stop(brake);
      }
    }

    void rightBrake() {
      // Stop all right motors
      for (auto rightMotor : this->RIGHT_MOTORS) {
        rightMotor.stop(brake);
      }
    }

    void stopAll() {
      // Stop all motors
      leftBrake();
      rightBrake();
      wait(0.5, sec);
    }

    //////////////////////////////////////////////////
    // Sensor data
    //////////////////////////////////////////////////

    // Get the current angle according to the IMU
    float getIMUAngle() {
      return normalizeAngle(-1 * this->IMU.heading(degrees));
    }

    // Total rotation angle for all left motors
    float getLeftRotation() {
      // Read all left motor encoders
      // Use the average if there are multiple motors
      float total = 0;
      for (auto leftMotor : this->LEFT_MOTORS) {
        total += leftMotor.position(degrees);
      }

      return total / this->LEFT_MOTORS.size();
    }

    // Total rotation angle for all right motors
    float getRightRotation() {
      // Read all right motor encoders
      // Use the average if there are multiple motors
      float total = 0;
      for (auto rightMotor : this->RIGHT_MOTORS) {
        total += rightMotor.position(degrees);
      }

      return total / this->RIGHT_MOTORS.size();
    }

    // Convert rotation angle to linear distance
    float rotationToDistance(float rotation) {
      float distance = (
        rotation *
        M_PI *
        this->WHEEL_DIAMETER *
        this->EXTERNAL_RATIO *
        this->ENCODER_FACTOR
        ) / 360.0;

      return distance;
    }

    // Calculate total linear distance from all motor encoders
    float getEncoderDistance() {
      float leftDistance = rotationToDistance(getLeftRotation());
      float rightDistance = rotationToDistance(getRightRotation());
      return (leftDistance + rightDistance) / 2.0;
    }

    // Get distance behind and object according to a front-mounted distance sensor
    float getFrontDistance() {
      return -1 * (this->FRONT_DISTANCE.objectDistance(inches) + this->CENTER_TO_FRONT_DISTANCE);
    }

    //////////////////////////////////////////////////
    // State estimation
    //////////////////////////////////////////////////

    void updateAngle() {
      // Read angle from the IMU and add to our current angle
      this->currentDeg = normalizeAngle(this->currentDeg + getIMUAngle());
    }

    void updatePose() {
      updateAngle();
      // Get average distance travelled, assuming we are driving straight
      float distance = getEncoderDistance();

      // Apply the straight path to our current pose to get a new pose
      this->currentX = this->currentX + distance * cos(toRadians(this->currentDeg));
      this->currentY = this->currentY + distance * sin(toRadians(this->currentDeg));
    }

    void printPose() {
      std::cout << this->currentX << ", " << this->currentY << ", " << this->currentDeg << std::endl;
    }

    //////////////////////////////////////////////////
    // Control loops
    //////////////////////////////////////////////////

    // Turn for a specific amount of time
    void timeTurn(float targetTime, turnType direction) {
      // Reset all sensors
      resetSensors();
      // Reset the timer
      Brain.Timer.clear();

      float totalTime = 0;
      float percent = this->maxPercent;

      // Drive until target time has been reached
      while(totalTime < targetTime) {
        // Get total elapsed time from the timer
        totalTime = Brain.Timer.time(sec);

        // Get drive percent with acceleration
        percent = getAccelerationPercent(totalTime, targetTime);

        // Turn based on provided direction
        if (direction == left) {
          drive(-percent, percent);
        }
        else if (direction == right) {
          drive(percent, -percent);
        }

        wait(this->INTERVAL_TIME, sec);
      }

      stopAll();
      // Update current angle estimate
      updateAngle();
    }

    // Drive straight for a specific amount of time
    void timeStraight(float targetTime, directionType direction) {
      // Reset all sensors
      resetSensors();
      // Reset the timer
      Brain.Timer.clear();

      float totalTime = 0;
      float percent = this->maxPercent;

      // Drive until target time has been reached
      while(totalTime < targetTime) {
        // Get total elapsed time from the timer
        totalTime = Brain.Timer.time(sec);

        // Get drive percent with acceleration
        percent = getAccelerationPercent(totalTime, targetTime);

        // Drive straight based on provided direction
        if (direction == forward) {
          drive(percent, percent);
        }
        else if (direction == reverse) {
          drive(-percent, -percent);
        }

        wait(this->INTERVAL_TIME, sec);
      }

      stopAll();
      // Update current pose estimate
      updatePose();
    }

    // Turn past the target based on IMU data
    void imuTurn(float targetAngle) {
      // Reset all sensors
      resetSensors();

      float totalAngle = 0;
      float percent = this->maxPercent;

      // Drive until we are past the target angle
      while(fabs(totalAngle) < fabs(targetAngle)) {
        // Get angle from the last reset point from the IMU
        totalAngle = getIMUAngle();

        // Get drive percent with acceleration
        percent = getAccelerationPercent(totalAngle, targetAngle);

        // Turn based on the sign of target angle
        if (0 <= targetAngle) {
          drive(-percent, percent);
        }
        else {
          drive(percent, -percent);
        }

        wait(this->INTERVAL_TIME, sec);
      }

      stopAll();
      // Update current angle estimate
      updateAngle();
    }

    // Drive straight past the target based on encoder data
    void encoderStraight(float targetDistance) {
      // Reset all sensors
      resetSensors();

      float totalDistance = 0;
      float percent = this->maxPercent;

      // Drive until we are past the target distance
      while(fabs(totalDistance) < fabs(targetDistance)) {
        // Get distance from the last reset point from the encoders
        totalDistance = getEncoderDistance();

        // Get drive percent with acceleration
        percent = getAccelerationPercent(totalDistance, targetDistance);

        // Drive straight based on the sign of target distance
        if (0 <= targetDistance) {
          drive(percent, percent);
        }
        else {
          drive(-percent, -percent);
        }

        wait(this->INTERVAL_TIME, sec);
      }

      stopAll();
      // Update current pose estimate
      updatePose();
    }

    // Turn to the target using PID control and IMU data
    void pidTurn(float targetAngle) {
      // Reset all sensors and controllers
      resetSensors();
      this->turnController.reset();

      float error = targetAngle;
      float percent = this->maxPercent;

      // Drive until we are close to the target angle
      while(this->turnAccuracy < fabs(error)) {
        // Get angle from the last reset point from the IMU
        float totalAngle = getIMUAngle();
        // Get elapsed time from the timer for each cycle
        float time = Brain.Timer.time(sec);
        Brain.Timer.clear();

        // Get drive percent with acceleration
        percent = getAccelerationPercent(totalAngle, targetAngle);

        // Use the PID controller to calculate a command based on angle error
        error = targetAngle - totalAngle;
        float command = this->turnController.calculate(error, time);
        // Clamp the command to the max percent before driving
        command = clamp(command, -percent, percent);
        drive(-command, command);

        wait(this->INTERVAL_TIME, sec);
      }

      stopAll();
      // Update current angle estimate
      updateAngle();
    }

    // Drive straight to the target using PID control and encoder data
    void pidStraight(float targetDistance) {
      // Reset all sensors and controllers
      resetSensors();
      this->straightController.reset();
      this->angularController.reset();

      float distanceError = targetDistance;
      float percent = this->maxPercent;

      // Drive until we are close to the target distance
      while(this->straightAccuracy < fabs(distanceError)) {
        // Get distance from the last reset point from the encoders
        float totalDistance = getEncoderDistance();
        // Get angle from the last reset point from the IMU
        float totalAngle = getIMUAngle();
        // Get elapsed time from the timer for each cycle
        float time = Brain.Timer.time(sec);
        Brain.Timer.clear();

        // Get drive percent with acceleration
        percent = getAccelerationPercent(totalDistance, targetDistance);

        // Use the PID controller to calculate a command based on distance error
        distanceError = targetDistance - totalDistance;
        float command = this->straightController.calculate(distanceError, time);
        // Clamp the command to the max percent before driving
        command = clamp(command, -percent, percent);

        // Use the PID controller to calculate an offset based on angle error
        float angleError = 0 - totalAngle;
        float offset = this->angularController.calculate(angleError, time);
        // Scale the offset based on the commanded speed
        offset = offset * fabs(command / 100.0);

        // Drive with command and offset adjustment
        drive(command - offset, command + offset);

        wait(this->INTERVAL_TIME, sec);
      }

      stopAll();
      // Update current pose estimate
      updatePose();
    }

    // Turn until object is detected with a front-mounted distance sensor
    void frontTurn(float distanceThreshold, turnType direction) {
      // Reset all sensors
      resetSensors();

      // Take initial measurement from the distance sensor
      float totalDistance = getFrontDistance();
      float percent = this->maxPercent;

      // Drive until measured distance reaches a specific threshold
      while(distanceThreshold < fabs(totalDistance)) {
        // Get angle from the last reset point from the IMU
        float totalAngle = getIMUAngle();
        // Get measurement from the distance sensor
        totalDistance = getFrontDistance();

        // Get drive percent with acceleration
        percent = getAccelerationPercent(totalAngle, 180);

        // Turn based on provided direction
        if (direction == left) {
          drive(-percent, percent);
        }
        else if (direction == right) {
          drive(percent, -percent);
        }

        wait(this->INTERVAL_TIME, sec);
      }

      stopAll();
      // Update current angle estimate
      updateAngle();
    }

    // Drive straight until object is at a certain distance with a front-mounted distance sensor
    void frontStraight(float targetDistance) {
      // Reset all sensors and controllers
      resetSensors();
      this->straightController.reset();
      this->angularController.reset();

      // Take initial measurement from the distance sensor
      float initialDistance = getFrontDistance();
      float distanceError = targetDistance - initialDistance;
      float percent = this->maxPercent;

      // Drive until measured distance is close to target distance
      while(this->straightAccuracy < fabs(distanceError)) {
        // Get measurement from the distance sensor
        float totalDistance = getFrontDistance();
        // Get angle from the last reset point from the IMU
        float totalAngle = getIMUAngle();
        // Get elapsed time from the timer for each cycle
        float time = Brain.Timer.time(sec);
        Brain.Timer.clear();

        // Get drive percent with acceleration
        percent = getAccelerationPercent(initialDistance - totalDistance, initialDistance);

        // Use the PID controller to calculate a command based on distance error
        distanceError = targetDistance - totalDistance;
        float command = this->straightController.calculate(distanceError, time);
        // Clamp the command to the max percent before driving
        command = clamp(command, -percent, percent);

        // Use the PID controller to calculate an offset based on angle error
        float angleError = 0 - totalAngle;
        float offset = this->angularController.calculate(angleError, time);
        // Scale the offset based on the commanded speed
        offset = offset * fabs(command / 100.0);

        // Drive with command and offset adjustment
        drive(command - offset, command + offset);

        wait(this->INTERVAL_TIME, sec);
      }

      stopAll();
      // Update current pose estimate
      updatePose();
    }

    //////////////////////////////////////////////////
    // Path planning
    //////////////////////////////////////////////////

    // Drive to an absolute target with a turn, straight, and turn
    void driveTo(float x, float y, float deg) {
      // Calculate angle from current angle to target pose
      float firstAngle = normalizeAngle(toDegrees(atan2(y - this->currentY, x - this->currentX)) - this->currentDeg);
      pidTurn(firstAngle);

      // Calculate distance from current pose to target pose
      float distance = sqrt(pow(x - this->currentX, 2) + pow(y - this->currentY, 2));
      pidStraight(distance);

      // Calculate angle from current angle to target angle
      float secondAngle = normalizeAngle(deg - this->currentDeg);
      pidTurn(secondAngle);
    }
};

#endif