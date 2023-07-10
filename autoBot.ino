#include <CapacitiveSensor.h>
#include <Servo.h>

CapacitiveSensor Sensor1 = CapacitiveSensor(2, 3);
CapacitiveSensor Sensor2 = CapacitiveSensor(4, 5);
CapacitiveSensor Sensor3 = CapacitiveSensor(6, 7);
CapacitiveSensor Sensor4 = CapacitiveSensor(8, 9);

CapacitiveSensor sensors[] = {Sensor1, Sensor2, Sensor3, Sensor4};

int baseServoPin = 10;
int topServoPin = 11;
Servo baseServo;
Servo topServo;

const int ledPins[] = {A0, A1, A2, A3};
int i = 0;
int j = 0;
int val = 0;
int sum = 0;
int angle = 0;
int note = 0;
int led = 0;
int mode = 0;
int k = 0;
int rocky = 0;

int pressed[] = {0, 0, 0, 0};

const int rocky_top[] = {4, 4, 4, 4, 5, 5, 4, 2, 0, 0, 0, 0, 1, 0, 1, 2,
                         4, 4, 4, 4, 5, 5, 4, 2, 0, 0, 0, 2, 2, 2, 1, 0};
const int rocky_pause[] = {2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 3,
                           2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 3};

const int jingle_bells[] = {2, 2, 2, 2, 2, 2, 2, 4, 0, 1, 2, 3, 3, 3, 3, 3,
                            2, 2, 2, 2, 1, 1, 2, 1, 4, 2, 2, 2, 2, 2, 2, 2,
                            4, 0, 1, 2, 3, 3, 3, 3, 3, 2, 2, 2, 4, 4, 3, 1, 0};
const int jingle_pause[] = {1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 2, 1, 1, 2, 1,
                            1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4};

void setup() {
  Serial.begin(9600);

  baseServo.attach(baseServoPin);
  topServo.attach(topServoPin);

  baseServo.write(110);
  topServo.write(0);

  for (i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {

   if (mode == 1) {
    led = rocky_top[rocky];
    led_on();
  } else if (mode == 2) {
    auto_play_rocky_top();
  } else if (mode == 3) {
    auto_play_jingle();
  }

  check_buttons();
  if (sum == 4) {
    switch_mode();
  }

  if (sum > 0) {
    get_note();
    get_angle();

    if (mode == 0) {
      led = note;
      led_on();
      baseServo.write(angle);
      delay(10);
      move_top();
      all_leds_off();
    } else if (mode == 1) {
      if (note == rocky_top[rocky]) {
        all_leds_off();
        baseServo.write(angle);
        delay(10);
        move_top();
        rocky++;
        if (rocky == 32) {
          mode = 0;
          rocky = 0;
        }
      }
    }
  }

  delay(50);
}

void switch_mode() {
  mode = (mode + 1) % 4;
  all_leds_off();
  delay(200);
  all_leds_on();
  delay(500);
  all_leds_off();
}

void auto_play_rocky_top() {

  for (k = 0; k < 32; k++) {
    check_buttons();
    if (sum == 4) {
      switch_mode();
      break;
    }

    note = rocky_top[k];
    led = note;
    led_on();
    get_angle();
    baseServo.write(angle);
    delay(10);
    move_top();
    delay(175 * rocky_pause[k]);
    all_leds_off();
  }
  delay(500);
}

void auto_play_jingle() {
  for (k = 0; k < 49; k++) {
    check_buttons(); 
    if (sum == 4) {
      switch_mode();
      break;
    }

    note = jingle_bells[k];
    led = note;
    led_on();
    get_angle();
    baseServo.write(angle);
    delay(10);
    move_top();
    delay(250 * jingle_pause[k]);
    all_leds_off();
  }
  delay(500);
}

void led_on() {

  if (led == 0) {
    digitalWrite(ledPins[0], HIGH);
  } else if (led == 1) {
    digitalWrite(ledPins[1], HIGH);
  } else if (led == 2) {
    digitalWrite(ledPins[2], HIGH);
  } else if (led == 3) {
    digitalWrite(ledPins[3], HIGH);
  } else if (led == 4) {
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], HIGH);
  } else if (led == 5) {
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[2], HIGH);
  } else if (led == 6) {
    digitalWrite(ledPins[2], HIGH);
    digitalWrite(ledPins[3], HIGH);
  } else if (led == 7) {
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[2], HIGH);
    digitalWrite(ledPins[3], HIGH);
  }
}

void get_note() {

  if (sum == 1) {
    for (i = 0; i < 4; i++) {
      if (pressed[i]) {
        note = i; 
      } 
    }
  } else {
    if (pressed[0] && pressed[1] && !pressed[2] && !pressed[3]) {
      note = 4;
    } else if (!pressed[0] && pressed[1] && pressed[2] && !pressed[3]) {
      note = 5;
    } else if (!pressed[0] && !pressed[1] && pressed[2] && pressed[3]) {
      note = 6;
    } else {
      note = 7;
    }
  }
}

void get_angle() {
  if (note == 0) {
    angle = 110;
  } else if (note == 1) {
    angle = 101;
  } else if (note == 2) {
    angle = 92;
  } else if (note == 3) {
    angle = 86;
  } else if (note == 4) {
    angle = 76;
  } else if (note == 5) {
    angle = 68;
  } else if (note == 6) {
    angle = 60;
  } else if (note == 7) {
    angle = 50;
  }
}

void move_top() {
  topServo.write(90);
  delay(175);
  topServo.write(0);
}

void check_buttons() {
  val = 0;
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 4; i++) {
      val = sensors[i].capacitiveSensor(30);
      
      if (val > 20) {
        pressed[i] = 1;
      } else {
        pressed[i] = 0;
      }
      val = 0;
    } 
    delay(10);
  }

  sum = 0;
  for (i = 0; i < 4; i++) {
    sum += pressed[i]; 
  }
}

void all_leds_on() {
  for (j = 0; j < 4; j++) {
    digitalWrite(ledPins[j], HIGH);
  }
}

void all_leds_off() {
  for (j = 0; j < 4; j++) {
    digitalWrite(ledPins[j], LOW);
  }
}



