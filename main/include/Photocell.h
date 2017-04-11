#ifndef PHOTOCELL_H_
#define PHOTOCELL_H_

// Callback function prototype
typedef void (*PhotocellCallbackFunction)(uint8_t);

/*
 * Call this before doing anything
 */
void PhotocellInit(void);

/*
 * Get ambience light
 * return value: 0-8, error at 10
 */
uint8_t PhotocellGetAbienceLightLevel(void);

/*
 * Register the callback function
 */
uint8_t PhotocellRegisterCallback(PhotocellCallbackFunction function);

#endif
