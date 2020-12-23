
#define blue 2
#define white 3
#define red 4
#define blue_level_signal 5
#define white_level_signal 6
#define red_level_signal 7
#define pump 8
#define room_light 9
#define room_fan 10

bool lightState = false;
bool fanState = false;

char received_text;

bool blueState = LOW;
bool whiteState = LOW;
bool pumpState = LOW;
bool PERMISSION = true;  /// Main Permission to turn Off the pump

unsigned long startTimeBlue = 0;
int intervalBlue = 800;

unsigned long startTimeWhite = 0;
int intervalWhite = 185;


void setup() {

  Serial.begin(9600);
  pinMode(room_light, OUTPUT);
  pinMode(room_fan, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(white, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(blue_level_signal, INPUT_PULLUP);
  pinMode(white_level_signal, INPUT_PULLUP);
  pinMode(red_level_signal, INPUT_PULLUP);
}

void loop() {

  if (Serial.available() > 0) {
    received_text = Serial.read();
    if (received_text == 'L') turn_on_light(); else turn_on_fan();

  }

  if (digitalRead(blue_level_signal)) digitalWrite(blue, LOW);
  if (digitalRead(white_level_signal)) digitalWrite(white, LOW);
  if (digitalRead(red_level_signal)) {
    digitalWrite(red, LOW);  //reset the permission
    PERMISSION = true;
  }


  if (!digitalRead(blue_level_signal) && (startTimeBlue + intervalBlue < millis())) turnItOn();
  if (!digitalRead(white_level_signal) && (startTimeWhite + intervalWhite < millis())) turnItOn2();


  if (PERMISSION && !digitalRead(red_level_signal)) {

    digitalWrite(red, HIGH);
    digitalWrite(white, LOW);
    digitalWrite(blue, LOW);
    delay(5000);

    if (PERMISSION && !digitalRead(red_level_signal)) {
      pumpState = !pumpState;

      digitalWrite(red, LOW);
      delay(200);
      digitalWrite(red, HIGH);
      delay(200);
      digitalWrite(red, LOW);
      delay(200);
      digitalWrite(red, HIGH);
      delay(200);
      digitalWrite(red, LOW);
      delay(200);
      digitalWrite(red, HIGH);

      digitalWrite(pump, pumpState); //turns off the pump when the tank is full

      PERMISSION = false; ///  ensures pump triggers only once !!!

      delay(10000);  //debouncing on course

    }
  }  //multiple checking of conditions to enhance debouncing

}


void turn_on_light() {
  lightState = !lightState;
  digitalWrite(room_light, lightState);
}

void turn_on_fan() {
  fanState = !fanState;
  digitalWrite(room_fan, fanState);
}


void turnItOn() {
  blueState = !blueState;
  digitalWrite(blue, blueState);
  startTimeBlue = millis();
}

void turnItOn2() {
  whiteState = !whiteState;
  digitalWrite(white, whiteState);
  startTimeWhite = millis();

}
