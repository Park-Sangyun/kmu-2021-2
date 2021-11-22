#include <Servo.h>

// Arduino pin assignment
#define PIN_IR A0
#define PIN_SERVO 10

// configurable parameters
#define SND_VEL 346.0 // sound velocity at 24 celsius degree (unit: m/s)
#define INTERVAL 100 // sampling interval (unit: ms)
#define _DIST_MIN 180 // minimum distance to be measured (unit: mm)
#define _DIST_MAX 360 // maximum distance to be measured (unit: mm)
#define _DIST_ALPHA 0.4 // EMA weight of new sample (range: 0 to 1). Setting this value to 1 effectively disables EMA filter.

#define _DUTY_MIN 553 // servo full clockwise position (0 degree)
#define _DUTY_NEU 1477 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counterclockwise position (180 degree)

// global variables
float timeout; // unit: us
float dist_min, dist_max, dist_raw, dist_prev, dist_ema, alpha; // unit: mm
unsigned long last_sampling_time; // unit: ms
float scale; // used for pulse duration to distance conversion
Servo myservo;

void setup() {
// initialize GPIO pins
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);

// initialize USS related variables
  dist_min = _DIST_MIN; 
  dist_max = _DIST_MAX;
  alpha = _DIST_ALPHA;
  timeout = (INTERVAL / 2) * 1000.0; // precalculate pulseIn() timeout value. (unit: us)
  dist_raw = dist_prev = 0.0; // raw distance output from USS (unit: mm)
  scale = 0.001 * 0.5 * SND_VEL;

// initialize serial port
  Serial.begin(57600);

// initialize last sampling time
  last_sampling_time = 0;
}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return val;
}

void loop() {
// wait until next sampling time. 
// millis() returns the number of milliseconds since the program started. Will overflow after 50 days.
  if(millis() < last_sampling_time + INTERVAL) return;

// get a distance reading from the USS
  dist_raw = ir_distance();
  float dist_cali = 100 + 300.0 / (387 - 94) * (dist_raw - 94);

// output the read value to the serial port
  Serial.print("Min:100,dist_cali:");
  Serial.print(dist_cali);
  Serial.print(",servo:");
  Serial.print(myservo.read());  
  Serial.println(",Max:300");

// 거리에 따른 서보 모터 각도 조절
if(dist_cali <= 255.0) {
     myservo.writeMicroseconds(1807);
  }
  else if(255.0 < dist_cali) {
    myservo.writeMicroseconds(1477);
  }
  
// update last sampling time
  last_sampling_time += INTERVAL;
}
