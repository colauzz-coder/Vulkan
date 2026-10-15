#include "../headers/glfw.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>
#include "../../instance/headers/instance.h"
#include "../../../glfw/headers/glfw.h"
//#include "../../../glfw/headers/glfw.h"
//#include "../../../instance/headers/instance.h"
uint32_t glfw_extension_count;
const char** get_glfw_required_extensions(){
    const char** glfw_extensions_first = glfwGetRequiredInstanceExtensions(&glfw_extension_count); //this function returns all the required glfw extensions so that the vulkan api can have compatibility with the window system(glfw). the function also puts the amount of required glfw extensions inside the passed parameter
    const char** glfw_extensions;
    if(validation_layers_enabled == true){ //if the validation layers are enabled & exist
        glfw_extensions = malloc(sizeof(const char*) * (glfw_extension_count + 1)); //allocates memory for a new array that will store the "VK_EXT_debug_utils" extension
        for(uint32_t i = 0; i < glfw_extension_count; i++){
            glfw_extensions[i] = glfw_extensions_first[i]; //copies every glfw extension to our new array that stores the extension, leaving the last element empty for the "VK_EXT_debug_utils" debug extension that we will add
        }
        glfw_extension_count += 1;
        glfw_extensions[glfw_extension_count - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME; //adds the extension that is needed for our messenger, callbacks and debug stuff from vulkan
        if(strcmp(glfw_extensions[glfw_extension_count - 1], VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 1){ //if the extension wasn't added, we free the allocated memory
            free(glfw_extensions);
            return NULL;
        }
    }
    return glfw_extensions; //returns all the required glfw extensions + the extension needed for debugging in vulkan
}