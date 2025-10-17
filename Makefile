# Builds the app using the cmake script
.PHONY: build
build:
	@mkdir -p build 
	@cd build && cmake .. && make all && ./drawww

.PHONY: clean
clean:
	@rm -rf build

# Debugs runtime errors using the lldb debugger
.PHONY: debug
debug:
	@cd build && lldb drawww

# Builds the wasm files
.PHONY: wasm
wasm:
	@mkdir -p build-wasm
	@cd build-wasm && emcmake cmake .. && emmake make
	@echo "Built wasm artifacts in build-wasm/"

	@# Next we copy the built artifacts to the frontend folder
	@cd frontend
	@mv build-wasm/drawww.wasm build-wasm/drawww.js build-wasm/drawww.data frontend/
	@echo "Copied wasm artifacts to frontend/"

# Runs a HTTP server to serve the drawww web assembly app locally
.PHONY: serve-wasm 
serve-wasm:
	@python -m http.server --directory ./frontend 8080