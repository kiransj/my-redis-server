CPP:=g++
#CFLAGS := -g3 
CFLAGS := -Os -s -ffunction-sections -fdata-sections 
WARNINGS:= -Wall

INCLUDES := -I./inc
OUTPUT   := ./a.out

CPP_FILES := 
CPP_FILES += src/util.cpp
CPP_FILES += src/tree.cpp
CPP_FILES += src/zlist.cpp
CPP_FILES += src/keyvalue.cpp
CPP_FILES += src/redis.cpp
CPP_FILES += src/test.cpp

CPP_OBJECTS := $(CPP_FILES:.cpp=.o)    

all: $(OUTPUT)

$(OUTPUT):$(CPP_OBJECTS)
	@echo "building $@"
	@$(CPP) $(CPP_OBJECTS) $(C_OBJECTS) -lrt -o $@

%.o:%.cpp
	@echo "compiling $^"
	@$(CPP) $(CFLAGS) $(WARNINGS) $(INCLUDES) -c $^  -o $@

clean:
	@rm -f $(C_OBJECTS) $(CPP_OBJECTS) $(OUTPUT) *.core
