#include "../headers/instance.h"
#include "../../layers/instance/headers/instance.h"
#include "../../layers/extensions/headers/glfw.h"
#include "../../layers/extensions/headers/vulkan_debug.h"
#include <vulkan/vulkan_core.h>
extern uint32_t glfw_extension_count;
VkInstance instance;
void create_instance(GLFWwindow* window){
    if(validation_layers_enabled == false && !check_validation_layers()){ //checks if validation layers are enabled and if the validation layers we want exist
        printf("[ ERROR ] there are no validation layers/they are not enabled\n");
        exit(EXIT_FAILURE);
    }else{
        printf("[ OK ] validation layers found/validation layers enabled\n");
    }

    VkApplicationInfo app_info = { //pass some basic informations about our vulkan application, it also optimizes our vulkan application
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO, //the vulkan api needs to know the type of the created struct everytime a struct is created
        .pApplicationName = "vulkan application", //pointer to the name of our applcation
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0), //our application version, vulkan gives us a function to create versions in the correct format
        .pEngineName = "vulkan engine", //pointer to the name of our engine
        .engineVersion = VK_MAKE_VERSION(1, 0, 0), //our engine version
        .apiVersion = VK_API_VERSION_1_3, //the version of the vulkan api, vulkan gives types to reference vulkan api versions
        .pNext = NULL //can point to more information
    };

    const char** glfw_extensions = get_glfw_required_extensions(); //the vulkan api needs some extensions to have interaction with the window system and draw things at the screen, this function returns/gets all the glfw required extensions so that vulkan can have compatibility with the window system
    VkInstanceCreateInfo create_info = { //important for telling the vulkan api which validation layers and extensions we are going to use
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, //type of the structure
        .pApplicationInfo = &app_info, //pointer to all the application informations that we created
        .enabledExtensionCount = glfw_extension_count, //the number of extensions to enable
        .ppEnabledExtensionNames = glfw_extensions, //the names of each one of the extensions
        .enabledLayerCount = 0, //the amount of layers to enable, we keep it as zero since we don't have any right in this moment
        .pNext = NULL
    };

    if(validation_layers_enabled == true){ //if validation layers are enabled in the system
        create_info.enabledLayerCount = validation_layers_amount; //sets the amount of validation layers that vulkan can enable
        create_info.ppEnabledLayerNames = validation_layers; //sets all the validation layers to the validation layers we want/need
        create_info.pNext = &debug_messenger_create_info;
    }else{
        create_info.enabledLayerCount = 0; //if validation layers are not enabled in the system there are no validation layers to vulkan to enable
        create_info.pNext = NULL;
    }

    uint32_t instance_extension_amount = 0; //the amount of required instance extensions
    vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_amount, NULL); //returns the number of required instance extensions
    VkExtensionProperties instance_extensions[instance_extension_amount]; //array that will store all the required instance extensions, each element(each extension) in this array has some informations about it like the extension name and version of the extension
    vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_amount, instance_extensions); //returns all the required instance extensions inside the array we created

    //checking if the user wants the names of the instance extensions
    char instance_print_option[10];
    char instance_print_option_yes[4] = "yes";
    char instance_print_option_no[4] = "no";
option_instance:
    printf("[ OK ] instance extensions found, print them? --- yes --- no ---\n");
    scanf("%s", instance_print_option);
    if(strcmp(instance_print_option, instance_print_option_yes) == 0){
        for(uint32_t i = 0; i < instance_extension_amount; i++){
            if(i == 0){
                printf("\n%s\n", instance_extensions[i].extensionName);
            }else{
                printf("%s\n", instance_extensions[i].extensionName);
            }
        }
    }
    if(strcmp(instance_print_option, instance_print_option_no) == 0){
        printf("ignored instance names\n");
    }
    //checking if the instance could be created correctly
    VkResult instance_result = vkCreateInstance(&create_info, NULL, &instance); //the vulkan api gives us the "VkResult" type which we can store results of functions and check if they worked or not, we create the instances after configurating them
    if(instance_result != VK_SUCCESS){ //the vulkan api functions return "VK_SUCCESS" if everything went good. checks if the result of the "vkCreateInstance"
        printf("[ ERROR ] instance couldn't be created\n");
        exit(EXIT_FAILURE);
    }else{
        printf("[ OK ] instance created\n");
    }
}
