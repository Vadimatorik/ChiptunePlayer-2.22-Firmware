#include "gui_element_window_indexing_supported_files.h"

namespace AyPlayer {

WindowIndexingSupportedFiles::WindowIndexingSupportedFiles	(	const MakiseStyle_SList*		style,
																const MakiseStyle_SListItem*	itemsStyle,
																MContainer*						c,
																int32_t							x,
																int32_t							y,
																uint32_t						w,
																uint32_t						h				) {
	this->str	=	new char[ 30 ];

	m_create_slist(	&this->sl,
					c,
					mp_rel( x, y, w, h ),
					this->str,
					nullptr,
					nullptr,
					MSList_List,
					style,
					itemsStyle );

	/// Привязка list-а к его элементам.
	for ( int i = 0; i < 4; i++ ) {
		this->slItem[ i ].text	=	nullptr;
		m_slist_add( &this->sl, &this->slItem[ i ] );
	}
}

void WindowIndexingSupportedFiles::setActualState ( const char* const stateMessage ) {
	memcpy( this->str, stateMessage, 30 );
}

// Метод сдвигает вниз все строки (1 удаляется) и добавляет вверх новую.
void WindowIndexingSupportedFiles::addItem ( const char* const itemName ) {
	/// Если раньше там была не пустая строка.
	if ( this->slItem[ this->cout - 1 ].text != nullptr ) {
		delete[] this->slItem[  this->cout - 1 ].text;
	}

	for ( uint32_t l =  this->cout - 1; l > 0 ; l-- ) {
		this->slItem[ l ].text = this->slItem[ l - 1 ].text;
	}

	uint32_t lenString = strlen( itemName ) + 1;
	this->slItem[ 0 ].text = new char[ lenString ];
	strcpy( this->slItem[ 0 ].text, itemName );
}

WindowIndexingSupportedFiles::~WindowIndexingSupportedFiles() {
	delete[] this->str;
	makise_g_cont_rem( &this->sl.el );
}

}

