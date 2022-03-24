#ifndef I2C_LCD
#define I2C_LCD
#include "i2clcd_main.h"
#endif

#ifndef I2C_FUNC
#define I2C_FUNC
#include "lcd_functions.h"
#endif

#include <linux/seq_file.h> 

int init_char(struct i2c_client *client);
void char_cleanup(int device_created);
int lcd_dev_open(struct inode *inode, struct file *file);
int lcd_dev_release(struct inode *inode, struct file *file);
long lcd_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
ssize_t lcd_dev_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
ssize_t lcd_dev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);

#define SUCCESS 0
#define BUF_LEN 80