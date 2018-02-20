/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.c
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "../GLCD/GLCD.h"
#include "../TIMER/timer.h"
#include "TouchPanel.h"
#include <string.h>
#include <stdio.h>
#include "../DAC/dac.h"
#include "../GAMES/games.h"
extern uint8_t text_mode;
/* Private variables ---------------------------------------------------------*/
Matrix matrix ;
Coordinate  display ;
uint16_t Capt_flag;
uint16_t Calibration_vars[9];
Window_size actual_window;
uint16_t Detection_color;
Coordinate * Ptr;
Coordinate * Ptr2;
/* DisplaySample LCD�����϶�Ӧ��ads7843����ADֵ �磺LCD ����45,45 Ӧ�õ�X Y����ADC�ֱ�Ϊ3388,920 */	
Coordinate ScreenSample[3];
/* LCD�ϵ����� */
Coordinate DisplaySample[3] =   {
                                            { 45, 45 },
											{ 270,45},
                                            { 190,190}
	                            } ;

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 2   /* ��ֵ���� */

/*******************************************************************************
* Function Name  : TP_Init
* Description    : ADS7843�˿ڳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_Init(void) 
{ 
	PINSEL_CFG_Type PinCfg;
    SSP_CFG_Type SSP_ConfigStruct;

	/*
	 * Initialize SPI pin connect
	 * P0.6  - TP_CS - used as GPIO
	 * P0.7  - TP_SCK
	 * P0.8  - TP_SDO
	 * P0.9  - TP_SDI
	 * P2.13 - TP_IRQ - used as GPIO
	 */
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 7;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 8;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 9;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Funcnum = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 6;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Funcnum = 0;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 13;
	PINSEL_ConfigPin(&PinCfg);

    /* P2.13 TP_IRQ is Input */
    GPIO_SetDir(TP_IRQ_PORT_NUM, (1<<TP_IRQ_PIN_NUM), 0);
	GPIO_SetValue(TP_IRQ_PORT_NUM, (1<<TP_IRQ_PIN_NUM));  
    /* P0.6 TP_CS is output */
    GPIO_SetDir(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM), 1);
	GPIO_SetValue(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM)); 

	/* initialize SSP configuration structure to default */
	SSP_ConfigStructInit(&SSP_ConfigStruct);
	SSP_ConfigStruct.ClockRate = 500000;
	/* Initialize SSP peripheral with parameter given in structure above */
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);

	/* Enable SSP peripheral */
	SSP_Cmd(LPC_SSP1, ENABLE);
} 

/*******************************************************************************
* Function Name  : DelayUS
* Description    : ��ʱ1us
* Input          : - cnt: ��ʱֵ
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void DelayUS(uint32_t cnt)
{
  uint32_t i;
  for(i = 0;i<cnt;i++)
  {
     uint8_t us = 12; /* ����ֵΪ12����Լ��1΢�� */    
     while (us--)     /* ��1΢��	*/
     {
       ;   
     }
  }
}

/*******************************************************************************
* Function Name  : WR_CMD
* Description    : �� ADS7843д����
* Input          : - cmd: ���������
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static uint8_t WR_CMD (uint8_t cmd)  
{ 
	/* wait for current SSP activity complete */
	while (SSP_GetStatus(LPC_SSP1, SSP_STAT_BUSY) ==  SET);

	SSP_SendData(LPC_SSP1, (uint16_t) cmd);

	while (SSP_GetStatus(LPC_SSP1, SSP_STAT_RXFIFO_NOTEMPTY) == RESET);

	return (SSP_ReceiveData(LPC_SSP1));
} 

/*******************************************************************************
* Function Name  : RD_AD
* Description    : ��ȡADCֵ
* Input          : None
* Output         : None
* Return         : ADS7843���ض��ֽ�����
* Attention		 : None
*******************************************************************************/
static int RD_AD(void)  
{ 
	unsigned short buf,temp; 
	
	temp = WR_CMD(0x00);
	buf = temp << 8; 
	DelayUS(1); 
	temp = WR_CMD(0x00);;
	buf |= temp; 
	buf >>= 3; 
	buf &= 0xfff; 
	return buf; 
} 

/*******************************************************************************
* Function Name  : Read_X
* Description    : ��ȡADS7843ͨ��X+��ADCֵ 
* Input          : None
* Output         : None
* Return         : ADS7843����ͨ��X+��ADCֵ
* Attention		 : None
*******************************************************************************/
int Read_X(void)  
{  
	int i; 
	TP_CS(0); 
	DelayUS(1); 
	WR_CMD(CHX); 
	DelayUS(1); 
	i = RD_AD(); 
	TP_CS(1); 
	return i;    
} 

/*******************************************************************************
* Function Name  : Read_Y
* Description    : ��ȡADS7843ͨ��Y+��ADCֵ
* Input          : None
* Output         : None
* Return         : ADS7843����ͨ��Y+��ADCֵ
* Attention		 : None
*******************************************************************************/
int Read_Y(void)  
{  
	int i; 
	TP_CS(0); 
	DelayUS(1); 
	WR_CMD(CHY); 
	DelayUS(1); 
	i = RD_AD(); 
	TP_CS(1); 
	return i;     
} 

/*******************************************************************************
* Function Name  : TP_GetAdXY
* Description    : ��ȡADS7843 ͨ��X+ ͨ��Y+��ADCֵ
* Input          : None
* Output         : None
* Return         : ADS7843���� ͨ��X+ ͨ��Y+��ADCֵ 
* Attention		 : None
*******************************************************************************/
void TP_GetAdXY(int *x,int *y)  
{ 
	int adx,ady; 
	adx = Read_X(); 
	DelayUS(1); 
	ady = Read_Y(); 
	*x = adx; 
	*y = ady; 
} 

/*******************************************************************************
* Function Name  : TP_DrawPoint
* Description    : ��ָ�����껭��
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos)
{
	LCD_SetPoint(Xpos,Ypos,0xf800);     /* ���ĵ� */
	LCD_SetPoint(Xpos+1,Ypos,0xf800);
	LCD_SetPoint(Xpos,Ypos+1,0xf800);
	LCD_SetPoint(Xpos+1,Ypos+1,0xf800);	
}	

/*******************************************************************************
* Function Name  : DrawCross
* Description    : ��ָ�����껭ʮ��׼��
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DrawCross(uint16_t Xpos,uint16_t Ypos,uint16_t color)
{
	LCD_DrawLine(Xpos-15,Ypos,Xpos-2,Ypos,color);
	LCD_DrawLine(Xpos+2,Ypos,Xpos+15,Ypos,color);
	LCD_DrawLine(Xpos,Ypos-15,Xpos,Ypos-2,color);
	LCD_DrawLine(Xpos,Ypos+2,Xpos,Ypos+15,color);
	
	LCD_DrawLine(Xpos-15,Ypos+15,Xpos-7,Ypos+15,color);
	LCD_DrawLine(Xpos-15,Ypos+7,Xpos-15,Ypos+15,color);
	
	LCD_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15,color);
	LCD_DrawLine(Xpos-15,Ypos-7,Xpos-15,Ypos-15,color);
	
	LCD_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15,color);
	LCD_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15,color);
	
	LCD_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15,color);
	LCD_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7,color);	 
// Creamos rectangulo de deteccion asociado:
		menu_rect[num_bars_menu + num_botton_menu].x = Xpos-15;
		menu_rect[num_bars_menu + num_botton_menu].y = Ypos-15;
		menu_rect[num_bars_menu + num_botton_menu].w = 30;
		menu_rect[num_bars_menu + num_botton_menu].h = 30;
	    num_botton_menu++; 	
}	
/*******************************************************************************
* Function Name  : DrawArrow
* Description    : ��ָ�����껭ʮ��׼��
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
*                  - color: Color of the arrow 
*                  - apunta: Direction of the arrow
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DrawArrow(uint16_t Xpos,uint16_t Ypos,uint16_t color,uint16_t apunta)
{
	switch(apunta)
	{
		case 1:
		{
			LCD_DrawLine(Xpos,Ypos-15,Xpos,Ypos+15,color);
			LCD_DrawLine(Xpos-15,Ypos,Xpos,Ypos-15,color);
			LCD_DrawLine(Xpos,Ypos-15,Xpos+15,Ypos,color);
			break;
		}
		case 2:
		{
			LCD_DrawLine(Xpos,Ypos-15,Xpos,Ypos+15,color);
			LCD_DrawLine(Xpos-15,Ypos,Xpos,Ypos+15,color);
			LCD_DrawLine(Xpos+15,Ypos,Xpos,Ypos+15,color);
			break;
		}
		case 3:
		{
			LCD_DrawLine(Xpos-15,Ypos,Xpos+15,Ypos,color);
			LCD_DrawLine(Xpos,Ypos+15,Xpos+15,Ypos,color);
			LCD_DrawLine(Xpos,Ypos-15,Xpos+15,Ypos,color);
			break;
		}
		case 4:
		{
			LCD_DrawLine(Xpos-15,Ypos,Xpos+15,Ypos,color);
			LCD_DrawLine(Xpos,Ypos+15,Xpos-15,Ypos,color);
			LCD_DrawLine(Xpos,Ypos-15,Xpos-15,Ypos,color);
			break;
		}
		default:break;
	}
// Creamos rectangulo de deteccion asociado:
		menu_rect[num_bars_menu + num_botton_menu].x = Xpos-15;
		menu_rect[num_bars_menu + num_botton_menu].y = Ypos-15;
		menu_rect[num_bars_menu + num_botton_menu].w = 30;
		menu_rect[num_bars_menu + num_botton_menu].h = 30;
	    num_botton_menu++;		
}	

/*******************************************************************************
* Function Name  : TextButton
* Description    : ��ָ�����껭ʮ��׼��
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
*                  - str: String to print
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TextButton(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t color) {
	uint8_t margin = 3;
	uint8_t length_x = strlen(str)*8 + 2*margin;
	uint8_t length_y = 16 + 2*margin;

    if (text_mode == 0) {

		
		LCD_DrawLine(Xpos,	Ypos,	Xpos + length_x,	Ypos,color);		  //					-----
		LCD_DrawLine(Xpos,	Ypos+length_y,	Xpos + length_x,	Ypos+length_y,color);//					-----
		LCD_DrawLine(Xpos,	Ypos,	Xpos,	Ypos + length_y,color);		  //  					|_
		LCD_DrawLine(Xpos + length_x ,	Ypos, Xpos + length_x ,	Ypos + length_y,color);//	    _|
		
		GUI_Text(Xpos + margin,Ypos + margin,str,color,Blue);


		// Creamos rectangulo de deteccion asociado:
		menu_rect[num_bars_menu + num_botton_menu].x = Xpos;
		menu_rect[num_bars_menu + num_botton_menu].y = Ypos;
		menu_rect[num_bars_menu + num_botton_menu].w = length_x;
		menu_rect[num_bars_menu + num_botton_menu].h = length_y;
	    num_botton_menu++;
   }
    if (text_mode == 1) {
		LCD_DrawLine(Xpos,	Ypos,	Xpos - length_y,	Ypos,color);		  //			   --
		LCD_DrawLine(Xpos,	Ypos + length_x,	Xpos - length_y,	Ypos+length_x,color);//	 	   --
		LCD_DrawLine(Xpos,	Ypos,	Xpos,	Ypos + length_x,color);		  //  					|_
		LCD_DrawLine(Xpos - length_y ,	Ypos, 	Xpos - length_y ,Ypos + length_x,color);//	    _|

		GUI_Text(Xpos - margin,Ypos + margin,str,color,Blue);

 		// Creamos rectangulo de deteccion asociado:
	   	menu_rect[num_bars_menu + num_botton_menu].x = Xpos - length_y;
		menu_rect[num_bars_menu + num_botton_menu].y = Ypos;
		menu_rect[num_bars_menu + num_botton_menu].w = length_y;
		menu_rect[num_bars_menu + num_botton_menu].h = length_x;
	    num_botton_menu++;


   }

}





/*******************************************************************************
* Function Name  : TextButton
* Description    : ��ָ�����껭ʮ��׼��
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
*                  - str: String to print
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Window(uint16_t Xmin,uint16_t Ymin, uint16_t Xmax, uint16_t Ymax)
{
 
	LCD_WriteReg(0x0050,Ymin);	  /* ymin */
	LCD_WriteReg(0x0051,Ymax);	  /* ymax */

	LCD_WriteReg(0x0052,(MAX_X -1) - Xmax);	  /* xmin */
	LCD_WriteReg(0x0053,(MAX_X -1) - Xmin);	  /* xmax */
}

/*******************************************************************************
* Function Name  : CalibrationBar
* Description    : ��ָ�����껭ʮ��׼��
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/



void DrawCalibrationBar(uint16_t xmin, uint16_t ymin, uint16_t len_x, uint16_t len_y,uint16_t situation)
{
	uint16_t value;

		LCD_DrawLine(xmin-1,		ymin-1,			xmin+len_x,		ymin-1,			Black);
		LCD_DrawLine(xmin-1,		ymin+len_y,		xmin+len_x,		ymin+len_y,		Black);
		LCD_DrawLine(xmin-1,		ymin+len_y,		xmin-1,			ymin-1,		Black);	
		LCD_DrawLine(xmin+len_x,	ymin-1,			xmin+len_x,		ymin+len_y,		Black);

 	if (situation == 0) {	
	 	value = (len_y*Calibration_vars[num_bars_menu])/BARS_RANGE;
			
	 	DrawRectangle(xmin, ymin, len_x, value, Yellow);
		DrawRectangle(xmin, ymin + value, len_x,len_y - value,0);

	}
 	if (situation == 1) {	
	 	value = (len_x*Calibration_vars[num_bars_menu])/BARS_RANGE;
			
	 	DrawRectangle(xmin, ymin, value, len_y,Yellow);
		DrawRectangle(xmin + value, ymin, len_x - value, len_y,0);												 
	}

	// Creamos rectangulo de deteccion asociado:
		menu_rect[num_bars_menu + num_botton_menu].x = xmin;
		menu_rect[num_bars_menu + num_botton_menu].y = ymin;
		menu_rect[num_bars_menu + num_botton_menu].w = len_x;
		menu_rect[num_bars_menu + num_botton_menu].h = len_y;
		type_bar |= situation << num_bars_menu;
	    num_bars_menu++;

	Window(0,0,319,239);
}

void UpdateBar(uint16_t num_var, Rectangle bar, uint8_t situation){
	uint16_t value;
  
 	if (situation == 0) {	
	 	value = (bar.h*Calibration_vars[num_var])/BARS_RANGE;
			
	 	DrawRectangle(bar.x, bar.y, bar.w, value, Yellow);
		DrawRectangle(bar.x, bar.y + value, bar.w,bar.h - value,0);

	}
 	if (situation == 1) {	
	 	value = (bar.w*Calibration_vars[num_var])/BARS_RANGE;
			
	 	DrawRectangle(bar.x, bar.y, value, bar.h,Yellow);
		DrawRectangle(bar.x + value, bar.y, bar.w - value, bar.h,0);												 
	}
}

void Get_CalibrationValue(uint16_t num_var, Rectangle bar,uint16_t situation){

			if(situation==0){
				Calibration_vars[num_var]=(abs(display.y-bar.y)*BARS_RANGE)/(bar.h);
			}
			if (situation == 1){
				Calibration_vars[num_var]=(abs(display.x-bar.x)*BARS_RANGE)/(bar.w);
			}	
}

void DrawRectangle(uint16_t xmin, uint16_t ymin, uint16_t len_x, uint16_t len_y,uint16_t color) {


		if (xmin + len_x > MAX_X -1) {
			xmin = MAX_X -(1 + len_x);
		}


	   	if (ymin + len_y > MAX_Y -1) {
			ymin = MAX_Y -(1 + len_y);
		}
		LCD_SetCursor(xmin,ymin);

		Window(xmin,ymin,xmin+len_x -1,ymin+len_y-1);
	 	window_clear(len_x,len_y,color);

	Window(0,0,319,239);
}
/*******************************************************************************
* Function Name  : Get_CalibrationValue
* Description    : ��ָ�����껭ʮ��׼��
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/


/*******************************************************************************
* Function Name  : GetColor
* Description    : ��ָ�����껭ʮ��׼��
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
uint16_t GetColor(void)
{
uint16_t colors[9],media_r,media_g,media_b,media,i,suma_r=0,suma_g=0,suma_b=0;
	colors[0]=LCD_GetPoint(display.x,display.y);
	colors[1]=LCD_GetPoint(display.x,display.y+1);
	colors[2]=LCD_GetPoint(display.x,display.y-1);
	colors[3]=LCD_GetPoint(display.x+1,display.y+1);
	colors[4]=LCD_GetPoint(display.x+1,display.y);
	colors[5]=LCD_GetPoint(display.x+1,display.y-1);
	colors[6]=LCD_GetPoint(display.x-1,display.y);
	colors[7]=LCD_GetPoint(display.x-1,display.y+1);
	colors[8]=LCD_GetPoint(display.x-1,display.y-1);
	
	//5-6-5 RGB
	
	for(i=0;i<9;i++)
	suma_r+=(colors[i] >> 11);
	media_r=(suma_r/9);
	
	for(i=0;i<9;i++)
	suma_g+=((colors[i]&0x07E0) >> 5);
	media_g=(suma_g/9);
	
	for(i=0;i<9;i++)
	suma_b+=(colors[i]&0x001F);
	media_b=(suma_b/9);
	
	media=(media_r << 11)+(media_g<<5)+(media_b);
	Capt_flag=0;
	return media;
	
}

/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : �õ��˲�֮���X Y
* Input          : None
* Output         : None
* Return         : Coordinate�ṹ���ַ
* Attention		 : None
*******************************************************************************/
Coordinate *Read_Ads7846(void)
{
  static Coordinate  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count=0;
  int buffer[2][9]={{0},{0}};  /* ����X��Y���ж�β��� */
  do					       /* ѭ������9�� */
  {		   
    TP_GetAdXY(TP_X,TP_Y);  
	buffer[0][count]=TP_X[0];  
	buffer[1][count]=TP_Y[0];
	count++;  
  }
  while(!TP_INT_IN&& count<9);  /* TP_INT_INΪ�������ж�����,���û����������ʱTP_INT_IN�ᱻ�õ� */
  if(count==9)   /* �ɹ�����9��,�����˲� */ 
  {  
    /* Ϊ����������,�ֱ��3��ȡƽ��ֵ */
    temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
	temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
	temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;
	/* ����3�����ݵĲ�ֵ */
	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];
	/* ��������ֵȡ����ֵ */
	m0=m0>0?m0:(-m0);
    m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);
	/* �жϾ��Բ�ֵ�Ƿ񶼳�����ֵ���ޣ������3�����Բ�ֵ����������ֵ�����ж���β�����ΪҰ��,���������㣬��ֵ����ȡΪ2 */
	if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;
	/* �������ǵ�ƽ��ֵ��ͬʱ��ֵ��screen */ 
	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.x=(temp[0]+temp[2])/2;
	  else 
	    screen.x=(temp[0]+temp[1])/2;	
	}
	else if(m2<m1) 
	  screen.x=(temp[0]+temp[2])/2;
	else 
	  screen.x=(temp[1]+temp[2])/2;

	/* ͬ�� ����Y��ƽ��ֵ */
    temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
	temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
	temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];
	m0=m0>0?m0:(-m0);
	m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);
	if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;

	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.y=(temp[0]+temp[2])/2;
	  else 
	    screen.y=(temp[0]+temp[1])/2;	
    }
	else if(m2<m1) 
	   screen.y=(temp[0]+temp[2])/2;
	else
	   screen.y=(temp[1]+temp[2])/2;

	return &screen;
  }  
  return 0; 
}
	 
/* �����Ǵ�������Һ��������任��ת������ */
/* ֻ����LCD�ʹ�����������Ƕȷǳ�Сʱ,�����������湫ʽ */

/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : ����� K A B C D E F
* Input          : None
* Output         : None
* Return         : ����1��ʾ�ɹ� 0ʧ��
* Attention		 : None
*******************************************************************************/
uint8_t setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{

  uint8_t retTHRESHOLD = 0 ;
  /* K��(X0��X2) (Y1��Y2)��(X1��X2) (Y0��Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = 1;
  }
  else
  {
    /* A��((XD0��XD2) (Y1��Y2)��(XD1��XD2) (Y0��Y2))��K	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* B��((X0��X2) (XD1��XD2)��(XD0��XD2) (X1��X2))��K	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C��(Y0(X2XD1��X1XD2)+Y1(X0XD2��X2XD0)+Y2(X1XD0��X0XD1))��K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D��((YD0��YD2) (Y1��Y2)��(YD1��YD2) (Y0��Y2))��K	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E��((X0��X2) (YD1��YD2)��(YD0��YD2) (X1��X2))��K	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F��(Y0(X2YD1��X1YD2)+Y1(X0YD2��X2YD0)+Y2(X1YD0��X0YD1))��K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  return( retTHRESHOLD ) ;
}

/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    : ͨ�� K A B C D E F ��ͨ��X Y��ֵת��ΪҺ��������
* Input          : None
* Output         : None
* Return         : ����1��ʾ�ɹ� 0ʧ��
* Attention		 : None
*******************************************************************************/
uint8_t getDisplayPoint(Coordinate * displayPtr,
                     Coordinate * screenPtr,
                     Matrix * matrixPtr )
{
  uint8_t retTHRESHOLD = 0 ;

  if( matrixPtr->Divider != 0 )
  {
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
	/* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
  }
  else
  {
    retTHRESHOLD = 1;
  }
  return(retTHRESHOLD);
} 

/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : У׼������
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
/*void TouchPanel_Calibrate(void)
{
	uint8_t i;
	Coordinate * Ptr;
	
	for(i=0;i<3;i++)
	{
		LCD_Clear(Black);
		GUI_Text(10,10,"Touch crosshair to calibrate",0xffff,Black);
		DelayUS( 500 * 1000 );
		DrawCross(DisplaySample[i].x,DisplaySample[i].y,White);
		do
		{
			Ptr=Read_Ads7846();
		}
		while( Ptr == (void*)0 );
		ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;
	}
	setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix ) ;   
	LCD_Clear(Black);
}
*/


void TouchPanel_Calibrate(void)
{
	uint8_t i;

		ScreenSample[0].x=3420;
		ScreenSample[0].y=897;
		ScreenSample[1].x=764;
		ScreenSample[1].y=852;
		ScreenSample[2].x=1695;
		ScreenSample[2].y=3177;

	setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix ) ;   

}


/*							/// PA SACAR LOS VALORES DE CALIBRACION
void TouchPanel_Calibrate(void)
{
	uint8_t i;
	Coordinate * Ptr;
  uint8_t screenx[10],screeny[10];
	LCD_Clear(Black);
	for(i=0;i<3;i++)
	{
		
		GUI_Text(10,10,"Touch crosshair to calibrate",0xffff,Black);
		DelayUS( 500 * 1000 );
		DrawCross(DisplaySample[i].x,DisplaySample[i].y,White);
		do
		{
			Ptr=Read_Ads7846();
		}
		while( Ptr == (void*)0 );

	ScreenSample[i].x= Ptr->x;	ScreenSample[i].y= Ptr->y;
		conversor_IaS(ScreenSample[i].x,screenx);
		GUI_Text(80,50*(i+1),screenx,White,Black);
		conversor_IaS(ScreenSample[i].y,screeny);
		GUI_Text(200,50*(i+1),screeny,White,Black);

	}

	setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix ) ;  
	while(1){}
	LCD_Clear(Black);
}
*/


uint8_t DetectRectangle(Rectangle r){
   		if ((display.x >= r.x )&&(display.x <= r.x + r.w)&&
		(display.y >= r.y)&&(display.y <= r.y + r.h)) {

			LPC_DAC->DACCNTVAL = 1134;					// Set emision frecuency
			DMA_DAC_audio_Channel_Init( 0, click_audio,1462);	  //Make sound bitcheeeessss !!!
			
				delay_only = 1;
				delay_us (100000);   

				while (delay_only == 1) {}
				

			return 1;
		}
		else {
			return 0;
		}
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

/* Simplemente dibuja botones y barras y ya el sistema se encarga de hacer la deteccion de pulsado */
