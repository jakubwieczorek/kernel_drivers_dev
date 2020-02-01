/**
 * ADC simulator. Signal definition is sinus as an array of 1000
 * samples and max value 4095. ADC according to the sampling_rate gives every sample if 
 * sampling_rate is 1000, every second sample if 500, every 3rd sample
 * if sampling_rate if 333 and so on. Data from driver can be fetched 
 * every two second, if user process try faster, then it will be block and
 * notfied when sample will be available. Every two seconds to emulate 
 * that communication with a device is not immediate and device has an
 * internal queue where samples are stored. 
 *
 * User can set a sampling_rate 
 * by communication with driver via opening virtual file in /dev/my_dev0 
 * and calling dedicated read and write functions or direct communication
 * through echo command like echo 500 > /dev/my_dev0.
 * 
 * To register driver call:
 * insmod module.ko
 * */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include<linux/workqueue.h>

#define BUF_LEN 5

MODULE_LICENSE("GPL");

#define SUCCESS 0
#define DEVICE_NAME "my_dev"

// driver registration structures
dev_t my_dev=0;
struct cdev *my_cdev = NULL;
static struct class *my_class = NULL;
static struct timer_list my_timer;

DECLARE_WAIT_QUEUE_HEAD(wq);

char *ADC_values[] = {
"2048","2060","2073","2086","2099","2112","2125","2138","2150","2163","2176","2189","2202","2215","2227","2240","2253","2266","2279","2291","2304","2317","2330","2342","2355","2368","2380","2393","2406","2419","2431","2444","2456","2469","2482","2494","2507","2519","2532","2544","2557","2569","2582","2594","2606","2619","2631","2643","2656","2668","2680","2692","2705","2717","2729","2741","2753","2765","2777","2789","2801","2813","2825","2837","2849","2861","2872","2884","2896","2908","2919","2931","2942","2954","2966","2977","2988","3000","3011","3023","3034","3045","3056","3068","3079","3090","3101","3112","3123","3134","3145","3155","3166","3177","3188","3198","3209","3220","3230","3241","3251","3261","3272","3282","3292","3302","3313","3323","3333","3343","3353","3363","3372","3382","3392","3402","3411","3421","3430","3440","3449","3458","3468","3477","3486","3495","3504","3513","3522","3531","3540","3549","3558","3566","3575","3583","3592","3600","3609","3617","3625","3633","3641","3649","3657","3665","3673","3681","3689","3696","3704","3711","3719","3726","3734","3741","3748","3755","3762","3769","3776","3783","3790","3797","3803","3810","3816","3823","3829","3836","3842","3848","3854","3860","3866","3872","3878","3883","3889","3895","3900","3906","3911","3916","3921","3927","3932","3937","3942","3946","3951","3956","3961","3965","3970","3974","3978","3983","3987","3991","3995","3999","4003","4006","4010","4014","4017","4021","4024","4027","4031","4034","4037","4040","4043","4046","4048","4051","4054","4056","4059","4061","4063","4066","4068","4070","4072","4074","4075","4077","4079","4080","4082","4083","4085","4086","4087","4088","4089","4090","4091","4092","4092","4093","4094","4094","4094","4095","4095","4095","4095","4095","4095","4095","4094","4094","4094","4093","4092","4092","4091","4090","4089","4088","4087","4086","4085","4083","4082","4080","4079","4077","4075","4074","4072","4070","4068","4066","4063","4061","4059","4056","4054","4051","4048","4046","4043","4040","4037","4034","4031","4027","4024","4021","4017","4014","4010","4006","4003","3999","3995","3991","3987","3983","3978","3974","3970","3965","3961","3956","3951","3946","3942","3937","3932","3927","3921","3916","3911","3906","3900","3895","3889","3883","3878","3872","3866","3860","3854","3848","3842","3836","3829","3823","3816","3810","3803","3797","3790","3783","3776","3769","3762","3755","3748","3741","3734","3726","3719","3711","3704","3696","3689","3681","3673","3665","3657","3649","3641","3633","3625","3617","3609","3600","3592","3583","3575","3566","3558","3549","3540","3531","3522","3513","3504","3495","3486","3477","3468","3458","3449","3440","3430","3421","3411","3402","3392","3382","3372","3363","3353","3343","3333","3323","3313","3302","3292","3282","3272","3261","3251","3241","3230","3220","3209","3198","3188","3177","3166","3155","3145","3134","3123","3112","3101","3090","3079","3068","3056","3045","3034","3023","3011","3000","2988","2977","2966","2954","2942","2931","2919","2908","2896","2884","2872","2861","2849","2837","2825","2813","2801","2789","2777","2765","2753","2741","2729","2717","2705","2692","2680","2668","2656","2643","2631","2619","2606","2594","2582","2569","2557","2544","2532","2519","2507","2494","2482","2469","2456","2444","2431","2419","2406","2393","2380","2368","2355","2342","2330","2317","2304","2291","2279","2266","2253","2240","2227","2215","2202","2189","2176","2163","2150","2138","2125","2112","2099","2086","2073","2060","2048","2035","2022","2009","1996","1983","1970","1957","1945","1932","1919","1906","1893","1880","1868","1855","1842","1829","1816","1804","1791","1778","1765","1753","1740","1727","1715","1702","1689","1676","1664","1651","1639","1626","1613","1601","1588","1576","1563","1551","1538","1526","1513","1501","1489","1476","1464","1452","1439","1427","1415","1403","1390","1378","1366","1354","1342","1330","1318","1306","1294","1282","1270","1258","1246","1234","1223","1211","1199","1187","1176","1164","1153","1141","1129","1118","1107","1095","1084","1072","1061","1050","1039","1027","1016","1005","994","983","972","961","950","940","929","918","907","897","886","875","865","854","844","834","823","813","803","793","782","772","762","752","742","732","723","713","703","693","684","674","665","655","646","637","627","618","609","600","591","582","573","564","555","546","537","529","520","512","503","495","486","478","470","462","454","446","438","430","422","414","406","399","391","384","376","369","361","354","347","340","333","326","319","312","305","298","292","285","279","272","266","259","253","247","241","235","229","223","217","212","206","200","195","189","184","179","174","168","163","158","153","149","144","139","134","130","125","121","117","112","108","104","100","96","92","89","85","81","78","74","71","68","64","61","58","55","52","49","47","44","41","39","36","34","32","29","27","25","23","21","20","18","16","15","13","12","10","9","8","7","6","5","4","3","3","2","1","1","1","0","0","0","0","0","0","0","1","1","1","2","3","3","4","5","6","7","8","9","10","12","13","15","16","18","20","21","23","25","27","29","32","34","36","39","41","44","47","49","52","55","58","61","64","68","71","74","78","81","85","89","92","96","100","104","108","112","117","121","125","130","134","139","144","149","153","158","163","168","174","179","184","189","195","200","206","212","217","223","229","235","241","247","253","259","266","272","279","285","292","298","305","312","319","326","333","340","347","354","361","369","376","384","391","399","406","414","422","430","438","446","454","462","470","478","486","495","503","512","520","529","537","546","555","564","573","582","591","600","609","618","627","637","646","655","665","674","684","693","703","713","723","732","742","752","762","772","782","793","803","813","823","834","844","854","865","875","886","897","907","918","929","940","950","961","972","983","994","1005","1016","1027","1039","1050","1061","1072","1084","1095","1107","1118","1129","1141","1153","1164","1176","1187","1199","1211","1223","1234","1246","1258","1270","1282","1294","1306","1318","1330","1342","1354","1366","1378","1390","1403","1415","1427","1439","1452","1464","1476","1489","1501","1513","1526","1538","1551","1563","1576","1588","1601","1613","1626","1639","1651","1664","1676","1689","1702","1715","1727","1740","1753","1765","1778","1791","1804","1816","1829","1842","1855","1868","1880","1893","1906","1919","1932","1945","1957","1970","1983","1996","2009","2022","2035","2048"
};

const size_t SAMPLES = 1000;

long sampling_rate = 1000;
int done = 1;

void my_timer_callback(struct timer_list *t)
{
	done = 1; // data available after two seconds
	mod_timer(t, jiffies + msecs_to_jiffies(2000));
	wake_up_interruptible(&wq); // wake up user process
}

const char* getSample(void)
{
	static long current_sample_idx = 0; // we don't know were is current sample so 0 is okay
	
	current_sample_idx += SAMPLES / sampling_rate; // calculate were is next sample

	return ADC_values[current_sample_idx % SAMPLES]; // return sample but previously ensure that idx is not bigger than SAMPLES
}

ssize_t my_read2(struct file *filp, char __user *buf, size_t count, loff_t *off)
{
	if(done) // if data available
	{
		const char *msg = getSample(); // calculate sample
		done = 0;
		count = sizeof(msg);
		__copy_to_user(buf, msg, count);
		
		return count;
	} else
	{
		wait_event_interruptible(wq, (done == 1)); //  when no data block user process
		return 0;
	}
	
	return count;
}

ssize_t my_write(struct file *filp, const char __user *buf, size_t count, loff_t *off)
{
  int kstrtol_return;
  long new_sampling_rate;
  if (count <=  BUF_LEN) // if sent data length is <= BUF_LEN, which is 5, so 
  {
	  // convert char user buffer directly to long value, which is sampling rate
  	  kstrtol_return = kstrtoul_from_user(buf, count, 10, &new_sampling_rate); //copy data from user space to kernel space

	  if(!kstrtol_return && new_sampling_rate <= 1000 && new_sampling_rate > 0)
	  {
		sampling_rate = new_sampling_rate; // if no problem with conversion apply new sampling rate
	  	return count;	
	  } else
	  {
		printk(KERN_ERR "Error during reading data, code = %d\n", kstrtol_return);
		return -EINVAL; // parsig error
	  }
  } else 
  {
	  return -ENOSPC; // no space
  }
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
  printk(KERN_ALERT "ADC simulator driver deregistration\n");
  
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
  printk(KERN_ALERT "ADC simulator driver registration\n");

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
 

  // timer: 
  /* setup your timer to call my_timer_callback */
  timer_setup(&my_timer, my_timer_callback, 0);
  /* setup timer interval to 2000 msecs */
  mod_timer(&my_timer, jiffies + msecs_to_jiffies(2000));

  return SUCCESS;
}

module_init(my_init);
module_exit(my_cleanup);
