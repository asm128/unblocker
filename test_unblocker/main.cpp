#include "ubk_domainer.h"

int main() {
	::ubk::SSMTPMapBlock			smtpOriginal;
	const ::gpk::view_const_string	strings	[]	=
		{"prueba0@prueba.com"
		,"prueba1@prueba.com"
		,"prueba2@hotmail.com"
		,"prueba0@gmail.com"
		};
	::gpk::array_pod<char_t>		file;
	for(uint32_t iString = 0; iString < ::gpk::size(strings); ++iString) {
		const int32_t indexOfMap	= smtpOriginal.AddSMTPMap(strings[iString]);
		ree_if((uint32_t)indexOfMap != iString, "%s", "Test failed!");
		::gpk::array_pod<char_t>		stored;
		smtpOriginal.GetEMail(indexOfMap, stored);
		ree_if(stored != strings[iString], "%s", "Test failed!");
	}
	smtpOriginal.Save(file);

	::ubk::SSMTPMapBlock			smtpCopy;
	smtpCopy.Load(file);
	for(uint32_t iDomain = 0; iDomain < smtpCopy.Domain.size(); ++iDomain) {
		ree_if(smtpCopy.Domain	[iDomain] != smtpOriginal.Domain	[iDomain], "%s", "Test failed!");
		ree_if(smtpCopy.Username[iDomain] != smtpOriginal.Username	[iDomain], "%s", "Test failed!");
	}

	return 0;
}
