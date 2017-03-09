#ifndef PHOTOCELL_H_
#define PHOTOCELL_H_

// Callback function prototype
typedef void (*PhotocellCallbackFunction)(uint16_t);

/*
 * Call this before doing anything
 */
void PhotocellInit();

/*
 * Get ambience light
 * return value: 0-8, error at 10
 */
uint8_t PhotocellGetAbienceLightLevel();

/*
 * Register the callback function
 */
uint16_t PhotocellRegisterCallback(PhotocellCallbackFunction function);

#endif
