#include "../headers/device.h"
#include "../../instance/headers/instance.h"
#include "../../layers/instance/headers/instance.h"
#include <stdio.h>
#include <stdlib.h>
//#include "../../layers/extensions/headers/glfw.h"
#include "../../glfw/headers/glfw.h"
VkDevice device;
VkPhysicalDevice physical_device = VK_NULL_HANDLE; //this variable is a handler that will store our chosen graphics device
uint32_t family_index = 0;

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
            printf("[ OK ] supported physical device found: %s\n", device_properties.deviceName);
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
        printf("[ OK ] found a supported physical device\n");
    }
}

uint32_t get_queue_family(void){ //gets the queue family we want, queue families takes care and stores queues inside her, a queue family is basically the one that gives the vulkan api the needed queues for executing something, each queue can only execute what it is able of, the things that different queues can execute are: transfer data to the gpu memory, compute things, draw pixels at the screen(graphics) and etc, each queue is responsible for executing something, so we need to tell vulkan what queue family he's gonna use for calling the queues inside it, when the vulkan api wants to execute something, she calls the queue that is responsible for that thing, thats why we have different queues, because vulkan can call different queues for making stuff
    if(physical_device == NULL){ //checks if the supported physical device exists
        exit(EXIT_FAILURE);
    }

    uint32_t queue_family_properties_amount = 0; //stores the amount of queue families
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_properties_amount, NULL); //returns the amount of existing queue families
    if(queue_family_properties_amount == 0){ //checks is there are any existing queue families, we return an error if not
        printf("[ ERROR ] no queue families found\n");
        exit(EXIT_FAILURE);
    }
    VkQueueFamilyProperties* queue_family_properties = malloc(sizeof(VkQueueFamilyProperties) * queue_family_properties_amount); //allocates memory for our queue families
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_properties_amount, queue_family_properties); //returns all the existing queue families
    if(queue_family_properties == NULL){ //checks if there are any existing queue families
        printf("[ ERROR ] no queue families found\n");
        exit(EXIT_FAILURE);
    }
    device_supports_presentation(); //checks if the device has queues that supports presentation queue, the presentation queue is used for displaying images at the window created by the window system
    for(uint32_t i = 0; i < queue_family_properties_amount; i++){ //loops through all the existing queue families
        if(queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentation_support == VK_TRUE){ //tries to find the queue family that supports graphics
            family_index = i; //if it finds the queue family that supports graphics, we store the family index            
            printf("[ OK ] supported queue families found, supported queue family index: %d\n", family_index);
        }else{
            continue; //if it doesn't find the queue family that supports graphics, we jump to the next queue family
        }
    }
    return family_index; //returns the queue family that supports the things we want
}

void create_logical_device(void){ //creates the logical device, the logical device is the one that will be used in vulkan, the physical device is the hardware(gpu) itself, we choose the device by the physical device and we configure it by the logical device
    uint32_t queue_family_index = get_queue_family(); //gets the index of the chosen queue family we are going to use to execute things with the device(gpu)
    const float queues_priorities = 1.0f; //stores the priority of each queue, that exists because vulkan executes the most important queues first and the least important ones at the end, we set it to 1.0f so that vulkan executes it all equally and treats every queue as important queues. 1.0f -> most important. 0.5 -> medium important. 0.0 -> less important
    VkDeviceQueueCreateInfo device_queue_create_info = { //this struct tells vulkan which queue family we want to use
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, //tells vulkan the type of the struct
        .queueFamilyIndex = queue_family_index, //the index of the queue family we are going to use
        .pQueuePriorities = &queues_priorities, //the priorities configuration for the queues
        .queueCount = 1, //the amount of queues we are going to use, we only want the graphics queue right now so it's only 1
        .flags = 0,
        .pNext = NULL
    };

    VkPhysicalDeviceFeatures physical_device_features; //stores the physical device features
    vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features); //gets the physical device features
    VkDeviceCreateInfo device_create_info = { //this struct is responsible for giving information about the device
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, //the type of the struct
        .pQueueCreateInfos = &device_queue_create_info, //pointer to the device queues configuration
        .queueCreateInfoCount = 1, //the amount of device queues configuration
        .pEnabledFeatures = &physical_device_features, //the amount of device features enabled
        .pNext = NULL
    };

    if(validation_layers_enabled == true){ //checks if the validation layers are enabled
        device_create_info.enabledLayerCount = validation_layers_amount; //the amount of enabled validation layers
        device_create_info.ppEnabledLayerNames = validation_layers; //the validation layers that can be used
    }else{ //if the validation layers are not enabled
        device_create_info.enabledLayerCount = 0;
        device_create_info.ppEnabledLayerNames = NULL;
    }
    VkResult device_result = vkCreateDevice(physical_device, &device_create_info, NULL, &device); //creates the logical device
    if(device_result != VK_SUCCESS){ //if the device creation was successful
        printf("[ ERROR ] couldn't create logical device\n");
        exit(EXIT_FAILURE);
    }else{ //if the device creation wasn't successful
        printf("[ OK ] logical device created\n");
    }
}

void get_queue(void){ //creates a handler for the queue we chose so that we can actually use it
    VkQueue graphics_queue;
    vkGetDeviceQueue(device, family_index, 0, &graphics_queue);
}