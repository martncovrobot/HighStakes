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


//this code is updated as of 3:47 pm. 11/7/2024




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


void turn(std::string direction, double degreesTurn, double timeTurn){

  double robotDiameter = 6; //center of robot to wheel in inches
  int outputGear = 60; //drives wheel
  int inputGear = 36; //driven by motor
  double linearDistance = (degreesTurn*3.1415*(robotDiameter/2))/180;
  double rotations = linearDistance/(3.25*3.1415);
  double motorRotations = rotations*(outputGear/inputGear);
  double speedRPM = (motorRotations/timeTurn)*60;

  leftSide.setStopping(brake);
  rightSide.setStopping(brake);

  if(direction=="left"){
    inertialSensor.resetRotation(); //reset rotation

    leftSide.setVelocity(speedRPM, rpm);
    rightSide.setVelocity(speedRPM, rpm);

    while(inertialSensor.rotation() < degreesTurn){
      leftSide.spin(reverse);
      rightSide.spin(forward);
    }
    leftSide.stop();
    rightSide.stop();
  }

  else if(direction=="right"){

    inertialSensor.resetRotation(); //reset rotation
   
    leftSide.setVelocity(speedRPM, rpm);
    rightSide.setVelocity(speedRPM, rpm);

    //int speed = (botRadius*degrees)/time
    while(inertialSensor.rotation() < degreesTurn){
      leftSide.spin(forward);
      rightSide.spin(reverse);
    }
    leftSide.stop();
    rightSide.stop();


  }

  else{

    //direction is incorrect

  }


}



void drive(std::string direction, double distanceDrive, double timeDrive){

  //function allows coder to input desired time for drive, code will automatically find speed to match it
  int outputGear = 60; //drives wheel
  int inputGear = 36; //driven by motor
  double rotations = distanceDrive/(3.25*3.1415);
  double motorRotations = rotations*(outputGear/inputGear);
  double speedRPM = (motorRotations/timeDrive)*60;

  if(direction == "forward"){

    leftSide.setStopping(brake);
    rightSide.setStopping(brake);

    leftSide.setVelocity(speedRPM, rpm);
    rightSide.setVelocity(speedRPM, rpm);

    leftSide.spinFor(forward, motorRotations*360, degrees, speedRPM, rpm, false);
    rightSide.spinFor(forward, motorRotations*360, degrees, speedRPM, rpm, true);

  }

  if(direction=="reverse"){
    
    leftSide.setStopping(brake);
    rightSide.setStopping(brake);

    leftSide.setVelocity(speedRPM, rpm);
    rightSide.setVelocity(speedRPM, rpm);

    leftSide.spinFor(reverse, motorRotations*360, degrees, speedRPM, rpm, false);
    rightSide.spinFor(reverse, motorRotations*360, degrees, speedRPM, rpm, true);
    
  }

}





void autonomous(void) {
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

  std::string goForward = "forward";
  std::string goBackward = "reverse";
  std::string turnLeft = "left";
  std::string turnRight = "right";

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
  hookMotor.setStopping(coast);

  intakeMotor.setVelocity(100,percent);
  hookMotor.setVelocity(100,percent);

  //Designate another thread to running the screen

  //thread(setScreen).detach(); //Screen Display (Temps and battery)
  //thread(matchTimer).detach(); //On-screen timer and haptics


  while (1) {

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

    
    if(Controller.ButtonR2.pressing()==true && Controller.ButtonR1.pressing()==false){
      //if the right trigger is being pressed AND the right bumper is not then it intakes
      intakeMotor.spin(forward);
    }

    else if(Controller.ButtonR1.pressing()==true && Controller.ButtonR2.pressing()==false){
      //if the right bumper is being pressed AND the right trigger is not then it outtakes
      intakeMotor.spin(reverse);
    }

    else{
      //if no buttons are being pressed OR both buttons are being pressed then it stops
      intakeMotor.stop();
    }
    

    //Mobile-Goal Pneumatics

    if(Controller.ButtonRight.pressing()==true && Controller.ButtonDown.pressing()==false){
      //if the right dpad is being pressed AND the down dpad is not then it extends
      mogoPistons.set(true);
    }
    else if(Controller.ButtonDown.pressing()==true && Controller.ButtonRight.pressing()==false){
      //if the down dpad is being pressed AND the right dpad is not then it contracts
      mogoPistons.set(false);
    }
    else{
      //if no buttons are being pressed OR both buttons are being pressed then it does nothing
    }

    

    //Hook Motor


    if(Controller.ButtonL2.pressing()==true && Controller.ButtonL1.pressing()==false){
      //if the right trigger is being pressed AND the right bumper is not then the hook goes up
      hookMotor.spin(forward);
    }

    else if(Controller.ButtonL1.pressing()==true && Controller.ButtonL2.pressing()==false){
      //if the left bumper is being pressed AND the left trigger is not then the hook goes down
      hookMotor.spin(reverse);
    }

    else{
      //if no buttons are being pressed OR both buttons are being pressed then it stops
      hookMotor.stop();
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
