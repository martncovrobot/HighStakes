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
#include "functions.h"

using namespace vex;
// A global instance of competition
competition Competition;

// define your global variables/objects
int autonMode;
bool clampDebounce;
bool swiperDebounce;
bool hapticDebounce;

std::string goForward = "forward";
std::string goBackward = "reverse";
std::string turnLeft = "left";
std::string turnRight = "right";


/*-------------------------------------------------------------*/
/* Assign Buttons Here                                         */
/*-------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/


//this code is updated as of 1:05 pm, 1/9/2025



void pre_auton(void) {
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  
  calibrateInertial();

  while(Competition.isAutonomous()==false){
    //autonomous switcher
    //this loop ends when autonomous starts
    if(Controller.ButtonX.pressing()==true) autonMode = 1; //blue ring / red goal
    else if(Controller.ButtonA.pressing()==true) autonMode = 2; // red ring / blue goal
    else if(Controller.ButtonUp.pressing()==true) autonMode = 3; // drive forward
    else if(Controller.ButtonLeft.pressing()==true) autonMode = 4; //skills
    Controller.Screen.clearLine();
    Controller.Screen.setCursor(1,1);
    switch(autonMode){
      case 1: Controller.Screen.print("RedGoal/BlueRing"); break;
      case 2: Controller.Screen.print("BlueGoal/RedRing"); break;
      case 3: Controller.Screen.print("Drive Forward"); break;
      case 4: Controller.Screen.print("Skills"); break;
      default: Controller.Screen.print("Select Autonomous"); break;
    }
    
    wait(0.5,seconds); //interval time

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



void autonomous(void) {

  //initializing autonomous

  leftSide.stop();
  rightSide.stop();

  /*  EXAMPLES

  generally do about 1 inch per second for the base speed, 0.01 for standard coefficient (unless it starts going off course, then increase)
  do the acceleration based on how fast you want it to go (50-150)

  format is like this "drive(direction, inches, seconds, acceleration, returnToCourse)"

  format for turn "turn(direction, degrees, seconds, acceleration)"
    i havent tested what the best acceleration is, so you will have to experiment with the numbers

  drive(goForward, 100, 100, 150, 0.01);  //fast
  wait(3,sec);
  drive(goForward, 20, 30, 50, 0.01);   //medium
  wait(3,sec);
  drive(goBackward, 70, 90, 50, 0.01);//medium

  */

  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  //thread(setScreen).detach(); // Screen displays motors and temps
  //thread(autonTimer).detach(); //On-screen timer and haptics


  //INSTRUCTIONS:
  //the function drive() has parameters (direction, inches, and seconds)
      //enter direction as either goForward or goBackward
      //inches can be entered as a decimal
      //seconds can be entered as a decimal. this is the amount of time the drive will take
            //start with big number and work your way to a faster time.
            //faster drives mean less accuracy
            //do not enter 0, it will not like that
  //the function turn() has parameters (direction, degrees, and seconds)
      //the direction can either be left or right
          //enter as turnLeft or turnRight
      //degrees can be a decimal
      //seconds can be entered as a decimal
          //faster time means less accuracy
          //do not enter 0 or else it will go crazy


  //here is an example of both functions
  //       drive(goForward, 100, 8);        the robot will drive forward for 100 inches, in 8 seconds
  //       turn(turnRight, 180, 4);         the robot will turn right for 180 degrees, in 4 seconds

  //here is how to make the motors spin
  //      (name of motor).spin(forward);        makes motor spin forever
  //      (name of motor).spinFor(360, degrees); makes motor spin one time(any double or int can be entered)
  //      (name of motor).spinFor(20, seconds); makes motor spin for 20 seconds
  //      (name of motor).stop();               stops motor
  //      (name of motor).setStopping(coast/hold/brake);  changes the stopping mode (coast has no friction, brake has medium, hold is strongest)

  //here is how to use pneumatics
  //      mogoPistons.set(true);                sets the solenoid to true. this will extend the pistons
  //      mogoPistons.set(false);               sets the solenoid to false. this will pull the pistons in

  rightSide.setVelocity(40,percent);
  leftSide.setVelocity(40,percent);

  intakeMotor.setVelocity(100,percent);
  
  rightSide.setStopping(hold);
  leftSide.setStopping(hold);

  autonMode = 4;

  calibrateInertial();

  if(autonMode==4){ //skills run

      //autonomous functions seem to work. drive works well at certain numbers, and turnInertial is accurate at low speeds
      //ggg
      intakeTwo.setVelocity(100,percent);
      intakeTwo.spin(reverse);
      wait(1,sec);
      intakeTwo.stop();
      drive(goForward, 10, 5, 200, 0.03);

      turnDegrees(580); //90 degrees

      mogoPistons.set(true);
      drive(goBackward, 20, 20, 150, 0.03);
      mogoPistons.set(false);

      turnInertial(turnRight, 90, 25);

      intakeMotor.spin(forward);
      intakeTwo.spin(forward);
      drive(goForward, 20, 20, 200, 0.03);

      turnInertial(turnRight, 90, 25);
      
      drive(goForward, 20, 20, 200, 0.01);

    
  }

  if(autonMode==2){//blue goal side / red ring side
    //bluegoalside/redringside
      leftSide.setVelocity(30,percent);
      rightSide.setVelocity(30,percent);
      mogoPistons.set(true);
      driveDegrees(-1700);  //1700 motor revolutions
      leftSide.setVelocity(15,percent);
      rightSide.setVelocity(15,percent);
      wait(0.5,sec);
      mogoPistons.set(false);
      intakeMotor.setVelocity(100,percent);
      intakeTwo.setVelocity(100,percent);
      intakeMotor.spin(forward);
      intakeTwo.spin(forward);
      turnDegrees(450);  //need to change to new function
      driveDegrees(700);  //need to change to new function
      wait(1,seconds);
      turnDegrees(990); //need to change to new function
      leftSide.setVelocity(50,percent);
      rightSide.setVelocity(50,percent);
      driveDegrees(1600); //need to change to new function
      intakeMotor.stop();
      intakeTwo.stop();
      leftSide.setVelocity(5,percent);
      rightSide.setVelocity(5,percent);
      driveDegrees(300);  //need to change to new function
  }
  if(autonMode==1){ //red goal side / blue ring side
      leftSide.setVelocity(30,percent);
      rightSide.setVelocity(30,percent);
      mogoPistons.set(true);
      driveDegrees(-1700);  //need to change to new function
      leftSide.setVelocity(15,percent);
      rightSide.setVelocity(15,percent);
      wait(0.5,sec);
      mogoPistons.set(false);
      intakeMotor.setVelocity(100,percent);
      intakeTwo.setVelocity(100,percent);
      intakeMotor.spin(forward); 
      intakeTwo.spin(forward);
      turnDegrees(-450); //need to change to new function
      driveDegrees(700);  //need to change to new function
      wait(1,seconds);
      turnDegrees(-990);  //need to change to new function
      leftSide.setVelocity(50,percent);
      rightSide.setVelocity(50,percent);
      driveDegrees(1600); //need to change to new function
      leftSide.setVelocity(5,percent);
      rightSide.setVelocity(5,percent);
      driveDegrees(300);  //need to change to new function
  }
  if(autonMode==3){
    //drive forward to get off the starting line
    drive(goForward, 100, 15, 50, 1);  //need to change to new function
  }
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


  //Controller Scheme

  /*
  * 1 = intake forward
  * 2 = intake reverse
  * 3 = hook forward
  * 4 = hook reverse
  * 5 = mogo extend
  * 6 = mogo contract
  */
  //drivetrain is locked to analog sticks (tank controls) unless you want something else

  //set what each button does based on numbers

  /*
  int r1 = 1; // intake forw
  int r2 = 2; // intake revers
  int l1 = 3; // hook forwa
  int l2 = 4; // hook revers
  int buttonA;
  int buttonB;
  int buttonX;
  int buttonY;
  int dpadDown = 5; // mogo out
  int dpadUp = 6; // mogo in
  int dpadLeft;
  int dpadRight;

  */

  //Set each control mode:

  //Control Types (toggle/hold/set)
  //1 = toggle (press same button to activate and deactivate)
  //2 = hold (hold button to activate and release to deactivate)
  //3 = set (press button to activate, must press something else to deactivate)

  /*int intakeForwardMode;
  int intakeReverseMode;
  int hookForwardMode;
  int hookReverseMode;
  int mogoExtMode;
  int mogoContMode;*/


  //Create variables and configurations (This stuff only runs once after autonomous)
  //This is where to put speed, motor configurations, etc. that you want to start with (they can be changed during the loop too)

  leftSide.setStopping(coast);
  rightSide.setStopping(coast);

  intakeMotor.setStopping(coast);
  intakeTwo.setStopping(coast);

  intakeMotor.setVelocity(100,percent);
  intakeTwo.setVelocity(100,percent);

  while (1) { //driver loop; catches thread and listens for controller input

    //Everything inside this loop runs every 20ms during driving phase

    //Robot Info

    //DriveTrain Motors
    
    leftSide.setVelocity(Controller.Axis3.position(), percent); //sets the speed of the left motors
    rightSide.setVelocity(Controller.Axis2.position(), percent); //sets the speed of the right motors

    leftSide.spin(forward); //motors always spinning, but if joystick centered then speed = 0 so it doesnt matter
    rightSide.spin(forward);

    /*
    if(Controller.ButtonR1.pressing()==true){

      switch(r1){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }

    }
    if(Controller.ButtonR2.pressing()==true){
      switch(r2){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    if(Controller.ButtonL1.pressing()==true){
      switch(l1){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    if(Controller.ButtonL2.pressing()==true){
      switch(l2){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    if(Controller.ButtonA.pressing()==true){
      switch(buttonA){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    if(Controller.ButtonB.pressing()==true){
      switch(buttonB){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    if(Controller.ButtonX.pressing()==true){
      switch(buttonX){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    if(Controller.ButtonY.pressing()==true){
      switch(buttonY){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    if(Controller.ButtonUp.pressing()==true){
      switch(dpadUp){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    if(Controller.ButtonLeft.pressing()==true){
      switch(dpadLeft){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    if(Controller.ButtonRight.pressing()==true){
      switch(dpadRight){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    if(Controller.ButtonDown.pressing()==true){
      switch(dpadDown){
        case 1: intakeMotor.spin(forward) break;
        case 2: intakeMotor.spin(reverse) break;
        case 3: hookMotor.spin(forward) break;
        case 4: hookMotor.spin(reverse) break;
        case 5: mogoPistons.set(true) break;
        case 6: mogoPistons.set(false) break;
        default: break;
      }
    }
    
    */

    //Intake Motor

    if(Controller.ButtonR2.pressing()==true && Controller.ButtonR1.pressing()==false){
      //if the right trigger is being pressed AND the right bumper is not then it outtakes
      intakeMotor.spin(reverse);
      intakeTwo.spin(reverse);
    }

    else if(Controller.ButtonR1.pressing()==true && Controller.ButtonR2.pressing()==false){
      //if the right bumper is being pressed AND the right trigger is not then it intakes
      intakeMotor.spin(forward);
      intakeTwo.spin(forward);
    }

    else{
      //if no buttons are being pressed OR both buttons are being pressed then it stops
      intakeMotor.stop();
      intakeTwo.stop();
    }
    

    //Pneumatics

    if(Controller.ButtonRight.pressing()==true){//button is being pressed

      if(mogoPistons.value()==0 && clampDebounce==false){ //if mogo is retracted
        clampDebounce=true;
        mogoPistons.set(true);
      }

      if(mogoPistons.value()==1 && clampDebounce==false){  //if mogo is extended
        clampDebounce=true;
        mogoPistons.set(false); //retract
      }
    }
    if(Controller.ButtonRight.pressing()==false){//button is not being pressed
      clampDebounce=false;
    }

    if(Controller.ButtonY.pressing()==true){//button is being pressed

      if(swiperPiston.value()==0 && swiperDebounce==false){ //if mogo is retracted
        swiperDebounce=true;
        swiperPiston.set(true);
      }

      if(swiperPiston.value()==1 && swiperDebounce==false){  //if mogo is extended
        swiperDebounce=true;
        swiperPiston.set(false); //retract
      }
    }
    if(Controller.ButtonY.pressing()==false){//button is not being pressed
      swiperDebounce=false;
    }

    //Controller Haptics

    if(sideController.installed()==true){//dont run into error if controller is not being used
      if(sideController.ButtonR2.pressing()==true && hapticDebounce == false){
        Controller.rumble("-"); //continuous vibration
        sideController.rumble("-");
      }
      if(sideController.ButtonA.pressing()==true && hapticDebounce == false){
        hapticDebounce = true;
        Controller.rumble(". . ."); //three quick taps
        sideController.rumble(". . .");
      }
      if(sideController.ButtonR1.pressing()==true && hapticDebounce == false){
        hapticDebounce = true;
        Controller.rumble(" . "); //rapid fire taps
        sideController.rumble(" . ");
      }
      else if(sideController.ButtonA.pressing()==false && sideController.ButtonR2.pressing()==false && sideController.ButtonR1.pressing()==false && hapticDebounce == true){
        hapticDebounce = false; //reset debounce
      }
    }

    wait(20, msec);

  } //end of the loop

} //end of the usercontrol(driver phase)segment

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the autonomous selector and pre-auton
  pre_auton();
  //ends when autonomous starts

  

  //waitForTimer(&Competition); //waits until driver control starts, and starts timer

  // Prevent main from exiting with an infinite loop.
  while (true) {
    
    wait(100, msec);
  }
}
