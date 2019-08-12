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
