#include "slim_horizontal_list.h"

#if defined(ELEMENT_SLIM_HORIZONTAL_LIST_ENABLED) && defined(ELEMENT_SCROLL_STRING_ENABLED)

#ifdef __cplusplus
extern "C" {
#endif

static MResult draw ( MElement* b, MakiseGUI *gui );

static char *name = "SlimHorizontalList";

void mCreateSlimHorizontalList (	MSlimHorizontalList*						b,
									MContainer*									c,
									MPosition									pos,
									const MakiseStyle_SMSlimHorizontalList*		style	) {
	MElement *e = &b->el;

	m_element_create(	e, name, b,
						 1, MFocusPrior_NotFocusble,
						 pos, &draw,
						 0,	 0,
						 NULL,	 NULL,
						 0, 0	);

	/// Изначально строки нет.
	b->stringBase	=	NULL;
	b->stringNow	=	NULL;

	b->style		=	style;

	b->maxItem		=	0;
	b->currenItem	=	0;

	b->dxPixChar	=	0;
	b->dxPixNow		=	0;

	makise_g_cont_add( c, e );

#if ( MAKISE_ENABLE_DEBUG_OUTPUT > 0 )
	MAKISE_DEBUG_OUTPUT( "SlimHorizontalList %d created\n", e->id );
#endif
}

static void initStringPointer ( MSlimHorizontalList*	b, const char* stringItem ) {
	b->dxPixNow		=	0;

	uint32_t len;
	len = makise_d_string_width( b->stringBase, MDTextAll, b->style->fontString );
	if ( len > b->el.position.width - 8 * 2 - 2 ) {
		b->dxPixChar	=	makise_d_string_width( b->stringNow, 1, b->style->fontString );
	} else {
		b->dxPixChar	=	0;		/// Сдвиг не нужен.
	}
}

int mSlimHorizontalListScrollString (	MSlimHorizontalList*	b	) {
	if ( b->stringNow == NULL )
		return 0;

	uint32_t	len;
	len = makise_d_string_width( b->stringNow, MDTextAll, b->style->fontString );

	/// Прокрутили весь символ.
	if ( b->dxPixNow != b->dxPixChar ) {			/// Если не прокрутили весь пиксель, то прокручиваем.
		b->dxPixNow++;

		/// Тест на то, что при следующей итерации мы не перейдем в начало.
		if ( ( !( len - b->dxPixNow - 1 > b->el.position.width - 8 * 2 - 2 ) ) &&
				( b->dxPixNow == b->dxPixChar ) ) {
			return 1;
		}

		return 0;
	}

	b->stringNow++;							/// Сдвигаемся на 1 символ.
	len = makise_d_string_width( b->stringNow, MDTextAll, b->style->fontString );

	/// Если строка не вмещается в экран.
	if ( len > b->el.position.width - 8 * 2 - 2 ) {
		b->dxPixNow = 1;
		b->dxPixChar	=	makise_d_string_width( b->stringNow, 1, b->style->fontString );
	} else {
		b->stringNow = b->stringBase;
		initStringPointer( b, b->stringBase );
		return 1;
	}

	return 0;
}

void mSlimHorizontalListSetStringCurrentItem (	MSlimHorizontalList*	b, const char* stringItem	) {
	b->stringBase	=	stringItem;
	b->stringNow	=	stringItem;

	initStringPointer( b, stringItem );
}

void mSlimHorizontalSetItemCount (	MSlimHorizontalList*	b, uint32_t itemCount	) {
	b->maxItem	=	itemCount - 1;
}

void mSlimHorizontalListLeft (	MSlimHorizontalList*	b, const char* stringItem	) {
	if ( b->currenItem > 0 ) {
		b->currenItem--;
		initStringPointer( b, stringItem );
	}
}

void mSlimHorizontalListRight (	MSlimHorizontalList*	b, const char* stringItem	) {
	if ( b->currenItem != b->maxItem ) {
		b->currenItem++;
		initStringPointer( b, stringItem );
	}
}

static const uint8_t B_arrowLeftEmpty_data[] = {
	0xc8, 0x9a, 0xa9, 0x8c, 0x00
};

const MakiseBitmap B_arrowLeftEmpty = {
	.width		=	4,
	.height		=	8,
	.data		=	B_arrowLeftEmpty_data
};

static const uint8_t B_arrowLeftFull_data[] = {
	0xc8, 0xfe, 0xef, 0x8c, 0x00
};

const MakiseBitmap B_arrowLeftFull = {
	.width		=	4,
	.height		=	8,
	.data		=	B_arrowLeftFull_data
};

static const uint8_t B_arrowRightFull_data[] = {
	0x31, 0xf7, 0x7f, 0x13, 0x00
};

const MakiseBitmap B_arrowRightFull = {
	.width		=	4,
	.height		=	8,
	.data		=	B_arrowRightFull_data
};

static const uint8_t B_arrowRightEmpty_data[] = {
	0x31, 0x95, 0x59, 0x13, 0x00
};

const MakiseBitmap B_arrowRightEmpty = {
	.width		=	4,
	.height		=	8,
	.data		=	B_arrowRightEmpty_data
};


static MResult draw ( MElement* b, MakiseGUI *gui ) {
	MSlimHorizontalList *p = ( MSlimHorizontalList* )b->data;

	/// Рамка на весь объект.
	_m_e_helper_draw_box_param( gui->buffer, &b->position, p->style->borderColor, p->style->bgColor, 0 );

	MPosition			pos		=		b->position;

	/// Печатаем строку.
	/// Строка будет съезжать влево на левый квадрат со стрелкой.
	/// это нормально. Его перетрут дальше (перетрут мусор, который вылезет за пределы.
	if ( p->stringNow != NULL ) {
		makise_d_string(	gui->buffer,
							p->stringNow,
							MDTextAll,								/// Размер будет ограничен внутри автоматически.
							pos.real_x + 2 + 8 - 1 - p->dxPixNow,
							pos.real_y + 2,
							MDTextPlacement_LeftUp,
							p->style->fontString,
							p->style->stringColor	);

		/// Затираем строку между левой палкой и строкой.
		makise_d_line(	gui->buffer,
						pos.real_x + 8,
						pos.real_y + 1,
						pos.real_x + 8,
						pos.real_y + pos.height - 2,
						p->style->bgColor );

		/// Затираем строку между правой палкой и строкой.
		makise_d_line(	gui->buffer,
						pos.real_x + b->position.width - 8 - 1,
						pos.real_y + 1,
						pos.real_x + b->position.width - 8 - 1,
						pos.real_y + pos.height - 2,
						p->style->bgColor );
	}

	/// Вычисляем габариты для прямоугольника, окружающего левую стрелку.
	pos.width					=		8;

	/// Рамка под левое поле.
	_m_e_helper_draw_box_param( gui->buffer, &pos, p->style->borderColor, p->style->bgColor, 0 );

	/// Рисуем левую стрелку.
	if ( p->currenItem != 0 ) {
		makise_d_bitmap( gui->buffer, pos.real_x + 2, pos.real_y + 2, &B_arrowLeftFull, p->style->bgArrowColor );			/// У нас не самый крайний элемент слева.
	} else {
		makise_d_bitmap( gui->buffer, pos.real_x + 2, pos.real_y + 2, &B_arrowLeftEmpty, p->style->bgArrowColor );			/// 0-й элемент (самый левый).
	}

	/// Вычисляем габариты для прямоугольника, окружающего правую стрелку.
	pos.real_x		+=	b->position.width - 8;
	pos.width		=	8;

	/// Рамка под правое поле.
	_m_e_helper_draw_box_param( gui->buffer, &pos, p->style->borderColor, p->style->bgColor, 0 );

	/// Рисуем левую стрелку.
	if ( p->currenItem !=  p->maxItem ) {
		makise_d_bitmap( gui->buffer, pos.real_x + 2, pos.real_y + 2, &B_arrowRightFull, p->style->bgArrowColor );			/// У нас не самый крайний элемент слева.
	} else {
		makise_d_bitmap( gui->buffer, pos.real_x + 2, pos.real_y + 2, &B_arrowRightEmpty, p->style->bgArrowColor );			/// 0-й элемент (самый левый).
	}

	return M_OK;
}



#ifdef __cplusplus
}
#endif

#endif
