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


void runHaptics(int hapticType){
    //types of haptics:
    //1 = one long buzz (happens at 30 seconds left)
    //2 = three quick buzzes (happens at 15 seconds left) (not allowed to touch corners)
    //3 = two long buzzes (happens at 10 seconds left) (not allowed to impede hangs)

    switch(hapticType){

      case 1:
        Controller.rumble("-");
      break;

      case 2:
        Controller.rumble("...");
      break;

      case 3:
        Controller.rumble("- -");
      break;


    }



}

void autonTimer(){
  //timer that updates screen and haptics during autonomous
  //this thread will self-terminate after the round ends
  for(int timer = 15; timer > 0; timer--){
    //loop ends after 15 seconds
    //int timer is the duration of the loop
    //during the autonomous phase
    Controller.Screen.setCursor(2,8); //Center of Screen
    Controller.Screen.clearLine();
    Controller.Screen.print(timer);
    wait(1,sec); //run code 1 times per sec
  }


}

void matchTimer(){

  //timer that runs during the match
  //this thread will self-terminate 
  for(int timer = 105; timer > 0; timer--){
    //loop ends after 105 seconds (1 minute and 45 seconds)
    //int timer is the duration of the loop
    //during the driver phase
    if (timer == 30){
      //30 seconds left
      runHaptics(1);
    }
    if (timer == 15){
      //15 seconds left. cant touch corners
      runHaptics(2);
    }
    if (timer == 10){
      //10 seconds left. cant touch hanging
      runHaptics(3);
    }
    Controller.Screen.setCursor(2,8); //Center of Screen
    Controller.Screen.clearLine();
    Controller.Screen.print(timer);
    wait(1,sec); //run code 1 times per sec
  }
}

void skillsTimer(){

  for(int timer = 60; timer > 0; timer--){
    //loop ends after 60 seconds (1 minute)
    //int timer is the duration of the loop
    //during the skills phase
    if (timer == 30){
      //30 seconds left
      runHaptics(1);

    }
    if (timer == 15){
      //15 seconds left
      runHaptics(2);
    }
    if (timer == 10){
      //10 seconds left
      runHaptics(3);
    }
    Controller.Screen.setCursor(2,8); //Center of Screen
    Controller.Screen.clearLine();
    Controller.Screen.print(timer);
    wait(1,sec); //run code 1 times per sec
  }

}


void setScreen(){

  //this thread will not self terminate
  //needs testing to see if vex systems will terminate thread
  while(1){

    int batteryLife = vexBatteryCapacityGet(); //Get Robot Battery

    int tempLFM = LFM.temperature(); //Get Temperature of Motors
    int tempLMM = LMM.temperature();
    int tempLBM = LBM.temperature();
    int tempRFM = RFM.temperature();
    int tempRMM = RMM.temperature();
    int tempRBM = RBM.temperature();


    //Controller.Screen.clearScreen();

    Controller.Screen.clearLine();

    Controller.Screen.setCursor(0, 0); //top left of controller
    Controller.Screen.print("BTRY"); 

    Controller.Screen.setCursor(0,6); //top of screen, right after "Battery:"    
    Controller.Screen.print(batteryLife); //print the current battery level(%)

    Controller.Screen.setCursor(1, 13); //top of controller but right of "temps"
    Controller.Screen.print(tempLFM); // Temperature of Left Front Motor

    Controller.Screen.setCursor(1, 17); //top of controller but right of LFM
    Controller.Screen.print(tempRFM); // Temperature of Right Front Motor

    Controller.Screen.clearLine();

    Controller.Screen.setCursor(2, 16); //right below LFM
    Controller.Screen.print(tempLMM); // Temperature of Left Middle Motor (The higher up one on the robot)

    Controller.Screen.setCursor(2, 20); //to the right of LMM
    Controller.Screen.print(tempRMM); // right middle motor (higher up one on robot);

    Controller.Screen.clearLine();

    Controller.Screen.setCursor(3, 16); // right below LMM
    Controller.Screen.print(tempLBM); //Left Back motor (lower one in the back)

    Controller.Screen.setCursor(3,20); // to the right of LBM
    Controller.Screen.print(tempRBM);

    wait(500,msec); //screen update timer (twice per second)

  } //end of loop

   
  
  


}//end of function

int autoNumber = 0; 
bool auto_started = false;


//Max of 3 


void pre_auton(void) {

  while(!auto_started){
    Brain.Screen.clearScreen();

    // Always print the current auto option
    //Brain.Screen.printAt(5, 20, "NOAH SEITZ is awesome");
    Brain.Screen.printAt(5, 40, "Selected Auton:");

    // Update the displayed auton name based on the autoNumber value
    switch(autoNumber){
        case 0: 
          Brain.Screen.printAt(5, 60, "RedRingSide/BlueGoalSide");
          break;
        case 1: 
          Brain.Screen.printAt(5, 60, "RedGoalSide/BlueRingSide");
          break;
        case 2:
          Brain.Screen.printAt(5, 60, "DriveForward");
          break;
        case 3:
          Brain.Screen.printAt(5, 60, "Skills");
          break;
    }    

    // Check for screen press to change auton selection
    if(Brain.Screen.pressing()){
        while(Brain.Screen.pressing()){
            autoNumber++;
            if(autoNumber == 4){
                autoNumber = 0; // Wrap around to 0 after 3
            }
        }
    }

    // No need for a long wait here. Just make sure it updates constantly.
    wait(20, msec); // This makes the loop run frequently and keeps the screen updated

}
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


//Revoltuion * gear ratio ( wheel diamtere * pi) = inches
 

void driveDegrees(double number){
  leftSide.spinFor(number,degrees,false);
  rightSide.spinFor(number,degrees);
}

void turnDegrees(double number){
  leftSide.spinFor(number,degrees,false);
  rightSide.spinFor(-number,degrees);
}


void autonomous(void) {
    auto_started = true;
    switch(autoNumber){
      case 0:
      leftSide.setVelocity(30,percent);
      rightSide.setVelocity(30,percent);
      clamp.set(true);
      driveDegrees(-1700);
      leftSide.setVelocity(15,percent);
      rightSide.setVelocity(15,percent);
      wait(0.5,sec);
      clamp.set(false);
      intake.setVelocity(100,percent);
      intake.spin(reverse);
      turnDegrees(510); 
      driveDegrees(600);
      wait(1,seconds);
      turnDegrees(990);
      leftSide.setVelocity(50,percent);
      rightSide.setVelocity(50,percent);
      driveDegrees(1550);
      intake.stop();
      hookMotor.spin(forward);
      leftSide.setVelocity(1,percent);
      rightSide.setVelocity(1,percent);
      driveDegrees(100);
      break;

      case 1:
      leftSide.setVelocity(30,percent);
      rightSide.setVelocity(30,percent);
      clamp.set(true);
      driveDegrees(-1700);
      leftSide.setVelocity(15,percent);
      rightSide.setVelocity(15,percent);
      wait(0.5,sec);
      clamp.set(false);
      intake.setVelocity(100,percent);
      intake.spin(reverse);
      turnDegrees(-510); 
      driveDegrees(600);
      wait(1,seconds);
      turnDegrees(-990);
      leftSide.setVelocity(50,percent);
      rightSide.setVelocity(50,percent);
      driveDegrees(1550);
      leftSide.setVelocity(1,percent);
      rightSide.setVelocity(1,percent);
      driveDegrees(100);

      break;

      case 2:

      break; 

      case 3: 

        //540 turn = 90 degrees

        leftSide.setStopping(brake);
        rightSide.setStopping(brake);
        intake.setVelocity(100,percent);


        leftSide.setVelocity(10,percent);
        rightSide.setVelocity(10,percent);
        clamp.set(true);//open clamp
        driveDegrees(-900);//into goal
        wait(0.5,sec);
        clamp.set(false);//close clamp
        turnDegrees(-250);//turn abt 45 degrees left
        intake.spin(reverse);
        driveDegrees(2000);//into low ring
        wait(0.5,sec);
        turnDegrees(-810);//135 degrees left
        clamp.set(true);//open clamp
        intake.stop();
        driveDegrees(-1000);//push goal into corner
        driveDegrees(500);//back away from goal
      break;
    }
}

  

  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  //thread(setScreen).detach(); // Screen displays motors and temps
  //thread(autonTimer).detach(); //On-screen timer and haptics

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

  intakeMotor.setStopping(coast);
  hookMotor.setStopping(coast);

  intakeMotor.setVelocity(100,percent);
  hookMotor.setVelocity(100,percent);
  intake.setVelocity(100,percent);

  //Designate another thread to running the screen

  //thread(setScreen).detach(); //Screen Display (Temps and battery)
  //thread(matchTimer).detach(); //On-screen timer and haptics


  bool rightPressed = false;
  bool clampToggle = false;

  while (1) {

    bool buttonR2 = Controller.ButtonRight.pressing();

    if(buttonR2 && !rightPressed){
    rightPressed = true;
    clampToggle =!clampToggle;
    }
      else if(!buttonR2){
        rightPressed = false;
      }

    if(clampToggle){
      clamp.set(true);
    }
    else{
      clamp.set(false);
    }


    //Everything inside this loop runs every 20ms during driving phase

    //Robot Info

    /* Controller Screen Concept
    
    BTRY:87                      Temps  45*     50*
    RSVR:80                             55*     45*
                                        50*     40*
    
    */

    //DriveTrain Motors
    
    leftSide.setVelocity(Controller.Axis3.position(), percent); //sets the speed of the left motors
    rightSide.setVelocity(Controller.Axis2.position(), percent); //sets the speed of the right motors

    leftSide.spin(forward); //motors always spinning, but if joystick centered then speed = 0 so it doesnt matter
    rightSide.spin(forward);


    //Intake Motor
    
    if(Controller.ButtonR1.pressing()){
      intake.spin(reverse);
    }
    else if(Controller.ButtonR2.pressing()){
      intake.spin(forward);
    }
    else{
      intake.stop(coast);
    }

    wait(20, msec);

  } //end of the loop

} //end of the usercontrol(driver phase) segment

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
