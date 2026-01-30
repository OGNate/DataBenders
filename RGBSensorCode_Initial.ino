#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("FOUND TCS34725 Sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  if (c > 50) {
    float sum = c;
    float red = r / sum;
    float green = g / sum;
    float blue = b / sum;

    Serial.print("R: "); Serial.print(r); Serial.print(" ");
    Serial.print("G: "); Serial.print(g); Serial.print(" ");
    Serial.print("B: "); Serial.print(b); Serial.print(" ");
    Serial.print("C: "); Serial.print(c); Serial.println(" ");

    //Calulate the color temperature nad lux
    uint16_t colorTemp = tcs.calculateColorTemperature(r, g, b);
    uint16_t lux = tcs.calculateLux(r, g, b);

    Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.println(lux, DEC);

    // Determine the color
    if (red > green && red > blue) {
      Serial.println("Detected Color: RED");
    } else if (green > red && green > blue) {
      Serial.println("Detected Color: GREEN");
    } else if (blue > red && blue > green) {
      Serial.println("Detected Color: BLUE");
    } else {
      Serial.println("Detected Color: UNKNOWN");
    }
  } else {
    Serial.println("No Significant color detected");
  }
}
