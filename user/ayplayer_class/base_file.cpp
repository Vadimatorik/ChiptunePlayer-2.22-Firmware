#include "base.h"

namespace AyPlayer {

int Base::initFileList ( void ) {
	std::shared_ptr< char > fatFspath( new char[ 1024 ], std::default_delete< char[] >() );

	strcpy( fatFspath.get(), "0:" );

	return EOK;
}

}
