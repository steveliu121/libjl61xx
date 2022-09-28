#include <stdint.h>

#include "jl_config.h"
#include "driver/jl_reg_io.h"

#define lowWord(w)  ((w) & 0xffff)
#define highWord(w) ((w) >> 16)
#define lowByte(w) ((w) & 0xff)
#define highByte(w) ((w) >> 8)

#define i2c_address 0x54

#define IDA_BASE 0x780
#define IDA_SI0 8
#define IDA_C_ENABLE 0x1

#define IDA_C     (IDA_BASE + 0)
#define IDA_I     (IDA_BASE + 1)
#define IDA_A_L   (IDA_BASE + 2)
#define IDA_A_H   (IDA_BASE + 3)
#define IDA_WD_L  (IDA_BASE + 4)
#define IDA_WD_H  (IDA_BASE + IDA_SI0 + 5)
#define IDA_RD_L  (IDA_BASE + IDA_SI0 + 6)
#define IDA_RD_H  (IDA_BASE + 7)

jl_ret_t __apb_reg_read(jl_uint32 reg, jl_uint32 *buf, jl_uint8 size)
{
	jl_uint16 dataL;
	jl_uint16 dataH;

	i2c_start();
	i2c_send_byte(I2C_MSB_W);
	i2c_wait_ack();
	//Step1:Set IDA_I
	i2c_write_byte(highByte(IDA_I), lowByte(IDA_I));

	//Step2:Set IDA_I Value
	i2c_write_byte(highByte(size - 1), lowByte(size - 1));

	//Step3:Set IDA_A_L
	i2c_write_byte(highByte(IDA_A_L), lowByte(IDA_A_L));

	//Step4:Set IDA_A_L Value
	i2c_write_byte(highByte(lowWord(reg)), lowByte(lowWord(reg)));

	//Step5:Set IDA_A_H
	i2c_write_byte(highByte(IDA_A_H), lowByte(IDA_A_H));

	//Step6:Set IDA_A_H Value
	i2c_write_byte(highByte(highWord(reg)), lowByte(highWord(reg)));

	//Step7:Set IDA_C
	i2c_write_byte(highByte(IDA_C), lowByte(IDA_C));

	//Step8:Set IDA_C Value
	i2c_write_byte(highByte(IDA_C_ENABLE), lowByte(IDA_C_ENABLE));

	i2c_wait_ack();
	i2c_stop();

	i2c_start();
	//Step9:Set IDA_RD_L
	i2c_write_byte(highByte(IDA_RD_L), lowByte(IDA_RD_L));
	i2c_send_byte(I2C_MSB_R);
	i2c_wait_ack();

	dataL = i2c_receive_byte(1);
	i2c_stop();

	i2c_start();
	//Step10:Set IDA_RD_H
	i2c_write_byte(highByte(IDA_RD_H), lowByte(IDA_RD_H));
	i2c_send_byte(I2C_MSB_R);
	i2c_wait_ack();

	dataH = i2c_receive_byte(0);
	i2c_stop();

	*buf = dataL | (dataH<<8);
	return JL_ERR_OK;
}


jl_ret_t __apb_reg_write(jl_uint32 reg, jl_uint32 *buf, jl_uint8 size)
{
	uint8_t i = size;

	i2c_start();
	i2c_send_byte(I2C_MSB_W);
	i2c_wait_ack();
	//Step1:Set IDA_WD_L
	i2c_write_byte(highByte(IDA_WD_L), lowByte(IDA_WD_L));

	//Step2:Set IDA_WD_L Value
	i2c_write_byte(highByte(lowWord(*buf)), lowByte(lowWord(*buf)));

	//Step3:Set IDA_WD_H
	i2c_write_byte(highByte(IDA_WD_H), lowByte(IDA_WD_H));

	//Step4:Set IDA_WD_H Value
	i2c_write_byte(highByte(highWord(*buf)), lowByte(highWord(*buf)));

	//Step5:Set IDA_A_L
	i2c_write_byte(highByte(IDA_A_L), lowByte(IDA_A_L));

	//Step6:Set IDA_A_L Value
	i2c_write_byte(highByte(lowWord(reg)), lowByte(lowWord(reg)));

	//Step7:Set IDA_A_H
	i2c_write_byte(highByte(IDA_A_H), lowByte(IDA_A_H));

	//Step8:Set IDA_A_H Value
	i2c_write_byte(highByte(highWord(reg)), lowByte(highWord(reg)));

	//Step9:Set IDA_I
	i2c_write_byte(highByte(IDA_I), lowByte(IDA_I));

	//Step10:Set IDA_I Value
	i2c_write_byte(highByte(size), lowByte(size));

	while (i--) {
		i2c_send_byte(*buf);
		i2c_wait_ack();
		buf++;
	}
	i2c_stop();
	i2c_delay_ms(3);

	return JL_ERR_OK;
}


jl_api_ret_t jl_apb_reg_read(jl_uint32 reg, jl_uint32 *val)
{
	return __apb_reg_read(reg, val, 1);
}

jl_api_ret_t jl_apb_reg_burst_read(jl_uint32 reg, jl_uint32 *buf,
		jl_uint32 size)
{
	return __apb_reg_read(reg, buf, size);
}

jl_api_ret_t jl_apb_reg_write(jl_uint32 reg, jl_uint32 val)
{
	return __apb_reg_write(reg, &val, 1);
}

jl_api_ret_t jl_apb_reg_burst_write(jl_uint32 reg, jl_uint32 *buf,
		jl_uint32 size)
{
	return __apb_reg_write(reg, buf, size);
}

