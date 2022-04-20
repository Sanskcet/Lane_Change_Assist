PROJNAME=LaneChangeAssist

BUILD_DIR=BUILD

SRC=src/main.c\
src/sensor.c\


TEST_SRC=test/test_sensor.c\
unity/unity.c\
src/sensor.c\

TEST_INC=-I test/test_libs

AVRDUDE:=avrdude

HFLAGS=-j .text -j .data -O ihex

INC=-Iinc -Iunity

INCLUDE_LIBS = -lcunit

ifdef OS 

	RM=del /q
	FixPath=$(subst /,\,$1)

	CC=avr-gcc.exe

	OUT = exe

	AVR_OBJ_CPY=avr-objcopy.exe

else 

	ifeq ($(shell uname),Linux)

	RM=rm -rf

	OUT = out

	FixPath=$1
		
		CC=avr-gcc

		AVR_OBJ_CPY=avr-objcopy
	
	endif
endif

TEST_OUTPUT=utest.$(OUT)

.PHONY: all analysis clean doc

all: $(BUILD_DIR)

	$(CC) -g -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL $(INC) $(SRC) -o $(call FixPath,$(BUILD_DIR)/$(PROJNAME).elf)

hex: $(call FixPath,$(BUILD_DIR)/$(PROJNAME).elf)

	$(AVR_OBJ_CPY) $(HFLAGS) $< $(call FixPath,$(BUILD_DIR)/$(PROJNAME).hex)

$(BUILD_DIR):

	mkdir $(BUILD_DIR)

analysis: $(SRC)
	cppcheck --enable=all $^

test: $(BUILD_DIR)
	gcc $(TEST_SRC) $(INC) $(TEST_INC) -o $(TEST_OUTPUT) $(INCLUDE_LIBS)
	./$(TEST_OUTPUT)
	
doc:

	make -C documentation

clean: 
	$(RM) BUILD
	make -C documentation clean
	rmdir $(BUILD_DIR)
