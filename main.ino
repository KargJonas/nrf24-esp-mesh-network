#include <I2Cdev.h>
#include <MPU6050.h>
#include <Servo.h>
#include <Wire.h>

// CONFIG
const int trim = 80;
const int pitch = 120;

double servoPosition = 0;
double lastServoPosition = 0;
const int MPU = 0x68;
int16_t ax, ay, az, tmp, gx, gy, gz;
Servo servo;

void setup()
{
  // Setting up Wire
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Setting up the servo
  // servo.attach(14);

  // Starting serial
  Serial.begin(9600);
}

void loop()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);

  gx = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gy = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gz = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

  ax = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  ay = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  // az = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  // Serial.print("GX:");
  // Serial.print(gx);
  // Serial.print("\t GY:");
  // Serial.print(gy);
  // Serial.print("\t GZ:");
  // Serial.print(gz);
  // Serial.print("\t TMP:");
  // Serial.print(tmp);
  // Serial.print("\t AX:");
  // Serial.print(ax);
  // Serial.print("\t AY:");
  // Serial.println(ay);
  // Serial.print("\tAZ:");
  // Serial.println(az);

  // servoPosition = trim;
  // servoPosition += ((gx / 20000.0) * pitch) / (ax / 700);

  gx -= 2630;

  Serial.println(gx);

  // Serial.println(gx / abs(ax + 0.0));

  // if (servoPosition < 0) {
  //   servoPosition = 0;
  // } else if (servoPosition > 180) {
  //   servoPosition = 180;
  // }

  // servo.write(servoPosition);

  // if (servoPosition - lastServoPosition >= 1) {
  //   servo.write(servoPosition);
  //   lastServoPosition = servoPosition;
  // }

  delay(100);
}