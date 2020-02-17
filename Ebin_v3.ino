#include <Servo.h>

#define trig 3//9
#define echo 4//10
#define ledPin 13
#define motor_pin 9

#define close_lid_angle 160
#define open_lid_angle 40

//servo on pin 9
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

bool current_state = false;
bool previous_state = false;

float distance, duration;
void go_to_close_lid_position();

void setup() {

  Serial.begin(9600);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(11, INPUT);

  myservo.attach(motor_pin);  // attaches the servo on pin 9 to the servo object
  // servo attach has been moved to toggle_lid function for motor buzzing solution
  go_to_close_lid_position();
}

void loop() {
  // put your main code here, to run repeatedly:

  ultra_sonic_trigger_pulse();
  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) * 0.0343;
  Serial.println(distance);
  //   use to test ultrasonic sensor output
  //   ultra_sonic_readin();

  update_lid_state();

  lid_action();

  delay(200);
}

void open_lid() {
  for (int i = close_lid_angle; i >= open_lid_angle; i--) {
    myservo.write(i);
    delay(7);
  }
  delay(1000);
}

void close_lid() {
  for (int i = open_lid_angle; i <= close_lid_angle; i++) {
    myservo.write(i);
    delay(7);
  }
  delay(1000);

}

void go_to_close_lid_position() {
  //  myservo.write(80);
  //  delay(700);
  myservo.write(0);
  myservo.write(180);
  myservo.write(160);
}

void ultra_sonic_trigger_pulse() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
}

void lid_action() {
  if (current_state && !previous_state) {
    //    Serial.println("OPEN");
    open_lid();
    delay(50);
  }
  else if (!current_state && previous_state) {
    //    Serial.println("COLSE");
    close_lid();
    delay(50);
  }
  else if (current_state && previous_state) {
    //    Serial.println("stay OPENED");
    delay(50);
  }
  else {
    //    Serial.println("stay CLOSED");
    delay(50);
  }
}


void update_lid_state() {
  previous_state = current_state;
  if (distance < 30) {
    current_state = true;
  }
  else if (digitalRead(11) == HIGH) {
    current_state = true;
  }
  else {
    current_state = false;
  }
}


void ultra_sonic_readin() {
  if (distance >= 400 || distance <= 2) {
    Serial.println("Out of range");
    delay(300);
  }
  else {
    Serial.println(distance);
    delay(300);
  }
}
