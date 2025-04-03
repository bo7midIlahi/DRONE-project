#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>

Servo ESC_1;
Servo ESC_2;
Servo ESC_3;
Servo ESC_4;

//defining led light pins
const int led_1_pin = A2;
const int led_2_pin = A3;
const int led_3_pin = 10;
const int led_4_pin = 2;

//radio
RF24 radio(4, 8); // CE, CSN
const byte address[6] = "00001";

//data to receive from nRF24L01
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

  //radio setup
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //led setup
  pinMode(led_1_pin,OUTPUT);
  pinMode(led_2_pin,OUTPUT);
  pinMode(led_3_pin,OUTPUT);
  pinMode(led_4_pin,OUTPUT);

  //bldc setup
  ESC_1.attach(3,1000,2000);
  ESC_2.attach(5,1000,2000);
  ESC_3.attach(6,1000,2000);
  ESC_4.attach(9,1000,2000);

  //blink
  for(int i=0; i<3; i++){
    digitalWrite(led_1_pin,HIGH);
    digitalWrite(led_2_pin,HIGH);
    digitalWrite(led_3_pin,HIGH);
    digitalWrite(led_4_pin,HIGH);

    delay(500);

    digitalWrite(led_1_pin,LOW);
    digitalWrite(led_2_pin,LOW);
    digitalWrite(led_3_pin,LOW);
    digitalWrite(led_4_pin,LOW);

    delay(500);
  };

  //calibration
  ESC_1.writeMicroseconds(2000);
  ESC_2.writeMicroseconds(2000);
  ESC_3.writeMicroseconds(2000);
  ESC_4.writeMicroseconds(2000);

  delay(2000);

  ESC_1.writeMicroseconds(1000);
  ESC_2.writeMicroseconds(1000);
  ESC_3.writeMicroseconds(1000);
  ESC_4.writeMicroseconds(1000);

  delay(3000);

  //blink
  for(int i=0; i<3; i++){
    digitalWrite(led_1_pin,HIGH);
    digitalWrite(led_2_pin,HIGH);
    digitalWrite(led_3_pin,HIGH);
    digitalWrite(led_4_pin,HIGH);

    delay(200);

    digitalWrite(led_1_pin,LOW);
    digitalWrite(led_2_pin,LOW);
    digitalWrite(led_3_pin,LOW);
    digitalWrite(led_4_pin,LOW);

    delay(200);
  };
}

void loop() {
  if (radio.available()) {
    struct data data;

    radio.read(&data, sizeof(data));
    int x = data.joystick.x;
    int y = data.joystick.y;
    int btn = data.joystick.btn;
    int throttle = map(data.throttle,0,180,1000,2000);
    int rotation = data.rotation;
    int light = data.light;

    //showing collected data
    Serial.print("throttle = ");
    Serial.print(throttle);
    Serial.print("\n");

    Serial.print("rotation = ");
    Serial.print(rotation);
    Serial.print("\n");

    Serial.print("x_axis_value = ");
    Serial.print(x);
    Serial.print("\n");
    Serial.print("y_axis_value = ");
    Serial.print(y);
    Serial.print("\n");
    Serial.print("joystick_btn = ");
    Serial.print(btn);
    Serial.print("\n\n\n");
    Serial.print("light = ");
    Serial.print(light);
    Serial.print("\n\n\n");

    
    ESC_1.writeMicroseconds(throttle);
    ESC_2.writeMicroseconds(throttle);
    ESC_3.writeMicroseconds(throttle);
    ESC_4.writeMicroseconds(throttle);
    
    //PITCH
    if(y < 10){
      ESC_1.writeMicroseconds(throttle - 30);
      ESC_2.writeMicroseconds(throttle - 30);
    }

    if(y > 1000){
      ESC_3.writeMicroseconds(throttle - 30);
      ESC_4.writeMicroseconds(throttle - 30);
    }

    //ROLL
    if(x < 10){
      ESC_1.writeMicroseconds(throttle - 30);
      ESC_4.writeMicroseconds(throttle - 30);
    }

    if(x > 1000){
      ESC_2.writeMicroseconds(throttle - 30);
      ESC_3.writeMicroseconds(throttle - 30);
    }

    //YAW
    if(rotation > 30) {
      ESC_1.writeMicroseconds(throttle - 30);
      ESC_3.writeMicroseconds(throttle - 30);
    }//left

    if(rotation < -20) {
      ESC_2.writeMicroseconds(throttle - 30);
      ESC_4.writeMicroseconds(throttle - 30);
    }//right

    //disengage
    if (btn == 0) {
      ESC_1.writeMicroseconds(1000);
      ESC_2.writeMicroseconds(1000);
      ESC_3.writeMicroseconds(1000);
      ESC_4.writeMicroseconds(1000);

      for(;;){
        digitalWrite(led_1_pin,HIGH);
        digitalWrite(led_2_pin,HIGH);
        digitalWrite(led_3_pin,HIGH);
        digitalWrite(led_4_pin,HIGH);

        delay(1000);

        digitalWrite(led_1_pin,LOW);
        digitalWrite(led_2_pin,LOW);
        digitalWrite(led_3_pin,LOW);
        digitalWrite(led_4_pin,LOW);

        delay(1000);
      };
    }

    //turinig led ON or OFF
    if(light == 1){
      digitalWrite(led_1_pin,HIGH);
      digitalWrite(led_2_pin,HIGH);
      digitalWrite(led_3_pin,HIGH);
      digitalWrite(led_4_pin,HIGH);
    }else {
      digitalWrite(led_1_pin,LOW);
      digitalWrite(led_2_pin,LOW);
      digitalWrite(led_3_pin,LOW);
      digitalWrite(led_4_pin,LOW);
    }

  }
  delay(100);
}
