#include "nes_main.h"

uint8 FrameCnt;

void NEStimer(int32 data)
{
uint32	clocks;	//CPUִ��ʱ��

FrameCnt++;		   //֡������


SpriteHitFlag = FALSE;	

for(PPU_scanline=0; PPU_scanline<20; PPU_scanline++)
{ 
   exec6502(CLOCKS_PER_SCANLINE);
}
exec6502(CLOCKS_PER_SCANLINE);



PPU_scanline++;	  //20++
PPU_Reg.R2 &= ~R2_SPR0_HIT;

for(PPU_scanline=21; PPU_scanline < SCAN_LINE_DISPALY_END_NUM; PPU_scanline++)
{
	if((SpriteHitFlag == TRUE) && ((PPU_Reg.R2 & R2_SPR0_HIT) == 0))
	{
	clocks = sprite[0].x * CLOCKS_PER_SCANLINE / NES_DISP_WIDTH;
	exec6502(clocks);//���ص��Ż�
	PPU_Reg.R2 |= R2_SPR0_HIT;
	exec6502(CLOCKS_PER_SCANLINE - clocks);
	}
	else
	{
	exec6502(CLOCKS_PER_SCANLINE);//��ʱ��
	}
   if(PPU_Reg.R1 & (R1_BG_VISIBLE | R1_SPR_VISIBLE))
    {					//��Ϊ�٣��ر���ʾ
         if(SpriteHitFlag == FALSE)
       {
          NES_GetSpr0HitFlag(PPU_scanline - SCAN_LINE_DISPALY_START_NUM);	
       }
   }

   //if(FrameCnt % 3 == 1)
	NES_RenderLine(PPU_scanline - SCAN_LINE_DISPALY_START_NUM);	


}
exec6502(CLOCKS_PER_SCANLINE);
PPU_Reg.R2 |= R2_VBlank_Flag;	//����VBANK ��־
 if(PPU_Reg.R0 & R0_VB_NMI_EN)
 {
   NMI_Flag = SET1;	//���һ֡ɨ�裬����NMI�ж�	
  }

}

uint16 * w_getScreenBuffer(void);
uint16 * LCDBUF;
int32 nesloop;

void NesFrameCycle(void)
{
	FrameCnt = 0;
	LCDBUF = (uint16*)malloc(240 * 320 * sizeof(uint16));
}
void nes_main(void)
{	
	NesHeader *neshreader = (NesHeader *) rom_file;
	NES_JoyPadInit();
	init6502mem(0,0,(&rom_file[0x10]),neshreader->romnum);  
	reset6502();
	PPU_Init((&rom_file[0x10] + (neshreader->romnum * 0x4000)), (neshreader->romfeature & 0x01));	/*PPU_��ʼ��*/	
	NesFrameCycle();
}
