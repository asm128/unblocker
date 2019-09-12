#include "gpk_array.h"

#ifndef UBK_PEOPLE_H_9283742923847
#define UBK_PEOPLE_H_9283742923847

namespace ubk
{
	//////////////////////----------------------------------------------------------------------
	struct SNames			{
		::gpk::array_obj<::gpk::view_const_string>							First;	// Unbound
		::gpk::array_obj<::gpk::view_const_string>							Last;
	};

	struct SNameMap			{
		::gpk::array_obj<::gpk::view_array<uint32_t>>						First;
		::gpk::array_obj<::gpk::view_array<uint32_t>>						Last;
	};

	struct SPeople			{

	};
} // namespace

#endif // UBK_PEOPLE_H_9283742923847
