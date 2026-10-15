#include "../headers/device.h"
#include "../../instance/headers/instance.h"
#include "../../layers/instance/headers/instance.h"
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>
//#include "../../layers/extensions/headers/glfw.h"
#include "../../glfw/headers/glfw.h"
queue_family_indices family_indices;
VkQueue graphics_queue = VK_NULL_HANDLE;
VkQueue present_queue = VK_NULL_HANDLE;
VkDevice device = VK_NULL_HANDLE;
VkPhysicalDevice physical_device = VK_NULL_HANDLE; //this variable is a handler that will store our chosen graphics device

void get_physical_device(void){
    bool is_device_supported = false; //this variable stores if the chosen graphics device is supported or no
    uint32_t devices_amount = 0; //this variable will store the amount of existing physical devices
    vkEnumeratePhysicalDevices(instance, &devices_amount, NULL); //gets the amount of existing physical devices
    if(devices_amount == 0){ //if no physical devices were found, we return an error
        printf("[ ERROR ] no physical devices found\n");
        exit(EXIT_FAILURE);
    }
    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * devices_amount); //allocates memory for the found physical devices
    vkEnumeratePhysicalDevices(instance, &devices_amount, devices); //returns the physical devices inside our array that will store all the existing physical devices
    if(devices == VK_NULL_HANDLE){
        printf("[ ERROR ] no physical devices found\n");
        free(devices);
        exit(EXIT_FAILURE);
    }else{
        printf("[ OK ] physical devices found\n");
    }

    for(uint32_t i = 0; i < devices_amount; i++){ //checks all the existing physical devices to find for the supported one
        bool device_supports_vulkan = false; //this variable stores if the device supports higher vulkan versions

        VkPhysicalDeviceProperties device_properties; //stores the device properties
        vkGetPhysicalDeviceProperties(devices[i], &device_properties); //gets the device properties

        VkPhysicalDeviceFeatures device_features; //stores the device features
        vkGetPhysicalDeviceFeatures(devices[i], &device_features); //gets the device features

        if(device_properties.apiVersion >= VK_API_VERSION_1_3){ //checks if this device supports higher versions of vulkan
            device_supports_vulkan = true;
        }else{
            device_supports_vulkan = false;
        }
        //checks if the device is supported, checks the type of the GPU, GPUs can have the types DISCRETE_GPU that is a separated processor connected to the system, CPU that is the CPU itself, not the GPU, used when there is no GPU in the system and other types. the geometry shader is for automating the process of drawing vertices at the screen, a vertex shader normally process each vertex individually for generating a triangle for example, the geometry shader gets those vertices and basically makes able to
        //re-make that same figure you made by the vertices without calculating each vertex again, it is not that good for performance but it helps a lot for things like voxel engines and some other stuff
        if(device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && device_features.geometryShader && device_supports_vulkan == true){
            //if this specific physical device is the supported one
            is_device_supported = true;
            physical_device = devices[i];
        }else{
            //if this physical device is not the supported one, we skip it
            continue;
        }
    }
    if(is_device_supported == false || physical_device == VK_NULL_HANDLE){ //checks if we found any supported physical device
        printf("[ ERROR ] couldn't find a supported physical device\n");
        free(devices);
        exit(EXIT_FAILURE);
    }
    if(is_device_supported == true && physical_device != VK_NULL_HANDLE){
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(physical_device, &device_properties);
        printf("[ OK ] supported physical device found: %s\n", device_properties.deviceName);
    }
}

queue_family_indices get_queue_family(void){ //gets the queue family we want, queue families takes care and stores queues inside her, a queue family is basically the one that gives the vulkan api the needed queues for executing something, each queue can only execute what it is able of, the things that different queues can execute are: transfer data to the gpu memory, compute things, draw pixels at the screen(graphics) and etc, each queue is responsible for executing something, so we need to tell vulkan what queue family he's gonna use for calling the queues inside it, when the vulkan api wants to execute something, she calls the queue that is responsible for that thing, thats why we have different queues, because vulkan can call different queues for making stuff
    if(physical_device == NULL){
        printf("[ ERROR ] couldn't choose queue family, device doesn't exist\n");
    }
    uint32_t queue_family_amount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_amount, NULL);
    if(queue_family_amount == 0){
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(physical_device, &device_properties);
        printf("[ ERROR ] couldn't find any queue families in the device %s\n", device_properties.deviceName);
    }else{
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(physical_device, &device_properties);
        printf("[ OK ] queue families found in the device %s\n", device_properties.deviceName);
    }
    VkQueueFamilyProperties* queue_family_properties = malloc(sizeof(VkQueueFamilyProperties) * queue_family_amount);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_amount, queue_family_properties);
    for(uint32_t i = 0; i < queue_family_amount; i++){
        if(queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            family_indices.graphics_family = i;
        }
        device_supports_presentation(i);
        if(presentation_support == VK_TRUE){
            family_indices.present_family = i;
        }
    }

    if(queue_family_properties->queueFlags & VK_QUEUE_GRAPHICS_BIT){
        printf("[ OK ] found graphics queue family\n");
    }else{
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(physical_device, &device_properties);
        printf("[ ERROR ] couldn't find graphics queue family in the device %s\n", device_properties.deviceName);
        exit(EXIT_FAILURE);
    }

    if(presentation_support == VK_TRUE){
        printf("[ OK ] found present queue family\n");
    }else{
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(physical_device, &device_properties);
        printf("[ ERROR ] couldn't find present queue family in the device %s\n", device_properties.deviceName);
        exit(EXIT_FAILURE);
    }

    return family_indices;
}

void create_logical_device(void){ //creates the logical device, the logical device is the one that will be used in vulkan, the physical device is the hardware(gpu) itself, we choose the device by the physical device and we configure it by the logical device
    const float queue_family_priorities = 1.0f;
    queue_family_indices device_queue_family_indices = get_queue_family();
    VkDeviceQueueCreateInfo device_graphics_queue_create_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pQueuePriorities = &queue_family_priorities,
        .queueFamilyIndex = device_queue_family_indices.graphics_family,
        .queueCount = 1,
        .pNext = NULL
    };

    VkDeviceQueueCreateInfo device_present_queue_create_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pQueuePriorities = &queue_family_priorities,
        .queueFamilyIndex = device_queue_family_indices.present_family,
        .queueCount = 1,
        .pNext = NULL
    };
    VkDeviceQueueCreateInfo device_queue_create_info[] = { device_graphics_queue_create_info, device_present_queue_create_info };
    
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    VkDeviceCreateInfo device_create_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pQueueCreateInfos = device_queue_create_info,
        .queueCreateInfoCount = 1,
        .pEnabledFeatures = &device_features,
        .pNext = NULL
    };

    if(validation_layers_enabled == true){
        device_create_info.ppEnabledLayerNames = validation_layers;
        device_create_info.enabledLayerCount = validation_layers_amount;
    }else{
        device_create_info.ppEnabledLayerNames = NULL;
    }
    VkResult device_create_result = vkCreateDevice(physical_device, &device_create_info, NULL, &device);
    if(device_create_result == VK_SUCCESS){
        printf("[ OK ] logical device created\n");
    }else{
        printf("[ ERROR ] couldn't create logical device\n");
        exit(EXIT_FAILURE);
    }
}

void get_queue(void){ //creates a handler for the queue we chose so that we can actually use it
    vkGetDeviceQueue(device, family_indices.graphics_family, 0,&graphics_queue);
    vkGetDeviceQueue(device, family_indices.present_family, 0, &present_queue);
}