#include "ubk_domainer.h"
#include "gpk_find.h"

::gpk::error_t														ubk::SSMTPMapBlock::GetEMail		(int32_t index, ::gpk::array_pod<char_t> & email)	{
	ree_if(((uint32_t)index) >= Domain.size(), "Index out of range: %i", index);
	const int32_t															indexViewDomain						= Domain	[index];
	const int32_t															indexViewUsername					= Username	[index];
	if(0 > indexViewDomain)
		return 0;
	gpk_necall(email.append(Allocator.Views[indexViewUsername], Allocator.Counts[indexViewUsername]), "%s", "Out of memory?");
	if(0 <= indexViewDomain) {
		gpk_necall(email.push_back('@'), "%s", "Out of memory?");
		gpk_necall(email.append(Allocator.Views[indexViewDomain], Allocator.Counts[indexViewDomain]), "%s", "Out of memory?");
	}
	return 0;
}

::gpk::error_t														ubk::SSMTPMapBlock::Save			(::gpk::array_pod<byte_t> & output)		const		{
	gpk_necall(::gpk::viewWrite(::gpk::view_const_uint16{Allocator.Counts.begin(), Allocator.Counts.size()}, output), "%s", "Out of memory?");
	for(uint32_t iArray = 0; iArray < Allocator.Counts.size(); ++iArray)
		gpk_necall(output.append(Allocator.Views[iArray], Allocator.Counts[iArray]), "%s", "Out of memory?");

	gpk_necall(::gpk::viewWrite(::gpk::view_array<const _tIndex>{Domain.begin(), Domain.size()}, output), "%s", "Out of memory?");
	gpk_necall(output.append(::gpk::view_const_byte{(const byte_t*)Username.begin(), Domain.size() * sizeof(_tIndex)}), "%s", "Out of memory?");
	return 0;
}

::gpk::error_t									ubk::SSMTPMapBlock::Load			(const ::gpk::view_const_byte & input)				{
	if(0 == input.size())
		return 0;

	const uint32_t								countArrays									= *(const uint32_t*)input.begin();
	uint32_t									offsetArraySize								= sizeof(uint32_t);
	typedef										uint16_t									_tViewLen;
	uint32_t									offsetArrayData								= offsetArraySize + sizeof(_tViewLen) * countArrays;
	for(uint32_t iArray = 0; iArray < countArrays; ++iArray) {
		const _tViewLen								currentArraySize							= *(_tViewLen*)&input[offsetArraySize];
		Allocator.View(&input[offsetArrayData], currentArraySize);
		offsetArrayData							+= currentArraySize;
		offsetArraySize							+= sizeof(_tViewLen);
	}
	const uint32_t								countMaps									= *(const uint32_t*)&input[offsetArrayData];
	uint32_t									offsetDataDomain							= offsetArrayData	+ sizeof(uint32_t);
	uint32_t									offsetDataUsername							= offsetDataDomain	+ sizeof(_tIndex) * countMaps;
	gpk_necall(Domain	.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Username	.resize(countMaps), "%s", "Out of memory?");
	for(uint32_t iMap = 0; iMap < countMaps; ++iMap) {
		Domain	[iMap]							= *(_tIndex*)&input[offsetDataDomain	];
		Username[iMap]							= *(_tIndex*)&input[offsetDataUsername];
		offsetDataDomain						+= sizeof(_tIndex);
		offsetDataUsername						+= sizeof(_tIndex);
	}
	return 0;
}

::gpk::error_t									ubk::SSMTPMapBlock::AddSMTPMap		(const ::gpk::view_const_char & textToAdd)		{
	::gpk::error_t										indexToReturn						= -1;

	int32_t												idxUsername							= -1;
	int32_t												idxDomain							= -1;
	const ::gpk::error_t								arrobaPos							= ::gpk::find('@', textToAdd);
	if(0 > arrobaPos) {
		gpk_necall(idxUsername = Allocator.View(textToAdd.begin(), (uint16_t)textToAdd.size()), "%s", "Out of memory?");
	}
	else {
		gpk_necall(idxUsername = Allocator.View(textToAdd.begin(), (uint16_t)arrobaPos), "%s", "Out of memory?");
		const uint32_t									offsetDomain						= arrobaPos + 1;
		gpk_necall(idxDomain = Allocator.View(textToAdd.begin() + offsetDomain, (uint16_t)(textToAdd.size() - offsetDomain)), "%s", "Out of memory?");
	}

	gpk_necall(indexToReturn = Username	.push_back(idxUsername)	, "%s", "Out of memory?");
	gpk_necall(indexToReturn = Domain	.push_back(idxDomain)	, "%s", "Out of memory?");
	return indexToReturn;
}
