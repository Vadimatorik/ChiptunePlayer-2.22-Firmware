#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MAKISEGUI_DRIVER_DEPTH						1
#define MAKISEGUI_BUFFER_DEPTH						1
#define MAKISEGUI_BUFFER_DEPTHMASK					1

#define MAKISE_E_SLIDER								1
#define MAKISE_E_SLIST								1

#define MAKISE_BUF_H								64
#define MAKISE_BUF_W								128

//Control section
//len of buffer with input events
#define MAKISE_GUI_INPUT_BUFFER_LEN					10
//use x-y pointing control devices. like mouse or touchscreen
#define MAKISE_GUI_INPUT_POINTER_ENABLE				0

// Enable element support.
#define MAKISE_E_MESSAGE_WINDOW						1

// Enable debug output.
#define MAKISE_DEBUG_OUTPUT						 // Only if debug output is enabled.

#define MAKISE_UNICODE								1

// Учитываем инверсию дисплея.
#define MAKISEGUI_DISPLAY_INVERTED					1

#define MAKISE_DEBUG_PRINT							( void )

#define ELEMENT_FSVIEWER_FATFS_ENABLED
#define ELEMENT_PLAY_BAR_ENABLED
#define ELEMENT_PLAY_LIST_ENABLED
#define ELEMENT_PLAYER_STATUS_BAR_ENABLED
#define ELEMENT_PROGRESS_BAR_ENABLED
#define ELEMENT_SLIST_ENABLED
#define ELEMENT_SCROLL_STRING_ENABLED
#define ELEMENT_SLIM_HORIZONTAL_LIST_ENABLED

#ifdef __cplusplus
}
#endif
