#if !defined(GPIO_INTERRUPT_DEV_H)
#define GPIO_INTERRUPT_DEV_H
#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#define LICENSE_V "GPL"
#define DRV_NAME "gpio_interrupt_drv"
#define DRV_MODULE_VERSION "1.0"
#define DRV_MODULE_AUTHOR "Shanaka"
#define DRV_MODULE_DESCRIPTION "GPIO test driver"

#define DEVICE_NAME "gpio_dev"
#define DEVICE_CLASS "gpio_device"

/* Userland device properties*/
static dev_t device_no;
static struct class *device_class;
static struct cdev device_cdev;

/* GPIO char buffer */
static char gpio_buffer[1];
// GPIO numbers
unsigned int gpios[] = {101};
char *gpio_names[] = {"GPIO_110"};
// interrupt number for gpio array
unsigned int irq_numbers[] = {0};

/**
 * @brief Handle interrupt routine
 * @param int irq_no interrupt identifier
 * @param void* device indentifier
 * @param irqreturn_t interrupt process return
 */
static irqreturn_t interrupt_handler(int irq_no, void *dev_id);

/**
 * @brief Return kernel data back to user buffer
 * @param input file pointer
 * @param char buffer pointer to access user data
 * @param size size to read
 * @param longlong offset
 */
static ssize_t return_data_to_user(struct file *File, char *user_buffer, size_t count, loff_t *offset);

/**
 * @brief Process user buffer data in kernel
 * @param input file pointer
 * @param char buffer pointer to access user data
 * @param size size to read
 * @param longlong offset
 */
static ssize_t process_user_data(struct file *File, const char *user_buffer, size_t count, loff_t *offset);

/**
 * @brief This function notify when the device driver file is open from userland
 * @param struct inode pointer to device_file instance
 * @param struct file pointer to a file instance
 * @return int 0-success
 */
static int user_device_open(struct inode *device_file, struct file *instance);

/**
 * @brief This function notify when the device driver file is closed from userland
 * @param struct inode pointer to device_file instance
 * @param struct file pointer to a file instance
 * @return int 0-success
 */
static int user_device_release(struct inode *device_file, struct file *instance);

/**
 * @brief structure to save the file operations functions
 * @param owner owner of the operation
 * @param open open operation function
 * @param release release operation function
*/
static struct file_operations file_ops = {
	.owner = THIS_MODULE,
	.open = user_device_open,
	.release = user_device_release,
    .read = return_data_to_user,
    .write = process_user_data
};

/**
 * @brief this function initialize the module
 * @return int 0-success 1-failure
*/
static int __init ModuleInit(void);


/**
 * @brief this function remove the module
 * @return int 0-success 1-failure
*/
static void __exit ModuleExit(void);


#endif