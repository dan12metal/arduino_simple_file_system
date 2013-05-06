/*
   Copyright 2013 Nusku Networks

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <SPI.h>
#include <Wire.h>

#include "DRIVERSPIMC23X256.h"
#include "DRIVERI2CMC24LCXXX.h"

#define INCLUDE_TEST_CHIP true

void setup() {
  // Start serial for debug output
  Serial.begin(9600);
  while (!Serial); // Wait for serial to open before doing anything further

  // Start SPI for drivers that require it
  Serial.println("Starting SPI bus");
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  //SPI.setClockDivider(SPI_CLOCK_DIV2);

  // Start I2C for drivers that require it
  Serial.println("Starting I2C bus");
  Wire.begin();

  // SRAM SPI device test
  Serial.println("Testing DriverSPIMC23x256");
  DriverSPIMC23x256* sram = new DriverSPIMC23x256(8);
  sram->testChip();

  // I2C EEPROM device test
  Serial.println("Testing DriverI2CMC24LCxxx");
  DriverI2CMC24LCxxx* eeprom = new DriverI2CMC24LCxxx(0, I2CMC24LCXXX_PAGE256);
  eeprom->testChip();
}

void loop() {

}
