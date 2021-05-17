# 按键模块的使用方法 How to Use Button.cpp

## 1、按键对象初始化

通过Button类的构造函数初始化按键对象

```cpp
Button btn1("UP", ev1, pin1);
```

其中ev1是一个EventInfo对象，pin1是一个PinInfo对象。

可以如下定义：

```c++
#define    HOLDING_TIME  0
#define    RELEASE_TIM  0
#define    SCAN_PERIOD  1
#define    HOLDING_TIME_THREAD  1500
#define    JOGGLE_TIME_THREAD  20
#define    RELEASE_TIME_THREAD  500
EventInfo ev1 = {HOLDING_TIME, RELEASE_TIM, SCAN_PERIOD, HOLDING_TIME_THREAD, JOGGLE_TIME_THREAD, RELEASE_TIME_THREAD};

uint8_t getPin1(void){
    uint8_t res = HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin);
    return res;
}
PinInfo pin1 = {0, "PC5", 0, getPin1};
```



## 2、注册按键事件对应的回调函数

对于每一个按键对象可以有如下7种事件

```c++
typedef enum {
    IMPENDING = 0,  //悬空
    PRESS_DOWN,  //按下
    PRESS_UP,   //弹起
    SINGLE_CLICK,   //单击
    DOUBLE_CLICK,   //双击
    LONG_TIME_HOLD_START,   //开始长按
    LONG_TIME_HOLD, //长按
    EVENT_NUM  //状态数量
}BTN_Event;
```

使用者可以根据每种事件可以定义自己的回调函数,之后将其注册到对应的按键对象当中，方法如下：

```c++
BtnCallback key0_SINGLE_CLICK(void){
    //user code begin here
}
BtnCallback key0_LONG_TIME_HOLD(void){
    //user code begin here
}

int main(void){
    Button btn1("UP", ev1, pin1);
    btn1.eventRegist(SINGLE_CLICK, key0_SINGLE_CLICK);  //regist btn callback funciton
  	btn1.eventRegist(LONG_TIME_HOLD,key0_LONG_TIME_HOLD);  //regist btn callback funciton
    return 0;
}
```



## 3、在合适的位置调用按键状态机函数

可以在定时器中断或者主循环中调用stateMachine()函数，执行按键状态的检测。

例如：

```c++
int main(void){
    Button btn1("UP", ev1, pin1);
    while(1){
		btn1.stateMachine();        
    }
    return 0;
}

```



## 4、完整的使用例程

### 文件结构

folder

​			button.cpp

​			button.h

​			main.cpp



```c++
//main.cpp
#include "button.h"

//define button event time, unit ms
#define    HOLDING_TIME  0
#define    RELEASE_TIM  0
#define    SCAN_PERIOD  1
#define    HOLDING_TIME_THREAD  1500
#define    JOGGLE_TIME_THREAD  20
#define    RELEASE_TIME_THREAD  500
EventInfo ev1 = {HOLDING_TIME, RELEASE_TIM, SCAN_PERIOD, HOLDING_TIME_THREAD, JOGGLE_TIME_THREAD, RELEASE_TIME_THREAD};

//define your get pin input function, according your own mcu
uint8_t getPin1(void){
    uint8_t res = HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin);
    return res;
}
PinInfo pin1 = {0, "PC5", 0, getPin1};

//define your btn event callback function
BtnCallback key0_SINGLE_CLICK(void){
    //user code begin here
}
BtnCallback key0_LONG_TIME_HOLD(void){
    //user code begin here
}

int main(void){
    Button btn1("UP", ev1, pin1);  //define btn object whose name is "UP"
    
    btn1.eventRegist(SINGLE_CLICK, key0_SINGLE_CLICK);  //regist btn callback funciton
  	btn1.eventRegist(LONG_TIME_HOLD,key0_LONG_TIME_HOLD);  //regist btn callback funciton
    
    while(1){
		btn1.stateMachine();  //run button scan funciton, corresponding callback funciton will be executed in this function,        
    }
    
    return 0;
}
```

