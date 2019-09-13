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
						::gpk::CViewManager<char_t>							AllocatorChars				= {};
						::gpk::CViewManager<_tIndex>						AllocatorMaps				= {};

						::gpk::error_t										Save						(::gpk::array_pod<byte_t> & output)								const;
						::gpk::error_t										Load						(const ::gpk::view_const_byte & input);

						::gpk::error_t										AddMap						(const ::gpk::view_const_char & textToAdd);
						::gpk::error_t										GetMapId					(const ::gpk::view_const_char & textToAdd)						const;
						::gpk::error_t										GetMap						(int32_t index, ::gpk::array_pod<char_t> & url)					const;
	};

	struct SMapBlockURL {
						::gpk::CViewManager<char_t>							Allocator					= {};

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
						::gpk::CViewManager<char_t>							Allocator					;

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

	GDEFINE_ENUM_TYPE(DOMAINER_SYSTEM, uint8_t);
	GDEFINE_ENUM_VALUE(DOMAINER_SYSTEM, URL		, 0);
	GDEFINE_ENUM_VALUE(DOMAINER_SYSTEM, EMAIL	, 1);
	GDEFINE_ENUM_VALUE(DOMAINER_SYSTEM, PATH	, 2);

	struct SDomainer {
		struct SDomainerTables {
							::gpk::SMapTable<::ubk::SMapBlockEMail>				Email;
							::gpk::SMapTable<::ubk::SMapBlockURL>				URL;
							::gpk::SMapTable<::ubk::SMapBlockPath>				Path;
		}																	Tables;

						::gpk::view_const_char								DBPath;

		inline			::gpk::error_t										MapGet						(DOMAINER_SYSTEM table, const uint64_t idRecord, ::gpk::array_pod<char_t> & email	)	{ switch(table) { case DOMAINER_SYSTEM_URL: return ::gpk::mapTableGetMap	(Tables.URL	, idRecord, DBPath, email);	case DOMAINER_SYSTEM_EMAIL: return ::gpk::mapTableGetMap	(Tables.Email	, idRecord, DBPath, email);	case DOMAINER_SYSTEM_PATH: return ::gpk::mapTableGetMap	(Tables.Path	, idRecord, DBPath, email);	default: error_printf("Invalid table type: %u '%s'", ::gpk::get_value_label(table).begin()); return -1; } }
		inline			int64_t												MapGetId					(DOMAINER_SYSTEM table, const ::gpk::view_const_char & email)							{ switch(table) { case DOMAINER_SYSTEM_URL: return ::gpk::mapTableMapId		(Tables.URL	, DBPath, email	);			case DOMAINER_SYSTEM_EMAIL: return ::gpk::mapTableMapId		(Tables.Email	, DBPath, email	);			case DOMAINER_SYSTEM_PATH: return ::gpk::mapTableMapId	(Tables.Path	, DBPath, email	);			default: error_printf("Invalid table type: %u '%s'", ::gpk::get_value_label(table).begin()); return -1; } }
		inline			int64_t												MapAdd						(DOMAINER_SYSTEM table, const ::gpk::view_const_char & email)							{ switch(table) { case DOMAINER_SYSTEM_URL: return ::gpk::mapTableMapAdd	(Tables.URL	, DBPath, email	);			case DOMAINER_SYSTEM_EMAIL: return ::gpk::mapTableMapAdd	(Tables.Email	, DBPath, email	);			case DOMAINER_SYSTEM_PATH: return ::gpk::mapTableMapAdd	(Tables.Path	, DBPath, email	);			default: error_printf("Invalid table type: %u '%s'", ::gpk::get_value_label(table).begin()); return -1; } }
	};

	static inline	::gpk::error_t										splitEmail					(const ::gpk::view_const_char & email, ::gpk::view_const_char & username, ::gpk::view_const_char & domain)		{ return ::gpk::split<const char_t>('@', email, username, domain); }
					::gpk::error_t										splitURL					(const ::gpk::view_const_char & textToAdd, ::ubk::URL_SCHEME & scheme, ::gpk::view_const_char & authority, ::gpk::view_const_char & path, ::gpk::view_const_char & query, ::gpk::view_const_char & fragment);

	GDEFINE_ENUM_TYPE(MAPTABLE_COMMAND, uint8_t);
	GDEFINE_ENUM_VALUE(MAPTABLE_COMMAND, GetMap		, 0);
	GDEFINE_ENUM_VALUE(MAPTABLE_COMMAND, GetMapId	, 1);
	GDEFINE_ENUM_VALUE(MAPTABLE_COMMAND, AddMap		, 2);

	struct SDomainerCommand {};
} // namespace

#endif // DOMAINER_H_2983749823748293
