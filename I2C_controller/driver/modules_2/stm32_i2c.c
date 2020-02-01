#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/hwmon.h>
#include <linux/semaphore.h>

#define REG_TEMP_HYST	0x01
#define REG_TEMP_OS	0x02

struct chip_data {
	struct i2c_client *client;
	struct semaphore update_lock;
	char valid;			/* !=0 if following fields are valid */
	unsigned long last_updated;	/* In jiffies */
	u16 temp_input;			/* Register values */
	u16 temp_max;
	u16 temp_hyst;
};


static int stm32_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int stm32_remove(struct i2c_client *client);

static struct i2c_device_id stm32_idtable[] = {
	{ "stm32_id", 0 },
	{}
};

MODULE_DEVICE_TABLE(i2c, stm32_idtable);

static struct i2c_driver stm32_i2c_driver = {
	.driver = {
		.name	= "stm32_i2c",
		.owner  = THIS_MODULE,

	},
	.id_table	= stm32_idtable,
	.probe 		= stm32_probe,
	.remove		= stm32_remove,
};


static void chip_update_client(struct i2c_client *client)
{
	struct chip_data *data = i2c_get_clientdata(client);

	down(&data->update_lock);
	dev_dbg(&client->dev, "%s\n", __FUNCTION__);
	++data->temp_input;
	++data->temp_max;
	++data->temp_hyst;
	data->last_updated = jiffies;
	data->valid = 1;
	up(&data->update_lock);
}

#define show(value)	\
static ssize_t show_##value(struct device *dev, struct device_attribute * attr, char *buf)	\
{								\
	struct i2c_client *client = to_i2c_client(dev);		\
	struct chip_data *data = i2c_get_clientdata(client);	\
								\
	chip_update_client(client);				\
	return sprintf(buf, "%d\n", data->value);		\
}
show(temp_max);
show(temp_hyst);
show(temp_input);

#define set(value, reg)	\
static ssize_t set_##value(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)	\
{								\
	struct i2c_client *client = to_i2c_client(dev);		\
	struct chip_data *data = i2c_get_clientdata(client);	\
	int temp = simple_strtoul(buf, NULL, 10);		\
								\
	down(&data->update_lock);				\
	data->value = temp;					\
	up(&data->update_lock);					\
	return count;						\
}
set(temp_max, REG_TEMP_OS);
set(temp_hyst, REG_TEMP_HYST);

static DEVICE_ATTR(temp_max, S_IWUSR | S_IRUGO, show_temp_max, set_temp_max);
static DEVICE_ATTR(temp_min, S_IWUSR | S_IRUGO, show_temp_hyst, set_temp_hyst);
static DEVICE_ATTR(temp_input, S_IRUGO, show_temp_input, NULL);

static int stm32_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct chip_data *data = NULL;
	int err = 0;
	printk("probe");

	data = kmalloc(sizeof(*data), GFP_KERNEL);
	if (!data) {
		err = -ENOMEM;
		goto error;
	}

	memset(data, 0x00, sizeof(*data));

	data->client = client;
	i2c_set_clientdata(client, data);

	data->valid = 0;
	#ifndef init_MUTEX
	sema_init(&data->update_lock,1);
	#else
	init_MUTEX(&data->update_lock);
	#endif

	/* Register sysfs files */
	device_create_file(&client->dev, &dev_attr_temp_max);
	device_create_file(&client->dev, &dev_attr_temp_min);
	device_create_file(&client->dev, &dev_attr_temp_input);

	return 0;

error:
	kfree(data);
	return err;
	return 0;
}

static int stm32_remove(struct i2c_client *client)
{
	printk("remove");
	return 0;
}

static int __init i2c_init(void)
{
	printk("init stm32_i2c");
	return i2c_add_driver(&stm32_i2c_driver);
}
static void __exit i2c_exit(void)
{
	printk("exit stm32_i2c");
	i2c_del_driver(&stm32_i2c_driver);
}

module_init(i2c_init);
module_exit(i2c_exit);

MODULE_LICENSE("GPL");
