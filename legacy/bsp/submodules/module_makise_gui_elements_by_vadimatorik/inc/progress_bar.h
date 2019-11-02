#pragma once

#include "makise_e.h"

#ifdef ELEMENT_PROGRESS_BAR_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef struct {
	uint32_t	bg_color;
	uint32_t	border_color;
	uint32_t	duty_color;
} MakiseStyle_ProgressBar;

// Progress bar.
typedef struct {
	MElement						el;

	uint8_t						 duty;
	uint8_t						 margin;

	MakiseStyle_ProgressBar*		style;
} MProgressBar;

void m_create_progress_bar ( MProgressBar*					b,
							 MContainer*					c,
							 MPosition						pos,
							 uint8_t						duty,			// Duty factor [ 0..100 ].
							 uint8_t						margin,
							 MakiseStyle_ProgressBar*		style );

void m_progress_bar_set_duty ( MProgressBar* b, uint8_t duty );

#ifdef __cplusplus
}
#endif

#endif
