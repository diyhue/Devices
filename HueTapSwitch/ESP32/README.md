## Based on
This is based on the ESP8266 version that can be found here https://github.com/diyhue/Devices/tree/master/HueTapSwitch
This is basicly the ESP32 variant.

I've used the 'DIOT ESP32 DEV V1' board. Quick note; you need to hold down the button of this board to upload via USB, or just add a capasitor (just google it)

## NOTES:
    -no shutdown_voltage control. (no official support for ESP32 available)
    -Button pins are hardcoded into WakeUp-Mask (these double as wake-up pins (like RESET wakes the ESP8266))
      Thought these pins are hardcoded into the WakeUp bitmask (uint64_t Mask = 0xF00000000) you could change these
      00001 = wake up with pin 1. 0101 = Wake up with pin 3 or 1.
      I tried to auto generate this, but it didn't seem stable/fast enough
      00000000000000000000000000000111100000000000000000000000000000000 = 0xF00000000 (pin 32,33,34,35)
    -Make sure to pull Button pins down, if there left floating (not connected) they might false trigger.
    -Existing firmware can be updated Over the Air (OTA). press button 1+4,
      The <LED_BUILTIN> will flash <EveryXms> also note it goes to auto sleep after <AutoSleepAfterXms> after boot (in case 1+4 are accidently pressed)


## How it works

When any button is pressed the pin will be pulled high, The ESP is set to wake up (with the MASK) if any of those pins are high

It's mandatory to set wifi name, wifi password, bridge emulator and static ip fields in the sketch header.

## Circuit diagram

![Circuit Diagram](https://raw.githubusercontent.com/diyhue/Devices/master/HueTapSwitch/ESP32/Schematic.png)