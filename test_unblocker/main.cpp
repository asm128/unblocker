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
			ce_if(idxCopyDomain		!= idxOriginDomain	, "%s", "Test failed!");
			ce_if(idxCopyUsername	!= idxOriginUsername, "%s", "Test failed!");
			const ::gpk::view_const_char		strOriginDomain		= {smtpOrigin	.Allocator.Views[idxOriginDomain	], smtpOrigin	.Allocator.Counts[idxOriginDomain	]};
			const ::gpk::view_const_char		strOriginUsername	= {smtpOrigin	.Allocator.Views[idxOriginUsername	], smtpOrigin	.Allocator.Counts[idxOriginUsername	]};
			const ::gpk::view_const_char		strCopyDomain		= {smtpCopy		.Allocator.Views[idxCopyDomain		], smtpCopy		.Allocator.Counts[idxCopyDomain		]};
			const ::gpk::view_const_char		strCopyUsername		= {smtpCopy		.Allocator.Views[idxCopyUsername	], smtpCopy		.Allocator.Counts[idxCopyUsername	]};
			ce_if(strCopyDomain		!= strOriginDomain		, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginDomain	).begin(), ::gpk::toString(strCopyDomain	).begin());
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
		ree_if(smtpOrigin	.Save(file), "%s", "Error!");
		ree_if(smtpCopy		.Load(file), "%s", "Error!");
	}
	{
		for(uint32_t iDomain = 0; iDomain < smtpCopy.Scheme.size(); ++iDomain) {
			const int32_t						idxCopyScheme		= smtpCopy.Scheme		[iDomain];
			const int32_t						idxCopyAuthority	= smtpCopy.Authority	[iDomain];
			const int32_t						idxCopyPath			= smtpCopy.Path			[iDomain];
			const int32_t						idxCopyQuery		= smtpCopy.Query		[iDomain];
			const int32_t						idxCopyFragment		= smtpCopy.Fragment		[iDomain];

			const int32_t						idxOriginScheme		= smtpOrigin.Scheme		[iDomain];
			const int32_t						idxOriginAuthority	= smtpOrigin.Authority	[iDomain];
			const int32_t						idxOriginPath		= smtpOrigin.Path		[iDomain];
			const int32_t						idxOriginQuery		= smtpOrigin.Query		[iDomain];
			const int32_t						idxOriginFragment	= smtpOrigin.Fragment	[iDomain];
			ce_if(idxCopyScheme		!= idxOriginScheme		, "%s", "Test failed!");
			ce_if(idxCopyAuthority	!= idxOriginAuthority	, "%s", "Test failed!");
			ce_if(idxCopyPath		!= idxOriginPath		, "%s", "Test failed!");
			ce_if(idxCopyQuery		!= idxOriginQuery		, "%s", "Test failed!");
			ce_if(idxCopyFragment	!= idxOriginFragment	, "%s", "Test failed!");
			const ::gpk::view_const_char		strOriginScheme		= (0 > idxOriginScheme		) ? ::gpk::view_const_char{} : ::gpk::view_const_char{smtpOrigin	.Allocator.Views[idxOriginScheme	], smtpOrigin	.Allocator.Counts[idxOriginScheme	]};
			const ::gpk::view_const_char		strOriginAuthority	= (0 > idxOriginAuthority	) ? ::gpk::view_const_char{} : ::gpk::view_const_char{smtpOrigin	.Allocator.Views[idxOriginAuthority	], smtpOrigin	.Allocator.Counts[idxOriginAuthority]};
			const ::gpk::view_const_char		strOriginPath		= (0 > idxOriginPath		) ? ::gpk::view_const_char{} : ::gpk::view_const_char{smtpOrigin	.Allocator.Views[idxOriginPath		], smtpOrigin	.Allocator.Counts[idxOriginPath		]};
			const ::gpk::view_const_char		strOriginQuery		= (0 > idxOriginQuery		) ? ::gpk::view_const_char{} : ::gpk::view_const_char{smtpOrigin	.Allocator.Views[idxOriginQuery		], smtpOrigin	.Allocator.Counts[idxOriginQuery	]};
			const ::gpk::view_const_char		strOriginFragment	= (0 > idxOriginFragment	) ? ::gpk::view_const_char{} : ::gpk::view_const_char{smtpOrigin	.Allocator.Views[idxOriginFragment	], smtpOrigin	.Allocator.Counts[idxOriginFragment	]};
			const ::gpk::view_const_char		strCopyScheme		= (0 > idxCopyScheme		) ? ::gpk::view_const_char{} : ::gpk::view_const_char{smtpCopy		.Allocator.Views[idxCopyScheme		], smtpCopy		.Allocator.Counts[idxCopyScheme		]};
			const ::gpk::view_const_char		strCopyAuthority	= (0 > idxCopyAuthority		) ? ::gpk::view_const_char{} : ::gpk::view_const_char{smtpCopy		.Allocator.Views[idxCopyAuthority	], smtpCopy		.Allocator.Counts[idxCopyAuthority	]};
			const ::gpk::view_const_char		strCopyPath			= (0 > idxCopyPath			) ? ::gpk::view_const_char{} : ::gpk::view_const_char{smtpCopy		.Allocator.Views[idxCopyPath		], smtpCopy		.Allocator.Counts[idxCopyPath		]};
			const ::gpk::view_const_char		strCopyQuery		= (0 > idxCopyQuery			) ? ::gpk::view_const_char{} : ::gpk::view_const_char{smtpCopy		.Allocator.Views[idxCopyQuery		], smtpCopy		.Allocator.Counts[idxCopyQuery		]};
			const ::gpk::view_const_char		strCopyFragment		= (0 > idxCopyFragment		) ? ::gpk::view_const_char{} : ::gpk::view_const_char{smtpCopy		.Allocator.Views[idxCopyFragment	], smtpCopy		.Allocator.Counts[idxCopyFragment	]};
			ce_if(strCopyScheme		!= strOriginScheme		, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginScheme		).begin(), ::gpk::toString(strCopyScheme	).begin());
			ce_if(strCopyAuthority	!= strOriginAuthority	, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginAuthority	).begin(), ::gpk::toString(strCopyAuthority	).begin());
			ce_if(strCopyPath		!= strOriginPath		, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginPath		).begin(), ::gpk::toString(strCopyPath		).begin());
			ce_if(strCopyQuery		!= strOriginQuery		, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginQuery		).begin(), ::gpk::toString(strCopyQuery		).begin());
			ce_if(strCopyFragment	!= strOriginFragment	, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginFragment	).begin(), ::gpk::toString(strCopyFragment	).begin());
		}
	}
	return 0;
}

int main() {
	gerror_if(errored(testDomainerSMTP()), "%s", "Error!");
	gerror_if(errored(testDomainerURL()), "%s", "Error!");
	return 0;
}
