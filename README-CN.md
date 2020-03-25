# Arduino Vibration Toys Controller

这是一个 Arduino 项目， 目的是实现一个 DIY 振动玩具 的 控制器



## 当前的一些信息

当前的开发基于 Arduino UNO , 以及 3 个 舵机 和 一个用来控制舵机的 4x4 矩阵键盘. 这些东西可以从许多途径进行购买. 

例如, 你可以从淘宝购买一个中国廉价版的 Arduino UNO , 大概只需要 4.5$ 美元 (或者 ¥30 人民币). 

一个 4x4 矩阵键盘 大概 1$ 美元 (或者 ¥7 人民币). 

开发时使用的是 3 个 SG90 9g 舵机 来代替 实际 振动玩具 上的电调和电机, 这3个舵机每大概个 1.5$ 美元 (或者 ¥10 人民币) . 



## 开发方法

1. **IDE**: 安装 Clion 
1. **插件**: 在Clion中安装此插件 [Arduino Support](https://plugins.jetbrains.com/plugin/11301-arduino-support/) . 你可以从 [github](https://github.com/vsch/CLionArduinoPlugin) 查看插件的帮助 .
1. **系统环境**: 必须在系统中安装 [Arduino IDE](https://www.arduino.cc/en/main/software) ， 否则上述插件无法找到必须的 SDK 及编译环境 .
1. **设置烧写用 COM 端口**: 在 Clion 的顶部菜单， 选择 **Tools** -> **Arduino Support** -> **change build settings** 来改变板子的类型设置并修改烧写用的COM端口设置 .
1. **串口监视器**: 名为 ```Serial Monitor``` 的串口监视器默认在 Clion 的底栏中, 可以使用它来监视串口的输出并与Arduino交互. 
1. **设置串口监视器用 COM 端口**: 串口监视器的 COM 端口设置是 串口监视器 面板的左上角一个小图标. 可以点击打开设置界面来设置 串口监视器 所连接的COM口.
1. **编译与烧写**: 详见 [这里](https://github.com/vsch/CLionArduinoPlugin#uploading-a-sketch)
