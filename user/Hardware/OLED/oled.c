

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "hal_i2c.h"
#include "string.h"
// #include "FreeRTOS.h"
// #include "task.h"

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT / 8)
static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE] = {0};
// OLED的显存
// 存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
#ifdef STM32F103
void IIC_Start()
{

	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	OLED_SCLK_Set();
	//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

void IIC_Wait_Ack()
{

	// GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
	// GPIOB->CRH |= 0x00080000;
	//	OLED_SDA = 1;
	//	delay_us(1);
	// OLED_SCL = 1;
	// delay_us(50000);
	/*	while(1)
		{
			if(!OLED_SDA)				//判断是否接收到OLED 应答信号
			{
				//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
				//GPIOB->CRH |= 0x00030000;
				return;
			}
		}
	*/
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m, da;
	da = IIC_Byte;
	OLED_SCLK_Clr();
	for (i = 0; i < 8; i++)
	{
		m = da;
		//	OLED_SCLK_Clr();
		m = m & 0x80;
		if (m == 0x80)
		{
			OLED_SDIN_Set();
		}
		else
			OLED_SDIN_Clr();
		da = da << 1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78); // Slave address,SA0=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x00); // write command
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78); // D/C#=0; R/W#=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x40); // write data
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
	if (cmd)
	{

		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}

#endif

#ifdef ESP_
#include "esp_log.h"

i2c_cmd_handle_t IIC_Cmd; // I2C	Handle_t I2C链表
// extern i2c_cmd_handle_t IIC_Cmd;		//I2C	Handle_t I2C链表
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
	// IIC_Cmd = i2c_cmd_link_create();
	uint8_t data[2];
	if (cmd)
		data[0] = 0x40; // Write_IIC_Data
	else
		data[0] = 0x00; // Write_IIC_Command
	data[1] = dat;

	i2c_master_start(IIC_Cmd);
	i2c_master_write_byte(IIC_Cmd, (0x78) | I2C_MASTER_WRITE, ACK_CHECK_DIS);
	i2c_master_write(IIC_Cmd, data, 2, ACK_CHECK_DIS);
	i2c_master_stop(IIC_Cmd);
	i2c_master_cmd_begin(I2C_NUM_0, IIC_Cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(IIC_Cmd);
}

void OLED_WR_Bytes(const uint8_t *dat, size_t len, unsigned cmd)
{
	// 分配足够大的缓冲区：控制字节 + 数据
	uint8_t *buffer = malloc(len + 1);
	if (!buffer)
		return;

	buffer[0] = cmd ? 0x40 : 0x00; // 控制字节
	memcpy(&buffer[1], dat, len);  // 数据

	i2c_cmd_handle_t IIC_Cmd = i2c_cmd_link_create();
	i2c_master_start(IIC_Cmd);
	i2c_master_write_byte(IIC_Cmd, (0x3C << 1) | I2C_MASTER_WRITE, ACK_CHECK_DIS);
	i2c_master_write(IIC_Cmd, buffer, len + 1, ACK_CHECK_DIS);
	i2c_master_stop(IIC_Cmd);

	esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, IIC_Cmd, 1000 / portTICK_PERIOD_MS);
	if (ret != ESP_OK)
	{
		ESP_LOGE("OLED", "I2C write failed: %s", esp_err_to_name(ret));
	}

	i2c_cmd_link_delete(IIC_Cmd);
	free(buffer);
}
#endif
void Write_IIC_Data(unsigned char IIC_Data)
{

	// IIC_Start();
	// Write_IIC_Byte(0x78); // D/C#=0; R/W#=0
	// IIC_Wait_Ack();
	// Write_IIC_Byte(0x40); // write data
	// IIC_Wait_Ack();
	// Write_IIC_Byte(IIC_Data);
	// IIC_Wait_Ack();
	// IIC_Stop();
	unsigned char send_data[2] = {0x40, IIC_Data};
	I2cSendBytes(0x78, &send_data[0], 2);
}
/**********************************************
//IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	// IIC_Start();
	// Write_IIC_Byte(0x78); // Slave address,SA0=0
	// IIC_Wait_Ack();
	// Write_IIC_Byte(0x00); // write command
	// IIC_Wait_Ack();
	// Write_IIC_Byte(IIC_Command);
	// IIC_Wait_Ack();
	unsigned char send_data[2] = {0, IIC_Command};
	I2cSendBytes(0x78, &send_data[0], 2);
}
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{

	if (cmd)
	{

		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}
/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m, n;
	for (m = 0; m < 8; m++)
	{
		OLED_WR_Byte(0xb0 + m, 0); // page0-page1
		OLED_WR_Byte(0x00, 0);	   // low column start address
		OLED_WR_Byte(0x10, 0);	   // high column start address
		for (n = 0; n < 128; n++)
		{
			OLED_WR_Byte(fill_Data, 1);
		}
	}
}

/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for (; Del_50ms > 0; Del_50ms--)
		for (m = 6245; m > 0; m--)
			;
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while (Del_1ms--)
	{
		for (j = 0; j < 5; j++)
			;
	}
}

// 坐标设置

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}
// 开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}
// 关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}
// 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	  // 设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(0, OLED_DATA);
	} // 更新显示
}

/***********************************
 * OLED_Clear_row_column
 * @function 对显示屏的一部分进行清除
 * @param row 		: 要清除的行数(0-7)
 * @param x_stat	:清除开始位置(x_轴 0-127)
 * @param x_end 	:清除结束位置(x_轴 0-127)
 * @date 2023 9 22
 * by wu_xc
 ************************************/
void OLED_Clear_row_column(u8 row, u8 x_stat, u8 x_end)
{
	u8 n;

	OLED_WR_Byte(0xb0 + row, OLED_CMD); // 设置页地址（0~7）
	OLED_WR_Byte(0x00, OLED_CMD);		// 设置显示位置—列低地址
	OLED_WR_Byte(0x10, OLED_CMD);		// 设置显示位置—列高地址
	for (n = x_stat; n < x_end; n++)
		OLED_WR_Byte(0, OLED_DATA);
	// 更新显示
}
void OLED_On(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	  // 设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(1, OLED_DATA);
	} // 更新显示
}
// 在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~63
// mode:0,反白显示;1,正常显示
// size:选择字体 16/12
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size)
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; // 得到偏移后的值
	if (x > Max_Column - 1)
	{
		x = 0;
		y = y + 2;
	}
	if (Char_Size == 16)
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 8; i++)
			OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 8; i++)
			OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
	}
	else
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 6; i++)
			OLED_WR_Byte(F6x8[c][i], OLED_DATA);
	}
}
// m^n函数
u32 oled_pow(u8 m, u8 n)
{
	u32 result = 1;
	while (n--)
		result *= m;
	return result;
}
// 显示2个数字
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// mode:模式	0,填充模式;1,叠加模式
// num:数值(0~4294967295);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2)
{
	u8 t, temp;
	u8 enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
	}
}
void OLED_ShowFloat(u8 x, u8 y, float num, u8 size2)
{
	// 将浮点数四舍五入转换为整数（扩大100倍）
	u32 scaled_num = (u32)(num * 100 + 0.5f);
	u32 integer_part = scaled_num / 100;
	u32 decimal_part = scaled_num % 100;
	u8 integer_digits = 0;
	// u32 temp = integer_part;

	// 计算整数部分位数（处理0的特殊情况）
	// if (temp == 0) integer_digits = 1;
	// else while (temp > 0) { temp /= 10; integer_digits++; }
	integer_digits = 3;
	// 显示整数部分
	OLED_ShowNum(x, y, integer_part, integer_digits, size2);

	// 显示小数点（根据字体大小计算偏移）
	OLED_ShowChar(x + integer_digits * (size2 / 2), y, '.', size2);

	// 显示两位小数部分（固定长度2位）
	OLED_ShowNum(x + (integer_digits + 1) * (size2 / 2), y, decimal_part, 2, size2);
}
// 显示一个字符号串
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 Char_Size)
{
	unsigned char j = 0;
	while (chr[j] != '\0')
	{
		OLED_ShowChar(x, y, chr[j], Char_Size);
		x += 8;
		if (x > 120)
		{
			x = 0;
			y += 2;
		}
		j++;
	}
}
// 显示汉字
void OLED_ShowCHinese(u8 x, u8 y, u8 no)
{
	u8 t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
		adder += 1;
	}
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}

// 初始化SSD1306
void OLED_Init(void)
{

#ifdef STM32F103

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  // 初始化GPIOD3,6
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7);

#endif
	// HAL_Delay(80);
	// vTaskDelay(80);
	OLED_WR_Byte(0xAE, OLED_CMD); //--display off
	OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
	OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
	OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
	OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
	OLED_WR_Byte(0x81, OLED_CMD); // contract control
	OLED_WR_Byte(0xFF, OLED_CMD); //--128
	OLED_WR_Byte(0xA1, OLED_CMD); // set segment remap
	OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
	OLED_WR_Byte(0xC8, OLED_CMD); // Com scan direction
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
	OLED_WR_Byte(0x00, OLED_CMD); //

	OLED_WR_Byte(0xD5, OLED_CMD); // set osc division
	OLED_WR_Byte(0x80, OLED_CMD); //

	OLED_WR_Byte(0xD8, OLED_CMD); // set area color mode off
	OLED_WR_Byte(0x05, OLED_CMD); //

	OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period
	OLED_WR_Byte(0xF1, OLED_CMD); //

	OLED_WR_Byte(0xDA, OLED_CMD); // set com pin configuartion
	OLED_WR_Byte(0x12, OLED_CMD); //

	OLED_WR_Byte(0xDB, OLED_CMD); // set Vcomh
	OLED_WR_Byte(0x30, OLED_CMD); //

	OLED_WR_Byte(0x8D, OLED_CMD); // set charge pump enable
	OLED_WR_Byte(0x14, OLED_CMD); //

	OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
	OLED_Display_On();
}

/***********************************
 * OLED_DrawPixel
 * @function 绘制单个像素点到缓冲区
 * @param x,y: 像素坐标(0<=x<128, 0<=y<64)
 * @param color: 像素颜色(0=黑色, 1=白色)
 * @date 2023-10-01
 ************************************/
void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
	// 检查坐标范围
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
		return;

	// 计算缓冲区索引和位偏移
	uint16_t index = x + (y / 8) * SSD1306_WIDTH;
	uint8_t bit_offset = y % 8;

	// 根据颜色更新缓冲区对应位
	if (color)
	{
		ssd1306_buffer[index] |= (1 << bit_offset); // 白色-置位
	}
	else
	{
		ssd1306_buffer[index] &= ~(1 << bit_offset); // 黑色-清位
	}
}
// 刷新整个屏幕，把缓冲区写入 SSD1306
void OLED_Update(void)
{
    // SSD1306 高度 64 → 共 8 页，每页 8 行
    for (uint8_t page = 0; page < (SSD1306_HEIGHT / 8); page++)
    {
        // 设置页地址
        OLED_WR_Byte(0xB0 + page, 0);   // 设置页起始地址 (0xB0~0xB7)
        OLED_WR_Byte(0x00, 0);          // 设置列低位地址
        OLED_WR_Byte(0x10, 0);          // 设置列高位地址

        // 写入一整页的数据（128字节）
        for (uint8_t col = 0; col < SSD1306_WIDTH; col++)
        {
            OLED_WR_Byte(ssd1306_buffer[page * SSD1306_WIDTH + col], 1);
        }
    }
}


