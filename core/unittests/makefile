INC := -I ../include -I ../../common/include
BUILD := ../build

cache_test: $(BUILD)/dram.o $(BUILD)/cache.o cache_test.o
	g++ $(INC) cache_test.o $(BUILD)/dram.o $(BUILD)/cache.o -o cache_test

$(BUILD)/dram.o:
	make build/dram.o -C ../

$(BUILD)/cache.o:
	make build/cache.o -C ../

cache_test.o: cache_test.cpp
	g++ $(INC) -c cache_test.cpp

clean:
	make clean -C ../
	rm cache_test cache_test.o
