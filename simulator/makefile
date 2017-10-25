# vim: set ts=4 nu ai:

CC := g++
CFLAGS := -std=c++11
SRCDIR := ./src
BUILDDIR := ./build
TARGET := ./bin/simRISCV

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC := -I ./includes
#VARIABLES := -D__USE_AC-D__LINUX_API

$(TARGET): $(OBJECTS)
	@mkdir -p bin
	@echo "Linking..."
	@echo " $(CC) $^ -o $(TARGET) "; $(CC) $^ -o $(TARGET) -D __USE_AC -D __LINUX_API

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -D __USE_AC -D __LINUX_API -c -o $@ $<
	
clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

#genericSimulator.o:
#	g++ -std=c++11 -I ./includes/ -c ./src/simulator/genericSimulator.cpp  -D __USE_AC
#elfFile.o:
#	g++ -I ./includes/ -c ./src/lib/elfFile.cpp -D__LINUX_API
#riscvSimulator.o:
#	g++ -std=c++11 -I ./includes/ -c ./src/simulator/riscvSimulator.cpp -D __USE_AC
#simRISCV.o:
#	g++ -std=c++11 -c -I ./includes/ ./src/tools/simRISCV.cpp -D __USE_AC -D __LINUX_API
#riscvISA.o:
#	g++ -std=c++11 -c -I ./includes/ ./src/isa/riscvISA.cpp -D __USE_AC

.PHONY: clean
