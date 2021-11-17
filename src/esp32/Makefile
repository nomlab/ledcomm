# Makefile

TARGETS     = receive_string receive_wifi_info

FQBN        = esp32:esp32:esp32

.PHONY: build 

build: build_receive_string build_receive_wifi_info

build_receive_string: receive_string/receive_string.ino
	arduino-cli compile -b $(FQBN) receive_string

build_receive_wifi_info: receive_wifi_info/receive_wifi_info.ino
	arduino-cli compile -b $(FQBN) receive_wifi_info

upload_receive_string: 
	arduino-cli upload -p /dev/ttyUSB0 -b $(FQBN) receive_string

upload_receive_wifi_info: 
	arduino-cli upload -p /dev/ttyUSB0 -b $(FQBN) receive_wifi_info
