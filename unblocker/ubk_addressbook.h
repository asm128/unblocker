#include "unblocker.h"

#include "gpk_view_manager.h"
#include "gpk_block.h"

#ifndef UBK_ADDRESSBOOK_H_2908749283742398749
#define UBK_ADDRESSBOOK_H_2908749283742398749

namespace ubk
{
	typedef			::gpk::SMapBlock											SMapBlockHours;
	typedef			::gpk::SMapBlock											SMapBlockPhones;

	//struct SMapBlockPhones	{
	//					::gpk::CViewManager<byte_t, 1023U*64U>						Allocator					;
	//
	//	typedef			::gpk::SInt24												_tIndex;
	//					::gpk::array_pod<_tIndex>									Indices						;
	//
	//					::gpk::error_t												Save						(::gpk::array_pod<byte_t> & output)								const;
	//					::gpk::error_t												Load						(const ::gpk::view_const_byte & input);
	//
	//					::gpk::error_t												AddMap						(const ::gpk::view_const_char & dataToAdd);
	//					::gpk::error_t												GetMapId					(const ::gpk::view_const_char & dataToAdd)						const;
	//					::gpk::error_t												GetMap						(int32_t index, ::gpk::array_pod<char_t> & data)				const;
	//};

	struct SImages			{
						::gpk::array_obj<::gpk::view_const_string>					Name;
						::gpk::array_obj<::gpk::view_const_string>					FileName;
						::gpk::array_obj<int64_t>									URL;
						::gpk::array_obj<::gpk::view_const_string>					Notice;
						::gpk::array_obj<::gpk::view_const_string>					Alt;
						::gpk::array_pod<::ubk::SImageProperty>						Property;
	};

	struct SAddresses		{
						::gpk::array_obj<::gpk::view_const_string>					AddressLine2;		// Unbound
						::gpk::array_obj<::gpk::view_const_string>					AddressLine1;
						::gpk::array_obj<::gpk::view_const_string>					ZIP;
						::gpk::array_obj<::gpk::view_const_string>					MapURL;
						::gpk::array_pod<::gpk::SCoord2<float>>						MapCoords;
	};

	struct SAddressMap		{
						int32_t														AddressLine2;
						int32_t														AddressLine1;
						int32_t														Neighbour;
						int32_t														City;
	};

	struct SAddressBook		{
						::gpk::array_pod<int64_t>									Images;
						::gpk::array_pod<int64_t>									Websites;
						::gpk::array_pod<int64_t>									Mails;
						::gpk::array_pod<int64_t>									Phones;
						::gpk::array_pod<int64_t>									Hours;
						::gpk::array_pod<::ubk::SAddressMap>						Addresses;
	};
} // namespace ubk

#endif // UBK_ADDRESSBOOK_H_2908749283742398749
