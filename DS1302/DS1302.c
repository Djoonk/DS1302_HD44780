/*
 * DS1302.c
 *
 *  Created on: Jul 10, 2023
 *      Author: oem
 */

#include "DS1302.h"
#include "stm32f4xx.h"
#include "mDelays.h"


extern uint8_t buf[7];
//-------------------------------------------------
//udelay
//-------------------------------------------------
static void udelay(uint32_t cnt)
{
//	for (uint32_t i = 0; i < (cnt * 1); i++)
//	{
//
//	}
void mDelay_US_Setup(unsigned int cnt);

}

//-------------------------------------------------
//Convert binary to bcd
//-------------------------------------------------
static uint8_t binary_to_bcd(uint8_t value)
{
	uint8_t m, n;
	uint8_t bcd;

	bcd = value;
	if(value >= 10)
	{
		m = value / 10;
		n = value % 10;
		bcd = (uint8_t)(m << 4) | n;
	}
	return bcd;
}

//-------------------------------------------------
//Convert bcd to binary
//-------------------------------------------------
static uint8_t bcd_to_binary(uint8_t value)
{
	uint8_t m, n;
	m = (uint8_t)(value >> 4) * 4;
	n = value & 0x0f;
	return m + n;

}




//-------------------------------------------------
//SDA output
//-------------------------------------------------
void sda_write(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStructure.Pin = DS1302_SDA;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DS1302_PORT, &GPIO_InitStructure);

}

//-------------------------------------------------
//SDA input
//-------------------------------------------------
void sda_read(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;


	GPIO_InitStructure.Pin = DS1302_SDA;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DS1302_PORT, &GPIO_InitStructure);
}



/*=== Clock Init ===*/
void ds1302_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStructure.Pin = DS1302_SDA | DS1302_CLK | DS1302_RST;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DS1302_PORT, &GPIO_InitStructure);

//	ds1302_write_byte(DS1302_SEC_ADDR_WRITE, 0x00);

	HAL_GPIO_WritePin(DS1302_PORT, DS1302_RST, GPIO_PIN_RESET);

//	CLK - LOW
	HAL_GPIO_WritePin(DS1302_PORT, DS1302_CLK, GPIO_PIN_RESET);

//	DWT->CTRL |= 1; // enable the counter for microsecond delay, see "void delayUS_DWT(uint32_t us)"
}
/*=== Clock Init ===*/

//-------------------------------------------------
//Send Command
//-------------------------------------------------
void ds1302_send_cmd(uint8_t cmd)
{
	for(int i = 0; i < 8; i++)
	{
//		DS1302_SDA = (bit)(сmd & 1);
		HAL_GPIO_WritePin(DS1302_PORT, DS1302_SDA, (cmd & 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//		DS1302_CLK = 1;
		HAL_GPIO_WritePin(DS1302_PORT, DS1302_CLK, GPIO_PIN_SET );
		udelay(1);
//		DS1302_CLK = 0;
		HAL_GPIO_WritePin(DS1302_PORT, DS1302_CLK, GPIO_PIN_RESET);
		udelay(1);
		cmd >>= 1;
	}

}
/*=== Send Command ===*/



//-------------------------------------------------
//Read byte from address
//-------------------------------------------------
uint8_t ds1302_read_byte(uint8_t addr)
{
	uint8_t tmp = 0;
	//	DS1302_RST = 1
	HAL_GPIO_WritePin(DS1302_PORT, DS1302_RST, GPIO_PIN_SET);

	//Setеting 0 to address bit
//	addr |= 0x01;

	//Send address
	ds1302_send_cmd(addr);

	sda_read();

	for(int i = 0; i < 8; i++)
	{
		tmp >>= 1;
//		if(DS1302_SDA)
		if( HAL_GPIO_ReadPin(DS1302_PORT, DS1302_SDA) )
		{
//			read 1 in MSB
			tmp|= 0x80;
		}
//		DS1302_CLK = 1
		HAL_GPIO_WritePin(DS1302_PORT, DS1302_CLK, GPIO_PIN_SET);
		udelay(1);
//		DS1302_CLK = 0
		HAL_GPIO_WritePin(DS1302_PORT, DS1302_CLK, GPIO_PIN_RESET);
		udelay(1);

	}
	sda_write();
//	ds1302_rst = 0
	HAL_GPIO_WritePin(DS1302_PORT, DS1302_RST, GPIO_PIN_RESET);

//	ds1302_sda = 0
	HAL_GPIO_WritePin(DS1302_PORT, DS1302_SDA, GPIO_PIN_RESET);

	return tmp;
}







/*=== Write byte from address ===*/
void ds1302_write_byte(uint8_t addr, uint8_t dat)
{
//	DS1302_RST = 1
	HAL_GPIO_WritePin(DS1302_PORT, DS1302_RST, GPIO_PIN_SET);
	ds1302_send_cmd(addr);
	udelay(100);
//	DS1302_SendCMD(dat);

	for (int i = 0; i < 8; i ++)
		{
	//		DS1302_SDA = (bit)(d & 1);
			HAL_GPIO_WritePin(DS1302_PORT, DS1302_SDA, (dat & 1) ?  GPIO_PIN_SET :  GPIO_PIN_RESET);
	//		DS1302_SCK = 1;
			HAL_GPIO_WritePin(DS1302_PORT, DS1302_CLK,  GPIO_PIN_SET);
			udelay(1);
	//		DS1302_SCK = 0;
			HAL_GPIO_WritePin(DS1302_PORT, DS1302_CLK,  GPIO_PIN_RESET);
			udelay(1);;
			dat >>= 1;
		}



//	DS1302_RST = 0
	HAL_GPIO_WritePin(DS1302_PORT, DS1302_RST, GPIO_PIN_RESET);
//	DS1302_SDA = 0
	HAL_GPIO_WritePin(DS1302_PORT, DS1302_SDA, GPIO_PIN_RESET);

}
/*=== Write byte from address ===*/


//-------------------------------------------------
//Set time
//-------------------------------------------------
void ds1302_set_time(ds1302_time *RTC_time)
{
//	set_second
	uint8_t seconds, hrs;
	seconds = ( 1 << 7 );
	seconds = binary_to_bcd(RTC_time->second);

	ds1302_write_byte(DS1302_SEC_ADDR_WRITE, seconds);

//	set minutes
	ds1302_write_byte(DS1302_MIN_ADDR_WRITE, binary_to_bcd(RTC_time->minutes));

//	set hour
	hrs = bcd_to_binary(RTC_time->hour);
	if (RTC_time->time_format == TIME_FORMAT_24HRS)
	{
		hrs &= ~(1 << 6);

	}
	else
	{

		hrs |= (1 << 6);
		hrs = (RTC_time->time_format == TIME_FORMAT_12HRS_PM) ? hrs | (1 << 5) : hrs & ~(1 << 5);
	}
	ds1302_write_byte(DS1302_HOUR_ADDR_WRITE, hrs);
}



/*=== Get time ===*/
void ds1302_get_time(ds1302_time *RTC_time)
{
//	Set seconds
	uint8_t seconds, hrs;
	seconds = bcd_to_binary(ds1302_read_byte(DS1302_SEC_ADDR_READ));
	seconds &= ~( 1 << 7 );
	RTC_time->second = seconds;
//	Set minutes
	RTC_time->minutes = bcd_to_binary(ds1302_read_byte(DS1302_MIN_ADDR_READ));
//	Set_hour
	hrs = ds1302_read_byte(DS1302_HOUR_ADDR_READ);
	if( hrs & ( 1 << 5 ))
	{
//		12 hr format
		RTC_time->time_format = !( (hrs & (1 << 5)) == 0 );
		hrs &= ~( 0x3 << 5 ); //clear 5 and 5 bit
	}else{
//		24 hr format
		RTC_time->time_format = TIME_FORMAT_24HRS;
	}
	RTC_time->hour = hrs;
}
/*=== Get time ===*/

/*=== Set Date ===*/
void ds1302_set_date(ds1302_date *RTC_date)
{
//	Set date
	ds1302_write_byte(DS1302_DATE_ADDR_WRITE, binary_to_bcd(RTC_date->date));
//	Set mounth
	ds1302_write_byte(DS1302_MOUTH_ADDR_WRITE, binary_to_bcd(RTC_date->month));
//	Set day
	ds1302_write_byte(DS1302_DATE_ADDR_WRITE, binary_to_bcd(RTC_date->day));
//	Set day	year
	ds1302_write_byte(DS1302_YEAR_ADDR_WRITE, binary_to_bcd(RTC_date->year));

}

/*=== Set Date ===*/

/*=== Get Date ===*/

/*=== Get Date ===*/




