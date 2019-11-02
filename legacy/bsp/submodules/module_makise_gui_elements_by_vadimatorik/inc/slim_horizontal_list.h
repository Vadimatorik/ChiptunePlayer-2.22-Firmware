#pragma once

#include "makise_e.h"

#if defined(ELEMENT_SLIM_HORIZONTAL_LIST_ENABLED) && defined(ELEMENT_SCROLL_STRING_ENABLED)

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"
#include "scroll_string.h"

typedef struct {
	const MakiseFont*							const fontString;
	const uint32_t								stringColor;
	const uint32_t								bgColor;
	const uint32_t								borderColor;
	const uint32_t								bgArrowColor;
} MakiseStyle_SMSlimHorizontalList;

typedef struct {
	MElement									el;
	const char*									stringBase;
	const char*									stringNow;
	const MakiseStyle_SMSlimHorizontalList*		style;

	uint32_t									maxItem;
	uint32_t									currenItem;

	/// На сколько пикселей нужно сдвинуть символ на экране перед тем,
	/// как переместить указатель на следующий символ.
	uint32_t									dxPixChar;
	uint32_t									dxPixNow;
} MSlimHorizontalList;

void		mCreateSlimHorizontalList					(	MSlimHorizontalList*						b,
															MContainer*									c,
															MPosition									pos,
															const MakiseStyle_SMSlimHorizontalList*		style	);

/// 0 - прокрутка где-то в середине строки.
/// 1 - в начале или в конце.
int			mSlimHorizontalListScrollString				(	MSlimHorizontalList*	b	);

void		mSlimHorizontalSetItemCount					(	MSlimHorizontalList*	b, uint32_t itemCount	);
void		mSlimHorizontalListSetStringCurrentItem		(	MSlimHorizontalList*	b, const char* stringItem	);

void		mSlimHorizontalListLeft						(	MSlimHorizontalList*	b, const char* stringItem	);
void		mSlimHorizontalListRight					(	MSlimHorizontalList*	b, const char* stringItem	);
#ifdef __cplusplus
}
#endif

#endif
