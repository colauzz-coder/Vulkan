#include "vulkan_setup/vulkan_main.h"
#include "vulkan_setup/glfw/headers/glfw.h"
#include <stdint.h>
//#include <stdio.h> --- right now not used
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdbool.h>
#include <vulkan/vulkan_core.h>
int main(void){
    static const uint32_t width = 800, height = 600;
    glfw_init();
    GLFWwindow* window = create_window(window, width, height);
    vulkan_init(window);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    vulkan_end(window);
    return EXIT_SUCCESS;
}
