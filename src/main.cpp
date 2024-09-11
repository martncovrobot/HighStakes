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

  /* IDEAS SECTION
  *
  *  Add a way to measure tank psi and add that on controller
  *  Add an overdrive feature using buttons (increase voltage temporarily for emergencies like fighting; might have to change the way the robot drives)
  *  Vibration during phases of match (last 30 seconds to be aware of time, last 15 so not touch corners, last 10 so not block hanging)
  *  
  * 
  */




  //Create variables and configurations (This stuff only runs once after autonomous)
  //This is where to put speed, motor configurations, etc. that you want to start with (they can be changed during the loop too)

  leftSide.setStopping(coast);
  rightSide.setStopping(coast);

  //Set timers for match

  Brain.resetTimer();
  


  while (1) {

    //Everything inside this loop runs every 20ms during driving phase

    //Robot Info

    int batteryLife = vexBatteryCapacityGet(); //Get Robot Battery

    int tempLFM = LFM.temperature(); //Get Temperature of Motors
    int tempLMM = LMM.temperature();
    int tempLBM = LBM.temperature();
    int tempRFM = RFM.temperature();
    int tempRMM = RMM.temperature();
    int tempRBM = RBM.temperature();

    Controller.Screen.clearScreen(); //Reset the screen to update the controller
                                     //Otherwise it just starts displaying off-screen

    Controller.Screen.setCursor(0, 0); //top left of controller
    Controller.Screen.print("BTRY"); 

    Controller.Screen.setCursor(0,4); //top of screen, right after "Battery:"
    Controller.Screen.print(batteryLife); //print the current battery level(%)

    Controller.Screen.setCursor(0, 10); //top right of controller
    Controller.Screen.print("Temps"); //temperature segment header

    Controller.Screen.setCursor(0, 15); //top of controller but right of "temps"
    Controller.Screen.print(tempLFM); // Temperature of Left Front Motor

    Controller.Screen.setCursor(0, 18); //top of controller but right of LFM
    Controller.Screen.print(tempRFM); // Temperature of Right Front Motor

    Controller.Screen.setCursor(1, 15); //right below LFM
    Controller.Screen.print(tempLMM) // Temperature of Left Middle Motor (The higher up one on the robot)

    Controller.Screen.setCursor(1, 18); //to the right of LMM
    Controller.Screen.print(tempRMM); // right middle motor (higher up one on robot);

    Controller.Screen.setCursor(2, 15); // right below LMM
    Controller.Screen.print(LBM); //Left Back motor (lower one in the back)

    Controller.Screen.setCursor(2,18); // to the right of LBM
    Controller.Screen.print(RBM);




    
    

    

    /* Controller Screen Concept
    
    BTRY:87                      Temps  45*     50*
    RSVR:80                             55*     45*
                                        50*     40*
    
    */

    //driver
    
    leftSide.setVelocity(Controller.Axis3.position(), percent); //sets the speed of the left motors
    rightSide.setVelocity(Controller.Axis2.position(), percent); //sets the speed of the right motors

    leftSide.spin(forward); //motors always spinning, but if joystick centered then speed = 0 so it doesnt matter
    rightSide.spin(forward);

    wait(20, msec);

  } //end of the loop

}//end of the usercontrol(driver phase) segment

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
