#include "ws2812.h"
#include "main.h"
#include "ARGB.h"
//----------------------------------------------------------------------------
extern TIM_HandleTypeDef htim3;
//----------------------------------------------------------------------------
uint16_t BUF_DMA [ARRAY_LEN] = {0};
uint8_t rgb_temp[12][3];
uint16_t DMA_BUF_TEMP[24];
//------------------------------------------------------------------
void ws2812_init(void)
{
  int i;
  for(i=DELAY_LEN;i<ARRAY_LEN;i++) BUF_DMA[i] = LOW;
}
//------------------------------------------------------------------
void ws2812_pixel_rgb_to_buf_dma(uint8_t Rpixel , uint8_t Gpixel, uint8_t Bpixel, uint16_t posX)
{
  volatile uint16_t i;
  for(i=0;i<8;i++)
  {
    if (BitIsSet(Rpixel,(7-i)) == 1)
    {
      BUF_DMA[DELAY_LEN+posX*24+i+8] = HIGH;
    }else
    {
      BUF_DMA[DELAY_LEN+posX*24+i+8] = LOW;
    }
    if (BitIsSet(Gpixel,(7-i)) == 1)
    {
      BUF_DMA[DELAY_LEN+posX*24+i+0] = HIGH;
    }else
    {
      BUF_DMA[DELAY_LEN+posX*24+i+0] = LOW;
    }
    if (BitIsSet(Bpixel,(7-i)) == 1)
    {
      BUF_DMA[DELAY_LEN+posX*24+i+16] = HIGH;
    }else
    {
      BUF_DMA[DELAY_LEN+posX*24+i+16] = LOW;
    }
  }
}
//------------------------------------------------------------------
void ws2812_prepareValue (uint8_t r00, uint8_t g00, uint8_t b00,
    uint8_t r01, uint8_t g01, uint8_t b01,
    uint8_t r02, uint8_t g02, uint8_t b02,
    uint8_t r03, uint8_t g03, uint8_t b03,
    uint8_t r04, uint8_t g04, uint8_t b04,
    uint8_t r05, uint8_t g05, uint8_t b05,
    uint8_t r06, uint8_t g06, uint8_t b06,
    uint8_t r07, uint8_t g07, uint8_t b07,
    uint8_t r08, uint8_t g08, uint8_t b08,
    uint8_t r09, uint8_t g09, uint8_t b09,
    uint8_t r10, uint8_t g10, uint8_t b10,
    uint8_t r11, uint8_t g11, uint8_t b11)
  {
  rgb_temp[0][0]=r00; rgb_temp[0][1]=g00; rgb_temp[0][2]=b00;
  rgb_temp[1][0]=r01; rgb_temp[1][1]=g01; rgb_temp[1][2]=b01;
  rgb_temp[2][0]=r02; rgb_temp[2][1]=g02; rgb_temp[2][2]=b02;
  rgb_temp[3][0]=r03; rgb_temp[3][1]=g03; rgb_temp[3][2]=b03;
  rgb_temp[4][0]=r04; rgb_temp[4][1]=g04; rgb_temp[4][2]=b04;
  rgb_temp[5][0]=r05; rgb_temp[5][1]=g05; rgb_temp[5][2]=b05;
  rgb_temp[6][0]=r06; rgb_temp[6][1]=g06; rgb_temp[6][2]=b06;
  rgb_temp[7][0]=r07; rgb_temp[7][1]=g07; rgb_temp[7][2]=b07;
  rgb_temp[8][0]=r08; rgb_temp[8][1]=g08; rgb_temp[8][2]=b08;
  rgb_temp[9][0]=r09; rgb_temp[9][1]=g09; rgb_temp[9][2]=b09;
  rgb_temp[10][0]=r10;rgb_temp[10][1]=g10;rgb_temp[10][2]=b10;
  rgb_temp[11][0]=r11;rgb_temp[11][1]=g11;rgb_temp[11][2]=b11;
}
//------------------------------------------------------------------
void ws2812_setValue(void)
{
  uint8_t n=0;
  for(n=0;n<12;n++)
  {
	  ARGB_SetRGB(n*12, rgb_temp[0][0], rgb_temp[0][1], rgb_temp[0][2]);
	  ARGB_SetRGB(n*12+1,rgb_temp[1][0], rgb_temp[1][1], rgb_temp[1][2]);
	  ARGB_SetRGB(n*12+2, rgb_temp[2][0], rgb_temp[2][1], rgb_temp[2][2]);
	  ARGB_SetRGB(n*12+3, rgb_temp[3][0], rgb_temp[3][1], rgb_temp[3][2]);
	  ARGB_SetRGB(n*12+4, rgb_temp[4][0], rgb_temp[4][1], rgb_temp[4][2]);
	  ARGB_SetRGB(n*12+5, rgb_temp[5][0], rgb_temp[5][1], rgb_temp[5][2]);
	  ARGB_SetRGB(n*12+6, rgb_temp[6][0], rgb_temp[6][1], rgb_temp[6][2]);
	  ARGB_SetRGB(n*12+7, rgb_temp[7][0], rgb_temp[7][1], rgb_temp[7][2]);
	  ARGB_SetRGB(n*12+8, rgb_temp[8][0], rgb_temp[8][1], rgb_temp[8][2]);
	  ARGB_SetRGB(n*12+9, rgb_temp[9][0], rgb_temp[9][1], rgb_temp[9][2]);
	  ARGB_SetRGB(n*12+10,rgb_temp[10][0],rgb_temp[10][1],rgb_temp[10][2]);
	  ARGB_SetRGB(n*12+11,rgb_temp[11][0],rgb_temp[11][1],rgb_temp[11][2]);
  }
}
//------------------------------------------------------------------
void ws2812_light(void)
{
  HAL_TIM_PWM_Start_DMA(&htim3,TIM_CHANNEL_1,(uint32_t*)&BUF_DMA,ARRAY_LEN);
}
//------------------------------------------------------------------
void ws2812_test01(void)
{
  uint8_t i,j;
  ws2812_prepareValue(128, 0, 0, 0, 128, 0, 0, 0, 128, 64, 64, 0,
      0, 64, 64, 64, 0, 64, 96, 32, 0, 96, 0, 32,
      32, 96, 0, 0, 96, 32, 0, 32, 96, 32, 0, 96);
  ws2812_setValue();
  for(j=0;j<50;j++)
  {
    memcpy((void*)DMA_BUF_TEMP,(void*)(BUF_DMA+48),48);
    for(i=0;i<143;i++)
    {
      memcpy((void*)(i*24+BUF_DMA+48),(void*)(i*24+BUF_DMA+72),48);
    }
    memcpy((void*)(BUF_DMA+48+3432),(void*)DMA_BUF_TEMP,48);
    ws2812_light();
    HAL_Delay(100);
  }
}
//----------------------------------------------------------------------------
void ws2812_test02(void)
{
  uint8_t i,j,jj;
  int k=0;
  for(jj=0;jj<10;jj++)
  {
    for(j=0;j<32;j++)
    {
      for(i=0;i<59;i++)
      {
        k=1000*(32-j)/32;
        ARGB_SetRGB(i, rgb_temp[i%12][0]*k/1000,rgb_temp[i%12][1]*k/1000,rgb_temp[i%12][2]*k/1000);
      }
      while (!ARGB_Show());
      HAL_Delay(10);
    }
    for(j=0;j<32;j++)
    {
      for(i=0;i<59;i++)
      {
        k=1000*(j+1)/32;
        ARGB_SetRGB(i,rgb_temp[i%12][0]*k/1000,rgb_temp[i%12][1]*k/1000,rgb_temp[i%12][2]*k/1000);
      }
      while (!ARGB_Show());
      HAL_Delay(10);
    }
  }
}
//-----------------------------------------------------------------------------
void ws2812_test03(void)
{
  uint8_t i,j,jj;
  int k=0;
  for(jj=0;jj<10;jj++)
  {
    for(j=0;j<32;j++)
    {
      for(i=0;i<59;i++)
      {
        if((i<12)||((i>=24)&&(i<36))||((i>=48)&&(i<60))||((i>=72)&&(i<84))||((i>=96)&&(i<108))||((i>=120)&&(i<132)))
        {
          k=1000*(32-j)/32;
          ARGB_SetRGB(i,rgb_temp[i%12][0]*k/1000,rgb_temp[i%12][1]*k/1000,rgb_temp[i%12][2]*k/1000);
        }
        else
        {
          k=1000*(j+1)/32;
          ARGB_SetRGB(i, rgb_temp[i%12][0]*k/1000,rgb_temp[i%12][1]*k/1000,rgb_temp[i%12][2]*k/1000);
        }
      }
      while (!ARGB_Show());
      HAL_Delay(15);
    }
    for(j=0;j<32;j++)
    {
      for(i=0;i<59;i++)
      {
        if((i<12)||((i>=24)&&(i<36))||((i>=48)&&(i<60))||((i>=72)&&(i<84))||((i>=96)&&(i<108))||((i>=120)&&(i<132)))
        {
          k=1000*(j+1)/32;
          ARGB_SetRGB(i,rgb_temp[i%12][0]*k/1000,rgb_temp[i%12][1]*k/1000,rgb_temp[i%12][2]*k/1000);
        }
        else
        {
          k=1000*(32-j)/32;
          ARGB_SetRGB(i,rgb_temp[i%12][0]*k/1000,rgb_temp[i%12][1]*k/1000,rgb_temp[i%12][2]*k/1000);
        }
      }
      while (!ARGB_Show());
      HAL_Delay(15);
    }
  }
}
//----------------------------------------------------------------------------
void ws2812_test04(uint8_t col)
{
  uint8_t i,j,jj;
  for(j=0;j<12;j++)
  {
    for(i=0;i<12;i++)
    {
      switch(col)
      {
        case 1:
          if(i<6)
            {rgb_temp[j*12+i][0]=128/((i+1)*2); rgb_temp[j*12+i][1]=0; rgb_temp[j*12+i][2]=0;}
          else
            {rgb_temp[j*12+i][0]=128/((12-i)*2); rgb_temp[j*12+i][1]=0; rgb_temp[j*12+i][2]=0;}
          break;
        case 2:
          if(i<6)
            {rgb_temp[j*12+i][0]=0; rgb_temp[j*12+i][1]=128/((i+1)*2); rgb_temp[j*12+i][2]=0;}
          else
            {rgb_temp[j*12+i][0]=0; rgb_temp[j*12+i][1]=128/((12-i)*2); rgb_temp[j*12+i][2]=0;}
          break;
        case 3:
          if(i<6)
            {rgb_temp[j*12+i][0]=0; rgb_temp[j*12+i][1]=0; rgb_temp[j*12+i][2]=128/((i+1)*2);}
          else
            {rgb_temp[j*12+i][0]=0; rgb_temp[j*12+i][1]=0; rgb_temp[j*12+i][2]=128/((12-i)*2);}
          break;
        case 4:
          if(i<6)
            {rgb_temp[j*12+i][0]=64/((i+1)*2); rgb_temp[j*12+i][1]=64/((i+1)*2); rgb_temp[j*12+i][2]=0;}
          else
            {rgb_temp[j*12+i][0]=64/((12-i)*2); rgb_temp[j*12+i][1]=64/((12-i)*2); rgb_temp[j*12+i][2]=0;}
          break;
        case 5:
          if(i<6)
            {rgb_temp[j*12+i][0]=64/((i+1)*2); rgb_temp[j*12+i][1]=0; rgb_temp[j*12+i][2]=64/((i+1)*2);}
          else
            {rgb_temp[j*12+i][0]=64/((12-i)*2); rgb_temp[j*12+i][1]=0; rgb_temp[j*12+i][2]=64/((12-i)*2);}
          break;
        case 6:
          if(i<6)
            {rgb_temp[j*12+i][0]=0; rgb_temp[j*12+i][1]=64/((i+1)*2); rgb_temp[j*12+i][2]=64/((i+1)*2);}
          else
            {rgb_temp[j*12+i][0]=0; rgb_temp[j*12+i][1]=64/((12-i)*2); rgb_temp[j*12+i][2]=64/((12-i)*2);}
          break;
      }
    }

  }
  ws2812_setValue();
  while (!ARGB_Show());
  HAL_Delay(100);
}
//----------------------------------------------------------------------------
void ws2812_test05(void)
{
  uint8_t i,j,jj;
  int k=0;
  for(jj=0;jj<40;jj++)
  {
    for(j=0;j<32;j++)
    {
      for(i=0;i<59;i++)
      {
        if(j<16) k=1000*(j)/16;
        else k=1000*(32-j)/16;
        if(i%12==0) ARGB_SetRGB(i,128*k/1000,128*(1000-k)/1000,0);
        if(i%12==1) ARGB_SetRGB(i,0,128*k/1000,128*(1000-k)/1000);
        if(i%12==2) ARGB_SetRGB(i,128*k/1000,0,128*(1000-k)/1000);
        if(i%12==3) ARGB_SetRGB(i,128*(1000-k)/1000,128*k/2000,128*k/2000);
        if(i%12==4) ARGB_SetRGB(i,128*k/2000,128*k/2000,128*(1000-k)/1000);
        if(i%12==5) ARGB_SetRGB(i,128*k/2000,128*(1000-k)/1000,128*k/2000);
        if(i%12==6) ARGB_SetRGB(i,128*(1000-k)/2000,128*(1000-k)/2000,128*k/1000);
        if(i%12==7) ARGB_SetRGB(i,128*k/1000,128*(1000-k)/2000,128*(1000-k)/2000);
        if(i%12==8) ARGB_SetRGB(i,128*(1000-k)/2000,128*k/1000,128*(1000-k)/2000);
        if(i%12==9) ARGB_SetRGB(i,128*(1000-k)/3000,128*(1000-k)/667,128*k/1000);
        if(i%12==10) ARGB_SetRGB(i,128*k/3000,128*(1000-k)/667,128*(1000-k)/1000);
        if(i%12==11) ARGB_SetRGB(i,128*(1000-k)/3000,128*k/667,128*(1000-k)/3000);
      }
      while (!ARGB_Show());
      HAL_Delay(20);
    }
  }
}
//----------------------------------------------------------------------------
