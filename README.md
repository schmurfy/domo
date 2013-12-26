
# introduction

This is my domotic project, it may or may not be of any use to someone else but here is how to use it:

# Requirements

You will need inotools to build/upload the project to an arduino chip as well as the arduino IDE, the reason is that the arduino IDE is... horrible and even worse when working with more than one file which is a joke for any real work.


# projects parts

There is two major parts for now in this project:
- the sensor is the code designed to run on a standalone atmega328 chip on battery and sends on a regular basis the metrics to the controller(s).
- the controller is the part which receives the metrics and transmit them to a linux host, in my case I use a (http://ninjablocks.com/collections/ninja-blocks/products/ninja-blocks-kit)[NinjaBlock] as controller but you may also want to use a raspberyPi + arduino.

# How does it work

Currently I use ebay 433MHz transmitter/receivers to communicate with VirtualWire on top of them and a basic XOR encryption.


# get to work

If you want to try this you can find my current schematics (https://upverter.com/schmurfy/efad0e9c69749d6c/Sensor/)[here], then uploading the code is a simple matter of going into the right folder ("apps/sensors", "apps/controller") and running:

```bash
$ ino build && ino upload
```

To use it without any arguments you need to create a ino.ini file in the folder with this in it (where _/dev/tty.xxx_ is the path to the usb port on which the arduino is accessible ):

```ini
[build]
board-model = uno

[upload]
board-model = uno
serial-port = /dev/tty.xxx

[serial]
serial-port = /dev/tty.xxx
```
