#ifndef GLFW_H
#define GLFW_H
#define GLFW_INCLUDE_VULKAN //glfw will automatically load the vulkan header with that included
#include <GLFW/glfw3.h>
#include <stdint.h>
extern VkSurfaceKHR surface;
extern VkBool32 presentation_support;
void glfw_init(void); //glfw initialization
GLFWwindow* create_window(GLFWwindow* window, uint32_t width, uint32_t height); //window creation
void create_surface(GLFWwindow* window);
void device_supports_presentation(void);
#endif