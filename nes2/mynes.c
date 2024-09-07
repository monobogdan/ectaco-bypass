#include "nes_datatype.h"
#include <stdio.h>
#include <stdlib.h>

/**
  ******************************************************************************
  * �汾: 240*240 ʹ��SKYSDK�汾:version:AF02.04.07(�ఴ������)  
  * FC-MTK-MRP-240*320-96K 
  * @version 002
  * @date 2012-2-4   
  * @b��ֲ��Ա: ���С�� 
  ******************************************************************************
  * @˵��:Դ����Դ������,��������,����Դ������Դ,Ŀǰ������ϵ��ٶȺ�����ֻ��ģ��40K
  * ���µ�ROM,����·���ֹ�ͬ�Ż�,����֮.
  * 
  ******************************************************************************
  */  
/* SKYSDK version:AF02.04.07 */

extern int nes_main(void);
extern void startopcodetable(void);
extern int32 nesloop;

int32 KEYCODE[8];//����ֵ
int8  KEY[8];   //�����߼� 1:���� ����:û��
uint8 index;
int32 ROM_len;
unsigned char *rom_file;

void drawTxt(char* txt)
{


}
void  keyinit(void)
{   
#ifdef SIM_MOD
    index=8;
#else
    index=0;
#endif
    memset(KEYCODE,0,sizeof(KEYCODE));
    memset(KEY,0,sizeof(KEY));
}
void Start()
{
    drawTxt("������...");
    nes_main();
}
int32 mrc_init(void)
{   
	FILE* f;
	long len;
	unsigned char* ptr;
	
	keyinit();
	ROM_len=0;
    nesloop=0;
	rom_file=NULL;

 f = fopen("\\rom.nes", "rb");
	if(!f)
	{
		return 0;
	}

	fseek(f, 0, SEEK_END);
	 len = ftell(f);
	fseek(f, 0, SEEK_SET);
	ptr = (unsigned char*)malloc(len);
	fread(ptr, 1, len, f);
	ROM_len = len;
	fclose(f);

	rom_file = ptr;
	
	if(rom_file!=NULL&&ROM_len>1024&&ROM_len<=43*1024)
	{
	  startopcodetable();
	   Start();
	}
	else
	{
	drawTxt("�ڴ治��...");
	}
	return 1;
}
int32 mrc_pause(void)
{
	return 0;
}
int32 mrc_resume(void)
{
	return 0;
}
int32 mrc_exitApp(void)
{	
	return 0;
}
#ifdef SIM_MOD
void keyscan(void)
{


}
int32 mrc_event(int32 code, int32 p0, int32 p1)
{
  if(MR_KEY_PRESS == code)
	{
		switch(p0)
		{
		case MR_KEY_SOFTRIGHT:
			 mrc_exit();
			 break;
		case MR_KEY_SELECT:
			 KEY[4]=1;
			 break;
        case MR_KEY_SOFTLEFT:  
			 KEY[5]=1;
			 break;
		case MR_KEY_LEFT:
			 KEY[1]=1;
			 break;
		case MR_KEY_RIGHT:
			 KEY[0]=1;
			 break;
		case MR_KEY_UP:
             KEY[3]=1;
			 break;
        case MR_KEY_DOWN:
			 KEY[2]=1;
			 break;
		case MR_KEY_1:
			 KEY[6]=1;
			 break;
		case MR_KEY_2:
			 break;
		case MR_KEY_3:
		     KEY[7]=1;
			 break;
		}
	}
	else 
	{
      keyinit();
	}
	
return MR_SUCCESS;

}
#else
int32 mrc_extRecvAppEvent(int32 app, int32 code, int32 param0, int32 param1)
{
return 0;
}
int32 mrc_extRecvAppEventEx(int32 code, int32 p0, int32 p1, int32 p2, int32 p3, int32 p4,int32 p5)
{
return 0;
}
void keyscan(void)
    {  

     }
void SetKeyCode(uint8 k)
{   


}
void NEXT(char* txt)
{
     SetKeyCode(index);
     drawTxt(txt);
}
int32 mrc_event(int32 code, int32 p0, int32 p1)
     {   
		 return 0;
     }
#endif
