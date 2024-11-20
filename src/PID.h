//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=10000, Ki=10000, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);