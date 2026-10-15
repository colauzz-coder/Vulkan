#include "../headers/glfw.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../instance/headers/instance.h"
#include "../../device/headers/device.h"
VkBool32 presentation_support = VK_FALSE;
VkSurfaceKHR surface = VK_NULL_HANDLE;
void glfw_init(void){
    if(!glfwInit()){
        printf("[ ERROR ] glfw couldn't initialize\n");
        exit(EXIT_FAILURE);
    }else{
        printf("[ OK ] glfw initialized\n");
    }
    if(glfwVulkanSupported()){ //checks if glfw supports vulkan, returns GLFW_TRUE if yes, returns GLFW_API_UNAVAILABLE if not
        printf("[ OK ] glfw supports vulkan\n");
    }else{
        printf("[ ERROR ] glfw doesn't support vulkan\n");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //tells glfw to no use the opengl context since it uses it as default
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //for handling resized windows
}

GLFWwindow* create_window(GLFWwindow* window, uint32_t width, uint32_t height){
    window = glfwCreateWindow(width, height, "vulkan", NULL, NULL);
    if(!window){
        printf("[ ERROR ] window couldn't be created\n");
        exit(EXIT_FAILURE);
    }else{
        printf("[ OK ] window created\n");
        glfwMakeContextCurrent(window);
    }
    return window;
}

void create_surface(GLFWwindow* window){
    VkResult result_window_surface = glfwCreateWindowSurface(instance, window, NULL, &surface);
    if(result_window_surface == VK_SUCCESS){
        printf("[ OK ] surface created\n");
    }else{
        printf("[ ERROR ] couldn't create surface: \n");
        exit(EXIT_FAILURE);
    }
}

void device_supports_presentation(void){
    VkResult result_device_surface_support = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, family_index, surface, &presentation_support);
    if(result_device_surface_support == VK_SUCCESS){
        printf("[ OK ] device presentation queue supported\n"); 
    }else{
        printf("[ ERROR ] device presentation queue isn't supported\n");
        exit(EXIT_FAILURE);
    }
}