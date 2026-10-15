#ifndef VULKAN_DEBUG_H
#define VULKAN_DEBUG_H
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
extern VkDebugUtilsMessengerEXT debug_messenger;
extern VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data);
static VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = {
    //passes all the informations about the callback like message severities, message types, the function that vulkan will execute as the callback function and etc
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT, //the type of the struct
    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT, //activates the types of severities we want to have, we make an OR operation that activates only certain bits that are responsible for certain severities and if they are 0 they are turned off, if 1 they are on, we activate only the ones we want/need
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = debug_callback, //pointer to our callback function so that vulkan uses it
    .pUserData = NULL,
    .pNext = NULL,
};
//message callbacks are good for the validation layers to send special messages deppending of the problem that it returns, making debugging easier, to make them we need a messenger
VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data); //the callback function for vulkan, some compilers may add some attributes to this function so the "VKAPI_ATTR" adds all the necessary attributes from the compiler, the "VKAPI_CALL" makes this function have support to different systems, since each system make a different call
void init_callback(); //initializes the callback
void destroy_debug_messenger(const VkAllocationCallbacks* allocator);
VkResult create_debug_messenger(const VkAllocationCallbacks* allocator);
#endif
