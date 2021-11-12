#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

// Normal Devices
//  6_1 = speed motor, 600 RPM
//  18_1 = normal motor, 200 RPM
//  36_1 = torque motor, 100 RPM
using namespace vex;
brain Brain;
motor motor1 (PORT1, ratio18_1, false);
motor motor2 (PORT2, ratio18_1, true);
motor motor3 (PORT3, ratio18_1, false);
motor motor4 (PORT4, ratio18_1, true);
motor claw (PORT20, ratio36_1, false);
// Motor 6 does not work currently
motor motor6 (PORT6, ratio18_1, true);
motor motor7 (PORT7, ratio36_1, false);
motor motor8 (PORT8, ratio36_1, true);
controller control;

// Sensors
sonar sonar1 (Brain.ThreeWirePort.A);