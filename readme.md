### Cylon-6: An EDID Fuzzer Based on Raspberry Pi Hardware

---

#### 1. Introduction

* Demo code for [Cylon-6](https://www.blackhat.com/us-19/arsenal/schedule/index.html#cylon--an-edid-fuzzer-based-on-raspberry-pi-hardware-17030), it is based on [rsta2/circle](https://github.com/rsta2/circle)

* The pdf: [BH_USA_Arsenal_Cylon6.pdf](https://github.com/kings-way/cylon-6/blob/master/pdf/BH_USA_Arsenal_Cylon6.pdf)

* The demo video: [https://youtu.be/K247L3hgy9Y](https://youtu.be/K247L3hgy9Y)


#### 2. How to run the demo

	git clone https://github.com/rsta2/circle
	git clone https://github.com/kings-way/cylon-6 circle/sample/cylon-6
	
	# Set RPi version in Rules.mk (RASPPI ?= 3 here)
	sed -i 's/RASPPI.*?=.*/RASPPI ?= 3/g' Rules.mk

	# build 'circle'
	apt install gcc-arm-none-eabi
	./makeall -j4
	
	# build sample 'cylon-6'
	cd sample/cylon-6/
	make

#### 3. Notice
#TODO

#### 4. Refer

https://en.wikipedia.org/wiki/Extended_Display_Identification_Data

https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf

https://www.raspberrypi.org/forums/viewtopic.php?t=87804

https://raspberrypi.stackexchange.com/questions/76109/raspberry-as-an-i2c-slave

http://abyz.me.uk/rpi/pigpio/cif.html#bscXfer

https://arduino.stackexchange.com/questions/58370/how-to-manage-variable-i2c-read-lengths-

https://www.geeks3d.com/20091230/vga-hack-how-to-make-a-vga-dummy-plug/

https://elixir.bootlin.com/linux/v4.18.6/source/drivers/i2c

https://elixir.bootlin.com/linux/v4.18.6/ident/__i2c_transfer

https://i2c.info/i2c-bus-specification
