#include "unblocker.h"

#include "gpk_enum.h"
#include "gpk_view_manager.h"
#include "gpk_block.h"
#include "gpk_json_expression.h"

#ifndef DOMAINER_H_2983749823748293
#define DOMAINER_H_2983749823748293

namespace ubk
{
	GDEFINE_ENUM_TYPE(URL_SCHEME, uint8_t);
	GDEFINE_ENUM_VALUE(URL_SCHEME, default	, 0);
	GDEFINE_ENUM_VALUE(URL_SCHEME, http		, 1);
	GDEFINE_ENUM_VALUE(URL_SCHEME, https	, 2);
	GDEFINE_ENUM_VALUE(URL_SCHEME, ftp		, 3);
	GDEFINE_ENUM_VALUE(URL_SCHEME, mailto	, 4);
	GDEFINE_ENUM_VALUE(URL_SCHEME, file		, 5);
	GDEFINE_ENUM_VALUE(URL_SCHEME, data		, 6);
	GDEFINE_ENUM_VALUE(URL_SCHEME, irc		, 7);

	struct SMapBlockPath {
		typedef			::gpk::SInt24										_tIndex;
						::gpk::CViewManager<char_t, 0xFFFFU>				AllocatorChars				= {};
						::gpk::CViewManager<_tIndex, 0xFFFFU>				AllocatorMaps				= {};

						::gpk::error_t										Save						(::gpk::array_pod<byte_t> & output)								const;
						::gpk::error_t										Load						(const ::gpk::view_const_byte & input);

						::gpk::error_t										AddMap						(const ::gpk::view_const_char & textToAdd);
						::gpk::error_t										GetMapId					(const ::gpk::view_const_char & textToAdd)						const;
						::gpk::error_t										GetMap						(int32_t index, ::gpk::array_pod<char_t> & url)					const;
	};

	struct SMapBlockURL {
						::gpk::CViewManager<char_t, 0xFFFFU>				Allocator					= {};

		typedef			::gpk::SInt24										_tIndex;
						::gpk::array_pod<::ubk::URL_SCHEME>					Scheme						= {};
						::gpk::array_pod<_tIndex>							Authority					= {};
						::gpk::array_pod<_tIndex>							Path						= {};
						::gpk::array_pod<_tIndex>							Query						= {};
						::gpk::array_pod<_tIndex>							Fragment					= {};

						::gpk::error_t										Save						(::gpk::array_pod<byte_t> & output)								const;
						::gpk::error_t										Load						(const ::gpk::view_const_byte & input);

						::gpk::error_t										AddMap						(const ::gpk::view_const_char & textToAdd);
						::gpk::error_t										GetMapId					(const ::gpk::view_const_char & textToAdd)						const;
						::gpk::error_t										GetMap						(int32_t index, ::gpk::array_pod<char_t> & url)					const;

		inline			::gpk::error_t										GetAuthority				(int32_t index, ::gpk::view_const_char & output)				const	{ const int32_t viewIndex = Authority	[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
		inline			::gpk::error_t										GetPath						(int32_t index, ::gpk::view_const_char & output)				const	{ const int32_t viewIndex = Path		[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
		inline			::gpk::error_t										GetQuery					(int32_t index, ::gpk::view_const_char & output)				const	{ const int32_t viewIndex = Query		[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
		inline			::gpk::error_t										GetFragment					(int32_t index, ::gpk::view_const_char & output)				const	{ const int32_t viewIndex = Fragment	[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
	};

	struct SMapBlockEMail {
						::gpk::CViewManager<char_t, 0xFFFFU>				Allocator					;

		typedef			::gpk::SInt24										_tIndex;
						::gpk::array_pod<_tIndex>							Username					;
						::gpk::array_pod<_tIndex>							Domain						;

						::gpk::error_t										Save						(::gpk::array_pod<byte_t> & output)								const;
						::gpk::error_t										Load						(const ::gpk::view_const_byte & input);

						::gpk::error_t										AddMap						(const ::gpk::view_const_char & textToAdd);
						::gpk::error_t										GetMapId					(const ::gpk::view_const_char & textToAdd)						const;
						::gpk::error_t										GetMap						(int32_t index, ::gpk::array_pod<char_t> & email)				const;

		inline			::gpk::error_t										GetDomain					(int32_t index, ::gpk::view_const_char & output)				const	{ const int32_t viewIndex = Domain		[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
		inline			::gpk::error_t										GetUsername					(int32_t index, ::gpk::view_const_char & output)				const	{ const int32_t viewIndex = Username	[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
	};

	struct SDomainer {
						::gpk::SMapTable<::ubk::SMapBlockEMail>				Email;
						::gpk::SMapTable<::ubk::SMapBlockURL>				URL;

						::gpk::view_const_char								DBPath;

		inline			::gpk::error_t										GetEMail					(const uint64_t idRecord, ::gpk::array_pod<char_t> & email	)			{ return ::gpk::mapTableGetMap(Email, idRecord, DBPath, email); }
		inline			::gpk::error_t										GetURL						(const uint64_t idRecord, ::gpk::array_pod<char_t> & url	)			{ return ::gpk::mapTableGetMap(URL	, idRecord, DBPath, url); }
		inline			int64_t												GetIdEMail					(const ::gpk::view_const_char & email)									{ return ::gpk::mapTableMapId(Email	, DBPath, email	);	}
		inline			int64_t												GetIdURL					(const ::gpk::view_const_char & url)									{ return ::gpk::mapTableMapId(URL	, DBPath, url	);	}
		inline			int64_t												AddEMail					(const ::gpk::view_const_char & email)									{ return ::gpk::mapTableMapAdd(Email, DBPath, email	);	}
		inline			int64_t												AddURL						(const ::gpk::view_const_char & url)									{ return ::gpk::mapTableMapAdd(URL	, DBPath, url	);	}
	};

	static inline	::gpk::error_t										splitEmail					(const ::gpk::view_const_char & email, ::gpk::view_const_char & username, ::gpk::view_const_char & domain)		{ return ::gpk::split<const char_t>('@', email, username, domain); }
					::gpk::error_t										splitURL					(const ::gpk::view_const_char & textToAdd, ::ubk::URL_SCHEME & scheme, ::gpk::view_const_char & authority, ::gpk::view_const_char & path, ::gpk::view_const_char & query, ::gpk::view_const_char & fragment);

	GDEFINE_ENUM_TYPE(DOMAINER_SYSTEM, uint8_t);
	GDEFINE_ENUM_VALUE(DOMAINER_SYSTEM, URL	, 0);
	GDEFINE_ENUM_VALUE(DOMAINER_SYSTEM, SMTP, 1);

	GDEFINE_ENUM_TYPE(MAPTABLE_COMMAND, uint8_t);
	GDEFINE_ENUM_VALUE(MAPTABLE_COMMAND, GetMap		, 0);
	GDEFINE_ENUM_VALUE(MAPTABLE_COMMAND, GetMapId	, 1);
	GDEFINE_ENUM_VALUE(MAPTABLE_COMMAND, AddMap		, 2);

	struct SDomainerCommand {};
} // namespace

#endif // DOMAINER_H_2983749823748293
