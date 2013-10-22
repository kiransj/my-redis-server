CC:=gcc

CFLAGS := -g3
#CFLAGS := -Os -s -ffunction-sections -fdata-sections 
WARNINGS:= -Wall

INCLUDES := -Iinc 
OUTPUT   := ./a.out

C_FILES :=
C_FILES += tree.c

REDIS_OBJECTS :=
REDIS_OBJECTS += $(C_FILES:.c=.o)	

all: $(OUTPUT)

$(OUTPUT):$(REDIS_OBJECTS)
	@echo "building $@"
	@$(CC) $(REDIS_OBJECTS) -o $@

%.o:%.c
	@echo "compiling $^"
	@$(CC) $(CFLAGS) $(WARNINGS) $(INCLUDES) -c $^  -o $@

clean:
	@rm -f $(REDIS_OBJECTS) $(OUTPUT) *.core
