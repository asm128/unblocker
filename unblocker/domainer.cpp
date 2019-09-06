#include "ubk_domainer.h"
#include "gpk_find.h"
#include "gpk_stdstring.h"
#include "gpk_storage.h"


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

::gpk::error_t									ubk::SURLMapBlock::AddURLMap		(const ::gpk::view_const_char & textToAdd)		{
	::gpk::error_t										indexToReturn						= -1;

	int32_t												idxAuthority						= -1;
	int32_t												idxPath								= -1;
	int32_t												idxQuery							= -1;
	int32_t												idxFragment							= -1;

	// Read scheme
	const ::gpk::error_t								colonpos							= ::gpk::find(':', textToAdd);
	uint32_t											nextOffset							= 0;
	if(0 > colonpos)
		gpk_necall(indexToReturn = Scheme.push_back(::ubk::URL_SCHEME_default), "%s", "Out of memory?");
	else {
		nextOffset										= (colonpos + 1);
		::gpk::array_pod<char_t>							scheme								= ::gpk::view_const_char{textToAdd.begin(), (uint32_t)colonpos};
		::gpk::tolower(scheme);
		gpk_necall(indexToReturn = Scheme.push_back(::gpk::get_value<::ubk::URL_SCHEME>(::gpk::view_const_string{scheme.begin(), scheme.size()})), "%s", "Out of memory?");
	}
	// Read either authority or path.
	if(nextOffset < textToAdd.size()) {
		::gpk::view_const_char								remainder							= ::gpk::view_const_char{textToAdd.begin() + nextOffset, textToAdd.size() - nextOffset};
		if(remainder.size() > 1 && remainder[0] == '/' && remainder[1] == '/') { // Read optional authority component
			remainder										= ::gpk::view_const_char{remainder.begin() + 2, remainder.size() - 2};
			if(remainder.size()) {
				const ::gpk::error_t								posPath								= ::gpk::find('/', remainder);
				if(0 > posPath) {
					gpk_necall(idxAuthority = Allocator.View(remainder.begin(), (uint16_t)remainder.size()), "%s", "Out of memory?");
					remainder										= {};
				}
				else {
					gpk_necall(idxAuthority = Allocator.View(remainder.begin(), (uint16_t)posPath), "%s", "Out of memory?");
					remainder										= {remainder.begin() + posPath, remainder.size() - posPath};
				}
			}
		}
		if(remainder.size() && remainder[0] == '/') {	// Read Path component
			remainder										= ::gpk::view_const_char{remainder.begin() + 1, remainder.size() - 1};
			if(remainder.size()) {
				const ::gpk::error_t								posQuery							= ::gpk::find('?', remainder);
				if(0 <= posQuery) { // QueryString found. Read up to there.
					gpk_necall(idxPath = Allocator.View(remainder.begin(), (uint16_t)posQuery), "%s", "Out of memory?");
					remainder										= ::gpk::view_const_char{remainder.begin() + posQuery, remainder.size() - posQuery};
				}
				else { // No QueryString found. Read until the end unless fragment is found.
					const ::gpk::error_t								posFragment							= ::gpk::find('#', remainder);
					if(0 > posFragment) { // Read until the end unless fragment is found.
						gpk_necall(idxPath = Allocator.View(remainder.begin(), (uint16_t)remainder.size()), "%s", "Out of memory?");
						remainder										= {};
					}
					else {
						gpk_necall(idxPath = Allocator.View(remainder.begin(), (uint16_t)posFragment), "%s", "Out of memory?");
						remainder										= ::gpk::view_const_char{remainder.begin() + posFragment, remainder.size() - posFragment};
					}
				}
			}
		}
		if(remainder.size() && remainder[0] == '?') {	// Read Query component
			remainder										= ::gpk::view_const_char{remainder.begin() + 1, remainder.size() - 1};
			if(remainder.size()) {
				const ::gpk::error_t								posFragment						= ::gpk::find('#', remainder);
				if(0 > posFragment) { // Read until the end unless fragment is found.
					gpk_necall(idxQuery = Allocator.View(remainder.begin(), (uint16_t)remainder.size()), "%s", "Out of memory?");
					remainder										= {};
				}
				else {
					gpk_necall(idxQuery = Allocator.View(remainder.begin(), (uint16_t)posFragment), "%s", "Out of memory?");
					remainder										= ::gpk::view_const_char{remainder.begin() + posFragment, remainder.size() - posFragment};
				}
			}
		}
		if(remainder.size() && remainder[0] == '#') {	// Read fragment component
			remainder										= ::gpk::view_const_char{remainder.begin() + 1, remainder.size() - 1};
			if(remainder.size())
				idxFragment										= Allocator.View(remainder.begin(), (uint16_t)remainder.size());
		}
	}
	gpk_necall(Authority.push_back(idxAuthority	), "%s", "Out of memory?");
	gpk_necall(Path		.push_back(idxPath		), "%s", "Out of memory?");
	gpk_necall(Query	.push_back(idxQuery		), "%s", "Out of memory?");
	gpk_necall(Fragment	.push_back(idxFragment	), "%s", "Out of memory?");
	return indexToReturn;
}

		::gpk::error_t												ubk::SDomainer::GetEMail			(const uint64_t idRecord, ::gpk::array_pod<char_t> & email)	{
	::gpk::SRecordMap														recordMap;
	::gpk::array_pod<char_t>												fileBytes;
	::gpk::error_t															indexBlock							= ::gpk::blockMapLoad(fileBytes, recordMap, Email, Email.DBName, DBPath, idRecord);
	gpk_necall(indexBlock, "Invalid record id: %llu.", idRecord);
	::ubk::SSMTPMapBlock													& loadedBlock						= Email.Block[indexBlock];
	if(fileBytes.size())
		gpk_necall(loadedBlock.Load(fileBytes), "Failed to load block data for record %llu.", idRecord);

	return loadedBlock.GetEMail(recordMap.IndexRecord, email);
}

		::gpk::error_t												ubk::SSMTPMapBlock::GetEMail		(int32_t index, ::gpk::array_pod<char_t> & email)	{
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

		::gpk::error_t												ubk::SSMTPMapBlock::Save			(::gpk::array_pod<byte_t> & output)					{
	::gpk::viewWrite(::gpk::view_const_uint16{Allocator.Counts.begin(), Allocator.Counts.size()}, output);
	for(uint32_t iArray = 0; iArray < Allocator.Counts.size(); ++iArray)
		output.append(Allocator.Views[iArray], Allocator.Counts[iArray]);

	::gpk::viewWrite(::gpk::view_array<const _tIndex>{Domain.begin(), Domain.size()}, output);
	output.append(::gpk::view_const_byte{(const byte_t*)Username.begin(), Domain.size() * sizeof(_tIndex)});
	return 0;
}

		::gpk::error_t												ubk::SSMTPMapBlock::Load			(const ::gpk::view_const_byte & input)				{
	if(0 == input.size())
		return 0;

	const uint32_t															countArrays							= *(const uint32_t*)input.begin();
	uint32_t																offsetArraySize						= sizeof(uint32_t);
	typedef																	uint16_t							_tViewLen;
	uint32_t																offsetArrayData						= offsetArraySize + sizeof(_tViewLen) * countArrays;
	for(uint32_t iArray = 0; iArray < countArrays; ++iArray) {
		const _tViewLen															currentArraySize					= *(_tViewLen*)&input[offsetArraySize];
		Allocator.View(&input[offsetArrayData], currentArraySize);
		offsetArrayData														+= currentArraySize;
		offsetArraySize														+= sizeof(_tViewLen);
	}
	const uint32_t															countMaps					= *(const uint32_t*)&input[offsetArrayData];
	uint32_t																offsetDataDomain			= offsetArrayData	+ sizeof(uint32_t);
	uint32_t																offsetDataUsername			= offsetDataDomain	+ sizeof(_tIndex) * countMaps;
	gpk_necall(Domain	.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Username	.resize(countMaps), "%s", "Out of memory?");
	for(uint32_t iMap = 0; iMap < countMaps; ++iMap) {
		Domain	[iMap]														= *(_tIndex*)&input[offsetDataDomain	];
		Username[iMap]														= *(_tIndex*)&input[offsetDataUsername];
		offsetDataDomain													+= sizeof(_tIndex);
		offsetDataUsername													+= sizeof(_tIndex);
	}
	return 0;
}

		::gpk::error_t												ubk::SURLMapBlock::Save				(::gpk::array_pod<byte_t> & output)					{
	::gpk::viewWrite(::gpk::view_const_uint16{Allocator.Counts.begin(), Allocator.Counts.size()}, output);
	for(uint32_t iArray = 0; iArray < Allocator.Counts.size(); ++iArray)
		output.append(Allocator.Views[iArray], Allocator.Counts[iArray]);

	::gpk::viewWrite(::gpk::view_array<const ::ubk::URL_SCHEME>{Scheme.begin(), Scheme.size()}, output);
	output.append(::gpk::view_const_byte{(const byte_t*)Authority	.begin(), Scheme.size() * sizeof(_tIndex)});
	output.append(::gpk::view_const_byte{(const byte_t*)Path		.begin(), Scheme.size() * sizeof(_tIndex)});
	output.append(::gpk::view_const_byte{(const byte_t*)Query		.begin(), Scheme.size() * sizeof(_tIndex)});
	output.append(::gpk::view_const_byte{(const byte_t*)Fragment	.begin(), Scheme.size() * sizeof(_tIndex)});
	return 0;
}

		::gpk::error_t												ubk::SURLMapBlock::Load				(const ::gpk::view_const_byte & input)				{
	if(0 == input.size())
		return 0;

	const uint32_t															countArrays							= *(const uint32_t*)input.begin();
	uint32_t																offsetArraySize						= sizeof(uint32_t);
	typedef																	uint16_t							_tViewLen;
	uint32_t																offsetArrayData						= offsetArraySize + sizeof(_tViewLen) * countArrays;
	for(uint32_t iArray = 0; iArray < countArrays; ++iArray) {
		const _tViewLen															currentArraySize					= *(_tViewLen*)&input[offsetArraySize];
		Allocator.View(&input[offsetArrayData], currentArraySize);
		offsetArrayData														+= currentArraySize;
		offsetArraySize														+= sizeof(_tViewLen);
	}
	const uint32_t															countMaps							= *(const uint32_t*)&input[offsetArrayData];
	uint32_t																offsetDataScheme					= offsetArrayData		+ sizeof(uint32_t);
	uint32_t																offsetDataAuthority					= offsetDataScheme		+ sizeof(::ubk::URL_SCHEME) * countMaps;
	uint32_t																offsetDataPath						= offsetDataAuthority	+ sizeof(_tIndex)			* countMaps;
	uint32_t																offsetDataQuery						= offsetDataPath		+ sizeof(_tIndex)			* countMaps;
	uint32_t																offsetDataFragment					= offsetDataQuery		+ sizeof(_tIndex)			* countMaps;
	gpk_necall(Scheme	.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Authority.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Path		.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Query	.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Fragment	.resize(countMaps), "%s", "Out of memory?");
	for(uint32_t iMap = 0; iMap < countMaps; ++iMap) {
		Scheme		[iMap]													= *(::ubk::URL_SCHEME*)&input[offsetDataScheme];
		Authority	[iMap]													= *(const _tIndex*)&input[offsetDataAuthority	];
		Path		[iMap]													= *(const _tIndex*)&input[offsetDataPath		];
		Query		[iMap]													= *(const _tIndex*)&input[offsetDataQuery		];
		Fragment	[iMap]													= *(const _tIndex*)&input[offsetDataFragment	];
		offsetDataScheme													+= sizeof(::ubk::URL_SCHEME);
		offsetDataAuthority													+= sizeof(_tIndex);
		offsetDataPath														+= sizeof(_tIndex);
		offsetDataQuery														+= sizeof(_tIndex);
		offsetDataFragment													+= sizeof(_tIndex);
	}
	return 0;
}

		::gpk::error_t												ubk::SURLMapBlock::GetURL			(int32_t index, ::gpk::array_pod<char_t> & url)		{
	const ::ubk::URL_SCHEME													scheme								= Scheme	[index];
	const int32_t															indexViewAuthority					= Authority	[index];
	if(::ubk::URL_SCHEME_default != scheme) {
		url.append(::gpk::get_value_label(scheme));
		url.push_back(':');
		if(-1 != indexViewAuthority) {
			url.push_back('/');
			url.push_back('/');
		}
	}
	if(-1 != indexViewAuthority)
		gpk_necall(url.append(Allocator.Views[indexViewAuthority], Allocator.Counts[indexViewAuthority]), "%s", "Out of memory?");
	const int32_t															indexViewPath						= Path		[index];
	const int32_t															indexViewQuery						= Query		[index];
	const int32_t															indexViewFragment					= Fragment	[index];
	if(-1 != indexViewPath || -1 != indexViewQuery || -1 != indexViewFragment)
		gpk_necall(url.push_back('/'), "%s", "Out of memory?");
	if(-1 != indexViewPath) {
		gpk_necall(url.append(Allocator.Views[indexViewPath], Allocator.Counts[indexViewPath]), "%s", "Out of memory?");
	}
	if(-1 != indexViewQuery) {
		gpk_necall(url.push_back('?'), "%s", "Out of memory?");
		gpk_necall(url.append(Allocator.Views[indexViewQuery], Allocator.Counts[indexViewQuery]), "%s", "Out of memory?");
	}
	if(-1 != indexViewFragment) {
		gpk_necall(url.push_back('#'), "%s", "Out of memory?");
		gpk_necall(url.append(Allocator.Views[indexViewFragment], Allocator.Counts[indexViewFragment]), "%s", "Out of memory?");
	}
	return 0;
}
