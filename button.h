#ifndef __BUTTON_H
#define __BUTTON_H
#include "stdint.h"
#define PIN_LEVEL   0
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
typedef void BtnCallback;
typedef BtnCallback (*CbFun)(void);

typedef struct PinInfo {
    uint8_t pressDownDef = 0;
    char* name = nullptr;
    uint8_t level = 0;
    uint8_t (* readPin)();  //fill the hardware read pin function
}PinInfo;
typedef struct EventInfo {
    uint16_t holdingTime = 0;
    uint16_t releaseTime = 0;
    uint16_t scanPeriod = 0;
    uint16_t holdingTimeThread = 0;
    uint16_t joggleTimeThread = 0;
    uint16_t releaseTimeThread = 0;
}EventInfo;
class Button {
private:
    char* name;
    uint8_t state;
    BTN_Event event;
    PinInfo pinInfo;
    EventInfo eventInfo;
    CbFun callbackFun[8];
public:
    Button(char* str, EventInfo ev, PinInfo pin);
    BTN_Event getState();
    BTN_Event stateMachine();
    void eventRegist(BTN_Event state, CbFun pFunction);
    void eventUpdate(BTN_Event state);
};
#endif