#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "hi_io.h"
#include "hi_gpio.h"
#include "hi_spi.h"

#include "st7789.h"
#include "font.h"
#include "hz_16x16.h"
#include "hz_32x32.h"



//SPI初始化
void spiInit(void)
{
	unsigned int ret;
    hi_spi_deinit(HI_SPI_ID_1); /* if wake_up from deep sleep, should deinit first */
	hi_spi_cfg_basic_info spi_cfg_basic_info = {
		.cpha = 0,//相位0
		.cpol = 0,//极性0
		.data_width = HI_SPI_CFG_DATA_WIDTH_E_8BIT,//位宽为8bit
		.endian = HI_SPI_CFG_ENDIAN_LITTLE,//小端传输
		.fram_mode = HI_SPI_CFG_FRAM_MODE_MOTOROLA,//摩托罗拉协议
		.freq = 7200000,//通信频率
	};
	
    hi_spi_cfg_init_param spi_init_param = {0};
    spi_init_param.is_slave = 0;
    ret = hi_spi_init(HI_SPI_ID_1, spi_init_param, &spi_cfg_basic_info); //基本参数配置
	printf("ret = %u\n",ret);
    hi_spi_set_loop_back_mode(HI_SPI_ID_1, 1);
    hi_io_set_func(HI_IO_NAME_GPIO_0, HI_IO_FUNC_GPIO_0_SPI1_CK);
    hi_io_set_func(HI_IO_NAME_GPIO_2, HI_IO_FUNC_GPIO_2_SPI1_TXD);
	hi_io_set_driver_strength(HI_IO_NAME_GPIO_10, HI_IO_DRIVER_STRENGTH_2);
    hi_spi_set_irq_mode(HI_SPI_ID_1, 0);
    hi_spi_set_dma_mode(HI_SPI_ID_1, 1);

	IoTGpioInit(LCD_PIN_RES);
	IoTGpioInit(LCD_PIN_DC);
	IoTGpioInit(LCD_PIN_CS);


	IoTGpioSetDir(LCD_PIN_RES, IOT_GPIO_DIR_OUT);
	IoTGpioSetDir(LCD_PIN_DC, IOT_GPIO_DIR_OUT);





}


//SPI初始化
void lcdGpioInit(void)
{
    IoTGpioInit(LCD_PIN_CS);
	IoTGpioInit(LCD_PIN_RES);
	IoTGpioInit(LCD_PIN_DC);
	IoTGpioInit(LCD_PIN_SDA);
	IoTGpioInit(LCD_PIN_SCL);


	// hi_io_set_func(LCD_PIN_CS,HI_IO_FUNC_GPIO_10_GPIO);
	// hi_io_set_func(LCD_PIN_RES,HI_IO_FUNC_GPIO_1_GPIO);
	// hi_io_set_func(LCD_PIN_DC,HI_IO_FUNC_GPIO_9_GPIO);
	// hi_io_set_func(LCD_PIN_SDA,HI_IO_FUNC_GPIO_2_GPIO);
	// hi_io_set_func(LCD_PIN_SCL,HI_IO_FUNC_GPIO_0_GPIO);


    IoTGpioSetDir(LCD_PIN_CS, IOT_GPIO_DIR_OUT);
	IoTGpioSetDir(LCD_PIN_RES, IOT_GPIO_DIR_OUT);
	IoTGpioSetDir(LCD_PIN_DC, IOT_GPIO_DIR_OUT);
	IoTGpioSetDir(LCD_PIN_SDA, IOT_GPIO_DIR_OUT);
	IoTGpioSetDir(LCD_PIN_SCL, IOT_GPIO_DIR_OUT);




}

//SPI单字节写入
void spiWrite(unsigned char data)
{
	unsigned int ret = 0;
	ret = hi_spi_host_write(HI_SPI_ID_1, &data, 1);
	// printf("ret = %u\n",ret);
}

//LCD初始化
void lcdInit(void)
{
    lcdGpioInit();


	CLR_LCD_RES; 
	usleep(100 * 1000);
	SET_LCD_RES; 
	usleep(100 * 1000);
    lcdSelectRegister(0x11); //Sleep out 
    usleep(120000);   
    

	lcdSelectRegister(0x36);
	lcdWriteDataU8 (0x00);
    
	lcdSelectRegister(0x3A);
	lcdWriteDataU8 (0x05);      //像素点的数据格式设置为 RGB：565
 
    //与屏的型号有关
	//lcdSelectRegister(0x20);  // 颜色值需按位取反
 
	lcdSelectRegister(0xB2);
	lcdWriteDataU8 (0x0C); 
	lcdWriteDataU8 (0x0C); 
	lcdWriteDataU8 (0x00); 
	lcdWriteDataU8 (0x33);
	lcdWriteDataU8 (0x33);
	 
	lcdSelectRegister(0xB7);
	lcdWriteDataU8 (0x35);
 
	lcdSelectRegister(0xBB);
	lcdWriteDataU8 (0x19);
	lcdSelectRegister(0xC0);
	lcdWriteDataU8 (0x2C);
	lcdSelectRegister(0xC2);
	lcdWriteDataU8 (0x01);
	lcdSelectRegister(0xC3);
	lcdWriteDataU8 (0x12);
	lcdSelectRegister(0xC4);
	lcdWriteDataU8 (0x20);
	lcdSelectRegister(0xC6);
	lcdWriteDataU8 (0x0F);
	lcdSelectRegister(0xd0);
	lcdWriteDataU8 (0xA4);
	lcdWriteDataU8 (0xA1);
 
	lcdSelectRegister(0xE0);    //Set Gamma
	lcdWriteDataU8 (0xD0);
	lcdWriteDataU8 (0x04);
	lcdWriteDataU8 (0x0D);
	lcdWriteDataU8 (0x11);
	lcdWriteDataU8 (0x13);
	lcdWriteDataU8 (0x2B);
	lcdWriteDataU8 (0x3F);
	lcdWriteDataU8 (0x54);
	lcdWriteDataU8 (0x4C);
	lcdWriteDataU8 (0x18);
	lcdWriteDataU8 (0x0D);
	lcdWriteDataU8 (0x0B);
	lcdWriteDataU8 (0x1F);
	lcdWriteDataU8 (0x23);
	
	lcdSelectRegister(0XE1);    //Set Gamma 
	lcdWriteDataU8 (0xD0);
	lcdWriteDataU8 (0x04);
	lcdWriteDataU8 (0x0C);
	lcdWriteDataU8 (0x11);
	lcdWriteDataU8 (0x13);
	lcdWriteDataU8 (0x2C);
	lcdWriteDataU8 (0x3F);
	lcdWriteDataU8 (0x44);
	lcdWriteDataU8 (0x51);
	lcdWriteDataU8 (0x2F);
	lcdWriteDataU8 (0x1F);
	lcdWriteDataU8 (0x1F);
	lcdWriteDataU8 (0x20);
	lcdWriteDataU8 (0x23);
    
	lcdSelectRegister(0x21);

	lcdSelectRegister(0x29);

}

void lcdwritebus(unsigned char data)
{
	uint8_t i;
	CLR_LCD_CS;
	for(i=0;i<8;i++)
	{			  
		CLR_LCD_SCL;
		if(data&0x80)
		{
		   SET_LCD_SDA;
		}
		else
		{
		   CLR_LCD_SDA;
		}
		SET_LCD_SCL;
		data<<=1;
	}	
  SET_LCD_CS;
} 

//写入命令
void lcdSelectRegister(unsigned char data)
{
    CLR_LCD_DC;
	lcdwritebus(data);
	SET_LCD_DC;
}

//写入一字节数据
void lcdWriteDataU8(unsigned char data)
{
	lcdwritebus(data);
}

//写入两字节数据
void lcdWriteDataU16(unsigned short data)
{
    lcdWriteDataU8(data>>8);
    lcdWriteDataU8(data);
}

// //写入命令
// void lcdSelectRegister(unsigned char data)
// {
//     CLR_LCD_DC;
//     spiWrite(data);
// }

// //写入一字节数据
// void lcdWriteDataU8(unsigned char data)
// {
//     SET_LCD_DC;
//     spiWrite(data);
// }

// //写入两字节数据
// void lcdWriteDataU16(unsigned short data)
// {
//     lcdWriteDataU8(data>>8);
//     lcdWriteDataU8(data);
// }

//设置光标位置
void lcdSetAddress(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{	     
    lcdSelectRegister(0x2A); 
    lcdWriteDataU16((x1 + 40)); 
	lcdWriteDataU16((x2 + 40)); 

    lcdSelectRegister(0x2B); 
    lcdWriteDataU16((y1 + 52)); 
    lcdWriteDataU16((y2 + 52)); 
    
    lcdSelectRegister(0x2C);
}

//清屏
void lcdClear(unsigned short color)
{
    lcdSetAddress(0, 0, 130, 162);
    
    for(unsigned char i = 0; i < 130; i++)
    {
        for (unsigned char j = 0; j < 162; j++)
	   	{
            lcdWriteDataU16(color);
	    }
    }
} 

//LCD画点
void LCD_DrawPoint(unsigned short x, unsigned short y, unsigned short color)
{
	lcdSetAddress(x, y, x, y);//设置光标位置
	lcdWriteDataU16(color);
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(unsigned short x, unsigned short y, unsigned short color)
{
	lcdWriteDataU16(color);
}	

//LCD开启显示
void LCD_DisplayOn(void)
{					   
	lcdSelectRegister(0x29);	//开启显示
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	lcdSelectRegister(0x28);	//关闭显示
}   

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(unsigned short sx, unsigned short sy, unsigned short ex, unsigned short ey, unsigned short color)
{      

	unsigned short i,j; 
	lcdSetAddress(sx,sy,ex-1,ey-1);//设置显示范围
	for(i=sy;i<ey;i++)
	{													   	 	
		for(j=sx;j<ex;j++)
		{
			lcdWriteDataU16(color);
		}
	} 	    

}  

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color)
{
	unsigned short t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1; //计算坐标增量 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	
	//设置单步方向 
	if( delta_x > 0 )
	{
		incx=1; 
	}
	else if( delta_x == 0 )//垂直线 
	{
		incx=0;
	}
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}

	
	if( delta_y > 0 )
	{
		incy=1; 
	}
	else if( delta_y == 0 )//水平线 
	{
		incy=0;
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 
	
	if( delta_x > delta_y )//选取基本增量坐标轴 
	{
		distance = delta_x; 
	}
	else
	{
		distance=delta_y; 
	}
	
	for(t = 0; t <= distance + 1; t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow, uCol, color);//画点 
		xerr += delta_x ; 
		yerr += delta_y ; 
		if( xerr > distance ) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		
		if( yerr > distance ) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}  
}  

void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color)
{
	LCD_DrawLine(x1, y1, x2, y1, color);
	LCD_DrawLine(x1, y1, x1, y2, color);
	LCD_DrawLine(x1, y2, x2, y2, color);
	LCD_DrawLine(x2, y1, x2, y2, color);
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(unsigned short x0, unsigned short y0, unsigned char r, unsigned short color)
{
	int a, b;
	int di;
	a = 0;
	b = r;	  
	di = 3 - ( r<<1 );             //判断下个点位置的标志
	while( a <= b )
	{
		LCD_DrawPoint(x0+a, y0-b, color);
 		LCD_DrawPoint(x0+b, y0-a, color);        
		LCD_DrawPoint(x0+b, y0+a, color);       
		LCD_DrawPoint(x0+a, y0+b, color);
		LCD_DrawPoint(x0-a, y0+b, color);
 		LCD_DrawPoint(x0-b, y0+a, color);
		LCD_DrawPoint(x0-a, y0-b, color);
  		LCD_DrawPoint(x0-b, y0-a, color);
		a++;
		//使用Bresenham算法画圆     
		if( di < 0 )
		{
			di += 4 * a + 6;	 
		} 
		else
		{
			di += 10 + 4 * ( a - b );   
			b--;
		} 						    
	}
} 	

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_DrawFullCircle(unsigned short Xpos, unsigned short Ypos, unsigned short Radius, unsigned short Color)
{
	uint16_t x, y, r = Radius;
	for(y = Ypos - r; y < Ypos + r; y++)
	{
		for(x = Xpos - r;x < Xpos + r; x++)
		{
			if(((x - Xpos) * (x - Xpos) + (y - Ypos) * (y - Ypos)) <= r * r)
			{
				LCD_DrawPoint(x, y, Color);
			}
		}
	}
}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(unsigned short x, unsigned short y, unsigned char num, unsigned char size, unsigned char mode, unsigned short pen_color, unsigned short back_color)
{  							  
    unsigned char temp, t1, t;
	unsigned short y0 = y;
	unsigned char csize = ( size / 8 + ( (size % 8) ? 1 : 0)) * (size / 2); //得到字体一个字符对应点阵集所占的字节数	
 	num = num - ' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	
	for(t = 0; t < csize; t++)
	{   
		if(size == 12)//调用1206字体
		{
			temp = asc2_1206[num][t]; 
		}
		else if(size == 16)//调用1608字体
		{
			temp=asc2_1608[num][t];	
		}
		else if(size == 24)	//调用2412字体
		{
			temp=asc2_2412[num][t];
		}
		else 
			return; //没有的字库
		
		for(t1 = 0; t1 < 8; t1++)
		{			    
			if( temp & 0x80 )
			{
				LCD_DrawPoint(x, y, pen_color);
			}
			else if( mode == 0)
			{
				LCD_DrawPoint(x, y, back_color);
			}
			temp <<= 1;
			y++;
			
			if(y >= 240)//超区域
			{
				return;	
			}	
			
			if((y-y0) == size)
			{
				y = y0;
				x++;
				if(x>=320)//超区域
				{
					return;	
				}
				break;
			}
		}  	 
	}  	    	   	 	  
}

//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char size, unsigned char *p, unsigned short pen_color, unsigned short back_color)
{         
	unsigned char x0 = x;
	width += x;
	height += y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x >= width)
		{
			x = x0;
			y += size;
		}
		
        if(y >= height)//退出
		{
			break;
		}
		
        LCD_ShowChar(x, y, *p, size, 0, pen_color, back_color);
        x += size / 2;
        p++;
    }  
}

//程序汉字显示
//说明：汉字是用取模软件制作好的 指定的汉字 汉字大小是16x16	 即32个字节存储一个汉字
// 		这部分汉字是存在程序空间 所以汉字的多少直接影响程序空间的剩余量
//      主要方便于就显示几个指定的汉字来说就不用调用字库了 
//x  y  要显示汉字的坐标
//c[2]  汉字的数据 一个汉字两个字节表示
//dcolor 汉字的颜色   bgcolor 背景颜色
void GUI_sprintf_hz1616(unsigned short x, unsigned short y, unsigned char c[2], unsigned short pen_color, unsigned short back_color)
{
	unsigned char i, j, k, m; //定义临时变量
	unsigned short x0, y0;
	x0 = x;
	y0=y;	
	lcdSetAddress(x, y, x + 16 - 1, y + 16 - 1); //选择坐标位置	

	for (k = 0; k < 64; k++) //64表示自建汉字库中的个数，如超过，可以扩增
	{                        
		if ((code_GB_16[k].Index[0]==c[0]) && (code_GB_16[k].Index[1]==c[1])) //寻找对应汉字	 一个汉字需要两个字节
		{ 
			for(i = 0; i < 32; i++) //32个字节 每个字节都要一个点一个点处理 所以是处理了32X8次
			{								    
				m = code_GB_16[k].Msk[i];							//读取对应字节数据
				for(j = 0; j < 8; j++) 									//显示一个字节  一个字节8位 也就是8个点
				{
					//判断是否是要写入点
					if( (m&0x80) == 0x80)
					{
						LCD_Fast_DrawPoint(x, y, pen_color); // 如果是 给字体颜色
					}						 	 	
					else 			
					{
						LCD_Fast_DrawPoint(x, y, back_color); //如果不是 为背景色  给颜色
					}						
					m <<= 1; //左移一位  判断下一点
					x++;
					
					if(x - x0 == 16)
					{
						x = x0;
						y++;
						if(y - y0 >= 16)
						{
							return;
						}
					}
				} 
			}
		}  
	}	
}

//显示字符串 
//x1 y1 显示的初始位置
//*str 要显示的数据
//dcolor 显示字符的颜色
//bgcolor 显示字符的背景颜色  
void GUI_sprintf_hz16x(unsigned short x1, unsigned short y1, unsigned char *str, unsigned short pen_color, unsigned short back_color)	  
{  
	unsigned char l = 0;
	while(*str)
	{	
		if(*str>=0x80)
		{
			GUI_sprintf_hz1616(x1 + l * 8, y1, (unsigned char*)str, pen_color, back_color);
			str += 2;
			l += 2;
		}
	}	
}

//显示32x32单个汉字
//(x，y) 是显示内容的左上角坐标，c[2]表示汉字的两个字节
//dcolor 汉字的颜色   bgcolor 背景颜色
//说明：在指定位置上显示一个32x32的汉字
void GUI_sprintf_hz3232(unsigned short x, unsigned short y, unsigned char c[2], unsigned short pen_color, unsigned short back_color)
{
	unsigned char i, j, k;
	unsigned short x0, y0;
	x0 = x;
	y0 = y;	

	lcdSetAddress(x, y, x + 32 - 1, y + 32 - 1); //选择坐标位置	
	
	for (k = 0; k < 100; k++) //自建汉字库（k最大值和汉字库中汉字个数有关），循环查询内码
	{ 
		if ((code_GB_32[k].Index[0]==c[0])&&(code_GB_32[k].Index[1]==c[1])) //寻找要显示的汉字
		{ 
			for(i = 0; i < 128; i++) //1个汉字要128个字节点来显示
			{
				unsigned char m = code_GB_32[k].Msk[i]; //一个字节一个字节的显示
				for(j = 0; j < 8; j++) 
				{
					//判断是否是要写入点
					if((m&0x80)==0x80) // 如果是,给字体颜色
					{
						LCD_Fast_DrawPoint(x,y,pen_color);		
					}
					else //如果不是,为背景色
					{
						LCD_Fast_DrawPoint(x,y,back_color);	
					}
					m<<=1;
					x++;
					if(x - x0 == 32)
					{
						x = x0;
						y++;
						if(y - y0 >= 32)
						{
							return;
						}
					}
				}    
			}
		}  
	}	
}

//显示32x32汉字 和32x16 字母串
//x1，y1是显示内容的左上角坐标，
//dcolor 汉字的颜色   bgcolor 背景颜色
//*str 要显示的数据
void GUI_sprintf_hz32x(unsigned short x1, unsigned short y1, unsigned char *str, unsigned short pen_color, unsigned short back_color)	 
{  
	unsigned char l = 0;	  
	while(*str)
	{
		if(*str>=0x80)		 	        
		{
			GUI_sprintf_hz3232(x1 + l * 8, y1, (unsigned char*)str, pen_color, back_color);
			l += 4;
			str += 2;
		}
	}
}
