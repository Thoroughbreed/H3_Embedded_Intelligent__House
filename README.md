# Intelligent house
#### H3 Embedded II final project
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#requirements">Requirements</a>
      <ul>
        <li><a href="#components">Components</a></li>
	    <li><a href="#libraries">Libraries</a></li>
      </ul>
    </li>
    <li>
      <a href="#case">Case</a>
      <ul>
        <li><a href="#summary&sol;rundown">Summary&sol;rundown</a></li>
        <li><a href="#getting-started">Getting started</a></li>
      </ul>
    </li>
    <li><a href="#flowcharts">Flowcharts</a></li>
       <ul>
        <li><a href="#hipo-diagram">HIPO Diagram</a></li>
        <li><a href="#full-chart">Full chart</a></li>
        <li><a href="#alarm">Alarm</a></li>
        <li><a href="#climate">Climate</a></li>
        <li><a href="#entry">Entry</a></li>
      </ul>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

## Requirements
- [x] Configure several different peripherals
- [x] Read values from sensors in a sensible manner
- [x] Configure some sort of HID (display, keypad etc.)
- [x] Present measurements for the user in a user-friendly manner
<p align="right">(<a href="#top">back to top</a>)</p>

### Components
| Device | Amount | Function |
|-|-|-|
| Arduino Mega 2560 | 1 | Microcontroller |
| DHT11 | 1 | Temperature and humidity sensor |
| MQ-2 | 1 | Gas/air quality sensor |
| Servo | 2 | Controlling window and garage door |
| REED switch | 1 | Intrusion detection, front door |
| PIR sensor | 1 | Intrusion (motion) detection, living room |
| RFID reader | 1 | Entry system |
| 4x4 Keypad | 1 | Entry system |
| 16x2 LCD | 1 | Main display (armed, disarmed etc. |
| 128x64 OLED | 1 | Peripheral display around the house (time, temp, status etc.|
| RTC | 1 | Clock providing correct time for the controller |
| Computer | 1 | (Optional - provides logging function and remote controlling)
* The system is built for expansion, there is very little work in attaching multiple devices to the system, that could be more intrusion sensors, a buzzer, multiple displays etc.
<p align="right">(<a href="#top">back to top</a>)</p>

### Libraries
| Name | Version | Component |
|-|-|-|
| Adafruit SSD1306 | 2.5.0 | OLED display
| DS3231 |1.0.7 | RTC
| DHT sensor library | 1.4.3 | DHT11 sensor
| Keypad |3.1.1 | 4x4 keypad
| Liquid Crystal | 1.0.7 | LCD
| MFRC522 | 1.1.8 | RFID reader
| Servo |1.1.6 | Servo motors
| Wire | 1.8.2 | I<sup>2</sup>C communication
| SPI | 1.9.8 | SPI communication       
<p align="right">(<a href="#top">back to top</a>)</p>

# Case
Build a master unit for the *Intelligent house* - the unit must be the "heart" of the communication, and control several functions around the house. 
* Perimeter alarm system
* Internal alarm system
* Climate control
* Gas detection
* Entry system
* User-friendly interaction
<p align="right">(<a href="#top">back to top</a>)</p>

##  Summary/rundown
The **IntelliHouse2000** is a all-in-one microcontroller that provides climate control, monitoring and alarm/entry functions for the entire house. You can have sensors in all rooms, and set the parameters for each sensor. 
If an event is triggered while the alarm is armed, no apparent function will happen in the house, but the log will be updated and the user will get a message<sup>1</sup> with the event, timestamp and what sensor triggered it. If however the alarm is disarmed (that is, the user is home) **IntelliHouse2000** will take action on the event.
> If you forget to turn off your car in the garage, and the sensor detects rising CO<sub>2</sub> levels, the user will be warned, displays around the house will show the event, and the garage door will open incrementally until the sensor value returns to normal

> If the humidity in the house rises rapidly, the appropriate window will be opened incrementally until the sensor detects a drop in humidity. As an extra function<sup>2</sup> you can add weather sensor as well, so the window *doesn't* open if the humidity outside is higher than inside, or it rains.

> No matter what action have been taken (open doors, windows etc.) those will automatically close when the alarm system is armed. This happens with both perimeter and full arm.
<p align="right">(<a href="#top">back to top</a>)</p>

## Getting started

The setup is fairly straightforward. In this project we used <a href="">MicrochipStudio</a> from Atmel, but you can also use the Arduino IDE for this. 

To add a device go to the appropriate section (e.g. Climate) and locate the header-file (ends with .h) - there you can see the pin-defines, where all you have to do it add your new device to that list like this: `#define NewSensor 8` where *NewSensor* is the "friendly name" of your sensor, and *8* is the pin-number you're using on the board.
Next thing is to "start" the sensor `DHT newDHT(NewSensor, Type)`where *newDHT* is the name of the object and *Type* is the type of sensor (e.g. DHT11).

Then all you do is to add the check to the source code
```
temperature2 = newDHT.readTemperature();
humidity2 = newDHT.readHumidity();
```

Make sure to read the setup for each device, some communicate by one-wire (like the DHT11), some use SPI *(Serial Peripheral Interface)*, some use I<sup>2</sup>C *(Called **Wire** in Arduino)* and others use analog input.

<p align="right">(<a href="#top">back to top</a>)</p>

# Flowcharts

## HIPO Diagram
![HIPO Diagram](https://github.com/jaa2019/H3_Embedded_Intelligent__House/blob/master/HIPO.png?raw=true)
<p align="right">(<a href="#top">back to top</a>)</p>

## Full chart
![Full chart](https://github.com/jaa2019/H3_Embedded_Intelligent__House/blob/master/Full_diagram.drawio.svg?raw=true)
<p align="right">(<a href="#top">back to top</a>)</p>

## Alarm
![HIPO Diagram](https://github.com/jaa2019/H3_Embedded_Intelligent__House/blob/master/Alarm.png?raw=true)
<p align="right">(<a href="#top">back to top</a>)</p>

## Climate
![HIPO Diagram](https://github.com/jaa2019/H3_Embedded_Intelligent__House/blob/master/Climate.png?raw=true)
<p align="right">(<a href="#top">back to top</a>)</p>

## Entry
![HIPO Diagram](https://github.com/jaa2019/H3_Embedded_Intelligent__House/blob/master/Entry.png?raw=true)
<p align="right">(<a href="#top">back to top</a>)</p>

## License
* Hardware: CC-BY-LA (Creative Commons)
* Software: LGPL, GPLv2

<p align="right">(<a href="#top">back to top</a>)</p>


## Contact

Jan Andreasen - [@andreasen_jan](https://twitter.com/andreasen_jan) - jan@tved.it

Project Link: [https://github.com/jaa2019/H3_Embedded_Intelligent__House](https://github.com/jaa2019/H3_Embedded_Intelligent__House)

<p align="right">(<a href="#top">back to top</a>)</p>
