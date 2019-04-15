#include <Servo.h>

//SETUP PINS HERE
const int trigPin = 2; //Sensor Trigger Pin Constant
const int echoPin = 3; //Sensor Pin Consant
const int screamPin = 5; // "I see something" Pin
const int servoPin=9; // Servo Control Pin

//VARIABLE SETUP HERE
const int maxdist = 15; // Hard limit on the distance. Won't ACT (getdist() WILL return still) if the distance is over this limit.
const int resolution=4; //Resolution. Use whole, positive numbers. The higher it goes, the faster it will go, but the less data will be given. Ideal values will be divisible by 180 or whateverr

//EVERYTHING ELSE
unsigned long starttime1 = 0; 
unsigned long lag = 0;
int ang;
long duration = 0;
float distance = 0;
bool scream = true;
int dist;
unsigned long demoDuration=0; 
Servo servo1; 

void setup() {
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
  starttime1 = millis(); //Start off by notating how many miliseconds from "relative epoch" (epoch being begin of arduino's execution iirc) we are before all of this.
  
  digitalWrite(trigPin, LOW);  // 
  delayMicroseconds(5);        // All of this shit here sends out a pulse to the ultrasonic sensor. 
  digitalWrite(trigPin, HIGH); // Think of it like a "pulseOut"
  delayMicroseconds(10);       // 
  digitalWrite(trigPin, LOW);  //

  duration = pulseIn(echoPin, HIGH, 10000); //This is the juicy bit, where it uses the arudino "pulseIn" to have the sensor return data from the sensor. Can't tell you 100% how the sensor does it, but we get a duration. 
                                   //^^^ This timeout, 10 miliseconds, also determines the max distance. Keep this in mind if you decide to tamper with this in "tightening"
  distance = duration * 0.034 / 2; // Do some physics math to find the distance based on the speed of sound (we want distance after all)
  
  if ((micros() - starttime1) < 10000) { // Make sure lag doesn't end up as a value higher than the delay. Also - IF YOU ARE TIGHTENING THE DELAYS, MAKE SURE YOU CHANGE THIS!!! Not killer, but advisable still.
    lag = (micros() - starttime1); // See how many miliseconds this entire thing took. Will n
  } else {
    lag = 0; // Otherwise set to zero so it doesn't do any real change
  }
  starttime1 = 0; // Reset this, not sure why its required but ill figure this out later
  return distance; // End this function by returning the distance.
}

//Incriment the servo function. Better named "Incriment, unless we can't, then don't"
void incServo() {
  if (ang < 179) { // Servo has a max of 180 rotation. If yours doesn't or is smaller (pleb), change it here I guess
    ang += resolution; // Step by "resolution". This is done to have some flexibility in the speed, data redundancy, and resolution in this script. Variable set above. 
  } else {
    ang=0; // Reset to the starting position. You can set a range here if you'd like. Use negative numbers to scare your servo.
    servo1.write(ang); // Set it to 0 right now, as there will be a 1 second delay before the reset if we don't. Just figure it'd be nice. Not required.
    Serial.println("\n\n\n\n\n\n\n\n\n#00000"); // Notate we completed a full cycle. Same "signal" as the thing in start()
    delay(1000); // Wait a second. Lots of time to see that this ended and act accordingly. 
  }
  servo1.write(ang); // By now, we would have had a value set. This, as seen before, writes it to the servo.
  delay(50); // delay, as to not tell the servo to step faster than it can physically move
}


//Now: To execute all of this.
void loop() {
  Serial.println("=============================="); // DEBUGGING
  demoDuration=micros(); // same lag thing as before but this is actually important (i guess)
  distance = getdist(); // Runs the sensor, puts the output as the distance. Probably redundant. Don't care.
  Serial.print("Distance: "); // NOT DEBUGGING!!! This sends the "header" over first. Required to differentiate stuff 
  Serial.println(distance); // The meaty goodness: the entire fucking point of this script all lead up to basically this line.

  delayMicroseconds(10000 - lag); // Account for lag, lag being from the getdist thing. 
  Serial.print("Lag:"); // Same header thing
  Serial.println(lag); // beep boop same fuckn shit
  
  incServo(); // Move the servo forward after data has been read.
  if((micros()-demoDuration) < 100000){ // Same lag-check as before and all that.
  delayMicroseconds(100000-(micros()-demoDuration)); // This equalizes each loop time (Relatively speaking. It's off by 1-2 miliseconds as the CPU isnt exactly a 4 gigahertz intel I7 (my hypothesis atleast). A lot better than the 2-9 miliseconds. Increaces ACCURACY
  }
  Serial.print("Cycle Time:"); // Headerass
  Serial.println(micros()-demoDuration); // aaa
  demoDuration=0; // reset duration
  Serial.println("=============================="); //Debug Dickshit
}
