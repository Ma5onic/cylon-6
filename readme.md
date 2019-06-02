### Cylon-6: An EDID Fuzzer Based on Raspberry Pi Hardware

---

#### 1. Prepare the code

	git clone https://github.com/rsta2/circle
	git clone https://github.com/kings-way/cylon-6 circle/sample/cylon-6
	
	# patch the code
	cd circle
	patch -p1 < sample/cylon-6/i2cslave.patch

	# Set RPi version in Rules.mk (RASPPI ?= 3 here)
	sed -i 's/RASPPI.*?=.*/RASPPI ?= 3/g' Rules.mk


#### 2. Build
	
	apt install gcc-arm-none-eabi
	./makeall -j4
	
	cd sample/cylon-6/
	make


#### 3. Run
 #TODO
