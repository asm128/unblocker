#include "ubk_domainer.h"
#include "gpk_find.h"

::gpk::error_t									ubk::SMapBlockEMail::MapGet			(int32_t index, ::gpk::array_pod<char_t> & email)	const	{
	ree_if(((uint32_t)index) >= Domain.size(), "Index out of range: %i", index);
	const int32_t										indexViewDomain						= Domain	[index];
	const int32_t										indexViewUsername					= Username	[index];
	if(0 > indexViewDomain)
		return 0;
	gpk_necall(email.append(Allocator.Views[indexViewUsername], Allocator.Counts[indexViewUsername]), "%s", "Out of memory?");
	if(0 <= indexViewDomain) {
		gpk_necall(email.push_back('@'), "%s", "Out of memory?");
		gpk_necall(email.append(Allocator.Views[indexViewDomain], Allocator.Counts[indexViewDomain]), "%s", "Out of memory?");
	}
	return 0;
}

::gpk::error_t									ubk::SMapBlockEMail::MapAdd			(const ::gpk::view_const_char & textToAdd)		{
	::gpk::error_t										indexToReturn						= -1;
	::gpk::view_const_char								domain								= {};
	::gpk::view_const_char								username							= {};
	::gpk::split<const char_t>('@', textToAdd, username, domain);
	int32_t												idxUsername							= username	.size() ? Allocator.View(username	.begin(), (uint16_t)username	.size()) : -1;
	int32_t												idxDomain							= domain	.size() ? Allocator.View(domain		.begin(), (uint16_t)domain		.size()) : -1;
	gpk_necall(indexToReturn = Username	.push_back(idxUsername)	, "%s", "Out of memory?");
	gpk_necall(indexToReturn = Domain	.push_back(idxDomain)	, "%s", "Out of memory?");
	return indexToReturn;
}

::gpk::error_t									ubk::SMapBlockEMail::MapId			(const ::gpk::view_const_char & textToAdd) 	const	{
	::gpk::view_const_char								domain								= {};
	::gpk::view_const_char								username							= {};
	::ubk::splitEmail(textToAdd, username, domain);
	for(uint32_t iAddress = 0; iAddress < Domain.size(); ++iAddress) {
		const int32_t										idDomain							= Domain[iAddress];
		const uint16_t										currentDomainLen					= (-1 == idDomain) ? 0 : Allocator.Counts[idDomain];
		if(currentDomainLen != domain.size())
			continue;
		if(currentDomainLen && 0 != memcmp(Allocator.Views[idDomain], domain.begin(), currentDomainLen))
			continue;
		for(; iAddress < Domain.size(); ++iAddress) {
			if(idDomain != Domain[iAddress])
				continue;
			const uint32_t										idUsername						= Username[iAddress];
			const uint16_t										currentUsernameLen				= (-1 == idUsername) ? 0 : Allocator.Counts[idUsername];
			if(currentUsernameLen != username.size())
				continue;
			if(currentUsernameLen && 0 == memcmp(Allocator.Views[idUsername], username.begin(), currentUsernameLen))
				return iAddress;
		}
	}
	return -1;
}

::gpk::error_t									ubk::SMapBlockEMail::Save			(::gpk::array_pod<byte_t> & output)		const		{
	gpk_necall(::gpk::viewManagerSave(Allocator, output), "%s", "Unknown error!");
	gpk_necall(::gpk::viewWrite(::gpk::view_array<const _tIndex>{Domain.begin(), Domain.size()}, output), "%s", "Out of memory?");
	gpk_necall(output.append(::gpk::view_const_byte{(const byte_t*)Username.begin(), Domain.size() * sizeof(_tIndex)}), "%s", "Out of memory?");
	return 0;
}

::gpk::error_t									ubk::SMapBlockEMail::Load			(const ::gpk::view_const_byte & input)				{
	if(0 == input.size())
		return 0;
	const int32_t										offsetArrayData						= ::gpk::viewManagerLoad(Allocator, input);
	gpk_necall(offsetArrayData, "Failed to load strings: %s.", "Unknown error!");
	const uint32_t										countMaps									= *(const uint32_t*)&input[offsetArrayData];
	uint32_t											offsetDataDomain							= offsetArrayData	+ sizeof(uint32_t);
	uint32_t											offsetDataUsername							= offsetDataDomain	+ sizeof(_tIndex) * countMaps;
	gpk_necall(Domain	.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Username	.resize(countMaps), "%s", "Out of memory?");
	for(uint32_t iMap = 0; iMap < countMaps; ++iMap) {
		Domain	[iMap]									= *(_tIndex*)&input[offsetDataDomain	];
		Username[iMap]									= *(_tIndex*)&input[offsetDataUsername	];
		offsetDataDomain								+= sizeof(_tIndex);
		offsetDataUsername								+= sizeof(_tIndex);
	}
	return 0;
}
