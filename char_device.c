#ifndef CHAR_DEVICE
#define CHAR_DEVICE
#include "char_device.h"
#endif

#define NAME "i2clcd"

static int major = -1;
static struct cdev mycdev;
static struct class *myclass = NULL;
struct i2c_client *glob_client;

int Device_Open = 0;

static char msg[BUF_LEN];
static char *msg_Ptr;

static struct file_operations fops = {
    .write = lcd_dev_write};

void char_cleanup(int device_created)
{
    if (device_created)
    {
        device_destroy(myclass, major);
        cdev_del(&mycdev);
    }
    if (myclass)
        class_destroy(myclass);
    if (major != -1)
        unregister_chrdev_region(major, 1);
}

int init_char(struct i2c_client *client)
{
    int device_created = 0;

    glob_client = client;

    /* cat /proc/devices */
    if (alloc_chrdev_region(&major, 0, 1, NAME "_proc") < 0)
        goto error;
    /* ls /sys/class */
    if ((myclass = class_create(THIS_MODULE, NAME "_sys")) == NULL)
        goto error;
    /* ls /dev/ */
    if (device_create(myclass, NULL, major, NULL, NAME "_dev") == NULL)
        goto error;
    device_created = 1;
    cdev_init(&mycdev, &fops);
    if (cdev_add(&mycdev, major, 1) == -1)
        goto error;
    return 0;
error:
    char_cleanup(device_created);
    return -1;
}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
ssize_t
lcd_dev_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
    int ret;
    char *text = (char *)kmalloc(len, GFP_KERNEL);
    if (NULL == text)
    {
        return -ENOMEM;
    }

    ret = copy_from_user(text, buf, len);

    if(ret == -1)
    return -1;
    
    text[len - 1] = 0x00;

    lcd_print(glob_client, text);

    kfree(text);
    return len;
}
