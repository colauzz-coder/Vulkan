CC := gcc

#directories
BUILD := build
VULKAN := vulkan_setup

#flags
FLAGS := -Wall -Wextra -O0 -g -lvulkan -lglfw

#files
SRC := $(shell find -type f -name '*.c')
#the "-type f -name" part means that it finds every file(thats why the "f") with the name .c at the end of it
#the "shell find" command find every file that exists with the fix at the end of the name and it gets into directories that are inside the specific directory we got inside
#meanwhile the "wildcard" command doesn't get inside directories that are inside the directory we specified, it only finds for files inside the specific one
OBJ := $(patsubst $(VULKAN)/%.c, $(BUILD)/%.o, $(SRC)) 
#gets every ".c" file inside the "vulkan_setup" directory and creates the same file but as ".o" inside the "build" directory

#links all the ".o" files to the "vulkan" executable
vulkan: $(OBJ)
	$(CC) $(OBJ) -o $@ -lglfw -lvulkan

#compiles all the ".c" files to ".o" files
$(BUILD)/%.o: $(VULKAN)/%.c
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

#the focus of this makefile is creating the "vulkan" executable
all: vulkan

#cleaning the build directory and deleting the executable
clean:
	rm -rf $(BUILD)
	rm -f vulkan

#command for running
run: vulkan
	./vulkan

.PHONY: all clean run

