/* Arduino SPIMemory Library v.3.4.0
 * Copyright (C) 2019 by Prajwal Bhattaram
 * Created by Prajwal Bhattaram - 14/11/2016
 * Modified by Prajwal Bhattaram - 03/06/2018
 *
 * This file is part of the Arduino SPIMemory Library. This library is for
 * Flash and FRAM memory modules. In its current form it enables reading,
 * writing and erasing data from and to various locations;
 * suspending and resuming programming/erase and powering down for low power operation.
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License v3.0
 * along with the Arduino SPIMemory Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "diagnostics.h"

//Subfunctions for troubleshooting function
void Diagnostics::_printErrorCode(void) {
  SerialUSB.print(F("Error code: 0x"));
  if (errorcode < 0x10) {
    SerialUSB.print(F("0"));
  }
  SerialUSB.println(errorcode, HEX);
}

void Diagnostics::_printSupportLink(void) {
  SerialUSB.print(F("If this does not help resolve/clarify this issue, "));
  SerialUSB.println(F("please raise an issue at http://www.github.com/Marzogh/SPIMemory/issues with the details of what your were doing when this error occurred"));
}
//Troubleshooting function. Called when #ifdef RUNDIAGNOSTIC is uncommented at the top of SPIMemory.h.
void Diagnostics::troubleshoot(uint8_t _code, bool printoverride) {
  bool _printoverride;
  errorcode = _code;
#if defined (RUNDIAGNOSTIC) && !defined (ARDUINO_ARCH_AVR)
  _printoverride = true;
#elif defined (RUNDIAGNOSTIC) && defined (ARDUINO_ARCH_AVR)
  _printErrorCode();
#endif
#if !defined (RUNDIAGNOSTIC)
  _printoverride = printoverride;
#endif
  if (_printoverride) {
  #if defined (ARDUINO_ARCH_AVR)
    _printErrorCode();
  #else
    switch (_code) {
      case SUCCESS:
      SerialUSB.println(F("Function executed successfully"));
      break;

      case NORESPONSE:
      SerialUSB.println(F("Check your wiring. Flash chip is non-responsive."));
      break;

      case CALLBEGIN:
      SerialUSB.println(F("*constructor_of_choice*.begin() was not called in void setup()"));
      break;

      case UNKNOWNCHIP:
      SerialUSB.println(F("Unable to identify chip. Are you sure this chip is supported?"));
      //SerialUSB.println(F("Chip details:"));
      //TODO: Insert a diagnostics subroutine here.
      break;

      case UNKNOWNCAP:
      SerialUSB.println(F("Unable to identify capacity. Is this chip officially supported? If not, please define a `CAPACITY` constant and include it in flash.begin(CAPACITY)."));
      break;

      case CHIPBUSY:
      SerialUSB.println(F("Chip is busy."));
      SerialUSB.println(F("Make sure all pins have been connected properly"));
      break;

      case OUTOFBOUNDS:
      SerialUSB.println(F("Page overflow has been disabled and the address called exceeds the memory"));
      break;

      case CANTENWRITE:
      SerialUSB.println(F("Unable to Enable Writing to chip."));
      SerialUSB.println(F("Please make sure the HOLD & WRITEPROTECT pins are pulled up to VCC"));
      break;

      case PREVWRITTEN:
      SerialUSB.println(F("This sector already contains data."));
      SerialUSB.println(F("Please make sure the sectors being written to are erased."));
      break;

      case LOWRAM:
      SerialUSB.println(F("You are running low on SRAM. Please optimise your program for better RAM usage"));
      /*#if defined (ARDUINO_ARCH_SAM)
        SerialUSB.print(F("Current Free SRAM: "));
        SerialUSB.println(freeRAM());
      #endif*/
      break;

      case UNSUPPORTEDFUNC:
      SerialUSB.println(F("This function is not supported by the flash memory hardware."));
      break;

      case SYSSUSPEND:
      SerialUSB.println(F("Unable to suspend/resume operation."));
      break;

      case ERRORCHKFAIL:
      SerialUSB.println(F("Write Function has failed errorcheck."));
      break;

      case UNABLETO4BYTE:
      SerialUSB.println(F("Unable to enable 4-byte addressing."));
      break;

      case UNABLETO3BYTE:
      SerialUSB.println(F("Unable to disable 4-byte addressing."));
      break;

      case CHIPISPOWEREDDOWN:
      SerialUSB.println(F("The chip is currently powered down."));
      break;

      case NOSFDP:
      SerialUSB.println(F("The Flash chip does not support SFDP."));
      break;

      case NOSFDPERASEPARAM:
      SerialUSB.println(F("Unable to read Erase Parameters from chip. Reverting to library defaults."));
      break;

      case NOSFDPERASETIME:
      SerialUSB.println(F("Unable to read erase times from flash memory. Reverting to library defaults."));
      break;

      case NOSFDPPROGRAMTIMEPARAM:
      SerialUSB.println(F("Unable to read program times from flash memory. Reverting to library defaults."));
      break;

      case NOCHIPSELECTDECLARED:
      SerialUSB.println(F("No Chip Select pin defined in the custom SPI Array."));
      break;

      default:
      SerialUSB.println(F("Unknown error"));
      break;
    }
    if (_code == ERRORCHKFAIL || _code == CANTENWRITE || _code == UNKNOWNCHIP || _code == NORESPONSE) {
      _printSupportLink();
    }
  #endif
  }
}

Diagnostics diagnostics; // default instantiation of Diagnostics object
