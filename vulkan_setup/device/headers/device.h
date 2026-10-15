#ifndef DEVICE_H
#define DEVICE_H
#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
extern VkDevice device;
extern VkPhysicalDevice physical_device;
extern uint32_t family_index;
void get_physical_device(void); //the function for getting the graphics device
uint32_t get_queue_family(void); //the function for getting the right queue family that supports what we need
void create_logical_device(void); //the function for creating the actual device that we use with vulkan, this is called logical device
#endif
