#ifndef INSTANCE_H
#define INSTANCE_H
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdint.h>
extern VkInstance instance;
void create_instance(GLFWwindow* window);
#endif
