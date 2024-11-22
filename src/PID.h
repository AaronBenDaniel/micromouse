//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=30000, Ki=30000, Kd=1000;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);