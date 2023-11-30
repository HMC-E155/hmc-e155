# FreeRTOS Demo for STM32L432KC

## Steps to Configure

1. Download FreeRTOS from the [FreeRTOS website](https://www.freertos.org/index.html).
2. Modify and prune codebase to leave only portions relevant to your specific compiler and architecture in `portable`.
3. Set up a new project in SEGGER Embedded Studio
4. Move files in and configure preprocessor include paths
5. Modify `FreeRTOSConfig.h` with desired options.
6. Modify `FreeRTOS.h` to include device-specific libraries (e.g., CMSIS)
7. Run examples.


## Resources

- [FreeRTOS Homepage](https://www.freertos.org/index.html)
- [Helpful wiki page](https://wiki.segger.com/How_to_create_a_simple_FreeRTOS_project_with_Embedded_Studio) from SEGGER's documentation with step-by-step instructions for configuring FreeRTOS in SEGGER Embedded Studio.
- Video [here](https://www.loom.com/share/1d36fd2235a8419096f5672c5d008b35) for a tutorial on how to set FreeRTOS up in SEGGER Embedded Studio.