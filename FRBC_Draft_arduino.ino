#include <Stepper.h>

unsigned long previousMilliseconds = 0; 

int emlimitSwitch = 13;

//Each Stepper Motor takes 4 pins 
int rotationalStepperPin1 = 7, rotationalStepperPin2 = 6, rotationalStepperPin3 = 5, rotationalStepperPin4 = 4; 

int UV_RelayPin = 12; 
int cycletime=15000;
int totaltime=60000;
int thistime=0;
int starttime=0;
int endtime=0;
int loopp=0;

int LED_onPin = 9; 
//int LED_emergencyStopIndicator = 8;  

int OnButtonPin = 11; 

const int stepsPerRevolution = 200; //Depends on stepper motor 
const int mmToOpen = 0; //TBD 
const int mmToClose = 0; //TBD

const int UV_Runtime_1 = 0; //TBD
const int UV_Runtime_2 = 0; //TBD
const int UV_Runtime_3 = 0; //TBD
const int UV_Runtime_4 = 0; //TBD 

const int rotationInDegrees1 = 0; //TBD
const int rotationInDegrees2 = 0; //TBD 
const int rotationInDegrees3 = 0; //TBD 
const int rotationInDegrees4 = 0; //TBD  

boolean EMERGENCY_INTERRUPT_STATE = false;
boolean ON_STATE = false; 
boolean SANATIZE_STATE = false; 
boolean OFF_STATE = false; 
boolean isClosed = false; 

Stepper rotationalStepperMotor(stepsPerRevolution, rotationalStepperPin1, rotationalStepperPin2, rotationalStepperPin3, rotationalStepperPin4); 

void setup(){
  pinMode(LED_onPin, OUTPUT); 
  pinMode(emlimitSwitch, INPUT);
  pinMode(rotationalStepperPin1, INPUT);
  pinMode(rotationalStepperPin2, INPUT);
  pinMode(rotationalStepperPin3, INPUT); 
  pinMode(rotationalStepperPin4, INPUT);  
  pinMode(UV_RelayPin, OUTPUT); 
  pinMode(OnButtonPin, INPUT); 
  attachInterrupt(emlimitSwitch, emergencyInterrupt, CHANGE); 
}

boolean waitMilliseconds(int milliseconds){
  unsigned long currentMilliseconds = millis();
  if(currentMilliseconds-previousMilliseconds >= milliseconds){
    previousMilliseconds = currentMilliseconds;
    return true;
  }else{
    return false; 
  }
}
void emergencyInterrupt(){
  EMERGENCY_INTERRUPT_STATE = true; 
   digitalWrite(UV_RelayPin, LOW);
   loopp=0;
   while(loopp < 10){
    digitalWrite(LED_onPin, LOW);
    while(!waitMilliseconds(20));
    digitalWrite(LED_onPin, HIGH); 
    while(!waitMilliseconds(20));
    loopp = loopp + 1;
   }
}


void onProcedure(){
    SANATIZE_STATE = true; 
}

void offProcedure(){
  if(isClosed){
    openContainer();
  }
}

void sanatizeProcedure(){
  thistime=0;
  while (!thistime == totaltime)
     {
     closeContainer();
     digitalWrite(UV_RelayPin, HIGH);
     starttime=millis();
     endtime=starttime;
    while ((endtime-starttime) <= (cycletime))
    //loop executes for totaltime
    { 
    rotationalStepperMotor.step(stepsPerRevolution); 
    endtime=millis();
    thistime=thistime + endtime;
    }
  }
    digitalWrite(UV_RelayPin, LOW);
    SANATIZE_STATE = false; 
    ON_STATE = false; 
    OFF_STATE = true;  
}

void openContainer(){
  //Move stepper to extrude the table outside of the container for remote operation. 
  digitalWrite(LED_onPin, LOW);
  isClosed = false; 
}
void closeContainer(){
  //Move stepper to retract the table back into the container. 
  digitalWrite(LED_onPin, HIGH); 
  isClosed = true;
}



void setRelay(boolean toggle){
  if(toggle){
    digitalWrite(UV_RelayPin, HIGH);
  }else{
    digitalWrite(UV_RelayPin, LOW); 
  }
}

void setRotationalMotorDegrees(int degree){
  rotationalStepperMotor.step(degrees_to_steps(degree));
}

void setRotationalMotorStep(int steps){
  rotationalStepperMotor.step(steps);
}


boolean isTriggeredOnButton(){
  if(digitalRead(OnButtonPin) == HIGH){
    return true; 
  }else{
    return false; 
  }
}

int mm_to_steps(double mm){
  //TODO: Convert mm to steps
}

int degrees_to_steps(double degree){
  //TODO: Convert degrees to steps 
}


void loop(){
  
    if(!EMERGENCY_INTERRUPT_STATE)
  {
    if(digitalRead(OnButtonPin) == LOW){
      ON_STATE = true; 
    }    
    if(ON_STATE){
      onProcedure();
    }
    if(SANATIZE_STATE){
      sanatizeProcedure();
    }
    if(OFF_STATE){
      offProcedure();
    }
  } else{
 //   blinkEmergencyLED(); 
  }
}


//void UV_timedRun(int milliseconds){
//  do{
//     setRelay(true);
//  }while(!waitMilliseconds(milliseconds));
//  
//    setRelay(false);
//}

//void blinkEmergencyLED(){
// digitalWrite(LED_emergencyStopIndicator, HIGH); 
// delay(100); 
// digitalWrite(LED_emergencyStopIndicator, HIGH); 
// delay(100); 
//}
