#include "base.h"

namespace AyPlayer {

int Base::printMessage ( RTL_TYPE_M type, const char* const message ) {
	return this->cfg->l->sendMessage( type, message );
}

int Base::printMessageAndArg ( RTL_TYPE_M type, const char* const message, const char* const arg ) {
	const uint32_t argLen				=	strlen( arg );
	const uint32_t messageLen			=	strlen( message );
	const uint32_t stringLen			=	argLen + messageLen + 2;

	char* outputMessage = new char[ stringLen ];
	if ( outputMessage == nullptr )		return ENOMEM;

	memcpy( &outputMessage[ 0 ], message, messageLen );
	outputMessage[ messageLen ] = '\t';
	memcpy( &outputMessage[ messageLen + 1 ], arg, argLen );
	outputMessage[ stringLen - 1 ] = 0;

	int result;
	result = this->printMessage( type, outputMessage );

	delete[] outputMessage;
	return result;
}

}
