#include "robot-config.h"
#include "helpfunctions.h"
#include "altvex.h"
#include <stdio.h>
 
competition Competition;
 
// Motor Groups
motor_group d_right(motor2, motor4);
motor_group d_left(motor1, motor3);
motor_group lift(motor7, motor8);

// Global variables, because VEX is stupid
const float D_SPEEDS[5] = {0.25, 0.5, 0.75, 1};
const float L_SPEEDS[4] = {0.25, 0.5, 1};
int ds_count = arrlen((int *)D_SPEEDS);
int ls_count = arrlen((int *)L_SPEEDS);
int ds_track = ds_count - 1;
int ls_track = ls_count - 1;
float dspeed = D_SPEEDS[ds_track - 1];
float lspeed = L_SPEEDS[ls_track - 1];
bool tank = false;
int ddir = 1;

// Motor rev adjusters
int dtype = NORMAL;
int ltype = TORQUE;
int ctype = TORQUE;

// Prototypes for movement settings
void dswap_type(void);
void dswap_speed_up(void);
void lswap_speed_up(void);
void dswap_speed_down(void);
void lswap_speed_down(void);
void dswap_dir(void);

void print_info(void);

// Prototypes for major stuff
void move_drive(float dr_val, float dl_val);
void move_lift();
void move_claw(void);
void pre_auton(void);
void autonomous(void);
void usercontrol(void);

// Main will set up the competition functions and callbacks.
int main() {
  pre_auton();
  print_info();
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  
  // Drive Speeds
  control.ButtonUp.released(dswap_speed_up);
  control.ButtonDown.released(dswap_speed_down);
  // Lift Speeds
  control.ButtonB.released(lswap_speed_down);
  control.ButtonX.released(lswap_speed_up);
  // Drive Type
  control.ButtonY.released(dswap_type);
  // Drive Direction
  control.ButtonA.released(dswap_dir);
  while (true) {
    task::sleep(100);// Sleep the task for a short amount of time to prevent wasted resources.
  }      
}

void pre_auton(void) {
  // Pre-autonomous, to set up anything that happens before a robot starts
  // Set non-drive brake types
  motor3.setStopping(brakeType::hold);
  motor4.setStopping(brakeType::hold);
  claw.setStopping(brakeType::hold);
  motor6.setStopping(brakeType::hold);
  motor8.setStopping(brakeType::hold);
}

void autonomous(void) {

} 

void usercontrol(void) {
  // Acceleration values of drive motors
  float drValue = 0;
  float dlValue = 0;
  
  // Start user control
  while (true) {
    move_drive(drValue, dlValue);
    move_lift();
    move_claw();

    // End of usercontrol
    task::sleep(20); // Sleep the task for a short amount of time to prevent wasted resources.
  }
}

void move_drive(float dr_val, float dl_val)
{
  // Implementation of tank drive
  if (tank && ddir == 1) {
    dr_val = control.Axis2.value() * dspeed * dtype;
    dl_val = control.Axis3.value() * dspeed * dtype;
  }
  else if (tank && ddir == -1) {
    dl_val = control.Axis2.value() * ddir * dspeed * dtype;
    dr_val = control.Axis3.value() * ddir * dspeed * dtype;
  }
  // Implementation of arcade drive
  else if (!tank) {
    dr_val = (control.Axis3.value() * ddir - control.Axis1.value()) * dspeed * dtype;
    dl_val = (control.Axis3.value() * ddir + control.Axis1.value()) * dspeed * dtype;
  }
  
  // Change motors by given values
  d_right.spin(directionType::fwd, dr_val, rpm);
  d_left.spin(directionType::fwd, dl_val, rpm);
}

void move_lift() {
  // Move lift
  if (control.ButtonR1.pressing()) {
    lift.spin(directionType::fwd, 100 * lspeed * ltype, velocityUnits::rpm);
  }
  else if (control.ButtonR2.pressing()) {
    lift.spin(directionType::rev, 100 * lspeed * ltype, velocityUnits::rpm);
  }
  else lift.stop();
}

void move_claw(void)
{
  if (control.ButtonL1.pressing()) {
    claw.spin(directionType::rev, 100 * ctype, velocityUnits::rpm);
  }
  else if (control.ButtonL2.pressing()) {
    claw.spin(directionType::fwd, 100 * ctype, velocityUnits::rpm);
  }
  else claw.stop();
}

// Swap between tank drive and arcade drive
void dswap_type(void) {
  tank = !tank;
  print_info();
}
// Change drive speeds
void dswap_speed_up(void) {
  ds_track++;
  if (ds_track > ds_count - 1) {
    ds_track = 0;
  }
  dspeed = D_SPEEDS[ds_track];

  print_info();
}
void dswap_speed_down(void) {
  ds_track = ds_track - 1;
  if (ds_track < 0) {
    ds_track = ds_count - 1;
  }
  dspeed = D_SPEEDS[ds_track];

  print_info();
}
// Change lift speeds
void lswap_speed_up(void) {
  // Cycle ls_track for every value in L_SPEEDS array
  ls_track++;
  if (ls_track > ls_count - 1) {
    ls_track = 0;
  }
  lspeed = L_SPEEDS[ls_track];

  print_info();
}
void lswap_speed_down(void) {
  ls_track = ls_track - 1;
  if (ls_track < 0) {
    ls_track = ls_count - 1;
  }
  lspeed = L_SPEEDS[ls_track];

  print_info();
}
// Change drive direction
void dswap_dir(void) {
  ddir = ddir * -1;
  print_info();
}

void print_info(void) {
  control.Screen.clearScreen();
  control.Screen.setCursor(1, 1);
  if (tank) {
    control.Screen.print("D_Type: Tank");
  }
  else {
    control.Screen.print("D_Type: Arcade");
  }
  if (ddir == 1) {
    control.Screen.print(", F");
  }
  else {
    control.Screen.print(", R");
  }
  control.Screen.setCursor(2, 1);
  control.Screen.print("D_Speed: %f", dspeed);
  control.Screen.setCursor(3, 1);
  control.Screen.print("L_speed: %f", lspeed);

  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(5, 5);
  Brain.Screen.setCursor(10, 5);
  Brain.Screen.print("Deg: %f\n", motor7.position(rotationUnits::deg));
  Brain.Screen.print("Deg: %f\n", motor8.position(rotationUnits::deg));
  Brain.Screen.print("Drive Type: %i\nLift Type: %i\nClaw Type: %i\n", dtype, ltype, ctype);
}