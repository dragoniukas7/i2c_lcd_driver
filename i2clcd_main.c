#ifndef I2C_FUNC
#define I2C_FUNC
#include "lcd_functions.h"
#endif

#ifndef CHAR_DEVICE
#define CHAR_DEVICE
	#include "char_device.h"
#endif

static struct i2c_adapter *adapter;
struct i2c_client *global_client;

MODULE_LICENSE("Dual BSD/GPL");

/*Register the table that identifies the device handled by this device driver*/
/*The important thing is the first name field. This determines the device name. The back is the data that can be used freely with this driver. Insert pointers and identification numbers*/
static struct i2c_device_id i2clcd_idtable[] = {
	{"i2clcd", 0},
	{}};

MODULE_DEVICE_TABLE(i2c, i2clcd_idtable);

static int i2clcd_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int version;
	printk("i2c_lcd_probe\n");
	printk("id.name = %s, id.driver_data = %ld", id->name, id->driver_data);
	printk("slave address = 0x%02X\n", client->addr);

	/*Usually here to check if the device is supported by this driver*/

	version = i2c_smbus_read_byte_data(client, 0x0f);
	printk("id = 0x%02X\n", version);

	global_client = client;

	lcd_init(client);
	init_char(client);

	return 0;
}

static int i2clcd_remove(struct i2c_client *client)
{
	printk("i2clcd_remove\n");
	return 0;
}

static struct i2c_driver i2clcd_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
	.id_table = i2clcd_idtable, // I2C devices supported by this device driver
	.probe = i2clcd_probe,		// Process called when the target I2C device is recognized
	.remove = i2clcd_remove,	// Process called when the target I2C device is removed
};


static int device_init(void)
{
	int ret;

	printk("i2clcd_init\n");

	/*Register this device driver as a device driver that uses the I2C bus.*/
	
	printk("get adapter:");

	udelay(100);

	adapter = i2c_get_adapter(1);
	if (!adapter)
	{
		printk("Failed to get adapter");
		return -EINVAL;
	}
	printk("return: %d", ret);

	printk("add driver:");

	ret = i2c_add_driver(&i2clcd_driver);
	if (ret)
	{
		printk("Driver added?");

		i2c_put_adapter(adapter);
		return ret;
	}
	return 0;
}

static void device_exit(void)
{
	turn_off(global_client);
	printk("i2clcd_exit\n");
	char_cleanup(1);

	i2c_del_driver(&i2clcd_driver);
}

module_init(device_init);
module_exit(device_exit);