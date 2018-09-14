#include "base.h"

namespace AyPlayer {

int Base::initFileLists ( void ) {
	std::shared_ptr< char > fatFspath( new char[ 1024 ], std::default_delete< char[] >() );

	strcpy( fatFspath.get(), "0:" );

	this->cfg->l->sendMessage( RTL_TYPE_M::RUN_MESSAGE_OK, "Init file lists!" );

	return EOK;
}

}
