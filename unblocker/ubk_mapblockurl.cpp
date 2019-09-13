#include "ubk_domainer.h"
#include "gpk_find.h"
#include "gpk_stdstring.h"

::gpk::error_t									ubk::splitURL						(const ::gpk::view_const_char & textToAdd, ::ubk::URL_SCHEME & scheme, ::gpk::view_const_char & authority, ::gpk::view_const_char & path, ::gpk::view_const_char & query, ::gpk::view_const_char & fragment)	{
	// Read scheme
	const ::gpk::error_t								colonpos							= ::gpk::find(':', textToAdd);
	uint32_t											nextOffset							= 0;
	if(0 > colonpos)
		scheme											= ::ubk::URL_SCHEME_default;
	else {
		nextOffset										= (colonpos + 1);
		::gpk::array_pod<char_t>							strscheme							= ::gpk::view_const_char{textToAdd.begin(), (uint32_t)colonpos};
		::gpk::tolower(strscheme);
		scheme											= ::gpk::get_value<::ubk::URL_SCHEME>(::gpk::view_const_string{strscheme.begin(), strscheme.size()});
	}

	// Read either authority or path.
	if(nextOffset < textToAdd.size()) {
		::gpk::view_const_char								remainder							= ::gpk::view_const_char{textToAdd.begin() + nextOffset, textToAdd.size() - nextOffset};
		if(remainder.size() > 1 && remainder[0] == '/' && remainder[1] == '/') { // Read optional authority component
			remainder										= ::gpk::view_const_char{remainder.begin() + 2, remainder.size() - 2};
			if(remainder.size())
				::gpk::splitAt<const char_t>('/', remainder, authority, remainder);
		}
		if(remainder.size() && remainder[0] == '/') {	// Read Path component
			remainder										= ::gpk::view_const_char{remainder.begin() + 1, remainder.size() - 1};
			if(remainder.size()) {
				const ::gpk::error_t								posQuery							= ::gpk::find('?', remainder);
				if(0 <= posQuery) { // QueryString found. Read up to there.
					path											= {remainder.begin(), (uint32_t)posQuery};
					remainder										= ::gpk::view_const_char{remainder.begin() + posQuery, remainder.size() - posQuery};
				}
				else // No QueryString found. Read until the end unless fragment is found.
					::gpk::splitAt<const char_t>('#', remainder, path, remainder);
			}
			if(remainder.size() && remainder[0] == '?') {	// Read optional Query component
				remainder										= ::gpk::view_const_char{remainder.begin() + 1, remainder.size() - 1};
				if(remainder.size())
					::gpk::splitAt<const char_t>('#', remainder, query, remainder);
			}
			if(remainder.size() && remainder[0] == '#') // Read optional fragment component
				fragment										= ::gpk::view_const_char{remainder.begin() + 1, remainder.size() - 1};
		}
	}
	return 0;
}

::gpk::error_t									ubk::SMapBlockURL::AddMap			(const ::gpk::view_const_char & textToAdd)		{
	::gpk::error_t										indexToReturn						= -1;

	::gpk::view_const_char								parts	[4]							= {};
	::gpk::SInt24										indices	[4]							= {};

	::ubk::URL_SCHEME									scheme								= ::ubk::URL_SCHEME_default;
	::ubk::splitURL(textToAdd, scheme, parts[0], parts[1], parts[2], parts[3]);
	// Read scheme
	for(uint32_t iPart = 0; iPart < ::gpk::size(parts); ++iPart)
		indices[iPart]									= parts[iPart].size() ? Allocator.View(parts[iPart].begin(), (uint16_t)parts[iPart].size()) : -1;
	gpk_necall(indexToReturn = Scheme	.push_back(scheme		), "%s", "Out of memory?");
	gpk_necall(indexToReturn = Authority.push_back(indices[0]	), "%s", "Out of memory?");
	gpk_necall(indexToReturn = Path		.push_back(indices[1]	), "%s", "Out of memory?");
	gpk_necall(indexToReturn = Query	.push_back(indices[2]	), "%s", "Out of memory?");
	gpk_necall(indexToReturn = Fragment	.push_back(indices[3]	), "%s", "Out of memory?");
	return indexToReturn;
}

::gpk::error_t									ubk::SMapBlockURL::GetMap			(int32_t index, ::gpk::array_pod<char_t> & url)		const	{
	const ::ubk::URL_SCHEME								scheme								= Scheme	[index];
	const int32_t										indexViewAuthority					= Authority	[index];
	if(::ubk::URL_SCHEME_default != scheme) {
		gpk_necall(url.append(::gpk::get_value_label(scheme)), "%s", "Out of memory?");;
		gpk_necall(url.push_back(':'), "%s", "Out of memory?");;
		if(-1 != indexViewAuthority) {
			gpk_necall(url.push_back('/'), "%s", "Out of memory?");;
			gpk_necall(url.push_back('/'), "%s", "Out of memory?");;
		}
	}
	if(-1 != indexViewAuthority)
		gpk_necall(url.append(Allocator.Views[indexViewAuthority], Allocator.Counts[indexViewAuthority]), "%s", "Out of memory?");
	const int32_t										indexViewPath						= Path		[index];
	const int32_t										indexViewQuery						= Query		[index];
	const int32_t										indexViewFragment					= Fragment	[index];
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

template<typename _tElement>
::gpk::error_t									recursivelyCompare					(uint32_t & iAddress, const ::gpk::CViewManager<_tElement> & allocator, const ::gpk::view_array<const ::gpk::view_const_char> parts, const uint32_t iPart, const ::gpk::view_array<const ::gpk::view_array<const ::gpk::SInt24>> & idLists, const uint32_t idParent, uint32_t countAddresses) {
	const ::gpk::view_const_char						currentPart							= parts[iPart];
	for(; iAddress < countAddresses; ++iAddress) {
		if(((int32_t)idParent) != idLists[iPart - 1][iAddress])
			continue;
		const uint32_t										idFragment							= idLists[iPart][iAddress];
		const uint16_t										currentFragmentLen					= (-1 == idFragment) ? 0 : allocator.Counts[idFragment];
		if(currentFragmentLen != currentPart.size())
			continue;
		if(parts.size() - 1 == iPart) {
			if(0 == currentFragmentLen || 0 == memcmp(allocator.Views[idFragment], currentPart.begin(), currentFragmentLen))
				return iAddress;
		}
		else {
			if(currentFragmentLen && 0 != memcmp(allocator.Views[idFragment], currentPart.begin(), currentFragmentLen))
				continue;
			return ::recursivelyCompare(iAddress, allocator, parts, iPart + 1, idLists, idFragment, countAddresses);
		}
	}
	return -1;
}

::gpk::error_t									ubk::SMapBlockURL::GetMapId			(const ::gpk::view_const_char & textToAdd) 	const	{
	::ubk::URL_SCHEME									scheme								= ::ubk::URL_SCHEME_default;
	::gpk::view_const_char								parts	[4]							= {};
	const ::gpk::view_array<const ::gpk::SInt24>		ids		[4]							= {Authority, Path, Query, Fragment};
	::ubk::splitURL(textToAdd, scheme, parts[0], parts[1], parts[2], parts[3]);
	for(uint32_t iAddress = 0; iAddress < Scheme.size(); ++iAddress) {
		const uint32_t										idScheme							= Scheme[iAddress];
		const uint16_t										currentSchemeLen					= Allocator.Counts[idScheme];
		if(idScheme != scheme)
			continue;
		for(; iAddress < Scheme.size(); ++iAddress) {
			if(idScheme != Scheme[iAddress])
				continue;
			const uint32_t										idAuthority						= Authority[iAddress];
			const uint16_t										currentAuthorityLen				= (-1 == idAuthority) ? 0 : Allocator.Counts[idAuthority];
			if(currentAuthorityLen != parts[0].size())
				continue;
			if(currentAuthorityLen && 0 != memcmp(Allocator.Views[idAuthority], parts[0].begin(), currentAuthorityLen))
				continue;

			::gpk::error_t										compare								= ::recursivelyCompare(iAddress, Allocator, parts, 1, ids, idAuthority, Scheme.size());
			if(0 <= compare)
				return compare;
		}
	}
	return -1;
}

::gpk::error_t									ubk::SMapBlockURL::Save				(::gpk::array_pod<byte_t> & output)		const		{
	gpk_necall(::gpk::viewManagerSave(Allocator, output), "%s", "Unknown error!");
	gpk_necall(::gpk::viewWrite(::gpk::view_array<const ::ubk::URL_SCHEME>{Scheme.begin(), Scheme.size()}, output)		, "%s", "Out of memory?");
	gpk_necall(output.append(::gpk::view_const_byte{(const byte_t*)Authority.begin(), Scheme.size() * sizeof(_tIndex)})	, "%s", "Out of memory?");
	gpk_necall(output.append(::gpk::view_const_byte{(const byte_t*)Path		.begin(), Scheme.size() * sizeof(_tIndex)})	, "%s", "Out of memory?");
	gpk_necall(output.append(::gpk::view_const_byte{(const byte_t*)Query	.begin(), Scheme.size() * sizeof(_tIndex)})	, "%s", "Out of memory?");
	gpk_necall(output.append(::gpk::view_const_byte{(const byte_t*)Fragment	.begin(), Scheme.size() * sizeof(_tIndex)})	, "%s", "Out of memory?");
	return 0;
}

::gpk::error_t									ubk::SMapBlockURL::Load				(const ::gpk::view_const_byte & input)				{
	if(0 == input.size())
		return 0;

	const int32_t										offsetArrayData						= ::gpk::viewManagerLoad(Allocator, input);
	gpk_necall(offsetArrayData, "Failed to load strings: %s.", "Unknown error!");
	const uint32_t										countMaps							= *(const uint32_t*)&input[(uint32_t)offsetArrayData];
	uint32_t											offsetDataScheme					= offsetArrayData		+ sizeof(uint32_t);
	uint32_t											offsetDataAuthority					= offsetDataScheme		+ sizeof(::ubk::URL_SCHEME) * countMaps;
	uint32_t											offsetDataPath						= offsetDataAuthority	+ sizeof(_tIndex)			* countMaps;
	uint32_t											offsetDataQuery						= offsetDataPath		+ sizeof(_tIndex)			* countMaps;
	uint32_t											offsetDataFragment					= offsetDataQuery		+ sizeof(_tIndex)			* countMaps;
	gpk_necall(Scheme	.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Authority.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Path		.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Query	.resize(countMaps), "%s", "Out of memory?");
	gpk_necall(Fragment	.resize(countMaps), "%s", "Out of memory?");
	for(uint32_t iMap = 0; iMap < countMaps; ++iMap) {
		Scheme		[iMap]								= *(::ubk::URL_SCHEME*)&input[offsetDataScheme];
		Authority	[iMap]								= *(const _tIndex*)&input[offsetDataAuthority	];
		Path		[iMap]								= *(const _tIndex*)&input[offsetDataPath		];
		Query		[iMap]								= *(const _tIndex*)&input[offsetDataQuery		];
		Fragment	[iMap]								= *(const _tIndex*)&input[offsetDataFragment	];
		offsetDataScheme								+= sizeof(::ubk::URL_SCHEME);
		offsetDataAuthority								+= sizeof(_tIndex);
		offsetDataPath									+= sizeof(_tIndex);
		offsetDataQuery									+= sizeof(_tIndex);
		offsetDataFragment								+= sizeof(_tIndex);
	}
	return 0;
}
