
#include "vex.h"
#include "robot-config.h"
#include "functions.h"
using namespace vex;




/*  AUTONOMOUS LOCOMOTION  */

void turn(std::string direction, double degreesTurn, double timeTurn, double relativeCoefficient)
{
  //relativeCoefficient is passed in, determines the acceleration speed coefficient (RPMS per degree)
  //standard coefficient is the global turn speed. doubling the number will double all turn speed (but not double acceleration)
  double standardCoefficient = 1;

  double robotDiameter = 12; // center of robot to wheel in inches
  double outputGear = 60;       // drives wheel
  double inputGear = 36;        // driven by motor

  double linearDistance = (degreesTurn * 3.1415 * (robotDiameter/2)) / 180;
  double rotations = linearDistance / (3.25 * 3.1415);
  double motorRotations = rotations * (outputGear / inputGear);
  double speedRPM = (motorRotations / timeTurn) * 60;

  leftSide.setStopping(brake);
  rightSide.setStopping(brake);

  if (direction == "left")
  {
    inertialSensor.resetHeading(); // reset rotation
    inertialSensor.setHeading(240, degrees);
    leftSide.setVelocity(-1*speedRPM, rpm);
    rightSide.setVelocity(speedRPM, rpm);

    leftSide.spin(forward);
    rightSide.spin(forward);

    while (inertialSensor.heading() > 240 - degreesTurn)
    {
      double degreesLeft = inertialSensor.heading() - (240-degreesTurn);
      double speedChange = relativeCoefficient*degreesLeft;

      setNegVelocity(&leftSide, standardCoefficient*speedRPM+speedChange);
      rightSide.setVelocity(standardCoefficient*speedRPM+speedChange,rpm);
    }
    leftSide.stop();
    rightSide.stop();
  }

  else if (direction == "right")
  {
    inertialSensor.resetHeading(); // reset rotation
    inertialSensor.setHeading(60, degrees);
    leftSide.setVelocity(speedRPM, rpm);
    rightSide.setVelocity(-1*speedRPM, rpm);

    leftSide.spin(forward);
    rightSide.spin(forward);

    while (inertialSensor.heading() < 60 + degreesTurn)
    {
      double degreesLeft = 60 + degreesTurn - inertialSensor.heading();
      double speedChange = relativeCoefficient*degreesLeft;

      leftSide.setVelocity(standardCoefficient*speedRPM+speedChange,rpm);
      setNegVelocity(&rightSide, standardCoefficient*speedRPM+speedChange);

    }
    leftSide.stop();
    rightSide.stop();
  }

}

void drive(std::string direction, double distanceDrive, double timeDrive, double relativeCoefficient, double standardCoefficient)
{
  //relative coefficient is how fast it accelerates and decelerates as it approaches the target
  
  // function allows coder to input desired time for drive, code will automatically find speed to match it
  double outputGear = 60; // drives wheel
  double inputGear = 36;  // driven by motor
  double rotations = distanceDrive / (3.25 * 3.1415);
  double motorRotations = rotations * (outputGear / inputGear);
  double speedRPM = (motorRotations / timeDrive) * 60;

  leftSide.setStopping(brake);  //set braking types
  rightSide.setStopping(brake);

  leftSide.setVelocity(speedRPM, rpm);  //set initial velocities
  rightSide.setVelocity(speedRPM, rpm);

  inertialSensor.setHeading(180,degrees); //reset heading for rotation tracking

  rightSide.setPosition(0,degrees); //reset motor positions for distance tracking
  leftSide.setPosition(0,degrees);

  if (direction == "forward")
  {
    //loop will catch thread, and monitor inertial sensor. it will calculate the perpendicular stray from the desired path, and autocorrect
    while(leftSide.position(degrees) < motorRotations*360 || rightSide.position(degrees) < motorRotations*360){
      if(leftSide.position(degrees) >= motorRotations*360){leftSide.stop();}
      else{leftSide.spin(forward);}
      if(rightSide.position(degrees) >= motorRotations*360){rightSide.stop();}
      else{rightSide.spin(forward);}

      //calculate the position on the path and determine speed boost
      double distanceRotations = motorRotations - ( (leftSide.position(degrees)/360) + (rightSide.position(degrees)/360) )/2;
      double extraSpeedRPMLeft = (distanceRotations/motorRotations) * relativeCoefficient * leftSide.position(degrees)/360;
      double extraSpeedRPMRight = (distanceRotations/motorRotations) * relativeCoefficient * rightSide.position(degrees)/360;

      //MAKE AN EQUATION FOR ACCELERATION AT BEGINNING OF DRIVE

      //if robot has turned to the right
      if(inertialSensor.heading() > 180){
        double degreeStray = inertialSensor.heading() - 180;  //calculates the number of degrees that the direction has strayed from the desired path
        double returnSpeedRPM = standardCoefficient*degreeStray*(speedRPM+extraSpeedRPMRight); //an additional (variable) rpm for each degree that the robot has strayed
        rightSide.setVelocity(speedRPM+returnSpeedRPM+extraSpeedRPMRight,rpm); //add the speed to the side
        leftSide.setVelocity((speedRPM-returnSpeedRPM)+extraSpeedRPMLeft,rpm);
      }
      //if robot has turned to the left
      else if(inertialSensor.heading() < 180){
        double degreeStray = 180 - inertialSensor.heading();  //calculates the number of degrees that the direction has strayed from the desired path
        double returnSpeedRPM = standardCoefficient*degreeStray*(speedRPM+extraSpeedRPMLeft); //an additional (variable) rpm for each degree that the robot has strayed
        leftSide.setVelocity(speedRPM+returnSpeedRPM+extraSpeedRPMLeft,rpm);  //add the speed to the side
        rightSide.setVelocity((speedRPM-returnSpeedRPM)+extraSpeedRPMRight,rpm);
      }
      /*
      else{//robot is perfectly on path
        leftSide.setVelocity(speedRPM+extraSpeedRPMLeft,rpm); //reset the speed so it doesn't go off path again
        rightSide.setVelocity(speedRPM+extraSpeedRPMRight,rpm);
      }
      */
      wait(10,msec);//loop delay to prevent overload
    }

  }//end of "if direction == forward"

  if (direction == "reverse")
  {
    //loop will catch thread, and monitor inertial sensor. it will calculate the perpendicular stray from the desired path, and autocorrect
    while(leftSide.position(degrees) > motorRotations*(-360) || rightSide.position(degrees) > motorRotations*(-360)){
      if(leftSide.position(degrees) <= motorRotations*(-360) ){leftSide.stop();}
      else{leftSide.spin(forward);}
      if(rightSide.position(degrees) <= motorRotations*(-360) ){rightSide.stop();}
      else{rightSide.spin(forward);}

      //calculate the position on the path and determine speed boost
      double distanceRotations = (-1)*motorRotations - ( (leftSide.position(degrees)/360) + (rightSide.position(degrees)/360) )/2;
      distanceRotations *= -1;  //change distance(rotations) to positive

      double extraSpeedRPMLeft = (distanceRotations/motorRotations) * relativeCoefficient * leftSide.position(degrees)/-360;
      double extraSpeedRPMRight = (distanceRotations/motorRotations) * relativeCoefficient * rightSide.position(degrees)/-360;
      //MAKE AN EQUATION FOR ACCELERATION AT BEGINNING OF DRIVE

      //if robot has turned to the right
      if(inertialSensor.heading() > 180){
        double degreeStray = inertialSensor.heading() - 180;  //calculates the number of degrees that the direction has strayed from the desired path
        double returnSpeedRPM = standardCoefficient*degreeStray*(speedRPM+extraSpeedRPMRight);
        setNegVelocity(&rightSide,speedRPM-returnSpeedRPM+extraSpeedRPMRight); //add the speed to the side
        setNegVelocity(&leftSide,(speedRPM+returnSpeedRPM)+extraSpeedRPMLeft);
      }
      //if robot has turned to the left
      else if(inertialSensor.heading() < 180){
        double degreeStray = 180 - inertialSensor.heading();  //calculates the number of degrees that the direction has strayed from the desired path
        double returnSpeedRPM = standardCoefficient*degreeStray*(speedRPM+extraSpeedRPMLeft);
        setNegVelocity(&leftSide, speedRPM-returnSpeedRPM+extraSpeedRPMLeft);
        setNegVelocity(&rightSide, (speedRPM+returnSpeedRPM)+extraSpeedRPMRight);
      }
      else{//robot is perfectly on path
        setNegVelocity(&leftSide, speedRPM+extraSpeedRPMLeft); //reset the speed so it doesn't go off path again
        setNegVelocity(&rightSide, speedRPM+extraSpeedRPMRight);
      }
      wait(20,msec);//loop delay to prevent overload
    }

    Brain.Screen.print("drove backwards");
  }//end of "if direction == forward"


  leftSide.stop();
  rightSide.stop();
}//end of drive function

void turnInertial(std::string direction, double degreesTurn, double velocity){
  int turnDir = (direction == "left") ? 1 : 2;
  inertialSensor.resetHeading();

  switch(turnDir){
    case 1:
      inertialSensor.setHeading(240,degrees);

      leftSide.spin(forward);
      rightSide.spin(forward);

      while(inertialSensor.heading() > 240-degreesTurn){
        leftSide.setVelocity(-1* (velocity + (inertialSensor.heading() > 240-degreesTurn) * 5), percent);
        rightSide.setVelocity(velocity + (inertialSensor.heading() > 240-degreesTurn) * 5, percent);
        
      }

    break;
    case 2:
      inertialSensor.setHeading(120,degrees);

      leftSide.setVelocity(velocity, percent);
      rightSide.setVelocity(-1*velocity, percent);

      while(inertialSensor.heading() < 120+degreesTurn){
        leftSide.spin(forward);
        rightSide.spin(forward);
      }
    break;
  }
  leftSide.stop();
  rightSide.stop();

}

/* BASIC LOCOMOTION */

void driveDegrees(double number)
{
  leftSide.spinFor(number, degrees, false);
  rightSide.spinFor(number, degrees);
}

void turnDegrees(double number)
{
  leftSide.spinFor(number, degrees, false);
  rightSide.spinFor(-number, degrees);
}

/*  SCREEN FUNCTIONS  */

void setScreen()
{

  // this thread will not self terminate
  // needs testing to see if vex systems will terminate thread
  while (1)
  {

    int batteryLife = vexBatteryCapacityGet(); // Get Robot Battery

    int tempLFM = LFM.temperature(); // Get Temperature of Motors
    int tempLMM = LMM.temperature();
    int tempLBM = LBM.temperature();
    int tempRFM = RFM.temperature();
    int tempRMM = RMM.temperature();
    int tempRBM = RBM.temperature();

    // Controller.Screen.clearScreen();

    Controller.Screen.clearLine();

    Controller.Screen.setCursor(0, 0); // top left of controller
    Controller.Screen.print("BTRY");

    Controller.Screen.setCursor(0, 6);    // top of screen, right after "Battery:"
    Controller.Screen.print(batteryLife); // print the current battery level(%)

    Controller.Screen.setCursor(1, 13); // top of controller but right of "temps"
    Controller.Screen.print(tempLFM);   // Temperature of Left Front Motor

    Controller.Screen.setCursor(1, 17); // top of controller but right of LFM
    Controller.Screen.print(tempRFM);   // Temperature of Right Front Motor

    Controller.Screen.clearLine();

    Controller.Screen.setCursor(2, 16); // right below LFM
    Controller.Screen.print(tempLMM);   // Temperature of Left Middle Motor (The higher up one on the robot)

    Controller.Screen.setCursor(2, 20); // to the right of LMM
    Controller.Screen.print(tempRMM);   // right middle motor (higher up one on robot);

    Controller.Screen.clearLine();

    Controller.Screen.setCursor(3, 16); // right below LMM
    Controller.Screen.print(tempLBM);   // Left Back motor (lower one in the back)

    Controller.Screen.setCursor(3, 20); // to the right of LBM
    Controller.Screen.print(tempRBM);

    wait(500, msec); // screen update timer (twice per second)

  } // end of loop

} // end of function

void autonTimer()
{
  // timer that updates screen and haptics during autonomous
  // this thread will self-terminate after the round ends
  for (int timer = 15; timer > 0; timer--)
  {
    // loop ends after 15 seconds
    // int timer is the duration of the loop
    // during the autonomous phase
    Controller.Screen.setCursor(2, 8); // Center of Screen
    Controller.Screen.clearLine();
    Controller.Screen.print(timer);
    wait(1, sec); // run code 1 times per sec
  }
}

void matchTimer(competition *comp)
{

  // timer that runs during the match
  // this thread will self-terminate

  Brain.Timer.clear(); // set timer to zero

  bool debounce1 = false;
  bool debounce2 = false;
  bool debounce3 = false; // create the debounces. only allows rumbles to run once

  while (comp->isDriverControl() == true)
  {
    // loop will close after driver control (allows brain to clean up)

    if (Brain.Timer.time(msec) > 75000 and debounce1 == false)
    { // when it is between 75 sec and 76 sec
      debounce1 = true;
      runHaptics(1);
    } // 75 seconds into match

    if (Brain.Timer.time(msec) >= 90000 and debounce2 == false)
    {
      debounce2 = true;
      runHaptics(2);
    } // 90 seconds in//15 seconds left

    if (Brain.Timer.time(msec) >= 95000 and debounce3 == false)
    {
      debounce3 = true;
      runHaptics(3);
    } // 95 seconds in//10 seconds left

    Controller.Screen.setCursor(2, 8); // Center of Screen
    Controller.Screen.clearLine();
    Controller.Screen.print(Brain.Timer.time());
    wait(250, msec);
  }

  // OLD CODE
  //     |
  //     V

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

void matchTimerThread(void *arg)
{
  matchTimer((competition *)arg);
}

void skillsTimer()
{

  for (int timer = 60; timer > 0; timer--)
  {
    // loop ends after 60 seconds (1 minute)
    // int timer is the duration of the loop
    // during the skills phase
    if (timer == 30)
    {
      // 30 seconds left
      runHaptics(1);
    }
    if (timer == 15)
    {
      // 15 seconds left
      runHaptics(2);
    }
    if (timer == 10)
    {
      // 10 seconds left
      runHaptics(3);
    }
    Controller.Screen.setCursor(2, 8); // Center of Screen
    Controller.Screen.clearLine();
    Controller.Screen.print(timer);
    wait(1, sec); // run code 1 times per sec
  }
}

/* CONTROLLER FUNCTIONS */

void runHaptics(int hapticType)
{
  // types of haptics:
  // 1 = one long buzz (happens at 30 seconds left)
  // 2 = three quick buzzes (happens at 15 seconds left) (not allowed to touch corners)
  // 3 = two long buzzes (happens at 10 seconds left) (not allowed to impede hangs)

  switch (hapticType)
  {

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

/* SENSOR FUNCTIONS */

void calibrateInertial()
{
  inertialSensor.calibrate();
  while (inertialSensor.isCalibrating())
  {
    wait(25, msec); // wait for inertial sensor to calibrate before continuing
  }
}

/* LOGIC FUNCTIONS */

void waitForTimer(competition *comp)
{
  bool debounce = false;
  while (debounce == false)
  {
    if (comp->isDriverControl() == true)
    {
      debounce = true; // debounce allows it to only run once
      // detach thread to start the game timer
      Controller.Screen.clearScreen();
      thread(matchTimerThread, (void *)comp).detach();
    }
    wait(50, msec); // interval time
  }
}

void setNegVelocity(motor_group *motorGroup, double vel){
  motorGroup->setVelocity(-1*vel, rpm);
}
void setNegVelocity(motor *motor, double vel){
  motor->setVelocity(-1*vel, rpm);
}