#ifndef DEVICE_H
#define DEVICE_H
#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
typedef struct{
    uint32_t graphics_family;
    uint32_t present_family;
}queue_family_indices;
extern VkQueue graphics_queue;
extern VkQueue present_queue;
extern VkDevice device;
extern VkPhysicalDevice physical_device;
void get_physical_device(void); //the function for getting the graphics device
queue_family_indices get_queue_family(void); //the function for getting the right queue family that supports what we need
void create_logical_device(void); //the function for creating the actual device that we use with vulkan, this is called logical device
#endif
