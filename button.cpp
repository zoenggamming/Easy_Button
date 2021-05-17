#include "button.h"

Button::Button(char* str, EventInfo ev, PinInfo pin)
{
    for (int i = 0; i < 8; i++)  callbackFun[i] = nullptr;
    this->state = 1;
    this->eventInfo = ev;
    this->pinInfo = pin;
    this->name = str;
}

BTN_Event Button::getState()
{
	return this->event;
}

BTN_Event Button::stateMachine()
{
    uint8_t n = this->state;
    uint8_t PinLevel = pinInfo.readPin();
    pinInfo.level = PinLevel;
    switch (n) {
    case 1:
        if (PinLevel == PIN_LEVEL) {
            this->event = PRESS_DOWN;
            this->state = 2;
            if (this->callbackFun[this->event] != nullptr)
                this->callbackFun[this->event]();
        }
        else {
            this->event = IMPENDING;
            if (this->callbackFun[this->event] != nullptr)
                this->callbackFun[this->event]();
        }
        eventInfo.holdingTime = 0;
        eventInfo.releaseTime = 0;
        break;
    case 2:
        if (PinLevel == PIN_LEVEL) {  //按键保持按下状态
            eventInfo.holdingTime += eventInfo.scanPeriod;
            if (eventInfo.holdingTime > eventInfo.holdingTimeThread) {    //按键处于长按状态
                this->state = 5;
                this->event = LONG_TIME_HOLD_START;
                if (this->callbackFun[this->event] != nullptr)
                    this->callbackFun[this->event]();
            }
            else {   //触发HOLDING状态
                this->state = 2;

            }
        }
        else {
            eventInfo.holdingTime += eventInfo.scanPeriod;
            if (eventInfo.holdingTime > eventInfo.joggleTimeThread) { //按键弹起
                this->event = PRESS_UP;
                this->state = 3;
                if (this->callbackFun[this->event] != nullptr)
                    this->callbackFun[this->event]();
            }
            else {   //发生抖动，进行滤波消抖
                this->event = IMPENDING;
                this->state = 1;
                if (this->callbackFun[this->event] != nullptr)
                    this->callbackFun[this->event]();
            }
        }
        break;
    case 3:
        if (PinLevel == PIN_LEVEL) {
            eventInfo.holdingTime += eventInfo.scanPeriod;
            if (eventInfo.holdingTime > eventInfo.joggleTimeThread) { //按键再次按下
                this->event = PRESS_DOWN;
                this->state = 4;
                if (this->callbackFun[this->event] != nullptr)
                    this->callbackFun[this->event]();
            }
            else {   //维持状态不变 
                this->state = 3;
            }
        }
        else {   //按键已经松开
            eventInfo.releaseTime += eventInfo.scanPeriod;
            if (eventInfo.releaseTime > eventInfo.releaseTimeThread) {    //按键单击
                this->event = SINGLE_CLICK;
                this->state = 1;
                if (this->callbackFun[this->event] != nullptr)
                    this->callbackFun[this->event]();
            }
            else {
                this->state = 3;
            }
        }
        break;
    case 4:
        if (PinLevel == PIN_LEVEL) {
            eventInfo.holdingTime += eventInfo.scanPeriod;
        }
        else {   //按键已经松开
            this->event = DOUBLE_CLICK;
            this->state = 1;
            if (this->callbackFun[this->event] != nullptr)
                this->callbackFun[this->event]();
        }
        break;
    case 5:
        if (PinLevel == PIN_LEVEL) {
            eventInfo.holdingTime += eventInfo.scanPeriod;
        }
        else {   //按键已经松开
            this->event = PRESS_UP;
            this->callbackFun[this->event];
            this->event = LONG_TIME_HOLD;
            if (this->callbackFun[this->event] != nullptr)
                this->callbackFun[this->event]();
            this->state = 1;
        }
        break;
    default:
        break;
    }
	return this->event;
}

void Button::eventRegist(BTN_Event state, CbFun pFunction)
{
	this->callbackFun[state] = pFunction;
}

void Button::eventUpdate(BTN_Event state)
{
	this->callbackFun[state]();
}



