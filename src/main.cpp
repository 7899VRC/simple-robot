/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Xavier                                                    */
/*    Created:      6/20/2026, 10:33:04 AM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "simple.h"

// Set this to false during competitions
bool TESTING = true;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
Robot robot;

// Include example functions or add your own functions here
#include "examples.h"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  robot = Robot(0, 0, 0);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................

  //e1_motor_directions();

  //e2_drive_and_stop();
  //p2_drive_and_stop();

  //e3_time_based_motion();
  //p3_time_based_motion();

  //e4_sensor_based_motion();
  //p4_sensor_based_motion();

  //e5_state_estimation();
  //p5_state_estimation();

  //e6_pid_based_turn();
  //p6_pid_based_turn();

  //e7_pid_based_straight();
  //p7_pid_based_straight();

  //e8_time_speed_control();
  //p8_time_speed_control();

  //e9_sensor_speed_control();
  //p9_sensor_speed_control();

  //e10_pid_speed_control();
  //p10_pid_speed_control();

  //e11_front_based_turn();
  //p11_front_based_turn();

  //e12_front_based_straight();
  //p12_front_based_straight();

  //e13_one_absolute();
  //p13_one_abolute();

  //e14_multiple_absolute();
  //p14_multiple_absolute();

  //e15_multiple_absolute_and_object();
  //p15_multiple_absolute_and_object();

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  std::cout << "-------------------- Starting Program --------------------" << std::endl;

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  if (TESTING) {
    autonomous();
  }
}
