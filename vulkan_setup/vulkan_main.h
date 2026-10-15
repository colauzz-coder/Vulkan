#ifndef VULKAN_MAIN_H
#define VULKAN_MAIN_H
#include <stdint.h>
#include <stdbool.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
static VkDebugUtilsMessengerEXT messenger;
void vulkan_init(GLFWwindow* window);
void vulkan_end(GLFWwindow* window);
#endif
