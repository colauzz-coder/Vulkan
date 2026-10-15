#include "../headers/instance.h"
bool check_validation_layers(void){ //checks if there are any validation layers
    uint32_t layer_count = 0; //stores the amount of validation layers that exist
    vkEnumerateInstanceLayerProperties(&layer_count, NULL); //returns the amount of existing validation layers
    VkLayerProperties* layers_properties = malloc(layer_count * sizeof(VkLayerProperties)); //reserves memory for all the existing validation layers
    vkEnumerateInstanceLayerProperties(&layer_count, layers_properties); //returns all the validation layers that were found

    for(uint32_t i = 0; i < validation_layers_amount; i++){ //makes a loop of the amount of validation layers we want/need
        bool layer_found = false; //the variable that stores if the validation layer we need was found
        for(uint32_t j = 0; j < layer_count; j++){ //makes a loop of the amount of existing validation layers
            if(strcmp(layers_properties[j].layerName, *validation_layers) == 0){ //compares the name of each existing validation layer we found with the function "vkEnumerateInstanceLayerProperties" with the validation layers we need
                layer_found = true; //if the layer was found
                break; //leaves the loop
            }
        }
        if(layer_found == false){ //if none of our validation layers were found the function return false
            return false;
        }
    }
    return true; //if the validation layers we want were found the function returns true
}

