#include "../headers/glfw.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../instance/headers/instance.h"
#include "../../device/headers/device.h"
VkBool32 presentation_support = VK_FALSE; //stores if the physical device supports presentation queues for drawing things
VkSurfaceKHR surface = VK_NULL_HANDLE; //stores the window surface
void glfw_init(void){ //configures our window system(glfw)
    if(!glfwInit()){ //initializes glfw
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

GLFWwindow* create_window(GLFWwindow* window, uint32_t width, uint32_t height){ //this function creates and returns a working window
    window = glfwCreateWindow(width, height, "vulkan", NULL, NULL); //the function glfwCreateWindow creates a window, so we make a pointer from the type GLFWwindow to point to the window
    if(!window){ //checks if the window was created successfully
        printf("[ ERROR ] window couldn't be created\n");
        exit(EXIT_FAILURE);
    }else{
        printf("[ OK ] window created\n");
        glfwMakeContextCurrent(window);
    }
    return window; //returns the window
}

//a surface is important so that vulkan see what is the actual window he's gonna use
void create_surface(GLFWwindow* window){ //creates a window surface, vulkan needs a window surface with the window system to actually draw at it
    if(!window){ //to create a window surface so that vulkan uses the window, we need a working and existing window first
        printf("[ ERROR ] couldn't create window surface, window doesn't exist\n");
        exit(EXIT_FAILURE);
    }

    VkResult result_window_surface = glfwCreateWindowSurface(instance, window, NULL, &surface); //the glfwCreateWindowSurface function creates the window surface and returns a VkResult that shows us if the window surface creation was successful
    if(result_window_surface == VK_SUCCESS){ //checks if the window surface was created successfully
        printf("[ OK ] surface created\n");
    }else{
        printf("[ ERROR ] couldn't create surface: \n");
        exit(EXIT_FAILURE);
    }
}

void device_supports_presentation(void){ //this function checks if the physical device(gpu) supports the presentation queue that is needed to show things
    VkResult result_device_surface_support = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, family_index, surface, &presentation_support); //the vkGetPhysicalDeviceSurfaceSupportKHR function returns if the specified queue family from the specified device can show things to an specific window surface
    if(result_device_surface_support == VK_SUCCESS){ //checks if it can show images to that window surface
        printf("[ OK ] device presentation queue supported\n"); 
    }else{
        printf("[ ERROR ] device presentation queue isn't supported\n");
        exit(EXIT_FAILURE);
    }
}