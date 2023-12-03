#include "RTC.h"
#include "stm32f1xx.h"

extern I2C_HandleTypeDef hi2c2;

#define RTC_ADDR 0x68<<1

uint8_t Demical2BCD(uint8_t num)
{
	return (num/10)<<4|(num%10);
}

uint8_t BCD2Demical(uint8_t num)
{
	return (num>>4)*10+(num&0x0F);
}

void RTC_WriteTime(DateTime *dt)
{
	uint8_t buff[8];
	buff[0] = 0;
	buff[1] = Demical2BCD(dt->second);
	buff[2] = Demical2BCD(dt->min);
	buff[3] = Demical2BCD(dt->hour);
	buff[4] = Demical2BCD(dt->day);
	buff[5] = Demical2BCD(dt->date);
	buff[6] = Demical2BCD(dt->month);
	buff[7] = Demical2BCD(dt->year);
	HAL_I2C_Master_Transmit(&hi2c2, RTC_ADDR, buff, 8, 100);
}

void RTC_ReadTime(DateTime *dt)
{
	uint8_t buff[7];
	uint8_t ADD_reg = 0;
	HAL_I2C_Master_Transmit(&hi2c2, RTC_ADDR, &ADD_reg, 1, 100);
	HAL_I2C_Master_Receive(&hi2c2, RTC_ADDR, buff, 7, 100);
	dt->second = BCD2Demical(buff[0]);
	dt->min = BCD2Demical(buff[1]);
	dt->hour = BCD2Demical(buff[2]);
	dt->day = BCD2Demical(buff[3]);
	dt->date = BCD2Demical(buff[4]);
	dt->month = BCD2Demical(buff[5]);
	dt->year = BCD2Demical(buff[6]);
}
