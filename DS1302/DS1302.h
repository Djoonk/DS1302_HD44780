/*
 * DS1302.h
 *
 *  Created on: Jul 10, 2023
 *      Author: Kalin Aleksander
 */



#ifndef DS1302_H_
#define DS1302_H_

#include "stm32f411xe.h"


/*Description PIN 3-Wire Interface*/
#define DS1302_PORT		GPIOA
#define DS1302_RST		GPIO_PIN_11
#define DS1302_SDA		GPIO_PIN_12
#define DS1302_CLK		GPIO_PIN_8


/*Description PIN 3-Wire Interface*/

/*Register addres for read*/
#define DS1302_SEC_ADDR_READ			0x81
#define DS1302_MIN_ADDR_READ			0x83
#define DS1302_HOUR_ADDR_READ			0x85
#define DS1302_DATE_ADDR_READ			0x87
#define DS1302_MOUTH_ADDR_READ			0x89
#define DS1302_YEAR_ADDR_READ			0x8D
#define DS1302_DAY_ADDR_READ			0x8B
/*Register addreses for read*/

/*Register addreses for write*/
#define DS1302_SEC_ADDR_WRITE			0x80
#define DS1302_MIN_ADDR_WRITE			0x82
#define DS1302_HOUR_ADDR_WRITE			0x84
#define DS1302_DATE_ADDR_WRITE			0x86
#define DS1302_MOUTH_ADDR_WRITE			0x88
#define DS1302_YEAR_ADDR_WRITE			0x8C
#define DS1302_DAY_ADDR_WRITE			0x8A
/*Register addreses for write*/


/*Time format*/
#define TIME_FORMAT_12HRS_AM	0
#define TIME_FORMAT_12HRS_PM	1
#define TIME_FORMAT_24HRS		2
/*Time format*/

#define DS1302_CHARGER					0x90
#define DS1302_CONTROL					0X8E
#define DS1302_INITIALPOWER				0x5C
#define DS1302_CHAR_DIS					0X00

//#define HEX2BCD(v)		( (v) % 10 + (v) / 10 * 16 )
//#define BCD2HEX(v)		( (v) % 16 + (v) / 16 * 10 )



/*Time struct*/
typedef struct
{
	uint8_t second;
	uint8_t minutes;
	uint8_t hour;
	uint8_t time_format;

} ds1302_time;
/*Time struct*/

/*Date struct*/
typedef struct
{
	uint8_t date;
	uint8_t month;
	uint8_t day;
	uint8_t year;

} ds1302_date;
/*Date struct*/

/*function*/
void sda_read();
void sda_write();
static uint8_t binary_to_bcd(uint8_t value);
static uint8_t bcd_to_binary(uint8_t value);
static void udelay(uint32_t cnt);
void ds1302_send_cmd(uint8_t cmd);
uint8_t ds1302_read_byte(uint8_t addr);
void ds1302_write_byte(uint8_t addr, uint8_t dat);
void ds1302_set_time(ds1302_time *RTC_time);
void ds1302_get_time(ds1302_time *RTC_time);
void ds1302_set_date(ds1302_date *RTC_date);
void ds1302_init();



/*function*/
#endif /* DS1302_H_ */
