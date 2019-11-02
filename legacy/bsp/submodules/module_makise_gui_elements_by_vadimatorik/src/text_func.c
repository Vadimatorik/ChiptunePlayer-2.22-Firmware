#include <stdint.h>

static void uint_to_2_char ( uint32_t* number, char* p ) {
	*p			= *number / 10 + '0';
	*(p + 1)	= *number % 10 + '0';
}

// Convert the time represented in seconds to hours:minutes:seconds (HH:MM:SS).
void convert_time_sec_to_char ( uint32_t time_sec, char* returnString ) {
	const uint32_t one_min	= 60;
	const uint32_t one_hour = 60 * one_min;

	// Convert sec on hours, minutes and seconds.
	uint32_t hour		= 0;
	uint32_t min		= 0;
	uint32_t sec		= time_sec;

	hour		= sec / one_hour;
	sec		-= hour * one_hour;

	min		= sec / one_min;
	sec		-= min * one_min;

	uint_to_2_char( &hour, &returnString[0] );
	uint_to_2_char( &min, &returnString[3] );
	uint_to_2_char( &sec, &returnString[6] );
}

void convert_uint32_t_to_string ( uint32_t value, char* returnString ) {
	char* p = returnString;
	uint32_t dev = 1000000000;
	uint32_t v = value;
	uint32_t flag_start_char = 0;
	for ( int l = 0; l < 9; l++ ) {
		uint32_t buf = v / dev;
		if ( flag_start_char != 0 ) {
			 *p = buf + '0';
			 p++;
		} else {
			if ( buf != 0 ) {
				flag_start_char = 0xFFFFFFFF;
				*p = buf + '0';
				p++;
			}
		}
		v %= dev;
		dev /= 10;
	}
	*p = v + '0';
	p++;
	*p = 0;
}
