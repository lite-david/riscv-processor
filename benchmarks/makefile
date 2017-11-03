objects = matmul4_4.o matmul8_8.o matmul16_16.o matmul32_32.o
executables = $(OUT_DIR)/matmul4_4.out $(OUT_DIR)/matmul8_8.out $(OUT_DIR)/matmul16_16.out $(OUT_DIR)/qsort_10.out $(OUT_DIR)/qsort_50.out $(OUT_DIR)/qsort_100.out 
dependancies = src/matmul.c include/matmul.h src/qsort.c include/qsort.h
CCX = riscv32-unknown-elf-gcc
INCLUDE = ./include
OUT_DIR = ./build
SIM = ../simulator/bin/simRISCV
SIM_OUT = ./simulator_output

all: directories $(SIM) $(executables)
$(OUT_DIR)/matmul4_4.out : $(dependancies)
	$(CCX) -I $(INCLUDE) src/matmul.c -o $(OUT_DIR)/matmul4_4.out -D SIZE=4
	$(SIM) -v -f $(OUT_DIR)/matmul4_4.out &> $(SIM_OUT)/matmul4_4.debug.out
$(OUT_DIR)/matmul8_8.out : $(dependancies)
	$(CCX) -I $(INCLUDE) src/matmul.c -o $(OUT_DIR)/matmul8_8.out -D SIZE=8
	$(SIM) -v -f $(OUT_DIR)/matmul8_8.out &> $(SIM_OUT)/matmul8_8.debug.out
$(OUT_DIR)/matmul16_16.out : $(dependancies)
	$(CCX) -I $(INCLUDE) src/matmul.c -o $(OUT_DIR)/matmul16_16.out -D SIZE=16
	$(SIM) -v -f $(OUT_DIR)/matmul16_16.out &> $(SIM_OUT)/matmul16_16.debug.out
$(OUT_DIR)/matmul32_32.out : $(dependancies)
	$(CCX) -I $(INCLUDE) src/matmul.c -o $(OUT_DIR)/matmul32_32.out -D SIZE=32
	$(SIM) -v -f $(OUT_DIR)/matmul32_32.out &> $(SIM_OUT)/matmul32_32.debug.out
$(OUT_DIR)/qsort_10.out : src/qsort.c include/qsort.h
	$(CCX) -I $(INCLUDE) src/qsort.c -o $(OUT_DIR)/qsort_10.out -D SIZE=10
	$(SIM) -v -f $(OUT_DIR)/qsort_10.out &> $(SIM_OUT)/qsort_10.debug.out
$(OUT_DIR)/qsort_50.out : src/qsort.c include/qsort.h
	$(CCX) -I $(INCLUDE) src/qsort.c -o $(OUT_DIR)/qsort_50.out -D SIZE=50
	$(SIM) -v -f $(OUT_DIR)/qsort_50.out &> $(SIM_OUT)/qsort_50.debug.out
$(OUT_DIR)/qsort_100.out : src/qsort.c include/qsort.h
	$(CCX) -I $(INCLUDE) src/qsort.c -o $(OUT_DIR)/qsort_100.out -D SIZE=100
	$(SIM) -v -f $(OUT_DIR)/qsort_100.out &> $(SIM_OUT)/qsort_100.debug.out
$(OUT_DIR)/qsort_1000.out : src/qsort.c include/qsort.h
	$(CCX) -I $(INCLUDE) src/qsort.c -o $(OUT_DIR)/qsort_1000.out -D SIZE=1000
	$(SIM) -v -f $(OUT_DIR)/qsort_1000.out &> $(SIM_OUT)/qsort_1000.debug.out
directories: ${OUT_DIR}
${OUT_DIR}:
	mkdir -p ${OUT_DIR}
	mkdir -p $(SIM_OUT)
$(SIM):
	make -C ../simulator
clean:
	rm -rf $(OUT_DIR) 
	rm -rf $(SIM_OUT)
.PHONY: all clean directories
