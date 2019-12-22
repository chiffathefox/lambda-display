
ECHO_PREFIX=Makefile: 
ARDUINO_ROOT=/home/denis/hardware/arduino/arduino
ARDUINO=$(ARDUINO_ROOT)/arduino
TARGET=lambda-display
TTY=/dev/ttyUSB0
TTY_BAUD=115200


all: install tty


install:
	@echo "$(ECHO_PREFIX)Compiling and installing ..."
	@echo

	$(ARDUINO) --upload "$(TARGET)" --port $(TTY)


tty:
	@screen $(TTY) $(TTY_BAUD)
