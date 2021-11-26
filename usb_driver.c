#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>


//probe function
//solicitó la inserción del dispositivo si y solo si ningún otro controlador se nos haya adelantado (sondeo)
/*
static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	printk(KERN_INFO "[*] UB Pen drive (%04X:%04X) conectado\n", id->idVendor, id->idProduct);
	return 0; // indica que administraremos este dispositivo
}
*/

static struct usb_device *device;

static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;

    iface_desc = interface->cur_altsetting;
    printk(KERN_INFO "Pen i/f %d seondeando: (%04X:%04X)\n",
            iface_desc->desc.bInterfaceNumber,
            id->idVendor, id->idProduct);
    printk(KERN_INFO "ID->bNumEndpoints: %02X\n",
            iface_desc->desc.bNumEndpoints);
    printk(KERN_INFO "ID->bInterfaceClass: %02X\n",
            iface_desc->desc.bInterfaceClass);

    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",
                i, endpoint->bEndpointAddress);
        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n",
                i, endpoint->bmAttributes);
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",
                i, endpoint->wMaxPacketSize,
                endpoint->wMaxPacketSize);
    }

    device = interface_to_usbdev(interface);
    return 0;
}

//disconnect
static void pen_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "[*] UB Pen drive: %d desconectado \n", device->devnum);
	//printk(KERN_INFO "[*] UB Pen drive desconectado \n");
}


static struct usb_device_id pen_table[] = 
{
	{USB_DEVICE(0x0951, 0x1665)}, // informacion obtenida usando el comando lsusb en la terminal
	// {USB_DEVICE(v,p)},
	{} //se termina la entrada
};
MODULE_DEVICE_TABLE(usb, pen_table);


//usb_driver
static struct usb_driver pen_driver = 
{
	.name 		= "UB - Driver usb",
	.id_table 	= pen_table,
	.probe 	= pen_probe,
	.disconnect 	= pen_disconnect,
};


static int __init pen_init(void)
{
	int ret = -1;
	printk(KERN_INFO "[*] UB Constructor del driver");
	printk(KERN_INFO "\t Registrando driver en el Kernel");
	ret = usb_register(&pen_driver);
	printk(KERN_INFO "\t Registraciòn completada");
	
	return ret;
}

static void __exit pen_exit(void)
{
	printk(KERN_INFO "[*] UB Destructor del driver");
	usb_deregister(&pen_driver);
	printk(KERN_INFO "Baja del driver completada");
}

module_init(pen_init);
module_exit(pen_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("UB");
MODULE_DESCRIPTION("Registro de pendrive via usb");
