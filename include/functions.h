
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "vex.h";
using namespace vex;

void turn(std::string direction, double degreesTurn, double timeTurn, double relativeCoefficient);
void drive(std::string direction, double distanceDrive, double timeDrive, double relativeCoefficient, double standardCoefficient);

void driveDegrees(double number);
void turnDegrees(double number);

void setScreen();
void autonTimer();
void matchTimer();
void skillsTimer();

void runHaptics(int hapticType);

void calibrateInertial();

void waitForTimer(competition* comp);

void setNegVelocity(motor_group* motorGroup, double vel);
void setNegVelocity(motor* motor, double vel);

#endif