# Builds the app using the cmake script
.PHONY: build
build:
	@mkdir -p build 
	@cd build && cmake .. && make all && ./drawww

# Debugs runtime errors using the lldb debugger
.PHONY: debug
debug:
	@cd build && lldb drawww
