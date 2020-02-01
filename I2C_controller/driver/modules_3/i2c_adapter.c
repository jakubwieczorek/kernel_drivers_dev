// it respresents i2c bus. Linux is always master.

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stddef.h>
#include <linux/i2c.h>
#include <linux/init.h>


static s32 stm32_access(struct i2c_adapter *adap, u16 addr,
		       unsigned short flags, char read_write,
		       u8 command, int size, union i2c_smbus_data *data)
{
	dev_info(&adap->dev, "%s was called with the following parameters:\n",
		 __FUNCTION__);
	dev_info(&adap->dev, "addr = %.4x\n", addr);
	dev_info(&adap->dev, "flags = %.4x\n", flags);
	dev_info(&adap->dev, "read_write = %s\n",
		 read_write == I2C_SMBUS_WRITE ? "write" : "read");
	dev_info(&adap->dev, "command = %d\n", command);
	
	if(size == I2C_SMBUS_WORD_DATA) 
	{
		if (read_write == I2C_SMBUS_WRITE)
		{
			dev_info(&adap->dev, "data = %.4x\n", data->word);
		}
	}

	return 0;
}

static u32 stm32_func(struct i2c_adapter *adapter)
{
	printk("siema siema stm32_func");
	return I2C_FUNC_SMBUS_WORD_DATA; // I2C_FUNC_I2C
}

static struct i2c_algorithm stm32_algorithm = {
	.smbus_xfer	= stm32_access,
	.functionality	= stm32_func,
};

static struct i2c_adapter stm32_adapter = {
	.owner		= THIS_MODULE,
	.class		= I2C_CLASS_HWMON,
	.algo		= &stm32_algorithm,
	.nr		= 7,
	.name		= "stm32_adapter",
};

static int __init stm32_init(void)
{
	return i2c_add_adapter(&stm32_adapter);
}

static void __exit stm32_exit(void)
{
	i2c_del_adapter(&stm32_adapter);
}

MODULE_DESCRIPTION("stm32 adapter");
MODULE_LICENSE("GPL");

module_init(stm32_init);
module_exit(stm32_exit);
