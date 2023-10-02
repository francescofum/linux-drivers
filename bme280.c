
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/uaccess.h>



/* private structure to hold device info */ 
struct bme280_dev {
        struct i2c_client *client; 
        struct miscdevice bme280_miscdevice;
        char name[8];
};

/* Read the values of pressure humidty and temperature from the sensor */
static ssize_t bme280_read_file(struct file *file, char __user *userbuf,
                size_t count, loff_t *ppos)
{
        // TODO:      
        int ret;
 
        return ret;
}

/* Write from userland to the bme280 sensor */
static ssize_t bme280_write_file(struct file *file, const char __user *userbuf,
                size_t count, loff_t *ppos)
{
        int ret; 
        

        return ret; 
}
/* File operations for the misc device.
 * Read/Write 
 */
static const struct file_operations bme280_fops = {
        .owner = THIS_MODULE,
        .read = bme280_read_file,
        .write = bme280_write_file, 
};


/* Probe function which does the following:
 *      - Creates a priv structure 
 *      - initialises and registers a misc device 
 *      - initialises and registers an i2c client
 * */
static int bme280_probe(struct i2c_client *client,
                const struct i2c_device_id *id)
{
        static int counter = 0; /* Keep track of how many BME280 sensors we have */
        
        /* Pointer to a the data structure which will contain both 
         * the misc device and the i2c client. This will be accessible
         * by both, from the file->private_data (followed by cointainer_of) in the misc device
         * and through the private data field of the i2c client.
         * */
        struct bme280_dev *bme280;

        /* Allocate new structure */ 
        bme280 = devm_kzalloc(&client->dev, sizeof(struct bme280_dev), GFP_KERNEL);

        /* Store the pointer to the i2c client so it will be accessible for the i2c calls. */
        i2c_set_clientdata(client, bme280);

        /* Store pointer  to the i2c client in the priv struct */
        bme280->client = client;

        /* Initialise the misc device */
        sprintf(bme280->name, "bme280%02d", counter++);
        dev_info(&client->dev,
                        "bme280_probe is executing for %s\n", bme280->name);
        bme280->bme280_miscdevice.name = bme280->name;
        bme280->bme280_miscdevice.minor = MISC_DYNAMIC_MINOR;
        bme280->bme280_miscdevice.fops = &bme280_fops; 

        /* Register the misc device */
        return misc_register(&bme280->bme280_miscdevice);

        dev_info(&client->dev,
                        "bme280_probe finished with %s\n", bme280->name);

        return 0;

}

/* Function called when driver is removed with rmmod. */
static int bme280_remove(struct i2c_client *client)
{


}

static const struct of_device_id bme280_dt_ids[] = {
        { .compatible = "ff,bme280ff"},
        {},
};
MODULE_DEVICE_TABLE(of, bme280_dt_ids);

static const struct i2c_device_id i2c_ids[] = {
        { .name = "bme280ff", },
        { } 
};
MODULE_DEVICE_TABLE(i2c, i2c_ids);

static struct i2c_driver bme280_driver = {
        .driver = {
                .name = "bme280ff",
                .owner = THIS_MODULE,
                .of_match_table = bme280_dt_ids,
        },
        .probe = bme280_probe,
        .remove = bme280_remove,
        .id_table = i2c_ids,
};

module_i2c_driver(bme280_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Francesco Fumagalli");
MODULE_DESCRIPTION("Driver for the BME280 Pressure/Temperature/Humidity sensor.");

