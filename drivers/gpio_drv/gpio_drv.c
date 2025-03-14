#include "gpio_drv.h"

MODULE_LICENSE(LICENSE_V);
MODULE_AUTHOR(DRV_MODULE_AUTHOR);
MODULE_DESCRIPTION(DRV_MODULE_DESCRIPTION);
MODULE_VERSION(DRV_MODULE_VERSION);

static ssize_t return_data_to_user(struct file *File, char *user_buffer, size_t count, loff_t *offset){
	int to_copy, not_copied, delta, i;
	to_copy = min(count, sizeof(gpio_buffer));
	for(i=0; i<to_copy; i++)
		gpio_buffer[i] = gpio_get_value(gpios[i]);
	not_copied = copy_to_user(user_buffer, gpio_buffer, to_copy);
	delta = to_copy - not_copied;
	printk(KERN_DEBUG "Copy data to user buffer \n");
	return delta;
}

static ssize_t process_user_data(struct file *File, const char *user_buffer, size_t count, loff_t *offset){
	int to_copy, not_copied, delta, i;
	to_copy = min(count, sizeof(gpio_buffer));
	not_copied = copy_from_user(gpio_buffer, user_buffer, to_copy);
	delta = to_copy - not_copied;
	for(i=0; i<to_copy; i++)
		gpio_set_value(gpios[i], ((gpio_buffer[i]) & (1<<i)) >> i);
	printk(KERN_DEBUG "Processed user data \n");
	return delta;
}

static int user_device_open(struct inode *device_file, struct file *instance){
	printk(KERN_NOTICE "Device /dev/ was opened\n");
	return 0;
}

static int user_device_release(struct inode *device_file, struct file *instance){
	printk(KERN_NOTICE "Device /dev/ was closed\n");
	return 0;
}

static int __init ModuleInit(void){
	int retval,i = 0;
	printk("%s()\n", __func__);
	printk(KERN_NOTICE "Loading " DRV_NAME " Driver \n");
	// TODO make device node number dynamic
	// register the device to userland
	if (alloc_chrdev_region(&device_no, 0, 1, DEVICE_NAME) <0){
		printk(KERN_ALERT "failed to register the char device %s\n",DEVICE_NAME);
		return -1;
	}
	printk(KERN_NOTICE "successufully registered device %d\n", device_no);
	// amd64
	//if ((device_class = class_create(DEVICE_CLASS)) == NULL){
	// arm64
	if ((device_class = class_create(THIS_MODULE, DEVICE_CLASS)) == NULL){
		printk(KERN_ALERT "failed to create device class %s\n",DEVICE_CLASS);
		unregister_chrdev_region(device_no, 1);
		return -1;
	}
	printk(KERN_NOTICE "successufully created class %d\n", device_no);
	if ((device_create(device_class, NULL, device_no, NULL, DEVICE_NAME)) == NULL){
		printk(KERN_ALERT "failed to create device file  %s %d", DEVICE_NAME, device_no);
		class_destroy(device_class);
		unregister_chrdev_region(device_no, 1);
		return -1;
	}
	printk(KERN_NOTICE "successufully created device %d\n", device_no);
	cdev_init(&device_cdev, &file_ops);
	printk(KERN_NOTICE "successufully initialized device %d\n", device_no);
	if (cdev_add(&device_cdev, device_no, 1) == -1){
		printk(KERN_ALERT "failed to register device %s %d\n", DEVICE_NAME, device_no);
		class_destroy(device_class);
		unregister_chrdev_region(device_no, 1);
		device_destroy(device_class, device_no);
		return -1;
	}

	printk(KERN_NOTICE "successufully added device %d\n", device_no);
    for(i=0; i<sizeof(gpio_buffer); i++) {
		if(gpio_request(gpios[i], gpio_names[i])) {
			printk(KERN_ALERT "Error Init GPIO %d\n", gpios[i]);
			for(;i>=0; i--){
				gpio_free(gpios[i]);
			}
			class_destroy(device_class);
			unregister_chrdev_region(device_no, 1);
			device_destroy(device_class, device_no);
			return -1;
		}
	}
	printk(KERN_NOTICE "successufully initialize device gpio %d\n", device_no);
	for(i=0; i<sizeof(gpio_buffer); i++) {
		if(gpio_direction_output(gpios[i], 0)) {
			printk(KERN_ALERT "Error setting GPIO %d to output\n", i);
			for(;i>=0; i--){
				gpio_free(gpios[i]);
			}
			class_destroy(device_class);
			unregister_chrdev_region(device_no, 1);
			device_destroy(device_class, device_no);
			return -1;
		}
	}
	printk(KERN_NOTICE "successufully module initialized %d\n", device_no);
	return retval;
}

static void __exit ModuleExit(void){
	printk("%s()\n", __func__);
	int i;
	for(i=0; i<sizeof(gpio_buffer); i++)
		gpio_free(gpios[i]);
	cdev_del(&device_cdev);
	class_destroy(device_class);
	unregister_chrdev_region(device_no, 1);
	device_destroy(device_class, device_no);
	// Following commented section is to setup a dummy device with static device number
	//unregister_chrdev(99, "device_number");
	printk(KERN_NOTICE "Unloading " DRV_NAME " Driver \n");
}

/**
 * Configure module initialization and exit functions
 */
module_init(ModuleInit);
module_exit(ModuleExit);
  
