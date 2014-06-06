#include "../ucos-ii/includes.h"               /* uC/OS interface */
#include "../ucos-ii/add/osaddition.h"
#include "../ucos-ii/add/ustring.h"
#include "../inc/drivers.h"
#include "../inc/sys/lib.h"
#include "../src/gui/gui.h"

#include <string.h>
#include <stdio.h>

OS_STK Main_Stack[STACKSIZE];
void Main_Task(void *pdata);
#define Main_Task_Prio 12

void onKey(unsigned int nkey, int fnkey)
{
     PDC pdc; // 创建绘图设备上下文结构
     U16 ch[10];
     char *str, tmp[10];

     if((nkey&KEY_DOWN) == 0)
          return;

     ClearScreen(); // 清屏
     pdc = CreateDC(); //创建绘图设备上下文

     switch(nkey&0xff) {
     case 'N':
          str = "NumLock";
          break;
     case '\r':
          str = "Enter";
          break;
     default:
          tmp[0] = nkey;
          tmp[1] = '\0';
          str = tmp;
     }

     strChar2Unicode(ch, str); // 将整形数据转化成Unicode
     TextOut(pdc, 150, 100, ch, TRUE, FONTSIZE_MIDDLE); // 文本模式下显示文字
     DestoryDC(pdc); // 删除绘图设备上下文
}

void Main_Task(void *pdata)
{
     POSMSG pMsg = 0; // 创建消息结构

     ClearScreen(); // 清屏

     // 消息循环
     for(;;){
          pMsg=WaitMessage(0); // 等待消息
          switch(pMsg->Message){
          case OSM_KEY: // 键盘消息
               onKey(pMsg->WParam, pMsg->LParam);
               break;
          }
          DeleteMessage(pMsg); // 删除消息,释放资源
     }
}

int main(void)
{
     ARMTargetInit();
     OSInit();
     OSInitUart();
     initOSFile();

#if USE_MINIGUI==0
     initOSMessage();
     initOSList();
     initOSDC();
     initOSCtrl();
     LoadFont();
#endif
     loadsystemParam();

     LCD_printf("Create task on uCOS-II...\n");
     OSTaskCreate(Main_Task,
                  NULL,
                  (OS_STK *)&Main_Stack[STACKSIZE-1],
                  Main_Task_Prio);
     OSAddTask_Init(1);

     LCD_printf("Starting uCOS-II...\n");
     LCD_printf("Entering graph mode...\n");
     LCD_ChangeMode(DspGraMode);

     OSStart();
     return 0;
}
