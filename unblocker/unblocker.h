#include "gpk_enum.h"
#include "gpk_coord.h"

#ifndef UNBLOCKER_H_2037849283742
#define UNBLOCKER_H_2037849283742

namespace ubk
{
#pragma pack(push, 1)
	struct SReferral {
							uint64_t								Type	: 8;
							uint64_t								Index	: 56;
	};

	static constexpr	const uint64_t							UNIX_TIME_YEAR_2000_00_00		= 0x386D4380;// 946684800ULL;

#pragma pack(push, 1)
	struct SImageProperty	{
							uint8_t									Remote		: 1;
	};

	struct SSimpleHour		{
							int8_t									Minute		;
							int8_t									Hour		: 5;
							int8_t									WeekDay		: 3;
	};

	typedef				::gpk::SRange<::ubk::SSimpleHour>		SOpenHourRange;

	struct SPhoneNumber		{
							char									Number		[28];
							char									Code		[4];
	};
#pragma pack(pop)

	GDEFINE_ENUM_TYPE(UNBLOCKER_SYSTEM, uint8_t)
	GDEFINE_ENUM_VALUE(UNBLOCKER_SYSTEM, UNKNOWN		, 0);
	GDEFINE_ENUM_VALUE(UNBLOCKER_SYSTEM, MAP_URL		, 1);
	GDEFINE_ENUM_VALUE(UNBLOCKER_SYSTEM, MAP_EMAIL		, 2);
	GDEFINE_ENUM_VALUE(UNBLOCKER_SYSTEM, VOCABULARY		, 3);
	GDEFINE_ENUM_VALUE(UNBLOCKER_SYSTEM, INFINIREPLY	, 4);
	GDEFINE_ENUM_VALUE(UNBLOCKER_SYSTEM, DEMOGRAPHER	, 5);
	GDEFINE_ENUM_VALUE(UNBLOCKER_SYSTEM, SESSION		, 6);
	GDEFINE_ENUM_VALUE(UNBLOCKER_SYSTEM, BUSINESS		, 7);
#pragma pack(pop)
} // namespace

#endif // OBE_CORE_H_2037849283742
