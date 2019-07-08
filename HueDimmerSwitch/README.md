![3dprints](https://cdn.thingiverse.com/renders/83/5f/33/4c/48/798d56b63f8fe922c5f16d0da61b041e_preview_featured.jpg)

## How Is working

When any button is pressed there will be a voltage difference on PNP transistor between the emitter and the base witch will trigger the NPN transistor and the ESP will receive a short pulse on RST pin for deep sleep wake up.
Because the circuit is supposed to be powered with Li-Ion battery there is a battery level monitor integrated witch will stop the operation when this become too low.
OTA firmware upgrade cam be activated by pressing button 1 and 4 in the same time (led will flash 5 times).
Is mandatory to set wifi name, wifi password, bridge emulator and static ip fields in the sketch header.

## Circuit diagram

![Circuit Diagram](https://github.com/diyhue/Devices/raw/master/HueDimmerSwitch/Hue_Tap-Dimmer_switch_circuit_prototype.png)


## PCB

![PCB](https://github.com/diyhue/Devices/raw/master/HueDimmerSwitch/Hue_Tap-Dimmer_switch_circuit_prototype_pcb.png)

The PCB that fit the [custom 3D print case made by Martin Cerny](https://www.thingiverse.com/thing:3641618) cand be ordered on JLCPCB or Elecrow. Just upload the gerber file from this folder and choose 1mm tick size. Will look better if the PCB color match the case.
