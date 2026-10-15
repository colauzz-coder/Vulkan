#include "../headers/vulkan_debug.h"
#include "../../instance/headers/instance.h"
#include "../../../instance/headers/instance.h"
VkDebugUtilsMessengerEXT messenger;
//the message types are an information of what that message is about, it could be a general thing that is not related to validation or performance, if it is related to performance that means that the vulkan api is used like a not optimized way, if the message is about validation is because of something happened that violates the specification(basically an error)
VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data){ //the first parameter is the severity of the debug message, it can be: VERBOSE, INFO, WARNING, ERROR, the validation layer can call debug_callback(WARNING, ...) or any other type. the second parameter is the type of the message, it can be: GENERAL, VALIDATION, PERFORMANCE, so it can be like debug_callback(WARNING, VALIDATION, ...) and it means that there is a warning related to validation. the third parameter is the data about the message, it is a pointer to a struct that has informations about the message, it is const because we can only read the data that comes to us but we can't change it
    if(message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT || message_type == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT){ //if the message returns something that may be an error
        printf("[ WARNING ] %s\n", callback_data->pMessage);
    }
    if(message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT){ //if the message returns an information of the application
        printf("[ INFO ] %s\n", callback_data->pMessage);
    }
    if(message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT){ //if the message returns an error
        printf("[ ERROR ] %s\n", callback_data->pMessage);
    }
    if(message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT){ //if the message is a verbose and technical
        printf("[ VERBOSE INFO ] %s\n", callback_data->pMessage);
    }
    return VK_FALSE; //the callback function returns "VK_FALSE" if the information sent should be treated as normal, not an error, if it returns "VK_TRUE", that means that it should be treated as an error and abort
}

VkResult create_debug_messenger(const VkAllocationCallbacks* allocator){ //
    PFN_vkCreateDebugUtilsMessengerEXT function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"); //the "vkCreateDebugUtilsMessengerEXT" as we can see in the name(EXT), it is an extension function, a real function we use are from the "vulkan_core.h" file, but there are some functions that are extension functions that returns the pointer of the function because they deppend of the extensions used, with "VkInstanceGetProcAddr" we get the pointer of it
    if(function == NULL){ //if the extension that has "vkCreateDebugUtilsMessengerEXT" doesn't exist or it is not enabled, our variable that stores it becomes NULL, we check if the function exists, if it does, the extension from that function is installed
        return VK_ERROR_EXTENSION_NOT_PRESENT; //returns the error "VK_ERROR_EXTENSION_NOT_PRESENT" since the extension for "vkCreateDebugUtilsMessengerEXT" doesn't exist
    }else{
        return function(instance, &debug_messenger_create_info, allocator, &messenger);
        //vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pMessenger)
    }
}

void destroy_debug_messenger(const VkAllocationCallbacks* allocator){
    PFN_vkDestroyDebugUtilsMessengerEXT function = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"); //function for destroying the messenger, function used is an extension function so we have to use "vkInstanceGetProcAddr"
    if(function == NULL){ //if the function doesn't exist that means that the extension is not in the system
        return;
    }else{
        function(instance, messenger, allocator); //if the extension and the function exists, we call it
    }
}

void init_callback(){ //initializes the debug callback
    if(validation_layers_enabled == false){ //validation layers must be enabled to initialize the callbacks since we use the callbacks together with the validation layers for debugging
        return;
    }
    VkResult result = create_debug_messenger(NULL);
    if(result != VK_SUCCESS){ //checks if the creation of the messenger was successful
        printf("[ ERROR ] couldn't create debug messenger\n");
        exit(EXIT_FAILURE);
    }else{
        printf("[ OK ] debug messenger created\n");
    }
}
