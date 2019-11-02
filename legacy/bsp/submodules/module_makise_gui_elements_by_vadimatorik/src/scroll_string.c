#ifdef ELEMENT_SCROLL_STRING_ENABLED

#include "scroll_string.h"

#ifdef __cplusplus
extern "C" {
#endif

static MResult draw ( MElement* b, MakiseGUI *gui );

static char *name = "ScrollString";

void mCreateScrollString (	MScrollString*							b,
							MContainer*								c,
							const char*								string,
							MPosition								pos,
							const MakiseStyle_SMScrollString*		style	) {
	MElement *e = &b->e;

	m_element_create(e, name, b,
			 1, MFocusPrior_NotFocusble,
			 pos, &draw,
			 0,	 0,
			 NULL,	 NULL,
			 0, 0);

	b->stringBase	=	string;
	b->stringNow	=	string;

	b->style		=	style;

	makise_g_cont_add( c, e );

#if ( MAKISE_ENABLE_DEBUG_OUTPUT > 0 )
	MAKISE_DEBUG_OUTPUT( "ScrollString %d created\n", e->id );
#endif
}

void mScrollStringScroll (	MScrollString*	b	) {
	uint32_t	len;
	len = makise_d_string_width( b->stringNow, MDTextAll, b->style->fontErcent );

	/// Если строка не вмещается в экран.
	if ( len > b->e.position.width ) {
		b->stringNow++;							/// Сдвигаемся на 1 символ.
	} else {
		b->stringNow	=	b->stringBase;		/// Строка с начала.
	}
}

void mScrollStringScrollReset (	MScrollString*	b	) {
	b->stringNow	=	b->stringBase;
}

static MResult draw ( MElement* b, MakiseGUI *gui ) {
	MScrollString *p = ( MScrollString* )b->data;

	/// Рамка вокруг строки.
	_m_e_helper_draw_box_param( gui->buffer, &b->position, p->style->borderColor, p->style->bgColor, 0 );

	/// Печатям строку.
	makise_d_string(	gui->buffer,
						p->stringNow,
						MDTextAll,					/// Размер будет ограничен внутри автоматически.
						b->position.real_x + 2,
						b->position.real_y + 2 +
							( ( b->position.height - 4 ) -				/// От высоты отнимаем толщину линии сверху и снизу + 1 пиксель между.
							p->style->fontErcent->height ) / 2,
						MDTextPlacement_LeftUp,
						p->style->fontErcent,
						p->style->fontErcentCol	);

	return M_OK;
}



#ifdef __cplusplus
}
#endif

#endif
