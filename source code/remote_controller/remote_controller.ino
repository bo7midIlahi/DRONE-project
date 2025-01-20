#include <nRF24L01.h>
#include <RF24.h>


//defining radio
RF24 radio(5, 8); // CE, CSN
const byte address[6] = "00001";

//defining vibration motor
const int motor_vibration_pin = 3;

//defining light pin
const int light_pin = 4; //============================================ ADDED LIGHT

//defining potentiometer
const int potentiometer_pin = A5;
const int potentiometer_rotary_pin = A1;

//defining joystick
const int x_axis_pin = A6;
const int y_axis_pin = A0;
const uint8_t joystick_btn_pin = 2;

//data to send via nRF24L01
struct joystick {
  int x;
  int y;
  int btn;
};

struct data {
  struct joystick joystick;
  int throttle;
  int rotation;
  int light;
};


void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  //setup the joystick btn && motor's && light
  pinMode(joystick_btn_pin, INPUT_PULLUP);    // Enable pull-up
  pinMode(motor_vibration_pin,OUTPUT);
  pinMode(light_pin,INPUT); //========================================================
}

void loop() {
  //readings
  int pot_value = analogRead(potentiometer_pin);
  pot_value = map(pot_value,0,1022,0,180);

  int pot_rotary_value = analogRead(potentiometer_rotary_pin);
  pot_rotary_value = map(pot_rotary_value,0,1023,-100,100);

  int x_axis_value = analogRead(x_axis_pin);
  int y_axis_value = analogRead(y_axis_pin);
  int joystick_btn = digitalRead(joystick_btn_pin);

  int light_btn = digitalRead(light_pin);

  if (joystick_btn==LOW) {
    digitalWrite(motor_vibration_pin,HIGH);
  }else {
    digitalWrite(motor_vibration_pin,LOW);
  };

  //showing collected data
  Serial.print("Potentiometer = ");
  Serial.print(pot_value);
  Serial.print("\n");
  
  Serial.print("Potentiometer rotary = ");
  Serial.print(pot_rotary_value);
  Serial.print("\n");

  Serial.print("x_axis_value = ");
  Serial.print(x_axis_value);
  Serial.print("\n");
  Serial.print("y_axis_value = ");
  Serial.print(y_axis_value);
  Serial.print("\n");
  Serial.print("joystick_btn = ");
  Serial.print(joystick_btn);
  Serial.print("\n");
  Serial.print("light = "); //  ===========================================================
  Serial.print(digitalRead(light_pin));
  Serial.print("\n\n"); // ======================================================

  //preparing data to send
  struct data data;

  data.joystick.x = x_axis_value;
  data.joystick.y = y_axis_value;
  data.joystick.btn = joystick_btn;

  data.throttle = pot_value;
  data.rotation = pot_rotary_value;
  data.light = light_btn;

  Serial.print("size of data : ");
  Serial.println(sizeof(data));
  Serial.print("\n");

  radio.write(&data, sizeof(data));
  delay(100);
}
