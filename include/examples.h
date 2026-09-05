#ifndef EXAMPLES_H
#define EXAMPLES_H


/* Delete this line for the next part
void e1_motor_directions() {
  // Spin each motor to see if orientation and port is correct
  for (auto leftMotor : robot.LEFT_MOTORS) {
    leftMotor.spin(forward, 20, pct);
    wait(1, sec);
    leftMotor.stop(brake);
  }

  for (auto rightMotor : robot.RIGHT_MOTORS) {
    rightMotor.spin(forward, 20, pct);
    wait(1, sec);
    rightMotor.stop(brake);
  }
}


/* Delete this line for the next part
void e2_drive_and_stop() {
  // Test out individual robot control functions
  robot.drive(30, 30);
  wait(1, sec);
  robot.stopAll();

  robot.drive(-30, -30);
  wait(1, sec);
  robot.stopAll();

  robot.drive(-30, 30);
  wait(1, sec);
  robot.stopAll();

  robot.drive(30, -30);
  wait(1, sec);
  robot.stopAll();
}

void p2_drive_and_stop() {
  // Use the drive and stopAll functions to move in a zig-zag pattern
  //  _ 
  //   |_
}


/* Delete this line for the next part
void e3_time_based_motion() {
  // Time-based drive straight and turn in place
  robot.timeStraight(2, forward);
  robot.timeTurn(1, left);
  robot.timeTurn(1, right);
  robot.timeStraight(2, reverse);
}

void p3_time_based_motion() {
  // Use the timeStraight and timeTurn functions to move in a zig-zag pattern
  //  _ 
  //   |_
}


/* Delete this line for the next part
void e4_sensor_based_motion() {
  // Sensor-based drive straight and turn in place
  robot.encoderStraight(10);
  robot.imuTurn(-90);
  robot.encoderStraight(10);
  robot.imuTurn(90);
  robot.encoderStraight(-10);
}

void p4_sensor_based_motion() {
  // Use the encoderStraight and imuTurn functions to move in an S-shape pattern
  //   _ 
  //  | |_|
}


/* Delete this line for the next part
void e5_state_estimation() {
  // Drive while keeping track of our current pose
  robot.encoderStraight(10);
  robot.imuTurn(-45);
  robot.encoderStraight(10);
  robot.printPose();
}

void p5_state_estimation() {
  // Move in an S-shape pattern and then print the final pose estimate
  //   _ 
  //  | |_|
}


/* Delete this line for the next part
void e6_pid_based_turn() {
  // PID-based turning
  // Test with high speed and high precision
  robot.setMaxPercent(90);
  robot.setTurnAccuracy(0.1);
  // Change these numbers to tune PID
  robot.setTurnPID(1, 0, 0);

  robot.pidTurn(-90);
  robot.printPose();
}

void p6_pid_based_turn() {
  // Tune your PID in order to turn 90 degrees
}


/* Delete this line for the next part
void e7_pid_based_straight() {
  // PID-based drive straight
  // Test with high speed and high precision
  robot.setMaxPercent(90);
  robot.setStraightAccuracy(0.1);
  // Change these numbers to tune straight PID
  robot.setStraightPID(10, 0, 0);
  // Change these numbers to tune angular PID
  robot.setAngularPID(3, 0, 0);

  robot.pidStraight(20);
  robot.printPose();
}

void p7_pid_based_straight() {
  // Tune your PID in order to drive straight 20 inches
}


/* Delete this line for the next part
void e8_time_speed_control() {
  // Max speed and acceleration control with time-based motions
  robot.setAcceleration(2);
  robot.setMaxPercent(50);

  robot.timeStraight(2, forward);
  robot.timeTurn(1, left);
  robot.timeTurn(1, right);
  robot.timeStraight(2, reverse);
}

void p8_time_speed_control() {
  // Use setAcceleration and setMaxPercent to try out different speeds and acceleration
  // with time-based motions
}


/* Delete this line for the next part
void e9_sensor_speed_control() {
  // Max speed and acceleration control with sensor-based motions
  robot.setAcceleration(2);
  robot.setMaxPercent(50);

  robot.encoderStraight(10);
  robot.imuTurn(-90);
  robot.encoderStraight(10);
  robot.imuTurn(90);
  robot.encoderStraight(-10);
}

void p9_sensor_speed_control() {
  // Use setAcceleration and setMaxPercent to try out different speeds and acceleration
  // with sensor-based motions
}


/* Delete this line for the next part
void e10_pid_speed_control() {
  // Max speed and acceleration control with PID-based motions
  robot.setAcceleration(5);
  robot.setMaxPercent(50);
  robot.setTurnAccuracy(0.5);
  robot.setTurnPID(0.5, 0.01, 0.01);
  robot.setStraightAccuracy(0.5);
  robot.setStraightPID(10, 0.05, 0.05);
  robot.setAngularPID(3, 0.02, 0.02);

  robot.pidTurn(-90);
  robot.pidStraight(30);
  robot.printPose();
}

void p10_pid_speed_control() {
  // Use setAcceleration and setMaxPercent to try out different speeds and acceleration
  // with PID-based motions
}


/* Delete this line for the next part
void e11_front_based_turn() {
  // Front-mounted distance sensor-based turn
  robot.setAcceleration(5);

  // Turn to the right until something is within 40 inches of the robot center
  robot.frontTurn(40, right);
  robot.printPose();
}

void p11_front_based_turn() {
  // Use frontTurn to turn until you see an object at a certain distance
}


/* Delete this line for the next part
void e12_front_based_straight() {
  // Front-mounted distance sensor-based straight
  robot.setAcceleration(5);
  robot.setMaxPercent(50);
  robot.setStraightAccuracy(0.5);
  robot.setStraightPID(10, 0.05, 0.05);
  robot.setAngularPID(3, 0.02, 0.02);

  // Drive straight until the robot center is within 30 inches of something
  robot.frontStraight(-30);
  robot.printPose();
}

void p12_front_based_straight() {
  // Use frontStraight to drive straight until you are within a certain distance of an object
}


/* Delete this line for the next part
void e13_one_absolute() {
  // Drive to an absolute pose
  robot.setStraightAccuracy(0.5);
  robot.setTurnAccuracy(0.5);
  robot.setStraightPID(10, 0.05, 0.05);
  robot.setAngularPID(3, 0.02, 0.02);
  robot.setTurnPID(0.5, 0.01, 0.01);

  robot.driveTo(10, 20, -90);
  robot.printPose();
}

void p13_one_abolute() {
  // Use driveTo to drive to an absolute pose starting from (0, 0, 0)
}


/* Delete this line for the next part
void e14_multiple_absolute() {
  // Drive to multiple absolute poses
  // Set the absolute starting pose
  robot.currentX = 0;
  robot.currentY = -48;
  robot.currentDeg = 90;

  robot.setStraightAccuracy(0.5);
  robot.setTurnAccuracy(0.5);
  robot.setStraightPID(10, 0.05, 0.05);
  robot.setAngularPID(3, 0.02, 0.02);
  robot.setTurnPID(0.5, 0.01, 0.01);

  robot.driveTo(-24, 0, 90);
  robot.driveTo(0, 48, 0);
  robot.printPose();
}

void p14_multiple_absolute() {
  // Use driveTo to drive from the bottom to left, top, right, and bottom of the field
}


/* Delete this line for the next part
void e15_multiple_absolute_and_object() {
  // Drive to multiple absolute poses and then drive straight to an object
  robot.currentX = 0;
  robot.currentY = -48;
  robot.currentDeg = 90;

  robot.setStraightAccuracy(0.5);
  robot.setTurnAccuracy(0.5);
  robot.setStraightPID(10, 0.05, 0.05);
  robot.setAngularPID(3, 0.02, 0.02);
  robot.setTurnPID(0.5, 0.01, 0.01);

  robot.driveTo(-24, 0, 90);
  robot.driveTo(-8, 48, 0);
  robot.frontStraight(-23);
  robot.printPose();
}

void p15_multiple_absolute_and_object() {
  // Drive to any goal on the field facing 0, 90, 180, or -90 degrees,
  // and then drive so that the front of the robot is within a couple inches of the goal
}


/* Do not delete this comment */
#endif