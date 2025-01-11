
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "vex.h";
using namespace vex;

void turn(std::string direction, double degreesTurn, double timeTurn);
void drive(std::string direction, double distanceDrive, double timeDrive);

void driveDegrees(double number);
void turnDegrees(double number);

void setScreen();
void autonTimer();
void matchTimer();
void skillsTimer();

void runHaptics(int hapticType);

void calibrateInertial();

void waitForTimer(competition* comp);

#endif