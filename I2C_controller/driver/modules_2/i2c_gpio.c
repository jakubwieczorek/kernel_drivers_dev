#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include<linux/workqueue.h>
#include<linux/i2c.h>

MODULE_LICENSE("GPL");

#define SUCCESS 0
#define DEVICE_NAME "my_i2c"

// driver registration structures
dev_t my_dev=0;
struct cdev *my_cdev = NULL;
static struct class *my_class = NULL;

ssize_t my_read2(struct file *filp, char __user *buf, size_t count, loff_t *off)
{
	return 0;
}

ssize_t my_write(struct file *filp, const char __user *buf, size_t count, loff_t *off)
{
	return 0;
}	

static int my_open(struct inode *inode, struct file *file)
{
  return SUCCESS;
}

static int my_release(struct inode *inode, struct file *file)
{
  return SUCCESS;
}

struct file_operations fops = {
  .owner = THIS_MODULE,
  .read=my_read2,
  .write=my_write,
  .open=my_open,
  .release=my_release,
};

static void my_cleanup(void)
{
  printk(KERN_ALERT "I2C driver deregistration\n");
  
  /* delete driver */
  if(my_dev && my_class) 
  {
    device_destroy(my_class,my_dev);
  }

  if(my_cdev) 
  {
	  cdev_del(my_cdev);
  }
  my_cdev=NULL;
 
  /* free driver number */
  unregister_chrdev_region(my_dev, 1);
  
  /* unregister class */
  if(my_class) 
  {
    class_destroy(my_class);
    my_class=NULL;
  }
}

static int my_init(void)
{
  int res;
  printk(KERN_ALERT "I2C driver registration\n");

  /* new class creation - to cooperate with udev system */
  my_class = class_create(THIS_MODULE, "my_class");
  if (IS_ERR(my_class)) 
  {
    printk(KERN_ERR "Error creating my_class class.\n");
    my_cleanup();
    return PTR_ERR(my_class);
  }

  /* device number allocation */
  res=alloc_chrdev_region(&my_dev, 0, 1, DEVICE_NAME);
  if(res) 
  {
    printk ("<1>Alocation of the device number for %s failed\n", DEVICE_NAME);
    my_cleanup();
    return res;
  };  
  my_cdev = cdev_alloc();
  if(my_cdev==NULL) 
  {
    printk (KERN_ERR "Allocation of cdev for %s failed\n", DEVICE_NAME);
    my_cleanup();
    return -ENODEV;
  }
  my_cdev->ops = &fops;
  my_cdev->owner = THIS_MODULE;
  
  /* adding new character devicem, there is also block device but not for these case */
  res=cdev_add(my_cdev, my_dev, 1);
  if(res) 
  {
    printk (KERN_ERR "Registration of the device number for %s failed\n", DEVICE_NAME);
    my_cleanup();
    return res;
  };
  
  /* new device creation in system */
  device_create(my_class, NULL, my_dev, NULL, "my_dev%d", MINOR(my_dev));
  printk (KERN_ALERT "Registeration is a success. The major device number %s is %d.\n", DEVICE_NAME, MAJOR(my_dev));
 
  return SUCCESS;
}

module_init(my_init);
module_exit(my_cleanup);

