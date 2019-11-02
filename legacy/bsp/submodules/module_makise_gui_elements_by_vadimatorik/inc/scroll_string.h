#pragma once

#include "makise_e.h"

#ifdef ELEMENT_SCROLL_STRING_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef struct {
	const MakiseFont*			const fontErcent;
	const uint32_t				fontErcentCol;
	const uint32_t				bgColor;
	const uint32_t				borderColor;
} MakiseStyle_SMScrollString;

typedef struct {
	MElement								e;
	const char*								stringBase;
	const char*								stringNow;
	const MakiseStyle_SMScrollString*		style;
} MScrollString;

void		mCreateScrollString				(	MScrollString*							b,
												MContainer*								c,
												const char*								string,
												MPosition								pos,
												const MakiseStyle_SMScrollString*		style	);

void		mScrollStringScroll				(	MScrollString*	b	);
void		mScrollStringScrollReset		(	MScrollString*	b	);

#ifdef __cplusplus
}
#endif

#endif
