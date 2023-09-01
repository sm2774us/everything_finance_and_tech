BUILD_DIR = build
DEBUG_DIR = Debug
FLAMEGRAPH = lib/FlameGraph

.PHONY: build

build: clean
	conan install . -if $(BUILD_DIR) --build=missing
	cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -H. -B $(BUILD_DIR)
	make -C $(BUILD_DIR) -j $(shell nproc)
	conan remove -f -s -b -- '*'

tidy:
	astyle --style=linux --exclude=build --recursive *.cpp,*.c,*.h,*.hpp
	find . -type f -name '*.orig' -delete

clean:
	rm -rf $(BUILD_DIR)
	conan remove -f -s -b -- '*'

run:
	$(BUILD_DIR)/bin/matching_service

bench:
	$(BUILD_DIR)/bin/benchmark

test:
	$(BUILD_DIR)/bin/unit_tests

flamegraph:
	rm -rf $(DEBUG_DIR)
	rm -f perf.data*
	mkdir $(DEBUG_DIR)
	perf record -a -s -g $(BUILD_DIR)/bin/benchmark
	perf script | $(FLAMEGRAPH)/stackcollapse-perf.pl > $(DEBUG_DIR)/out.perf-folded
	$(FLAMEGRAPH)/flamegraph.pl $(DEBUG_DIR)/out.perf-folded > $(DEBUG_DIR)/flamegraph.svg
	mv perf.data $(DEBUG_DIR)
