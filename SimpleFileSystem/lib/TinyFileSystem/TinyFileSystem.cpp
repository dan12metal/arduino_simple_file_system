/*
   Copyright 2013 Michael Crosson
 
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

#include <Arduino.h>
#include <avr/eeprom.h>
#include "TinyTools.h"

#include "TinyFileSystem.h"

long TinyFileSystem::GetEEPROMSize() {
	return EEPROMSize;
}

// This number indicates the address of the Start of the data storage
long TinyFileSystem::GetStartOfDataAddress() {
	int maximumAddressLength = TinyTools::GetNumberDigits(TinyFileSystem::GetEEPROMSize());
	return MaxFiles * (MaxFilenameSize + 1 + (2 * maximumAddressLength));
}

// Erases EEPROM
void TinyFileSystem::EraseEEPROM() {
	for (int i = 0; i < EEPROMSize; i++) {
		WriteToEEPROM(i, 0);
	}
}

// Return's true if file exist
bool TinyFileSystem::FileExists(String filename) {
	int i;
	int maximumAddressLength = TinyTools::GetNumberDigits(TinyFileSystem::GetEEPROMSize());

	// Check if file exists
	for (i = 0; i < MaxFiles * (MaxFilenameSize + 1 + (2 * maximumAddressLength)); i += MaxFilenameSize + 1 + (2 * maximumAddressLength)) {
		String portionToCheck = ReadStringBlockFromEEPROM(i, i + MaxFilenameSize + 1 + (2 * maximumAddressLength));
	
		if (portionToCheck.startsWith(filename)) { // File found
			return true;
		}
	}
	return false;
}

// Return's the start address of the file
long* TinyFileSystem::GetFileAddress(String filename) {
	int i;
	int maximumAddressLength = TinyTools::GetNumberDigits(TinyFileSystem::GetEEPROMSize());
	long* pointer;
	long result[2];
	pointer = result;

	result[0] = 0;
	result[1] = 0;

	// Find file
	for (i = 0; i < MaxFiles * (MaxFilenameSize + 1 + (2 * maximumAddressLength)); i += MaxFilenameSize + 1 + (2 * maximumAddressLength)) {
		String portionToCheck = TinyFileSystem::ReadStringBlockFromEEPROM(i, i + MaxFilenameSize + 1 + (2 * maximumAddressLength));
	
		if (portionToCheck.startsWith(filename)) // File found
		{
			String startIndexStr = portionToCheck.substring(MaxFilenameSize, MaxFilenameSize + maximumAddressLength);
			String endIndexStr = portionToCheck.substring(MaxFilenameSize + maximumAddressLength, MaxFilenameSize + 2 * maximumAddressLength);
			result[0] = startIndexStr.toInt();
			result[1] = endIndexStr.toInt();
		}
	}

	return pointer;
}


uint8_t TinyFileSystem::ReadFromEEPROM(int address) {
	return eeprom_read_byte((unsigned char *) address);
}

String TinyFileSystem::ReadStringBlockFromEEPROM(int addressStart, int addressEnd) {
	String result ="";
	for (int i=addressStart; i <=addressEnd; i++) {
		result = result + String(char(ReadFromEEPROM(i)));
	}
	return result;
}

void TinyFileSystem::WriteToEEPROM(int address, uint8_t value) {
	eeprom_write_byte((unsigned char *) address, value);
}
