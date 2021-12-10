# Makefile
FQBN        = esp32:esp32:esp32
DATA        = data.log
SCRIPT			= make_graph.py

.PHONY: build upload graph

build: make_graph_data/make_graph_data.ino
	arduino-cli compile -b $(FQBN) make_graph_data

upload: 
	arduino-cli upload -p /dev/ttyUSB0 -b $(FQBN) make_graph_data

graph:; python $(SCRIPT) $(DATA)
