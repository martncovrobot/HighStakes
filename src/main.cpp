/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Martin.Disibio                                            */
/*    Created:      8/22/2024, 7:14:28 AM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "robot-config.h"

using namespace vex;
// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

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


//drive backwards about 1 tile
//clamp on mobile goal
//move intake to put disc on mobile goal
//turn left for about 35-40 degrees
//drive forward about 1.5 tiles
//run intake
//turn 45 degrees to the left
//release mobile goal
//turn 90 degrees left
//drive forward for 1 tile


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

  //Create variables and configurations
  leftSide.setStopping(coast);
  rightSide.setStopping(coast);

  //setting up the screen to be configured in the loop
  
  
  //Controller.Screen.print("please work");
  
  //Controller.Screen.print(tempLMM);
  //Controller.Screen.print("second line");

  while (1) {

    //Robot Info

    int batteryLife = vexBatteryCapacityGet(); //Get Robot Battery
    int tempLMM = LMM.temperature();

    Controller.Screen.clearScreen();

    Controller.Screen.setCursor(0, 0); //top left of controller
    Controller.Screen.print("Battery:");
    Controller.Screen.setCursor(0,9);
    Controller.Screen.print(batteryLife);
    Controller.Screen.setCursor(0, 20);
    Controller.Screen.print(tempLMM);
    


    /*float tempLFM = LFM.temperature; //Get motor temperatures
    float tempLMM = LMM.temperature;
    float tempLBM = LBM.temperature;
    float tempRFM = RFM.temperature;
    float tempRMM = RMM.temperature;
    float tempRBM = RBM.temperature;*/

    

    /* Controller Screen Concept
    
    Battery Life: XX%                Temperatures:
                                        90*     94.2*
                                        89.24*  90.3
                                        93.2    95.8*
    
    */



    //Controller screen info
    //Controller.Screen.clearScreen();
    //Controller.Screen.print("Battery: ", batteryLife);
    //Controller.Screen.print("")

    //driver
    
    leftSide.setVelocity(Controller.Axis3.position(), percent);
    rightSide.setVelocity(Controller.Axis2.position(), percent);
    leftSide.spin(forward);
    rightSide.spin(forward);


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
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
