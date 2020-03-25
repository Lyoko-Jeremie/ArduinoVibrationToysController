# Arduino Vibration Toys Controller

[简体中文](./README-CN.md)

this is a Arduino project write to implement a Controller for DIY Vibration Toys


## now info

Now , the dev base on Arduino UNO , with 3 Servos support and a 4x4 matrix keyboard to control the Servos. you can buy it from many ways. 

Example, you can buy a Chinese low-cost version Arduino UNO from Alibaba ( or we also call it Taobao) by about 4.5$ USD (or ¥30 RMB). 

the 4x4 matrix keyboard cost about 1$ USD (or ¥7 RMB). 

when dev, use 3 SG90 9g Servos to instead Electronic Speed Control and Motor in real Vibration Toys, those 3 Servos every cost about 1.5$ USD (or ¥10 RMB) . 



## How to dev it

1. **IDE**: Clion 
1. **Plugin**: [Arduino Support](https://plugins.jetbrains.com/plugin/11301-arduino-support/) . you can find help info from [github](https://github.com/vsch/CLionArduinoPlugin) .
1. **Env**: must install [Arduino IDE](https://www.arduino.cc/en/main/software) in system to make above plugin can find SDK .
1. **Set Flash COM port**: on Clion top menu **Tools** -> **Arduino Support** -> **change build settings** to change Board setting and flash COM port setting .
1. **Serial Monitor**: the Serial Monitor at the bottom bar of Clion, you can use it to monitor port. 
1. **Set Monitor COM port**: the Serial Monitor COM port setting is a small icon at left-top of Serial Monitor Plane. you need to set it to change the port.
1. **Build and Flash**: see [this](https://github.com/vsch/CLionArduinoPlugin#uploading-a-sketch)
