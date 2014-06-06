#include "../ucos-ii/includes.h"
#include "../ucos-ii/add/osaddition.h"
#include "../ucos-ii/add/ustring.h"
#include "../inc/drivers.h"
#include "../inc/sys/lib.h"
#include "../src/gui/gui.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

void calculate();
void str2repol();
void readfile();
void clc();
void send();
void lcddisplay(char *str,int xx,int yy);
void rezdisplay(double res);
void onKey(unsigned int nkey, int fnkey);
void myontouch(int xxx,int yyy);
void myButtonall();
void redraw(char key);
void send(PButtonCtrl b);
char zb2key(int x,int y);
void dcr(char key);
char *doub2str(double d, char *str);

///******************任务定义***************///
void Main_Task(void *Id);
OS_STK Main_Stack[STACKSIZE]={0, };   //Main_Test_Task堆栈
#define Main_Task_Prio     12
void ontouch(unsigned int x, int y);
OS_STK test_Stack[STACKSIZE]={0, };   //test_Test_Task堆栈
void test_Task(void *Id);             //test_Test_Task
#define test_Task_Prio     15


char TextFilename[]="/sys/ucos/fj/2.txt";       //自定义声明
//文件test:   1+2+3+4+5+6+7+8+9=
//文件1:   1+2+3+4+5+6+7+8+9=
//文件2:   0.7+5*((1+2)/(2+4))=
//文件3:
FILE* pfile;
#define maxs 200
#define react 100
#define xz 130
#define yz 100
char cal[maxs];
char read[maxs];
int number = 1, clear = 1;
double result;
U16 str1[20];
char repol[maxs];       /*存储后缀表达式*/

PButtonCtrl b0;
PButtonCtrl b1;
PButtonCtrl b2;
PButtonCtrl b3;
PButtonCtrl b4;
PButtonCtrl b5;
PButtonCtrl b6;
PButtonCtrl b7;
PButtonCtrl b8;
PButtonCtrl b9;
PButtonCtrl b10;
PButtonCtrl b11;
PButtonCtrl b12;
PButtonCtrl b13;
PButtonCtrl b14;
PButtonCtrl b15;
PButtonCtrl b16;
PButtonCtrl b17;
PButtonCtrl b18;
PButtonCtrl b19;
//PTextCtrl b20;

U32 CtrlID0=200;
U32 CtrlID1=201;
U32 CtrlID2=202;
U32 CtrlID3=303;
U32 CtrlID4=204;
U32 CtrlID5=205;
U32 CtrlID6=306;
U32 CtrlID7=207;
U32 CtrlID8=208;
U32 CtrlID9=209;
U32 CtrlID10=210;
U32 CtrlID11=211;
U32 CtrlID12=212;
U32 CtrlID13=213;
U32 CtrlID14=214;
U32 CtrlID15=215;
U32 CtrlID16=216;
U32 CtrlID17=217;
U32 CtrlID18=218;
U32 CtrlID19=219;
U32 CtrlID20=220;
structRECT rect0;//定义矩形框结构
structRECT rect1;
structRECT rect2;
structRECT rect3;
structRECT rect4;
structRECT rect5;
structRECT rect6;
structRECT rect7;
structRECT rect8;
structRECT rect9;
structRECT rect10;
structRECT rect11;
structRECT rect12;
structRECT rect13;
structRECT rect14;
structRECT rect15;
structRECT rect16;
structRECT rect17;
structRECT rect18;
structRECT rect19;
structRECT rect20;


U16 Caption0[16];    //定义按键功能	
U16 Caption1[16];
U16 Caption2[16];
U16 Caption3[16];
U16 Caption4[16];
U16 Caption5[16];
U16 Caption6[16];
U16 Caption7[16];
U16 Caption8[16];
U16 Caption9[16];
U16 Caption10[16];
U16 Caption11[16];
U16 Caption12[16];
U16 Caption13[16];
U16 Caption14[16];
U16 Caption15[16];
U16 Caption16[16];
U16 Caption17[16];
U16 Caption18[16];
U16 Caption19[16];
U16 Caption_Actived[16];

U32 ButtonStyle=5;
U32 ButtonStyle_Actived=3;
U32 ButtonFontSize= 2;

U32 TextStyle=4;
U32 TextFontSize= 2;

/******************主函数***************///
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
 


   OSTaskCreate(Main_Task,
               NULL,
                (OS_STK *)&Main_Stack[STACKSIZE-1],
                Main_Task_Prio);
   OSAddTask_Init(1);
    LCD_ChangeMode(DspGraMode);

  lcddisplay("hi", xz, yz);
  OSStart();
  return 0;
}
//////////////////////////主任务/////////////////////////////////////////////////////////////////////////////
void Main_Task(void *Id)
{
  PButtonCtrl b;
  POSMSG pMsg = 0;//创建消息结构
  U32 tou;
  int x=0;
  int y=0;

  ClearScreen();
  myButtonall();

  for(;;) {
    pMsg = WaitMessage(0); //等待消息
    switch(pMsg->Message) {
      case OSM_KEY: // 键盘消息
        onKey(pMsg->WParam, pMsg->LParam);
        break;
      case OSM_TOUCH_SCREEN:
        x=pMsg->WParam&0xffff;
        y=pMsg->WParam>>16;
        myontouch(x,y);
        break;
    }
    DeleteMessage(pMsg);//删除消息,释放资源
  }
}

void onKey(unsigned int nkey, int fnkey)
{
  int ret; // return value ?
  char *str, skey[16];
  cal[0]='0';
  if(clear==1) { //清屏
    clear=0;clc();
  }

  lcddisplay(cal+1, xz, yz); //

  if((nkey&KEY_DOWN) == 0)
    return;

  switch(nkey&0xff){
  case 'N':
    str="NumLock";
    clc();
    readfile();
    break;
  case '\r':
    str="Enter";
    cal[number]='=';
    redraw(cal[number]);
    lcddisplay(cal+1,xz,yz);
    str1[number] = cal[number];
    str2repol();
    calculate(); // 计算
    for(ret=1; ret<=number; ret++)
    {
      cal[ret]='\0';
      if(ret == number){number=1;ret=2;}
    }
    clear=2;
    break;
  default:
    skey[0] = nkey;
    skey[1] = 0;
    str = skey;
    cal[number] = nkey;
    redraw(cal[number]);
    lcddisplay(cal+1, xz, yz);
    str1[number] = cal[number];
    number+=1;
    if(clear == 2)
      clear=1;
  }
  printf("%s\r",cal+1);
}

void myontouch(int xxx,int yyy)
{
  int ret;
  if(clear==1){clear=0;clc();}//清屏
  cal[number]=zb2key(xxx,yyy);
  redraw(cal[number]);
  switch(cal[number])
  {
  case 'd':
    printf("\ndelre\n");
    dcr(cal[number]);break;
  case 'c':
    dcr(cal[number]);break;
  case 'r':
    dcr(cal[number]);break;
  case '=':
    printf("表达式：%s\n",cal+1);
    lcddisplay(cal+1,xz,yz);
    str1[number]=cal[number];
    str2repol();
    calculate();
    clear=2;
    for(ret=1;ret<=number;ret++)
    {
      cal[ret]='\0';
      if(ret==number){number=1;ret=2;}
    }
    break;
  default:
    if(clear==2)   clear=1;
    str1[number]=cal[number];
    lcddisplay(cal+1,xz,yz);
    number++;
    break;
  }
  printf("%s\r",cal+1);
}

void str2repol()
{        /*将算术表达式转化为后缀表达式*/
  char str[maxs],strtem[maxs];   /*存储原算术表达式*/
  char stack[maxs];       /*作为栈使用*/
  char ch,*er;
  int i,t,top=0,j,sum,st;	
  i=0;                      /*获取用户输入的表达式*/
  do{
    i++;
    str[i]=cal[i];
  }while(cal[i]!='=' && i!=maxs);
  sum=i;        ////////////////////sum中记录了str表达式中的字符的个数
  t=1;i=1;
  if(str[1]=='-')
  {		
    for(st=1;st<sum;st++)
      strtem[st]=str[st+1];	
    str[1]='0';
    str[2]='-';
    for(st=3;st<=sum+1;st++)
      str[st]=strtem[st-2];
    sum++;
  }
  ch=str[i];i++;//ch为表达式中第一个字符
  while(ch!='='){
    switch(ch){
    case '(':                 /*判定为左括号*/
      top++;stack[top]=ch;
      break;
    case ')':                 /*判定为右括号*/
      while(stack[top]!='('){
        repol[t]=stack[top];top--;t++;
      }
      top--;
      break;
    case '+':                 /*判定为加减号*/
    case '-':
      while(top!=0&&stack[top]!='('){
        repol[t]=stack[top];top--;t++;
      }
      top++;stack[top]=ch;
      break;
    case '*':                  /*判定为乘除号*/
    case '/':
      while(stack[top]=='*'||stack[top]=='/'){
        repol[t]=stack[top];top--;t++;
      }
      top++;stack[top]=ch;
      break;
    case ' ':break;
    default:while(ch>='0'&&ch<='9'||ch=='.')
    {                 /*判定为数字*/
      repol[t]=ch;t++;
      ch=str[i];i++;
    }
    i--;
    repol[t]='=';
    t++;
    /*if(ch<'0'&&ch>'9'&&ch!='.')
    {
    er="error!";
    lcddisplay(er,xz,yz+20);
    ch='=';
    }*/
    }
	ch=str[i];i++;
  }
  while(top!=0){
    repol[t]=stack[top];t++;top--;
  }
  repol[t]='=';	
}
void calculate()
{                                 /*计算后缀表达式的值*/
  double stack[maxs],d0,d1,dd;                    /*作为栈使用*/
  unsigned int re;
  char ch,*restr,*rez;
  int t=1,top=0,counter,i;                  /*t为ex下标，top为stack下标*/
  static int zh=1;
  ch=repol[t];t++;
  while(ch != '='){
    switch(ch){
    case '+':
      stack[top-1] = stack[top-1] + stack[top];
      top--;
      break;
    case '-':
      stack[top-1]=stack[top-1]-stack[top];
      top--;
      break;
    case '*':
      stack[top-1]=stack[top-1]*stack[top];
      top--;
      break;

    case '/':
      if(stack[top]!=0)
        stack[top-1]=stack[top-1]/stack[top];
      else{
        printf("\n\t除零错误!\n");
        lcddisplay("ERROR!",xz,yz);
        // exit(0);                   /*异常退出*/
      }
      top--;
      break;
    default:        /*将数字字符转化为对应的数值*/
      d1=0;
      d0=0;
      counter=0;
      while(ch>='0'&&ch<='9'||ch=='.')
      {
        if(ch!='.'&&zh==1)
        {
          d1=10.0*d1+ch-'0';
        }
        if(ch=='.')zh=0;
        if(ch!='.'&&zh==0)
        {
          d0=10.0*d0+ch-'0';
          counter++;
        }
        ch=repol[t];t++;
      }//计算
      zh=1;
      for(i=0;i<counter;i++)
      {d0=d0/10.0;}
      dd=d1+d0;
      top++;
      stack[top]=dd;
    }
    ch=repol[t];t++;
  } //转换运算
  result=stack[top];
  printf("\n计算结果:%f\n\n",stack[top]);
  rez = doub2str(result,restr);
  lcddisplay(rez,xz,yz+20);
}

void lcddisplay(char *str,int xx,int yy)
{
  PDC pdc; //创建绘图设备上下文结构
  U16 chx[100];	
  pdc=CreateDC(); //创建绘图设备上下文f
  strChar2Unicode(chx, str);
  TextOut(pdc, xx, yy, chx, TRUE, FONTSIZE_MIDDLE); //文本模式下显示文字
  DestoryDC(pdc); //删除绘图设备上下文	
}

void myButtonall()
{
  PDC pdc;//定义绘图设备上下文结构
  pdc=CreateDC();//创建绘图设备上下文
  //定义按键功能
  Int2Unicode(1,Caption0);
  Int2Unicode(2,Caption1);
  Int2Unicode(3,Caption2);
  strChar2Unicode(Caption3,"+");
  Int2Unicode(4,Caption4);
  Int2Unicode(5,Caption5);
  Int2Unicode(6,Caption6);
  strChar2Unicode(Caption7,"-");
  Int2Unicode(7,Caption8);
  Int2Unicode(8,Caption9);
  Int2Unicode(9,Caption10);
  strChar2Unicode(Caption11,"*");
  strChar2Unicode(Caption12,"(");
  Int2Unicode(0,Caption13);
  strChar2Unicode(Caption14,")");
  strChar2Unicode(Caption15,"/");
  strChar2Unicode(Caption16,"del");
  strChar2Unicode(Caption17,"read");
  strChar2Unicode(Caption18,"clear");
  strChar2Unicode(Caption19,"enter" );
  strChar2Unicode(Caption_Actived,"AAA" );



  SetRect(&rect0, 165,165,235,235);	//第一行
  SetRect(&rect1, 245,165,315,235);
  SetRect(&rect2, 325,165,395,235);
  SetRect(&rect3, 405,165,475,235);
  SetRect(&rect4, 165,245,235,315);	//第二行
  SetRect(&rect5, 245,245,315,315);
  SetRect(&rect6, 325,245,395,315);
  SetRect(&rect7, 405,245,475,315);
  SetRect(&rect8, 165,325,235,395);//第三行
  SetRect(&rect9, 245,325,315,395);
  SetRect(&rect10, 325,325,395,395);
  SetRect(&rect11, 405,325,475,395);
  SetRect(&rect12, 165,405,235,475);//第四行
  SetRect(&rect13, 245,405,315,475);
  SetRect(&rect14, 325,405,395,475);
  SetRect(&rect15, 405,405,475,475);
  SetRect(&rect16, 5,165,155,315);//左1
  SetRect(&rect17, 5,325,155,475);//左2
  SetRect(&rect18, 485,165,635,315);//右1
  SetRect(&rect19, 485,325,635,475);//右2
  SetRect(&rect20, 165,5,475,155);//顶栏

  //设置间距10后坐标：
  b0=CreateButton(CtrlID0, &rect0, ButtonFontSize, ButtonStyle,Caption0, NULL);
  b1=CreateButton(CtrlID1, &rect1, ButtonFontSize, ButtonStyle, Caption1, NULL);
  b2=CreateButton(CtrlID2, &rect2, ButtonFontSize, ButtonStyle_Actived, Caption2, NULL);
  b3=CreateButton(CtrlID3, &rect3, ButtonFontSize, ButtonStyle, Caption3, NULL);
  b4=CreateButton(CtrlID4, &rect4, ButtonFontSize, ButtonStyle, Caption4, NULL);
  b5=CreateButton(CtrlID5, &rect5, ButtonFontSize, ButtonStyle, Caption5, NULL);
  b6=CreateButton(CtrlID6, &rect6, ButtonFontSize, ButtonStyle, Caption6, NULL);
  b7=CreateButton(CtrlID7, &rect7, ButtonFontSize, ButtonStyle, Caption7, NULL);
  b8=CreateButton(CtrlID8, &rect8, ButtonFontSize, ButtonStyle, Caption8, NULL);
  b9=CreateButton(CtrlID9, &rect9, ButtonFontSize, ButtonStyle, Caption9, NULL);
  b10=CreateButton(CtrlID10, &rect10, ButtonFontSize, ButtonStyle, Caption10, NULL);
  b11=CreateButton(CtrlID11, &rect11, ButtonFontSize, ButtonStyle, Caption11, NULL);
  b12=CreateButton(CtrlID12, &rect12, ButtonFontSize, ButtonStyle, Caption12, NULL);
  b13=CreateButton(CtrlID13, &rect13, ButtonFontSize, ButtonStyle, Caption13, NULL);
  b14=CreateButton(CtrlID14, &rect14, ButtonFontSize, ButtonStyle, Caption14, NULL);
  b15=CreateButton(CtrlID15, &rect15, ButtonFontSize, ButtonStyle, Caption15, NULL);
  b16=CreateButton(CtrlID16, &rect16, ButtonFontSize, ButtonStyle, Caption16, NULL);
  b17=CreateButton(CtrlID17, &rect17, ButtonFontSize, ButtonStyle, Caption17, NULL);
  b18=CreateButton(CtrlID18, &rect18, ButtonFontSize, ButtonStyle, Caption18, NULL);
  b19=CreateButton(CtrlID19, &rect19, ButtonFontSize, ButtonStyle, Caption19, NULL);	
  //b20=CreateTextCtrl(CtrlID20, &rect20, TextFontSize, TextStyle, NULL, NULL);



  DrawButton(b0);
  DrawButton(b1);
  DrawButton(b2);
  DrawButton(b3);
  DrawButton(b4);
  DrawButton(b5);
  DrawButton(b6);
  DrawButton(b7);
  DrawButton(b8);
  DrawButton(b9);
  DrawButton(b10);
  DrawButton(b11);
  DrawButton(b12);
  DrawButton(b13);
  DrawButton(b14);
  DrawButton(b15);
  DrawButton(b16);
  DrawButton(b17);
  DrawButton(b18);
  DrawButton(b19);
  //DrawTextCtrl(b20);
  DestoryDC(pdc);//删除绘图设备上下文

}

void send(PButtonCtrl b)
{
  POSMSG pmsg;
  pmsg=OSCreateMessage(NULL, OSM_KEY,2147483697,0);
  SendMessage(pmsg);
}

void redraw(char key)
{
  PDC pdc;//定义绘图设备上下文结构
  pdc=CreateDC();//创建绘图设备上下文
  switch(key)
  {
    case'1':DestoryButton(b0);
    b0=CreateButton(CtrlID0, &rect0, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b0);
    DestoryButton(b0);
    b0=CreateButton(CtrlID0, &rect0, ButtonFontSize, ButtonStyle, Caption0, NULL);
    OSTimeDly(react);
    DrawButton(b0);
    break;
    case'2':DestoryButton(b1);
    b1=CreateButton(CtrlID1, &rect1, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b1);
    DestoryButton(b1);
    b1=CreateButton(CtrlID1, &rect1, ButtonFontSize, ButtonStyle, Caption1, NULL);
    OSTimeDly(react);
    DrawButton(b1);
    break;
    case'3':DestoryButton(b2);
    b2=CreateButton(CtrlID2, &rect2, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b2);
    DestoryButton(b2);
    b2=CreateButton(CtrlID2, &rect2, ButtonFontSize, ButtonStyle, Caption2, NULL);
    OSTimeDly(react);
    DrawButton(b2);
    break;
    case'+':DestoryButton(b3);
    b3=CreateButton(CtrlID3, &rect3, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b3);
    DestoryButton(b3);
    b3=CreateButton(CtrlID3, &rect3, ButtonFontSize, ButtonStyle, Caption3, NULL);
    OSTimeDly(react);
    DrawButton(b3);
    break;
    case'4':DestoryButton(b4);
    b4=CreateButton(CtrlID4, &rect4, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b4);
    DestoryButton(b4);
    b4=CreateButton(CtrlID4, &rect4, ButtonFontSize, ButtonStyle, Caption4, NULL);
    OSTimeDly(react);
    DrawButton(b4);
    break;
    case'5':DestoryButton(b5);
    b5=CreateButton(CtrlID5, &rect5, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b5);
    DestoryButton(b5);
    b5=CreateButton(CtrlID5, &rect5, ButtonFontSize, ButtonStyle, Caption5, NULL);
    OSTimeDly(react);
    DrawButton(b5);
    break;
    case'6':DestoryButton(b6);
    b6=CreateButton(CtrlID6, &rect6, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b6);
    DestoryButton(b6);
    b6=CreateButton(CtrlID6, &rect6, ButtonFontSize, ButtonStyle, Caption6, NULL);
    OSTimeDly(react);
    DrawButton(b6);
    break;
    case'-':DestoryButton(b7);
    b7=CreateButton(CtrlID7, &rect7, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b7);
    DestoryButton(b7);
    b7=CreateButton(CtrlID7, &rect7, ButtonFontSize, ButtonStyle, Caption7, NULL);
    OSTimeDly(react);
    DrawButton(b7);
    break;
    case'7':DestoryButton(b8);
    b8=CreateButton(CtrlID8, &rect8, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b8);
    DestoryButton(b8);
    b8=CreateButton(CtrlID8, &rect8, ButtonFontSize, ButtonStyle, Caption8, NULL);
    OSTimeDly(react);
    DrawButton(b8);
    break;
    case'8':DestoryButton(b9);
    b9=CreateButton(CtrlID9, &rect9, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b9);
    DestoryButton(b9);
    b9=CreateButton(CtrlID9, &rect9, ButtonFontSize, ButtonStyle, Caption9, NULL);
    OSTimeDly(react);
    DrawButton(b9);
    break;
    case'9':DestoryButton(b10);
    b10=CreateButton(CtrlID10, &rect10, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b10);
    DestoryButton(b10);
    b10=CreateButton(CtrlID10, &rect10, ButtonFontSize, ButtonStyle, Caption10, NULL);
    OSTimeDly(react);
    DrawButton(b10);
    break;
    case'*':DestoryButton(b11);
    b11=CreateButton(CtrlID11, &rect11, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b11);
    DestoryButton(b11);
    b11=CreateButton(CtrlID11, &rect11, ButtonFontSize, ButtonStyle, Caption11, NULL);
    OSTimeDly(react);
    DrawButton(b11);
    break;
    case'(':DestoryButton(b12);
    b12=CreateButton(CtrlID12, &rect12, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b12);
    DestoryButton(b12);
    b12=CreateButton(CtrlID12, &rect12, ButtonFontSize, ButtonStyle, Caption12, NULL);
    OSTimeDly(react);
    DrawButton(b12);
    break;
    case'0':DestoryButton(b13);
    b13=CreateButton(CtrlID13, &rect13, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b13);
    DestoryButton(b13);
    b13=CreateButton(CtrlID13, &rect13, ButtonFontSize, ButtonStyle, Caption13, NULL);
    OSTimeDly(react);
    DrawButton(b13);
    break;
    case')':DestoryButton(b14);
    b14=CreateButton(CtrlID14, &rect14, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b14);
    DestoryButton(b14);
    b14=CreateButton(CtrlID14, &rect14, ButtonFontSize, ButtonStyle, Caption14, NULL);
    OSTimeDly(react);
    DrawButton(b14);
    break;
    case'/':DestoryButton(b15);
    b15=CreateButton(CtrlID15, &rect15, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b15);
    DestoryButton(b15);
    b15=CreateButton(CtrlID15, &rect15, ButtonFontSize, ButtonStyle, Caption15, NULL);
    OSTimeDly(react);
    DrawButton(b15);
    break;
    case'd':DestoryButton(b16);
    b16=CreateButton(CtrlID16, &rect16, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b16);
    DestoryButton(b16);
    b16=CreateButton(CtrlID16, &rect16, ButtonFontSize, ButtonStyle, Caption16, NULL);
    OSTimeDly(react);
    DrawButton(b16);
    break;
    case'r':DestoryButton(b17);
    b17=CreateButton(CtrlID17, &rect17, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b17);
    DestoryButton(b17);
    b17=CreateButton(CtrlID17, &rect17, ButtonFontSize, ButtonStyle, Caption17, NULL);
    OSTimeDly(react);
    DrawButton(b17);
    break;
    case'c':DestoryButton(b18);
    b18=CreateButton(CtrlID18, &rect18, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b18);
    DestoryButton(b18);
    b18=CreateButton(CtrlID18, &rect18, ButtonFontSize, ButtonStyle, Caption18, NULL);
    OSTimeDly(react);
    DrawButton(b18);
    break;
    case'=':DestoryButton(b19);
    b19=CreateButton(CtrlID19, &rect19, ButtonFontSize, ButtonStyle, Caption_Actived, NULL);
    DrawButton(b19);
    DestoryButton(b19);
    b19=CreateButton(CtrlID19, &rect19, ButtonFontSize, ButtonStyle, Caption19, NULL);
    OSTimeDly(react);
    DrawButton(b19);
    break;


  default:break;
  }
  DestoryDC(pdc);//删除绘图设备上下文	
}
char zb2key(int x,int y)
{
  char key;
  if((160<x)&&(x<=240)&&(165<=y)&&(y<=240))      key='1';
  else if((240<x)&&(x<=320)&&(165<=y)&&(y<=240)) key='2';
  else if((320<x)&&(x<=400)&&(165<=y)&&(y<=240)) key='3';
  else if((400<x)&&(x<=475)&&(165<=y)&&(y<=240)) key='+';

  else if((160<x)&&(x<=240)&&(240<y)&&(y<=320)) key='4';
  else if((240<x)&&(x<=320)&&(240<y)&&(y<=320)) key='5';
  else if((320<x)&&(x<=400)&&(240<y)&&(y<=320)) key='6';
  else if((400<x)&&(x<=475)&&(240<y)&&(y<=320)) key='-';

  else if((160<x)&&(x<=240)&&(320<y)&&(y<=400)) key='7';
  else if((240<x)&&(x<=320)&&(320<y)&&(y<=400)) key='8';
  else if((320<x)&&(x<=400)&&(320<y)&&(y<=400)) key='9';
  else if((400<x)&&(x<=475)&&(320<y)&&(y<=400)) key='*';

  else if((160<x)&&(x<=240)&&(400<y)&&(y<=480)) key='(';
  else if((240<x)&&(x<=320)&&(400<y)&&(y<=480)) key='0';
  else if((320<x)&&(x<=400)&&(400<y)&&(y<=480)) key=')';
  else if((400<x)&&(x<=475)&&(400<y)&&(y<=480)) key='/';

  else if((5<=x)&&(x<=160)&&(165<y)&&(y<=320))  key='d';
  else if((5<=x)&&(x<=160)&&(320<y)&&(y<=480))  key='r';
  else if((485<=x)&&(x<=635)&&(165<y)&&(y<=320))key='c';
  else if((485<=x)&&(x<=635)&&(320<y)&&(y<=480))key='=';
  else                                key=' ';
  return key;

}
void dcr(char key)
{
  char non[100],tem1[maxs];
  int n,del;
  for(n=0;n<100;n++)
  {
    non[n]=' ';
  }
  switch(key)
  {
  case 'd':
    clc();
    cal[number-1] = '\0';
    cal[number] = '\0';
    cal[number+1] = '\0';
    cal[number+2] = '\0';
    number=number-1;
    lcddisplay(cal+1,xz,yz);
    break;
  case 'c':clc();
  break;
  case 'r':
    readfile();
    break;
  default:break;
  }
}
void readfile()
{
  clear=2;
  pfile=fopen(TextFilename,"r");//打开文件
  if(pfile==NULL){//文件打开失败
    LCD_printf("Can't Open file!\n");
    for(;;) OSTimeDly(1000);
  }
  while(readline(read, sizeof(read), pfile)){//以行的方式读文件
    int ii;
    number=sizeof(read);
    for(ii=0; ii<=number; ii++){
      cal[ii+1]=read[ii];
    }

    printf("read=%s\n",read);
    printf("cal=%s\n",cal+1);

    lcddisplay(cal+1,xz,yz);
    str1[number]=cal[number];
    //trans();
    //compvalue();
    //clear=1;
    int ret;
    for(ret=1;ret<=number;ret++)
    {
      cal[ret]='\0';
      if(ret==number){number=1;ret=2;}
    }

  }
  fclose(pfile);//关闭文件

  //for(;;) OSTimeDly(1000);

}
void clc()
{
  char non[100];
  int n,ret;
  for(n=0;n<100;n++)
  {
    non[n]=' ';
  }
  lcddisplay(non,xz,yz);
  lcddisplay(non,xz,yz+20);
}

char *doub2str(double d, char *str)
{
  char str2[40];
  double f=-1.0;
  int j=0,k,i,st,tem,temz,tem2;
  if(d==0.0){str[0]='0';str[1]='\0';}
  else
  {
    if(d<0.0){printf("d=%f\t\n",d);tem2=d*f;d=tem2;st=1;str[0]='-';}
    else st=0;
    temz=(int)d;
    tem=d;
    i=(int)d; //浮点数的整数部分
    while(i!=0)
    {
      str2[j++]=i%10+'0';
      i /= 10;
    }
    for(k=0;k<j;k++) str[k+st] = str2[j-1-k]; 	
    if(st==1){j=j+st;st=0;}
    str[j++]='.';
    d -= (int)d;
    for(i=0;i<10;i++)
    {
      d *= 10;
      str[j++] = (int)d+'0';
      d -= (int)d;
    }
    while(str[--j]=='0');
    str[++j] = '\0';
  }
  return str;
}

int readline(char* str, int n, FILE *pfile)
{
  int i;
  for(i=0; i<=n; i++){
    if(fread(str, 1, 1, pfile)!=1)
      return 0;

    if(*str=='\n'){
      str++;
      *str=0;
      return i;
    }
    str++;
  }

  return i;
}