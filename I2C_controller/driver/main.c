#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>

//static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command,
//                                     int size, union i2c_smbus_data *data)
//{
//        struct i2c_smbus_ioctl_data args;
//
//        args.read_write = read_write;
//        args.command = command;
//        args.size = size;
//        args.data = data;
//        return ioctl(file,I2C_SMBUS,&args);
//}
//static inline __s32 i2c_smbus_read_word_data(int file, __u8 command)
//{
//        union i2c_smbus_data data;
//        if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
//                             I2C_SMBUS_WORD_DATA,&data))
//                return -1;
//        else
//                return 0x0FFFF & data.word;
//}

int main(void)
{
	int file = open("/dev/i2c-2", O_RDWR);
	if (file < 0) 
	{
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}

	int addr = 0x40; /* The I2C address */

	if (ioctl(file, I2C_SLAVE, addr) < 0) 
	{
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}

	__u8 reg = 0x10; /* Device register to access */
	__s32 res;
	char buf[10];

	/* Using SMBus commands */
	res = i2c_smbus_read_byte_data(file, reg);
	if (res < 0) 
	{
		/* ERROR HANDLING: i2c transaction failed */
	} else 
	{
		printf("i2c_smbus_read_word_data = %d", res); 
		/* res contains the read word */
	}
}
