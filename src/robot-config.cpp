#include "vex.h"
using namespace vex;

brain Brain;
controller Controller = controller(primary);
motor LFM = motor(PORT1, ratio6_1, false);
motor LMM = motor(PORT2, ratio6_1, false);
motor LBM = motor(PORT3, ratio6_1, false);
motor RFM = motor(PORT4, ratio6_1, true);
motor RMM = motor(PORT5, ratio6_1, true);
motor RBM = motor(PORT6, ratio6_1, true);

motor_group leftSide = motor_group(LFM,LMM,LBM);
motor_group rightSide = motor_group(RFM, RMM, RBM);