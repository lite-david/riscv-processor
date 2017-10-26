all:
	make catapult -C ./core
	make -C ./simulator
	make -C ./benchmarks
	make -C ./common
clean:
	make clean -C ./core
	make clean -C ./simulator
	make clean -C ./benchmarks
	make clean -C ./common
	rm -rf testdir

