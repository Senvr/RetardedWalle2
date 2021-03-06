#include <Servo.h>

//SETUP PINS HERE
const int trigPin = 2; //Sensor Trigger Pin Constant
const int echoPin = 3; //Sensor Pin Consant
const int screamPin = 5; // "I see something" Pin
const int servoPin=9; // Servo Control Pin

//VARIABLE SETUP HERE
const int tightening=100000; // Change this if you want to "tighten" the delays 
const int resolution=4; //Resolution. Use whole, positive numbers. The higher it goes, the faster it will go, but the less data will be given. Ideal values will be divisible by 180 or whateverr

//EVERYTHING ELSE
unsigned long starttime1 = 0; 
unsigned long lag = 0;
float ang;
long duration = 0;
float distance = 0;
bool scream = true;
int dist;
Servo servo1; 

void setup() {
  Serial.begin(9600); // start serial communication
  Serial.println("#00000"); // Signal to anything listening that we just started/restart
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(servoPin, OUTPUT); // Sets sets the servo pin as an Output
  pinMode(screamPin, OUTPUT); // Sets the Scream Pin as an Output
  servo1.attach(servoPin); // Tell the servo library to attach to the aformentioned servo defined previously
  servo1.write(1); // Set it to a default angle of 1
  
}

//Defined function to read sensor data. I did this so I could make things look and run a little better, and account for "lag" better.
float getdist() { // Float 'cause of fancy schmancy decimals 'n shit
  
  
  digitalWrite(trigPin, LOW);  // 
  delayMicroseconds(5);        // All of this shit here sends out a pulse to the ultrasonic sensor. 
  digitalWrite(trigPin, HIGH); // Think of it like a "pulseOut"
  delayMicroseconds(10);       // 
  digitalWrite(trigPin, LOW);  //

  starttime1 = micros(); //See how long it took for pulsein to get data
  
  duration = pulseIn(echoPin, HIGH, tightening); //This is the juicy bit, where it uses the arudino "pulseIn" to have the sensor return data from the sensor. Can't tell you 100% how the sensor does it, but we get a duration. 
  delayMicroseconds(tightening-(micros()-starttime1));

  
  distance = duration * 0.034 / 2; // Do some physics math to find the distance based on the speed of sound (we want distance after all)
  return distance; // End this function by returning the distance.
}

//Incriment the servo function. Better named "Incriment, unless we can't, then don't"
void incServo() {
  if (ang < 179) { // Servo has a max of 180 rotation. If yours doesn't or is smaller (pleb), change it here I guess
    ang += resolution; // Step by "resolution". This is done to have some flexibility in the speed, data redundancy, and resolution in this script. Variable set above. 
  } else {
    ang=0; // Reset to the starting position. You can set a range here if you'd like. Use negative numbers to scare your servo.
    servo1.write(ang); // Set it to 0 right now, as there will be a 1 second delay before the reset if we don't. Just figure it'd be nice. Not required.
    Serial.println("#00000"); // Notate we completed a full cycle. Same "signal" as the thing in start()
    delay(1000); // Wait a second. Lots of time to see that this ended and act accordingly. 
  }
  servo1.write(ang); // By now, we would have had a value set. This, as seen before, writes it to the servo.
  delay(10*resolution); // delay, as to not tell the servo to step faster than it can physically move
  
}


//Now: To execute all of this.
void loop() {
  distance = getdist(); // Runs the sensor, puts the output as the distance. Probably redundant. Don't care.
  Serial.print(ang);       // X         |  This is done to give a "coordinate" based on the step and the distance from the sensor.
  Serial.print(",");       // ,         |  Organized for easy programming. Subject to change. 
  Serial.println(distance);// Y         |  Eat my poopsack
    
  incServo(); // Move the servo forward after data has been read.

  
}
