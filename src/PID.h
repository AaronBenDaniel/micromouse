//Define Variables we'll be connecting to
double RotationalSetpoint, RotationalInput, RotationalOutput;

//Specify the links and initial tuning parameters
double RotationalKp=30000, RotationalKi=60000, RotationalKd=1000;
PID RotationalPID(&RotationalInput, &RotationalOutput, &RotationalSetpoint, RotationalKp, RotationalKi, RotationalKd, REVERSE);

//Define Variables we'll be connecting to
double LinearRSetpoint, LinearRInput, LinearROutput;

//Specify the links and initial tuning parameters
double LinearRKp=5000, LinearRKi=1000, LinearRKd=1000;
PID LinearRPID(&LinearRInput, &LinearROutput, &LinearRSetpoint, LinearRKp, LinearRKi, LinearRKd, DIRECT);

//Define Variables we'll be connecting to
double LinearLSetpoint, LinearLInput, LinearLOutput;

//Specify the links and initial tuning parameters
double LinearLKp=5000, LinearLKi=1000, LinearLKd=1000;
PID LinearLPID(&LinearLInput, &LinearLOutput, &LinearLSetpoint, LinearLKp, LinearLKi, LinearLKd, DIRECT);

