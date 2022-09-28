#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"
#include "gpio.h"
#include "config.h"

#define I2C_HIGH			GPIO_PIN_SET
#define I2C_LOW				GPIO_PIN_RESET
#define I2C_PAGE_SIZE			64

#define I2C_DELAY_TIME			50

#define UINT16_H(X)			(X >> 8)
#define UINT16_L(X)			(X & 0x00ff)

#define UINT32_1(X)			(X >> 24)
#define UINT32_2(X)			((X >> 16) & 0x00ff)
#define UINT32_3(X)			((X >> 8) & 0x00ff)
#define UINT32_4(X)			(X & 0x00ff)

/*****************Device Addr********************/
jl_uint8 I2C_MSB_W;        /* 0x00 WRITE MODE*/
jl_uint8 I2C_MSB_R = 0x01; /* 0x01 READ MODE*/

void i2c_delay_us(jl_uint16 time)
{
	jl_uint16 i = 0;

	while (time--) {
		i = 10;
		asm("nop");
		i--;
	}
}

void i2c_delay_ms(jl_uint16 time)
{
	for (int i = 0; i < time; i++) {
		for (int j = 0; j < 8000; j++) {
			asm("nop"); asm("nop");
			asm("nop"); asm("nop");
		}
	}
}

void i2c_start(void)
{
	i2c_sda_set(I2C_HIGH);
	i2c_scl_set(I2C_HIGH);
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_sda_set(I2C_LOW);
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_scl_set(I2C_LOW);
	i2c_delay_us(I2C_DELAY_TIME);
}

void i2c_stop(void)
{
	i2c_sda_set(I2C_LOW);
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_scl_set(I2C_HIGH);
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_sda_set(I2C_HIGH);
	i2c_delay_us(I2C_DELAY_TIME);
}

void i2c_ack(void)
{
	i2c_sda_set(I2C_LOW);
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_scl_set(I2C_HIGH);
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_scl_set(I2C_LOW);
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_sda_set(I2C_HIGH);
}

void i2c_no_ack(void)
{
	i2c_sda_set(I2C_HIGH);
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_scl_set(I2C_HIGH);
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_scl_set(I2C_LOW);
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_scl_set(I2C_HIGH);
}

jl_uint8 i2c_wait_ack(void)
{
	jl_uint8 ack;

	i2c_sda_set(I2C_HIGH);
	i2c_sda_dir_in();
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_scl_set(I2C_HIGH);
	i2c_delay_us(I2C_DELAY_TIME);
	ack = i2c_sda_get();
	i2c_sda_dir_out();
	i2c_delay_us(I2C_DELAY_TIME);
	i2c_scl_set(I2C_LOW);

	return ack;
}

void i2c_send_byte(jl_uint8 byte)
{
	int i = 8;
	jl_uint8 data, bit;

	data = byte;
	while (i--) {
		bit = !!(data & 0x80);
		i2c_sda_set(bit);
		i2c_delay_us(I2C_DELAY_TIME);
		i2c_scl_set(I2C_HIGH);
		i2c_delay_us(I2C_DELAY_TIME);
		i2c_scl_set(I2C_LOW);
		i2c_delay_us(I2C_DELAY_TIME);
		data <<= 1;
	}
}

jl_uint8 i2c_receive_byte(jl_uint8 ack)
{
	jl_uint8 data = 0, i = 8;

	i2c_sda_dir_in();
	while (i--) {
		data <<= 1;
		i2c_scl_set(I2C_HIGH);
		i2c_delay_us(I2C_DELAY_TIME);
		data += i2c_sda_get();
		i2c_scl_set(I2C_LOW);
		i2c_delay_us(I2C_DELAY_TIME);
	}
	i2c_sda_dir_out();
	if (ack == 0)
		i2c_no_ack();
	else
		i2c_ack();

	return data;
}

void i2c_module_init(void)
{
	i2c_gpio_init();
}


void i2c_write_byte(jl_uint8 addr, jl_uint8 data)
{
	i2c_start();
	i2c_send_byte(I2C_MSB_W);
	i2c_wait_ack();
	i2c_send_byte(addr);
	i2c_wait_ack();
	i2c_send_byte(data);
	i2c_wait_ack();
	i2c_stop();
	i2c_delay_ms(6);
}

void i2c_write_data_u32(jl_uint32 addr, jl_uint32 data)
{
	i2c_start();
	i2c_send_byte(0x34);
	i2c_wait_ack();
	i2c_send_byte(UINT32_1(addr));
	i2c_wait_ack();
	i2c_send_byte(UINT32_2(addr));
	i2c_wait_ack();
	i2c_send_byte(UINT32_3(addr));
	i2c_wait_ack();
	i2c_send_byte(UINT32_4(addr));
	i2c_wait_ack();

	i2c_send_byte(UINT32_4(data));
	i2c_send_byte(UINT32_4(data));
	i2c_send_byte(UINT32_4(data));
	i2c_send_byte(UINT32_4(data));

	i2c_wait_ack();
	i2c_stop();
	i2c_delay_ms(6);
}

jl_uint8 i2c_read_byte(jl_uint8 addr)
{

	jl_uint8 data;

	i2c_start();
	i2c_send_byte(I2C_MSB_W);
	i2c_wait_ack();
	i2c_send_byte(addr);
	i2c_wait_ack();
	i2c_stop();

	i2c_start();
	i2c_send_byte(I2C_MSB_R);
	i2c_wait_ack();

	data = i2c_receive_byte(0);
	i2c_stop();

	return data;
}

void i2c_write_data(jl_uint16 addr, jl_uint8 len, jl_uint8 *data)
{
	jl_uint8 i = len;

	i2c_start();
	i2c_send_byte(I2C_MSB_W);
	i2c_wait_ack();
	i2c_send_byte(UINT16_H(addr));
	i2c_wait_ack();
	i2c_send_byte(UINT16_L(addr));
	i2c_wait_ack();

	while (i--) {
		i2c_send_byte(*data);
		i2c_wait_ack();
		data++;
	}
	i2c_stop();
	i2c_delay_ms(3);
}

jl_uint8 i2c_read_data(jl_uint8 addr)
{
	jl_uint16 data;

	i2c_start();
	i2c_send_byte(I2C_MSB_W);
	i2c_wait_ack();
	i2c_send_byte(addr);
	i2c_wait_ack();
	i2c_stop();

	i2c_start();
	i2c_send_byte(I2C_MSB_R);
	i2c_wait_ack();

	data = i2c_receive_byte(1);
	data <<= 8;
	i2c_stop();

	i2c_start();
	i2c_send_byte(I2C_MSB_W);
	i2c_wait_ack();
	i2c_send_byte(addr);
	i2c_wait_ack();
	i2c_stop();
	i2c_start();
	i2c_send_byte(I2C_MSB_R);
	i2c_wait_ack();
	data |= i2c_receive_byte(0);
	i2c_stop();

	return data;
}

void i2c_set_dev(jl_uint8 dev)
{
	/*Configure Write/Read mode with last bit */
	I2C_MSB_W  = 0x00 + (dev<<1);
	I2C_MSB_R  = 0x01 + (dev<<1);
}

