/*
Firmware for http://www.instructables.com/id/Arduino-Drawing-Robot/
for use with Code.org Hour of Code Tutorials:
https://code.org/api/hour/begin/frozen
https://code.org/api/hour/begin/artist

Use the [</> Show Code] button to get the JavaScript and paste it in
to the loop() function.

*/

typedef int var;     // handles JavaScript var type

#include <Servo.h>

// setup servo
int servoPin = 8;
int PEN_DOWN = 0; // angle of servo when pen is down
int PEN_UP = 60;   // angle of servo when pen is up
Servo penServo;

float wheel_dia=63; //    # mm (increase = spiral out)
float wheel_base=114; //    # mm (increase = spiral in, ccw) 
int steps_rev=512; //        # 512 for 64x gearbox, 128 for 16x gearbox
int delay_time=6; //         # time between steps in ms

// Stepper sequence org->pink->blue->yel
int L_stepper_pins[] = {12, 10, 9, 11};
int R_stepper_pins[] = {4, 6, 7, 5};

int fwd_mask[][4] =  {{1, 0, 1, 0},
                      {0, 1, 1, 0},
                      {0, 1, 0, 1},
                      {1, 0, 0, 1}};

int rev_mask[][4] =  {{1, 0, 0, 1},
                      {0, 1, 0, 1},
                      {0, 1, 1, 0},
                      {1, 0, 1, 0}};


void setup() {
  randomSeed(analogRead(1)); 
  Serial.begin(9600);
  for(int pin=0; pin<4; pin++){
    pinMode(L_stepper_pins[pin], OUTPUT);
    digitalWrite(L_stepper_pins[pin], LOW);
    pinMode(R_stepper_pins[pin], OUTPUT);
    digitalWrite(R_stepper_pins[pin], LOW);
  }
  penServo.attach(servoPin);
  Serial.println("setup");
  
  penup();
  
  delay(1000);
}

void loop(){
  // Example from Puzzle 11 of Anna and Elsa Tutorial
  // https://studio.code.org/s/frozen/stage/1/puzzle/11
  // Replace it with your code:
  
 // for (var count2 = 0; count2 < 4; count2++) {
 //   for (var count = 0; count < 2; count++) {
      
   moveForward(50);
   turnLeft(90);
   moveForward(50);
   jumpForward(10);
   turnLeft(90);
   moveForward(50);
   turnRight(90);
   jumpForward(10);
   turnRight(90);
   moveForward(50);
   turnLeft(90);
   moveForward(50);
    jumpForward(10);
   turnLeft(90);
   moveForward(50);
  

 done();
 while(1);  // this stops execution
}




// ----- HELPER FUNCTIONS -----------
int step(float distance){
  int steps = distance * steps_rev / (wheel_dia * 3.1412); //24.61
  /*
  Serial.print(distance);
  Serial.print(" ");
  Serial.print(steps_rev);
  Serial.print(" ");  
  Serial.print(wheel_dia);
  Serial.print(" ");  
  Serial.println(steps);
  delay(1000);*/
  return steps;  
}


void forward(float distance){
  int steps = step(distance);
  //Serial.println(steps);
  for(int step=0; step<steps; step++){
    for(int mask=0; mask<4; mask++){
      for(int pin=0; pin<4; pin++){
        digitalWrite(L_stepper_pins[pin], rev_mask[mask][pin]);
        digitalWrite(R_stepper_pins[pin], fwd_mask[mask][pin]);
      }
      delay(delay_time);
    } 
  }
}


void backward(float distance){
  int steps = step(distance);
  for(int step=0; step<steps; step++){
    for(int mask=0; mask<4; mask++){
      for(int pin=0; pin<4; pin++){
        digitalWrite(L_stepper_pins[pin], fwd_mask[mask][pin]);
        digitalWrite(R_stepper_pins[pin], rev_mask[mask][pin]);
      }
      delay(delay_time);
    } 
  }
}


void right(float degrees){
  float rotation = degrees / 360.0;
  float distance = wheel_base * 3.1412 * rotation;
  int steps = step(distance);
  for(int step=0; step<steps; step++){
    for(int mask=0; mask<4; mask++){
      for(int pin=0; pin<4; pin++){
        digitalWrite(R_stepper_pins[pin], rev_mask[mask][pin]);
        digitalWrite(L_stepper_pins[pin], rev_mask[mask][pin]);
      }
      delay(delay_time);
    } 
  }   
}


void left(float degrees){
  float rotation = degrees / 360.0;
  float distance = wheel_base * 3.1412 * rotation;
  int steps = step(distance);
  for(int step=0; step<steps; step++){
    for(int mask=0; mask<4; mask++){
      for(int pin=0; pin<4; pin++){
        digitalWrite(R_stepper_pins[pin], fwd_mask[mask][pin]);
        digitalWrite(L_stepper_pins[pin], fwd_mask[mask][pin]);
      }
      delay(delay_time);
    } 
  }   
}


void done(){ // unlock stepper to save battery
  for(int mask=0; mask<4; mask++){
    for(int pin=0; pin<4; pin++){
      digitalWrite(R_stepper_pins[pin], LOW);
      digitalWrite(L_stepper_pins[pin], LOW);
    }
    delay(delay_time);
  }
}


void penup(){
  delay(1000);
  Serial.println("PEN_UP()");
  penServo.write(PEN_UP);
  delay(1000);
}


void pendown(){
  delay(1000);  
  Serial.println("PEN_DOWN()");
  penServo.write(PEN_DOWN);
  delay(1000);
}


void circle(float radius){
  circle(radius, 360);
}


void circle(float radius, float extent){
  int sides = 1 + int(4 + abs(radius) / 12.0);
  circle(radius, extent, sides);
}


void circle(float radius, float extent, int sides){
  // based on Python's Turtle circle implementation
  float frac = abs(extent) / 360;
  float w = 1.0 * extent / sides;
  float w2 = 0.5 * w;
  float l = 2.0 * radius * sin(w2 * 3.1412 / 180);
  if (radius < 0){
    l = -l;
    w = -w;
    w2 = -w2;
  }
  left(w2);
  Serial.print("circle: ");
  Serial.print("frac=");
  Serial.print(frac);
  Serial.print(" sides=");
  Serial.print(sides);
  Serial.print(" l=");
  Serial.print(l);
  Serial.print(" w=");
  Serial.print(w);
  Serial.print(" w2=");
  Serial.println(w2);

  for(int i=0; i<sides; i++){
    forward(l);
    left(w);
  }
  right(w2);
}


// ----- Elsa to Turtle commands -----------

void moveForward(float distance){
  forward(distance); 
}


void moveBackward(float distance){
  backward(distance); 
}


void turnLeft(float degrees){
  left(degrees);
}


void turnRight(float degrees){
  right(degrees);
}


void jumpForward(float distance){
  penup();
  moveForward(distance);
  pendown();
}


void jumpBackward(float distance){
  penup();
  moveBackward(distance);
  pendown();
}
