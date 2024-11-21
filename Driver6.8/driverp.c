#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/tty.h>       // Definiciones completas de tty_port y ktermios
#include <uapi/linux/termios.h> // En caso de problemas con ktermios
#include <linux/usb/serial.h>
#include <linux/tty_flip.h>

#define CH341_VENDOR_ID  0x1A86
#define CH341_PRODUCT_ID 0x7523

#define CH341_REQ_WRITE_REG 0x9A
#define CH341_REG_DIVISOR   0x13
#define CH341_REQ_MODEM_CTRL 0xA4
#define CH341_BIT_RTS       (1 << 6)
#define CH341_BIT_DTR       (1 << 5)

#define DEFAULT_BAUD_RATE 921600

// Tabla de dispositivos compatibles
static const struct usb_device_id ch341_id_table[] = {
    { USB_DEVICE(CH341_VENDOR_ID, CH341_PRODUCT_ID) },
    {}
};
MODULE_DEVICE_TABLE(usb, ch341_id_table);

// Configuración del baud rate
static int ch341_set_baudrate(struct usb_device *dev, unsigned int baud_rate) {
    int divisor = 0x100 - (48000000 / 16 / baud_rate);
    int result = usb_control_msg(dev, usb_sndctrlpipe(dev, 0), CH341_REQ_WRITE_REG,
                                 USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_DIR_OUT,
                                 CH341_REG_DIVISOR, divisor, NULL, 0, 1000);
    if (result < 0) {
        dev_err(&dev->dev, "Failed to set baud rate: %d\n", result);
    }
    return result;
}

// Control de RTS/DTR
static int ch341_set_handshake(struct usb_device *dev, u8 control) {
    return usb_control_msg(dev, usb_sndctrlpipe(dev, 0), CH341_REQ_MODEM_CTRL,
                           USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_DIR_OUT,
                           ~control, 0, NULL, 0, 1000);
}

// Implementación de ioctl para establecer RTS/DTR
static int ch341_tiocmset(struct tty_struct *tty,
                          unsigned int set, unsigned int clear) {
    struct usb_serial_port *port = tty->driver_data;
    struct usb_device *dev = port->serial->dev;
    u8 control = 0;

    if (set & TIOCM_RTS)
        control |= CH341_BIT_RTS;
    if (set & TIOCM_DTR)
        control |= CH341_BIT_DTR;
    if (clear & TIOCM_RTS)
        control &= ~CH341_BIT_RTS;
    if (clear & TIOCM_DTR)
        control &= ~CH341_BIT_DTR;

    return ch341_set_handshake(dev, control);
}

// Implementación de ioctl para obtener RTS/DTR
static int ch341_tiocmget(struct tty_struct *tty) {
    struct usb_serial_port *port = tty->driver_data;
    u8 control;
    unsigned int result = 0;

    control = ~CH341_BIT_RTS | ~CH341_BIT_DTR; // Simula las señales iniciales
    if (control & CH341_BIT_RTS)
        result |= TIOCM_RTS;
    if (control & CH341_BIT_DTR)
        result |= TIOCM_DTR;

    return result;
}

// Inicialización del puerto
static int ch341_port_probe(struct usb_serial_port *port) {
    struct usb_device *dev = port->serial->dev;
    int r;

    r = ch341_set_baudrate(dev, DEFAULT_BAUD_RATE);
    if (r < 0) {
        dev_err(&port->dev, "Failed to set baud rate: %d\n", r);
        return r;
    }

    r = ch341_set_handshake(dev, CH341_BIT_RTS | CH341_BIT_DTR);
    if (r < 0) {
        dev_err(&port->dev, "Failed to set handshake: %d\n", r);
        return r;
    }

    dev_info(&port->dev, "CH341 port probe completed\n");
    return 0;
}

// Apertura del puerto
static int ch341_open(struct tty_struct *tty, struct usb_serial_port *port) {
    int r;

    r = usb_serial_generic_open(tty, port);
    if (r < 0) {
        dev_err(&port->dev, "Generic open failed: %d\n", r);
        return r;
    }

    dev_info(&port->dev, "CH341 serial port opened\n");
    return 0;
}

// Eliminación del puerto
static void ch341_port_remove(struct usb_serial_port *port) {
    struct usb_device *dev = port->serial->dev;
    ch341_set_handshake(dev, 0);
    dev_info(&port->dev, "CH341 serial port removed\n");
}

// Driver USB serial
static struct usb_serial_driver ch341_device = {
    .driver = {
        .owner = THIS_MODULE,
        .name = "ch341-usbserial",
    },
    .id_table = ch341_id_table,
    .num_ports = 1,
    .port_probe = ch341_port_probe,
    .port_remove = ch341_port_remove,
    .open = ch341_open,
    .tiocmset = ch341_tiocmset,
    .tiocmget = ch341_tiocmget,
};

static struct usb_serial_driver *const serial_drivers[] = {
    &ch341_device, NULL
};

module_usb_serial_driver(serial_drivers, ch341_id_table);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adaptado para ESP32");
MODULE_DESCRIPTION("Driver CH341 para cargar archivos .ino en ESP32");
