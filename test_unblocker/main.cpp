#include "ubk_domainer.h"

int testDomainerSMTP() {
	::ubk::SSMTPMapBlock				smtpOrigin;
	::ubk::SSMTPMapBlock				smtpCopy;
	{
		const ::gpk::view_const_string		strings	[]	=
			{"prueba0@prueba.com"
			,"prueba1@prueba.com"
			,"prueba2@hotmail.com"
			,"prueba0@gmail.com"
			};
		::gpk::array_pod<char_t>			file;
		for(uint32_t iString = 0; iString < ::gpk::size(strings); ++iString) {
			const int32_t						indexOfMap			= smtpOrigin.AddSMTPMap(strings[iString]);
			ce_if((uint32_t)indexOfMap != iString, "%s", "Test failed!");
			::gpk::array_pod<char_t>			stored;
			ce_if(smtpOrigin.GetEMail(indexOfMap, stored), "%s", "Test failed!");
			ce_if(stored != strings[iString], "Stored: %s. Original: %s.", ::gpk::toString(stored).begin(), strings[iString].begin());
		}
		ree_if(smtpOrigin	.Save(file), "%s", "Error!");
		ree_if(smtpCopy		.Load(file), "%s", "Error!");
	}
	{
		for(uint32_t iDomain = 0; iDomain < smtpCopy.Domain.size(); ++iDomain) {
			const uint32_t						idxCopyDomain		= smtpCopy.Domain		[iDomain];
			const uint32_t						idxCopyUsername		= smtpCopy.Username		[iDomain];
			const uint32_t						idxOriginDomain		= smtpOrigin.Domain		[iDomain];
			const uint32_t						idxOriginUsername	= smtpOrigin.Username	[iDomain];
			ce_if(idxCopyDomain	!= idxOriginDomain	, "%s", "Test failed!");
			ce_if(idxCopyUsername	!= idxOriginUsername, "%s", "Test failed!");
			const ::gpk::view_const_char		strOriginDomain		= {smtpOrigin	.Allocator.Views[idxOriginDomain	], smtpOrigin	.Allocator.Counts[idxOriginDomain	]};
			const ::gpk::view_const_char		strOriginUsername	= {smtpOrigin	.Allocator.Views[idxOriginUsername	], smtpOrigin	.Allocator.Counts[idxOriginUsername	]};
			const ::gpk::view_const_char		strCopyDomain		= {smtpCopy		.Allocator.Views[idxCopyDomain		], smtpCopy		.Allocator.Counts[idxCopyDomain		]};
			const ::gpk::view_const_char		strCopyUsername		= {smtpCopy		.Allocator.Views[idxCopyUsername	], smtpCopy		.Allocator.Counts[idxCopyUsername	]};
			ce_if(strCopyDomain	!= strOriginDomain		, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginDomain	).begin(), ::gpk::toString(strCopyDomain	).begin());
			ce_if(strCopyUsername	!= strOriginUsername	, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginUsername).begin(), ::gpk::toString(strCopyUsername	).begin());
		}
	}
	return 0;
}

int testDomainerURL() {
	::ubk::SURLMapBlock						smtpOrigin;
	::ubk::SURLMapBlock						smtpCopy;
	{
		const ::gpk::view_const_string		strings	[]	=
			{ "https://prueba0@gmail.com/ruta?querystring#fragmento"
			, "https://prueba0@prueba.com/ruta"
			, "https://prueba1@prueba.com/ruta"
			, "https://prueba2@hotmail.com/"
			,
			};
		::gpk::array_pod<char_t>			file;
		for(uint32_t iString = 0; iString < ::gpk::size(strings); ++iString) {
			const int32_t						indexOfMap			= smtpOrigin.AddURLMap(strings[iString]);
			ce_if((uint32_t)indexOfMap != iString, "%s", "Test failed!");
			::gpk::array_pod<char_t>			stored;
			ce_if(smtpOrigin.GetURL(indexOfMap, stored), "%s", "Test failed!");
			ce_if(stored != strings[iString], "Stored: %s. Original: %s.", ::gpk::toString(stored).begin(), strings[iString].begin());
		}
	//	ree_if(smtpOrigin	.Save(file), "%s", "Error!");
	//	ree_if(smtpCopy		.Load(file), "%s", "Error!");
	//}
	//{
	//	for(uint32_t iDomain = 0; iDomain < smtpCopy.Domain.size(); ++iDomain) {
	//		const uint32_t						idxCopyDomain		= smtpCopy.Domain		[iDomain];
	//		const uint32_t						idxCopyUsername		= smtpCopy.Username		[iDomain];
	//		const uint32_t						idxOriginDomain		= smtpOrigin.Domain		[iDomain];
	//		const uint32_t						idxOriginUsername	= smtpOrigin.Username	[iDomain];
	//		ree_if(idxCopyDomain	!= idxOriginDomain	, "%s", "Test failed!");
	//		ree_if(idxCopyUsername	!= idxOriginUsername, "%s", "Test failed!");
	//		const ::gpk::view_const_char		strOriginDomain		= {smtpOrigin	.Allocator.Views[idxOriginDomain	], smtpOrigin	.Allocator.Counts[idxOriginDomain	]};
	//		const ::gpk::view_const_char		strOriginUsername	= {smtpOrigin	.Allocator.Views[idxOriginUsername	], smtpOrigin	.Allocator.Counts[idxOriginUsername	]};
	//		const ::gpk::view_const_char		strCopyDomain		= {smtpCopy		.Allocator.Views[idxCopyDomain		], smtpCopy		.Allocator.Counts[idxCopyDomain		]};
	//		const ::gpk::view_const_char		strCopyUsername		= {smtpCopy		.Allocator.Views[idxCopyUsername	], smtpCopy		.Allocator.Counts[idxCopyUsername	]};
	//		ree_if(strCopyDomain	!= strOriginDomain		, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginDomain	).begin(), ::gpk::toString(strCopyDomain	).begin());
	//		ree_if(strCopyUsername	!= strOriginUsername	, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginUsername).begin(), ::gpk::toString(strCopyUsername	).begin());
	//	}
	}
	return 0;
}

int main() {
	gerror_if(errored(testDomainerSMTP()), "%s", "Error!");
	gerror_if(errored(testDomainerURL()), "%s", "Error!");
	return 0;
}
