#include "ubk_domainer.h"

int main() {
	::ubk::SSMTPMapBlock			smtpOrigin;
	const ::gpk::view_const_string	strings	[]	=
		{"prueba0@prueba.com"
		,"prueba1@prueba.com"
		,"prueba2@hotmail.com"
		,"prueba0@gmail.com"
		};
	::gpk::array_pod<char_t>		file;
	for(uint32_t iString = 0; iString < ::gpk::size(strings); ++iString) {
		const int32_t indexOfMap	= smtpOrigin.AddSMTPMap(strings[iString]);
		ree_if((uint32_t)indexOfMap != iString, "%s", "Test failed!");
		::gpk::array_pod<char_t>		stored;
		smtpOrigin.GetEMail(indexOfMap, stored);
		ree_if(stored != strings[iString], "%s", "Test failed!");
	}
	smtpOrigin.Save(file);

	::ubk::SSMTPMapBlock			smtpCopy;
	smtpCopy.Load(file);
	for(uint32_t iDomain = 0; iDomain < smtpCopy.Domain.size(); ++iDomain) {
		const uint32_t idxCopyDomain		= smtpCopy.Domain		[iDomain];
		const uint32_t idxCopyUsername		= smtpCopy.Username		[iDomain];
		const uint32_t idxOriginDomain		= smtpOrigin.Domain		[iDomain];
		const uint32_t idxOriginUsername	= smtpOrigin.Username	[iDomain];
		ree_if(idxCopyDomain	!= idxOriginDomain	, "%s", "Test failed!");
		ree_if(idxCopyUsername	!= idxOriginUsername, "%s", "Test failed!");
		const ::gpk::view_const_char	strOriginDomain		= {smtpOrigin	.Allocator.Views[idxOriginDomain	], smtpOrigin	.Allocator.Counts[idxOriginDomain	]};
		const ::gpk::view_const_char	strOriginUsername	= {smtpOrigin	.Allocator.Views[idxOriginUsername	], smtpOrigin	.Allocator.Counts[idxOriginUsername	]};
		const ::gpk::view_const_char	strCopyDomain		= {smtpCopy		.Allocator.Views[idxCopyDomain		], smtpCopy		.Allocator.Counts[idxCopyDomain		]};
		const ::gpk::view_const_char	strCopyUsername		= {smtpCopy		.Allocator.Views[idxCopyUsername	], smtpCopy		.Allocator.Counts[idxCopyUsername	]};
		ree_if(strCopyDomain	!= strOriginDomain		, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginDomain	).begin(), ::gpk::toString(strCopyDomain	).begin());
		ree_if(strCopyUsername	!= strOriginUsername	, "Origin: %s. Copy: %s.", ::gpk::toString(strOriginUsername).begin(), ::gpk::toString(strCopyUsername	).begin());
	}
	return 0;
}
