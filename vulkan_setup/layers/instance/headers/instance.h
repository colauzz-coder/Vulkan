#ifndef LAYER_INSTANCE_H
#define LAYER_INSTANCE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <vulkan/vulkan.h>
//enables validation layers if the application is being ran with debug mode, if it is not being ran with debug mode it stays disabled, the "NDEBUG" thing stands for "NOT DEBUG" and if it is enabled that means that the debug mode is disabled, if it is disabled that means that the debug mode is enabled
#ifdef NDEBUG //if the program is at debug mode
    static const bool validation_layers_enabled = false; //our variable that will control if the validation layers are enabled or not
#else
    static const bool validation_layers_enabled = true;
#endif

static const char* validation_layers[] = { "VK_LAYER_KHRONOS_validation" }; //most validation layers already come together with this "VK_LAYER_KHRONOS_validation" layer that already comes with the vulkan sdk
static const uint32_t validation_layers_amount = sizeof(validation_layers) / sizeof(validation_layers[0]); //the amount of validation layers we want
bool check_validation_layers(void);

#endif
