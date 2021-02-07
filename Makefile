FQBN := "arduino:avr:nano:cpu=atmega328old"
#PORT := "/dev/ttyACM0"
PORT := "/dev/ttyUSB0"

.DEFAULT_GOAL := all

.PHONY: all
all: compile upload

.PHONY: install-core
install-core:
	arduino-cli core install arduino:avr

.PHONY: compile
compile:
	arduino-cli compile --fqbn $(FQBN)

.PHONY: upload
upload:
	arduino-cli upload --fqbn $(FQBN) --port $(PORT)
