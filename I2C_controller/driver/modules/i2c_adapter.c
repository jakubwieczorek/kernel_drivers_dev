// it respresents i2c bus. Linux is always master.

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stddef.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>

struct file *file_open(const char *path, int flags, int rights) 
{
    struct file *filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(KERNEL_DS);
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if (IS_ERR(filp)) 
    {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

void file_close(struct file *file) 
{
    filp_close(file, NULL);
}

static s32 stm32_access(struct i2c_adapter *adap, u16 addr,
		       unsigned short flags, char read_write,
		       u8 command, int size, union i2c_smbus_data *data)
{
	char *buf="Hello from i2c_adapter";
	void *uart_data2;
	loff_t size2;
	struct file *stm32_uart_file;
	
	mm_segment_t old_fs;
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
			stm32_uart_file = filp_open("/demo/test.txt", O_WRONLY, 0);
			if(stm32_uart_file == NULL)
			{
				return 0;
			}
			kernel_write(stm32_uart_file, buf, 22, &size2);
			filp_close(stm32_uart_file, NULL);
		} else 
		if (read_write == I2C_SMBUS_READ)
		{ 
			kernel_read_file_from_path("/demo/test.txt", &uart_data2, &size2, 0, READING_POLICY);
			printk("Read data: %s", (char*)uart_data2);
		}
		
	}

	return 0;
}

static u32 stm32_func(struct i2c_adapter *adapter)
{
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
