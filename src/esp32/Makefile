# Makefile

TARGETS     = receive_string receive_wifi_info

FQBN        = esp32:esp32:esp32
PKG         = esp32:esp32

.PHONY: install setup build upload
# all: $(TARGETS)
install:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/.local/bin sh
	export PATH=$PATH:~/.local/bin

setup: 
	arduino-cli core install $(PKG)

build: $(TARGETS)
	$(foreach sketch,$(TARGETS),arduino-cli compile -b $(FQBN) $(sketch))

upload_receive_string: 
	arduino-cli upload -p /dev/ttyUSB0 -b $(FQBN) receive_string

upload_receive_wifi_info: 
	arduino-cli upload -p /dev/ttyUSB0 -b $(FQBN) receive_string
