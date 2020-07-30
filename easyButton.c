#include "easyButton.h"
#include "stdio.h"
uint32_t holdingTime = 0;
uint32_t releaseTime = 0;
Button btnTest;
Button *btnHead;

Button *creatBtnLinkList(void){
  Button *head = (Button *)malloc(sizeof(Button));
  head->next = NULL;
  return head;
}
Button *creatBtn(void){
  Button *btn = (Button *)malloc(sizeof(Button));
  btn->next = NULL;
  return btn;
}
void addBtn(Button *head, Button *btnNode){
  Button *nowNode = head;
  for( ;nowNode->next != NULL; ){
    nowNode = nowNode->next;
  }
  nowNode->next = btnNode;
}

void scanBtnLinkList(Button *head){
  if(head != NULL){
    Button *nowBtn = head;
    for( ;nowBtn->next != NULL; ){
      nowBtn = nowBtn->next;
      BtnStateMachine(nowBtn);
    }
  }
}

void btnInit(Button *btn, uint8_t (*GPIO_ReadPin_x)(void), char* lable, uint8_t enableLevel){
    memset(btn, 0, sizeof(Button));
    btn->lable = lable;
    btn->state = 1;
    btn->hal_btnLevel = GPIO_ReadPin_x;
    btn->level = (*(btn->hal_btnLevel))();
    btn->enableLevel = enableLevel;
    btn->event = IMPENDING;
}

void btnEventRegister(Button *btn, PressEvent event, BtnCallback cbFunction){
    btn->cb[event] = cbFunction;
}

PressEvent getBtnEvent(Button *btn){
    return btn->event;
}

//give a example below
int btnx_SingleClick_Callback(void){
    /*USER CODE BEGIN*/
    // t = time(NULL);
    // local = localtime(&t);
    // printf("Event: a Single Click! ---time: %2d:%2d:%2d \n",local->tm_hour, local->tm_min, local->tm_sec);

    /*USER CODE END*/
    return 1;
}




void BtnStateMachine(Button *btn){
    uint8_t n = btn->state;
    uint8_t PinLevel = (*(btn->hal_btnLevel))();
    btn->level = PinLevel;
    switch(n){
        case 1:
            if(PinLevel == 1){
                btn->event = PRESS_DOWN;
                btn->state = 2;
                (*btn->cb[btn->event])();
            }
            else{
                btn->event = IMPENDING;
                (*btn->cb[btn->event])();
            }
            holdingTime = 0;
            releaseTime = 0;
            break;
        case 2:
            if(PinLevel == 1){  //按键保持按下状态
                holdingTime += scanPeriod;
                if(holdingTime > holdingTimeThread){    //按键处于长按状态
                    btn->state = 5;
                    btn->event = LONG_TIME_HOLD_START;
                    (*btn->cb[btn->event])();
                }
                else{   //触发HOLDING状态
                    btn->state = 2;
                    
                }
            }
            else{
                holdingTime += scanPeriod;
                if(holdingTime > joggleTimeThread){ //按键弹起
                    btn->event = PRESS_UP;
                    btn->state = 3;
                    (*btn->cb[btn->event])();
                }
                else{   //发生抖动，进行滤波消抖
                    btn->event = IMPENDING;
                    btn->state = 1;
                    (*btn->cb[btn->event])();
                }
            }
            break;
        case 3:
            if(PinLevel == 1){
                holdingTime += scanPeriod;
                if(holdingTime > joggleTimeThread){ //按键再次按下
                    btn->event = PRESS_DOWN;
                    btn->state = 4;
                    (*btn->cb[btn->event])();
                }
                else{   //维持状态不变 
                     btn->state = 3;
                }
            }
            else{   //按键已经松开
                releaseTime += scanPeriod;
                if(releaseTime > releaseTimeThread){    //按键单击
                    btn->event = SINGLE_CLICK;
                    btn->state = 1;
                    (*btn->cb[btn->event])();
                }
                else{
                    btn->state = 3;
                }
            }
            break;
        case 4:
            if(PinLevel == 1){
                holdingTime += scanPeriod;
            }
            else{   //按键已经松开
                btn->event = DOUBLE_CLICK;
                btn->state = 1;
                (*btn->cb[btn->event])();
            }
            break;
        case 5:
            if(PinLevel == 1){
                holdingTime += scanPeriod;
            }
            else{   //按键已经松开
                btn->event = PRESS_UP;
                btn->cb[btn->event];
                btn->event = LONG_TIME_HOLD;
                (*btn->cb[btn->event])();
                btn->state = 1;
            }
            break;
        default:
                break;
    }
}