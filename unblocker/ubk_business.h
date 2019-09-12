#include "gpk_array.h"
#include "gpk_coord.h"

#ifndef BUSINESS_H_0239784982374
#define BUSINESS_H_0239784982374

namespace ubk
{
	//////////////////////----------------------------------------------------------------------
	struct SShops			{

	};

	struct SCategories		{
		::gpk::array_obj<::gpk::view_const_string>							Category;
		::gpk::array_obj<::gpk::array_obj<::gpk::view_const_string>>		Subcategories;
		::gpk::array_pod<uint32_t>											Property;
	};


} // namespace

#endif // BUSINESS_H_0239784982374
