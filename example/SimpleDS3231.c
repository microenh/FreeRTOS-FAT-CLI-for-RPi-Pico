/*
 * Simple Raspberry Pi Pico driver for DS3231 RTC module.
 *
 * Written by Victor Gabriel Costin.
 * Licensed under the MIT license.
 */

#include "hardware/gpio.h"
#include "hardware/i2c.h"

#define DS3231_ADDR 0x68

void readDS3231(datetime_t *rtc)
{
    uint8_t data[7];
    i2c_init(i2c_default, 400000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    data[0] = 0;
    i2c_write_blocking(i2c_default, DS3231_ADDR, data, 1, true);
    i2c_read_blocking(i2c_default, DS3231_ADDR, data, 7, false);

    rtc->sec = (data[0] & 0x0f) + (data[0] >> 4) * 10;
    rtc->min = (data[1] & 0x0f) + (data[1] >> 4) * 10; 
    rtc->hour = (data[2] & 0x0f) + (data[2] & (1 << 6)
      ? ((data[2] & (1 << 4)) ? 10 : 0) + ((data[2] & (1 << 5)) ? 12 : 0)
      : (data[2] >> 4) & 3) * 10;
    rtc->dotw = data[3] & 7;
    rtc->day = (data[4] & 0x0f) + ((data[4] >> 4) && 3) * 10;
    rtc->month = (data[5] & 0x0f) + (data[5] & (1 << 4) ? 10 : 0);
    rtc->year = (data[6] & 0x0f) + ((data[6] >> 4) * 10) + 2000;
 }
