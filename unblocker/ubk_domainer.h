#include "unblocker.h"

#include "gpk_enum.h"
#include "gpk_view_manager.h"
#include "gpk_block.h"
#include "gpk_json_expression.h"

#ifndef DOMAINER_H_2983749823748293
#define DOMAINER_H_2983749823748293

namespace ubk
{
	GDEFINE_ENUM_TYPE(URL_SCHEME, uint8_t)
	GDEFINE_ENUM_VALUE(URL_SCHEME, default	, 0);
	GDEFINE_ENUM_VALUE(URL_SCHEME, http		, 1);
	GDEFINE_ENUM_VALUE(URL_SCHEME, https	, 2);
	GDEFINE_ENUM_VALUE(URL_SCHEME, ftp		, 3);
	GDEFINE_ENUM_VALUE(URL_SCHEME, mailto	, 4);
	GDEFINE_ENUM_VALUE(URL_SCHEME, file		, 5);
	GDEFINE_ENUM_VALUE(URL_SCHEME, data		, 6);
	GDEFINE_ENUM_VALUE(URL_SCHEME, irc		, 7);

	struct SURLMapBlock			{
				::gpk::CViewManager<char_t, 1023U*64U>		Allocator					;

		typedef	::gpk::SInt24								_tIndex;
				::gpk::array_pod<::ubk::URL_SCHEME>			Scheme						;
				::gpk::array_pod<_tIndex>					Authority					;
				::gpk::array_pod<_tIndex>					Path						;
				::gpk::array_pod<_tIndex>					Query						;
				::gpk::array_pod<_tIndex>					Fragment					;

				::gpk::error_t								Save						(::gpk::array_pod<byte_t> & output)					const;
				::gpk::error_t								Load						(const ::gpk::view_const_byte & input);

				::gpk::error_t								AddURLMap					(const ::gpk::view_const_char & textToAdd);
				::gpk::error_t								GetURLMapId					(const ::gpk::view_const_char & textToAdd)			const;
				::gpk::error_t								GetURL						(int32_t index, ::gpk::array_pod<char_t> & url)		const;

		inline	::gpk::error_t								GetAuthority				(int32_t index, ::gpk::view_const_char & output)	const	{ const int32_t viewIndex = Authority	[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
		inline	::gpk::error_t								GetPath						(int32_t index, ::gpk::view_const_char & output)	const	{ const int32_t viewIndex = Path		[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
		inline	::gpk::error_t								GetQuery					(int32_t index, ::gpk::view_const_char & output)	const	{ const int32_t viewIndex = Query		[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
		inline	::gpk::error_t								GetFragment					(int32_t index, ::gpk::view_const_char & output)	const	{ const int32_t viewIndex = Fragment	[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
	};

	struct SSMTPMapBlock			{
				::gpk::CViewManager<char_t, 1024*64>		Allocator					;

		typedef	::gpk::SInt24								_tIndex;
				::gpk::array_pod<_tIndex>					Username					;
				::gpk::array_pod<_tIndex>					Domain						;

				::gpk::error_t								Save						(::gpk::array_pod<byte_t> & output)					const;
				::gpk::error_t								Load						(const ::gpk::view_const_byte & input);

				::gpk::error_t								AddSMTPMap					(const ::gpk::view_const_char & textToAdd);
				::gpk::error_t								GetSMTPMapId				(const ::gpk::view_const_char & textToAdd)			const;
				::gpk::error_t								GetEMail					(int32_t index, ::gpk::array_pod<char_t> & email)	const;
		inline	::gpk::error_t								GetDomain					(int32_t index, ::gpk::view_const_char & output)	const	{ const int32_t viewIndex = Domain	[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
		inline	::gpk::error_t								GetUsername					(int32_t index, ::gpk::view_const_char & output)	const	{ const int32_t viewIndex = Username[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
	};

	struct SDomainer {
				::gpk::SMapBlock<::ubk::SSMTPMapBlock>		Email;
				::gpk::SMapBlock<::ubk::SURLMapBlock>		URL;

				::gpk::view_const_string					DBPath;
				int64_t										AddEMail					(const ::gpk::view_const_char & email);
				int64_t										AddURL						(const ::gpk::view_const_char & url);
				int64_t										GetIdURL					(const ::gpk::view_const_char & email)					;
				int64_t										GetIdEMail					(const ::gpk::view_const_char & url)					;
				::gpk::error_t								GetEMail					(const uint64_t id, ::gpk::array_pod<char_t> & email)	;
				::gpk::error_t								GetURL						(const uint64_t id, ::gpk::array_pod<char_t> & url)		;
	};

			::gpk::error_t								splitEmail						(const ::gpk::view_const_char & textToAdd, ::gpk::view_const_char & username, ::gpk::view_const_char & domain);
			::gpk::error_t								splitURL						(const ::gpk::view_const_char & textToAdd, ::ubk::URL_SCHEME & scheme, ::gpk::view_const_char & authority, ::gpk::view_const_char & path, ::gpk::view_const_char & query, ::gpk::view_const_char & fragment);
} // namespace

#endif // DOMAINER_H_2983749823748293
