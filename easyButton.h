//
// Created by zoeng on 2020/7/10.
//

#ifndef __EASYBUTTON_H
#define __EASYBUTTON_H
#include "stdint.h"
typedef uint8_t Bool;
#define TRUE    1
#define FALSE   0
#define scanPeriod  5
#define holdingTimeThread   1000
#define joggleTimeThread    3
#define releaseTimeThread   150
extern uint32_t holdingTime;
extern uint32_t releaseTime;

typedef int (*BtnCallback)(void);

typedef enum{
    IMPENDING = 0,  //悬空
    PRESS_DOWN,  //按下
    PRESS_UP,   //弹起
    SINGLE_CLICK,   //单击
    DOUBLE_CLICK,   //双击
    LONG_TIME_HOLD_START,   //开始长按
    LONG_TIME_HOLD, //长按
    EVENT_NUM  //状态数量
}PressEvent;

typedef struct Button{
    char *lable;
    //uint8_t repeat;
    uint8_t event;
    uint8_t level;
    uint8_t state;
    uint8_t enableLevel;
    uint8_t (*hal_btnLevel)(void);
    BtnCallback cb[EVENT_NUM]; 
    struct Button *next;
}Button;

Button *creatBtnLinkList(void);
Button *creatBtn(void);
void addBtn(Button *head, Button *btnNode);
void scanBtnLinkList(Button *head);
void btnInit(Button *btn, uint8_t (*GPIO_ReadPin_x)(void), char* lable, uint8_t enableLevel);
void btnEventRegister(Button *btn, PressEvent event, BtnCallback cbFunction);
void BtnStateMachine(Button *btn);
PressEvent getBtnEvent(Button *btn);

extern Button btnTest;
#endif //__EASYBUTTON_H
