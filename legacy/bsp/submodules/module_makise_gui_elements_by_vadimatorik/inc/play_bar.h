#pragma once

#include "makise_e.h"

#ifdef ELEMENT_PLAY_BAR_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef struct {
	uint32_t					bgColor;
	uint32_t					borderColor;
	uint32_t					dutyColor;
	uint32_t					timeColor;
	const MakiseFont*			font;
} MakiseStyle_PlayBar;

// Progress bar.
typedef struct {
	MElement						el;
	uint8_t							margin;
	uint32_t						upper_bound_time_sec;
	uint32_t						current_time_sec;
	const MakiseStyle_PlayBar*		style;
	char							current_time_string[9];
	char							upper_bound_time_string[9];
	uint8_t							duty;
	uint32_t						time_width;
	uint32_t						pb_width;
} MPlayBar;

void	mCreatePlayBar				(	MPlayBar*						b,
										MContainer*						c,
										MPosition						pos,
										uint8_t							margin_play_bar,
										const MakiseStyle_PlayBar*		style	);

void	mPlayBarIncSec				(	MPlayBar* b );
void	mPlayBarSetNewTrack			(	MPlayBar* b, uint32_t trackTimeSec	);
void	mPlayBarResetTrack			(	MPlayBar* b	);

#ifdef __cplusplus
}
#endif

#endif
