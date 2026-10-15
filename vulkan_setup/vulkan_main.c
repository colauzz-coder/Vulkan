#include "vulkan_main.h"
#include "device/headers/device.h"
#include "glfw/headers/glfw.h"
#include "instance/headers/instance.h"
#include "layers/extensions/headers/vulkan_debug.h"
#include "layers/instance/headers/instance.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>
void vulkan_init(GLFWwindow* window){
    create_instance(window);
    create_surface(window);
    init_callback();
    get_physical_device();
    create_logical_device();
}

void vulkan_end(GLFWwindow* window){ //deletes everything at the end of the application
    if(validation_layers_enabled == true){
        destroy_debug_messenger(NULL);
    }
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyDevice(device, NULL);
    vkDestroyInstance(instance, NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
}
