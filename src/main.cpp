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

// define your global variables/objects
int autonMode;
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


//this code is updated as of 9:22 am. 11/18/2024




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

  Brain.Timer.clear();//set timer to zero

  bool debounce1 = false; 
  bool debounce2 = false; 
  bool debounce3 = false; //create the debounces. only allows rumbles to run once

  
  while(Competition.isDriverControl()==true){
    //loop will close after driver control (allows brain to clean up)
    
    if(Brain.Timer.time(msec)>75000 and debounce1==false){ //when it is between 75 sec and 76 sec
      debounce1=true;
      runHaptics(1);
    }//75 seconds into match

    if(Brain.Timer.time(msec)>=90000 and debounce2==false){
      debounce2=true;
      runHaptics(2);
    }//90 seconds in//15 seconds left

    if(Brain.Timer.time(msec)>=95000 and debounce3==false){
      debounce3=true;
      runHaptics(3);
    }//95 seconds in//10 seconds left

    Controller.Screen.setCursor(2,8); //Center of Screen
    Controller.Screen.clearLine();
    Controller.Screen.print(Brain.Timer.time());
    wait(250,msec);
  }

  //OLD CODE 
  //    |
  //    V

  /*

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

  */


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

void calibrateInertial(){
  inertialSensor.calibrate();
  while(inertialSensor.isCalibrating()){
    wait(25,msec);//wait for inertial sensor to calibrate before continuing
  }
}


void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  calibrateInertial();

  while(Competition.isAutonomous()==false){
    //autonomous switcher
    //this loop ends when autonomous starts
    if(Controller.ButtonX.pressing()==true) autonMode = 1; //red side
    else if(Controller.ButtonA.pressing()==true) autonMode = 2; // blue side
    else if(Controller.ButtonUp.pressing()==true) autonMode = 3; // wall goal left
    else if(Controller.ButtonLeft.pressing()==true) autonMode = 4; //wall goal right
    Controller.Screen.clearLine();
    Controller.Screen.setCursor(1,1);
    switch(autonMode){
      case 1: Controller.Screen.print("Left Side"); break;
      case 2: Controller.Screen.print("Right Side"); break;
      case 3: Controller.Screen.print("Wall Goal Left"); break;
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


void turn(std::string direction, double degreesTurn, double timeTurn){

  double robotDiameter = 12; //center of robot to wheel in inches
  int outputGear = 60; //drives wheel
  int inputGear = 36; //driven by motor
  double linearDistance = (degreesTurn*3.1415*(robotDiameter/2))/180;
  double rotations = linearDistance/(3.25*3.1415);
  double motorRotations = rotations*(outputGear/inputGear);
  double speedRPM = (motorRotations/timeTurn)*60;

  leftSide.setStopping(brake);
  rightSide.setStopping(brake);

  if(direction=="left"){
    inertialSensor.resetHeading(); //reset rotation
    inertialSensor.setHeading(240, degrees);
    //rotations will be negative degrees
    leftSide.setVelocity(speedRPM, rpm);
    rightSide.setVelocity(speedRPM, rpm);

    while(inertialSensor.heading() > 240-degreesTurn){
      leftSide.spin(reverse);
      rightSide.spin(forward);
    }
    leftSide.stop();
    rightSide.stop();
  }

  else if(direction=="right"){
    //rotations will be positive degrees
    inertialSensor.resetHeading(); //reset rotation
    inertialSensor.setHeading(60, degrees);

    leftSide.setVelocity(speedRPM, rpm);
    rightSide.setVelocity(speedRPM, rpm);

    //int speed = (botRadius*degrees)/time
    while(inertialSensor.heading() < 60+degreesTurn){
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
  distanceDrive;
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

void waitForTimer(){
  bool debounce = false;
  while(debounce == false){
    if(Competition.isDriverControl()==true){
      debounce = true; //debounce allows it to only run once
      //detach thread to start the game timer
      thread(matchTimer).detach();
    }
    wait(50,msec); //interval time
  }
}


void autonomous(void) {

  //initializing autonomous
  leftSide.stop();
  rightSide.stop();

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
  //hookMotor.setVelocity(100,percent);

  //hookMotor.setStopping(brake);

  rightSide.setStopping(hold);
  leftSide.setStopping(hold);

  /*Outdated Autonomi

  if(autonMode==2){ //blue side auton
    Controller.Screen.print("2");
    mogoPistons.set(true);
    drive(goBackward, 26.832, 1);

    wait(0.25,seconds);

    mogoPistons.set(false);

    //0.45 seconds at 40% is 90 degrees


    turn(turnLeft, 45, 0.5);
    
    rightSide.spin(forward);
    leftSide.spin(reverse);
    wait(0.225,seconds);
    rightSide.stop();
    leftSide.stop();

    intakeMotor.spin(reverse);
    drive(goForward, 24, 2);
    wait(1,sec);
    intakeMotor.stop();
  }

  if(autonMode==1){ //red side auton
    Controller.Screen.print("1");
    mogoPistons.set(true);
    drive(goBackward, 26.832, 1);

    wait(0.25,seconds);

    mogoPistons.set(false);

    //0.45 seconds at 40% is 90 degrees

    turn(turnRight, 45, 0.5);
    /*
    rightSide.spin(reverse);
    leftSide.spin(forward);
    wait(0.225,seconds);
    rightSide.stop();
    leftSide.stop();

    intakeMotor.spin(reverse);
    drive(goForward, 24, 2);
    wait(1,sec);
    intakeMotor.stop();

  }

  if(autonMode==3){ //score on wall stake from the left
    Controller.Screen.print("3");

    drive(goBackward, 18, 2); //move ring out of the way
    wait(0.5,sec);
    drive(goForward, 6, 1);

    wait(1,sec);  //let robot settle

    rightSide.setVelocity(40,percent);
    leftSide.setVelocity(40,percent);

    turn(turnRight, 90, 0.5);
    /*
    rightSide.spin(reverse);
    leftSide.spin(forward);
    wait(0.45,seconds);
    rightSide.stop();
    leftSide.stop();

    wait(0.5,sec);

    drive(goBackward, 1, 0.5);

    intakeMotor.spinFor(-235, degrees);

  }

  */

  if(autonMode==4){ //skills run

    intakeMotor.setVelocity(75,percent);
    intakeTwo.setVelocity(100,percent);

    mogoPistons.set(true);

    drive(goBackward, 18, 0.75);
    mogoPistons.set(false);

    intakeMotor.spin(forward);
    intakeTwo.spin(forward);

    wait(2,sec);

    intakeTwo.spin(reverse);

    turn(turnLeft, 20, 0.1);  //jiggle the ring in
    turn(turnRight, 20, 0.1);

    wait(1,sec);
    

    intakeMotor.stop();
    intakeTwo.stop();

    turn(turnLeft, 155, 1.5); //used to be 150
    intakeMotor.spin(forward);
    intakeTwo.spin(forward);
    drive(goForward, 44, 1.5); //intake first ring
    wait(0.5,sec);

    turn(turnLeft, 20, 0.1);  //jiggle the ring in
    turn(turnRight, 20, 0.1);

    intakeTwo.spin(forward);

    turn(turnRight, 82, 1); //used to be 80
    drive(goForward, 40, 1);//intake second ring

    wait(1,sec);

    turn(turnLeft, 20, 0.1);  //jiggle the ring in
    turn(turnRight, 20, 0.1);

    intakeTwo.spin(forward);

    turn(turnRight, 85, 1.1); //used to be 85
  
    drive(goForward, 40, 1.1);//intake third ring

    wait(0.5,sec);

    intakeTwo.spin(forward);

    drive(goForward, 20, 0.9); //intake fourth ring
    wait(2,sec);

    intakeMotor.stop();
    intakeTwo.stop();

    

    turn(turnRight, 100, 1); //used to be 100
    mogoPistons.set(true);

    drive(goForward, 20, 0.9);

    mogoPistons.set(false);

    leftSide.setVelocity(35,percent);
    rightSide.setVelocity(35, percent);
    
    leftSide.spin(reverse);
    rightSide.spin(reverse);

    wait(1.25,sec);

    drive(goForward, 80, 1.5);

    turn(turnLeft, 185, 1.5); //used to be 180

    mogoPistons.set(true);

    drive(goBackward, 50, 1);

    mogoPistons.set(false);

    //second corner starts here

    turn(turnLeft, 90, 0.9); //turn toward first ring

    intakeTwo.spin(forward);
    intakeMotor.spin(forward);

    drive(goForward, 40, 0.75); //intake first ring
    
    wait(1,sec);

    intakeTwo.spin(reverse);
    turn(turnLeft, 20, 0.1);  //jiggle the ring in
    turn(turnRight, 20, 0.1);

    intakeTwo.spin(forward);
    intakeMotor.spin(forward);

    turn(turnLeft,90,0.9); //turn toward to second ring
    drive(goForward, 45, 0.75);//intake the second ring

    wait(2,sec);

    intakeTwo.spin(reverse);
    turn(turnLeft, 20, 0.1);  //jiggle the ring in
    turn(turnRight, 20, 0.1);

    intakeTwo.spin(forward);

    turn(turnLeft, 90, 0.9);//turn toward third ring

    drive(goForward, 30, 0.75);//intake third ring

    intakeTwo.spin(reverse);
    turn(turnLeft, 20, 0.1);  //jiggle the ring in
    turn(turnRight, 20, 0.1);

    intakeTwo.spin(forward);

    drive(goForward, 30, 0.75);//intake fourth ring

    intakeTwo.spin(reverse);
    turn(turnLeft, 20, 0.1);  //jiggle the ring in
    turn(turnRight, 20, 0.1);

    intakeTwo.spin(forward);

    drive(goBackward, 30, 0.75);

    turn(turnRight, 90, 0.9);//turn to fifth ring

    drive(goForward, 20, 0.75);//intake fifth ring

    intakeTwo.spin(reverse);
    turn(turnLeft, 20, 0.1);  //jiggle the ring in
    turn(turnRight, 20, 0.1);

    intakeTwo.stop();

    turn(turnRight, 100, 1.5);

    drive(goForward, 30, 0.75);


    

  }



  if(autonMode==2){}
  if(autonMode==1){}
  if(autonMode==3){}


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

  Controller.Screen.print("end of autonomous");
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
      //if the right trigger is being pressed AND the right bumper is not then it intakes
      intakeMotor.spin(reverse);
      intakeTwo.spin(reverse);
    }

    else if(Controller.ButtonR1.pressing()==true && Controller.ButtonR2.pressing()==false){
      //if the right bumper is being pressed AND the right trigger is not then it outtakes
      intakeMotor.spin(forward);
      intakeTwo.spin(forward);
    }

    else{
      //if no buttons are being pressed OR both buttons are being pressed then it stops
      intakeMotor.stop();
      intakeTwo.stop();
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


  /* Hook Motor

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

  */

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

  //waitForTimer(); //waits until driver control starts, and starts timer

  // Prevent main from exiting with an infinite loop.
  while (true) {
    
    wait(100, msec);
  }
}
