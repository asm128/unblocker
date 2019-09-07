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
			if(remainder.size()) {
				const ::gpk::error_t								posPath								= ::gpk::find('/', remainder);
				if(0 > posPath) {
					authority										= {remainder.begin(), (uint32_t)remainder.size()};
					remainder										= {};
				}
				else {
					authority										= {remainder.begin(), (uint32_t)posPath};
					remainder										= {remainder.begin() + posPath, remainder.size() - posPath};
				}
			}
		}
		if(remainder.size() && remainder[0] == '/') {	// Read Path component
			remainder										= ::gpk::view_const_char{remainder.begin() + 1, remainder.size() - 1};
			if(remainder.size()) {
				const ::gpk::error_t								posQuery							= ::gpk::find('?', remainder);
				if(0 <= posQuery) { // QueryString found. Read up to there.
					path											= {remainder.begin(), (uint32_t)posQuery};
					remainder										= ::gpk::view_const_char{remainder.begin() + posQuery, remainder.size() - posQuery};
				}
				else { // No QueryString found. Read until the end unless fragment is found.
					const ::gpk::error_t								posFragment							= ::gpk::find('#', remainder);
					if(0 > posFragment) { // Read until the end unless fragment is found.
						path											= remainder;
						remainder										= {};
					}
					else {
						path											= {remainder.begin(), (uint32_t)posFragment};
						remainder										= ::gpk::view_const_char{remainder.begin() + posFragment, remainder.size() - posFragment};
					}
				}
			}
			if(remainder.size() && remainder[0] == '?') {	// Read optional Query component
				remainder										= ::gpk::view_const_char{remainder.begin() + 1, remainder.size() - 1};
				if(remainder.size()) {
					const ::gpk::error_t								posFragment						= ::gpk::find('#', remainder);
					if(0 > posFragment) { // Read until the end unless fragment is found.
						query											= remainder;
						remainder										= {};
					}
					else {
						query											= {remainder.begin(), (uint32_t)posFragment};
						remainder										= ::gpk::view_const_char{remainder.begin() + posFragment, remainder.size() - posFragment};
					}
				}
			}
			if(remainder.size() && remainder[0] == '#') {	// Read optional fragment component
				remainder										= ::gpk::view_const_char{remainder.begin() + 1, remainder.size() - 1};
				if(remainder.size())
					fragment										= remainder;
			}
		}
	}

	return 0;
}

::gpk::error_t									ubk::SURLMapBlock::AddURLMap		(const ::gpk::view_const_char & textToAdd)		{
	::gpk::error_t										indexToReturn						= -1;

	int32_t												idxAuthority						= -1;
	int32_t												idxPath								= -1;
	int32_t												idxQuery							= -1;
	int32_t												idxFragment							= -1;

	::ubk::URL_SCHEME									scheme								= ::ubk::URL_SCHEME_default;
	::gpk::view_const_char								authority							= {};
	::gpk::view_const_char								path								= {};
	::gpk::view_const_char								query								= {};
	::gpk::view_const_char								fragment							= {};
	::ubk::splitURL(textToAdd, scheme, authority, path, query, fragment);
	// Read scheme
	idxAuthority									= authority	.size() ? Allocator.View(authority	.begin(), (uint16_t)authority	.size()) : -1;
	idxPath											= path		.size() ? Allocator.View(path		.begin(), (uint16_t)path		.size()) : -1;
	idxQuery										= query		.size() ? Allocator.View(query		.begin(), (uint16_t)query		.size()) : -1;
	idxFragment										= fragment	.size() ? Allocator.View(fragment	.begin(), (uint16_t)fragment	.size()) : -1;
	gpk_necall(indexToReturn = Scheme	.push_back(scheme		), "%s", "Out of memory?");
	gpk_necall(indexToReturn = Authority.push_back(idxAuthority	), "%s", "Out of memory?");
	gpk_necall(indexToReturn = Path		.push_back(idxPath		), "%s", "Out of memory?");
	gpk_necall(indexToReturn = Query	.push_back(idxQuery		), "%s", "Out of memory?");
	gpk_necall(indexToReturn = Fragment	.push_back(idxFragment	), "%s", "Out of memory?");
	return indexToReturn;
}

::gpk::error_t									ubk::SURLMapBlock::GetURL			(int32_t index, ::gpk::array_pod<char_t> & url)		{
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

::gpk::error_t									ubk::SURLMapBlock::Save				(::gpk::array_pod<byte_t> & output)		const		{
	gpk_necall(::gpk::viewWrite(::gpk::view_const_uint16{Allocator.Counts.begin(), Allocator.Counts.size()}, output), "%s", "Out of memory?");
	for(uint32_t iArray = 0; iArray < Allocator.Counts.size(); ++iArray)
		gpk_necall(output.append(Allocator.Views[iArray], Allocator.Counts[iArray]), "%s", "Out of memory?");

	gpk_necall(::gpk::viewWrite(::gpk::view_array<const ::ubk::URL_SCHEME>{Scheme.begin(), Scheme.size()}, output)		, "%s", "Out of memory?");
	gpk_necall(output.append(::gpk::view_const_byte{(const byte_t*)Authority.begin(), Scheme.size() * sizeof(_tIndex)})	, "%s", "Out of memory?");
	gpk_necall(output.append(::gpk::view_const_byte{(const byte_t*)Path		.begin(), Scheme.size() * sizeof(_tIndex)})	, "%s", "Out of memory?");
	gpk_necall(output.append(::gpk::view_const_byte{(const byte_t*)Query	.begin(), Scheme.size() * sizeof(_tIndex)})	, "%s", "Out of memory?");
	gpk_necall(output.append(::gpk::view_const_byte{(const byte_t*)Fragment	.begin(), Scheme.size() * sizeof(_tIndex)})	, "%s", "Out of memory?");
	return 0;
}

::gpk::error_t									ubk::SURLMapBlock::Load				(const ::gpk::view_const_byte & input)				{
	if(0 == input.size())
		return 0;

	const uint32_t										countArrays							= *(const uint32_t*)input.begin();
	uint32_t											offsetArraySize						= sizeof(uint32_t);
	typedef												uint16_t							_tViewLen;
	uint32_t											offsetArrayData						= offsetArraySize + sizeof(_tViewLen) * countArrays;
	for(uint32_t iArray = 0; iArray < countArrays; ++iArray) {
		const _tViewLen										currentArraySize					= *(_tViewLen*)&input[offsetArraySize];
		Allocator.View(&input[offsetArrayData], currentArraySize);
		offsetArrayData									+= currentArraySize;
		offsetArraySize									+= sizeof(_tViewLen);
	}
	const uint32_t										countMaps							= *(const uint32_t*)&input[offsetArrayData];
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

::gpk::error_t									ubk::SURLMapBlock::GetURLMapId		(const ::gpk::view_const_char & textToAdd) {
	::ubk::URL_SCHEME									scheme								= ::ubk::URL_SCHEME_default;
	::gpk::view_const_char								authority							= {};
	::gpk::view_const_char								path								= {};
	::gpk::view_const_char								query								= {};
	::gpk::view_const_char								fragment							= {};
	::ubk::splitURL(textToAdd, scheme, authority, path, query, fragment);
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
			if(currentAuthorityLen != authority.size())
				continue;
			if(currentAuthorityLen && 0 != memcmp(Allocator.Views[idAuthority], authority.begin(), currentAuthorityLen))
				continue;

			for(; iAddress < Scheme.size(); ++iAddress) {
				if(((int32_t)idAuthority) != Authority[iAddress])
					continue;
				const uint32_t										idPath							= Path[iAddress];
				const uint16_t										currentPathLen					= (-1 == idPath) ? 0 : Allocator.Counts[idPath];
				if(currentPathLen != path.size())
					continue;
				if(currentPathLen && 0 != memcmp(Allocator.Views[idPath], path.begin(), currentPathLen))
					continue;

				for(; iAddress < Scheme.size(); ++iAddress) {
					if(((int32_t)idPath) != Path[iAddress])
						continue;
					const uint32_t										idQuery							= Query[iAddress];
					const uint16_t										currentQueryLen					= (-1 == idQuery) ? 0 : Allocator.Counts[idQuery];
					if(currentQueryLen != query.size())
						continue;
					if(currentQueryLen && 0 != memcmp(Allocator.Views[idQuery], query.begin(), currentQueryLen))
						continue;

					for(; iAddress < Scheme.size(); ++iAddress) {
						if(((int32_t)idQuery) != Query[iAddress])
							continue;
						const uint32_t										idFragment							= Fragment[iAddress];
						const uint16_t										currentFragmentLen					= (-1 == idFragment) ? 0 : Allocator.Counts[idFragment];
						if(currentFragmentLen != fragment.size())
							continue;
						if(0 == currentFragmentLen || 0 == memcmp(Allocator.Views[idFragment], fragment.begin(), currentFragmentLen))
							return iAddress;
					}

				}

			}

		}
	}
	return -1;
}
