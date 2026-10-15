#ifndef LAYER_GLFW_H
#define LAYER_GLFW_H
#include <stdint.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
extern uint32_t glfw_extension_count; //stores the amount of required glfw extensions
const char** get_glfw_required_extensions(); //the function that will return all the required glfw extensions
void get_presentation_support(uint32_t family_index, VkPhysicalDevice physical_device);
#endif
