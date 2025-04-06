#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

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

Adafruit_MPU6050 mpu;

void blink(int repeat, int delay_ms) {
    if (repeat == 0)
    {
        while (1)
        {
            digitalWrite(led_1_pin,HIGH);
            digitalWrite(led_2_pin,HIGH);
            digitalWrite(led_3_pin,HIGH);
            digitalWrite(led_4_pin,HIGH);
    
            delay(delay_ms);
    
            digitalWrite(led_1_pin,LOW);
            digitalWrite(led_2_pin,LOW);
            digitalWrite(led_3_pin,LOW);
            digitalWrite(led_4_pin,LOW);
    
            delay(delay_ms);
        }   
    }

    for(int i=0; i<repeat; i++){
        digitalWrite(led_1_pin,HIGH);
        digitalWrite(led_2_pin,HIGH);
        digitalWrite(led_3_pin,HIGH);
        digitalWrite(led_4_pin,HIGH);
    
        delay(delay_ms);
    
        digitalWrite(led_1_pin,LOW);
        digitalWrite(led_2_pin,LOW);
        digitalWrite(led_3_pin,LOW);
        digitalWrite(led_4_pin,LOW);
    
        delay(delay_ms);
    };
}

void setup() {
  Serial.begin(115200);

  //********************radio setup********************//
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //********************LEDs setup********************//
  pinMode(led_1_pin,OUTPUT);
  pinMode(led_2_pin,OUTPUT);
  pinMode(led_3_pin,OUTPUT);
  pinMode(led_4_pin,OUTPUT);

  //********************bldc setup********************//
  ESC_1.attach(3,1000,2000);
  ESC_2.attach(5,1000,2000);
  ESC_3.attach(6,1000,2000);
  ESC_4.attach(9,1000,2000);

  //blink
  blink(3, 500);

  //********************calibration********************//
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
  blink(3, 500);

  //********************mpu 6050 setup********************//
  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    blink(5, 200);
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
 
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println("");

  //********************initial angles********************//
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  const int x_0 = a.acceleration.x;
  const int y_0 = a.acceleration.y;
}

void print_received_data(int x, int y, int btn, int throttle, int rotation, int light){
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
}

void disengage(Servo ESC_1, Servo ESC_2, Servo ESC_3, Servo ESC_4) {
    ESC_1.writeMicroseconds(1000);
    ESC_2.writeMicroseconds(1000);
    ESC_3.writeMicroseconds(1000);
    ESC_4.writeMicroseconds(1000);

    blink(0,50);
}

float PID(float error, float P, float I, float D, float prev_error, float prev_iterm) {
  return P*error + prev_error + I*((error+prev_error)/2)*0.25 + D*((error-prev_error)/0.25);
}

void loop() {
/*  //reading mpu data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
    
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
*/
  if (radio.available()) {
    struct data data;

    radio.read(&data, sizeof(data));
    int x = data.joystick.x;
    int y = data.joystick.y;
    int btn = data.joystick.btn;
    int throttle = map(data.throttle,0,180,900,2000);
    int rotation = data.rotation;
    int light = data.light;

    //********************showing received data********************//
    print_received_data(x, y, btn, throttle, rotation, light);

    //********************reading mpu data********************//
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");

    //********************PID*********************//
    float P = 12.3;
    float I = 8.549;
    float D = 0.7;

    //********************setting motor speed********************// in other words : hover
    if(x<713 && x> 309 && y<712 && y> 307){
      float setpoint = 0.0;
      float error_pitch = setpoint - a.acceleration.y;
      float error_roll = setpoint - a.acceleration.x;
      
      float prev_error_pitch = 0;
      float prev_Iterm_pitch = 0;
      float prev_error_roll = 0;
      float prev_Iterm_roll = 0;
      
      float input_pitch = PID(error_pitch, P, I, D, prev_error_pitch,prev_Iterm_pitch);
      float input_roll = PID(error_pitch, P, I, D, prev_error_pitch,prev_Iterm_pitch);
      
      prev_error_pitch = error_pitch;
      prev_Iterm_pitch = I;
      prev_error_roll = error_roll;
      prev_Iterm_roll = I;

      ESC_1.writeMicroseconds(throttle - input_pitch - input_roll);
      ESC_2.writeMicroseconds(throttle + input_pitch - input_roll);
      ESC_3.writeMicroseconds(throttle + input_pitch + input_roll);
      ESC_4.writeMicroseconds(throttle - input_pitch + input_roll);
    }

    //PITCH
    if(y < 10){
      float setpoint = -2.7;
      float error = setpoint - a.acceleration.x;
      float prev_error = 0;
      float prev_Iterm = 0;

      float input = PID(error, P, I, D, prev_error, prev_Iterm);

      prev_error = error;
      prev_Iterm = I;

      ESC_1.writeMicroseconds(throttle - input);
      ESC_2.writeMicroseconds(throttle - input);
    }

    if(y > 1000){
      float setpoint = 2.1;
      float error = setpoint - a.acceleration.x;
      float prev_error = 0;
      float prev_Iterm = 0;

      float input = PID(error, P, I, D, prev_error, prev_Iterm);

      prev_error = error;
      prev_Iterm = I;

      ESC_3.writeMicroseconds(throttle - input);
      ESC_4.writeMicroseconds(throttle - input);
    }

    //ROLL
    if(x < 10){
      float setpoint = -2.1;
      float error = setpoint - a.acceleration.y;
      float prev_error = 0;
      float prev_Iterm = 0;

      float input = PID(error, P, I, D, prev_error, prev_Iterm);

      prev_error = error;
      prev_Iterm = I;

      ESC_1.writeMicroseconds(throttle - input);
      ESC_4.writeMicroseconds(throttle - input);
    }

    if(x > 1000){
      float setpoint = 2.8;
      float error = setpoint - a.acceleration.y;
      float prev_error = 0;
      float prev_Iterm = 0;

      float input = PID(error, P, I, D, prev_error, prev_Iterm);

      prev_error = error;
      prev_Iterm = I;

      ESC_1.writeMicroseconds(throttle - input);
      ESC_4.writeMicroseconds(throttle - input);
    }

    //********************YAW********************//
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
        disengage(ESC_1,ESC_2,ESC_3,ESC_4);
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
