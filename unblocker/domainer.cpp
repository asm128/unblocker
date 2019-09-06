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

		::gpk::error_t												ubk::SDomainer::GetEMail	(const uint64_t idRecord, ::gpk::array_pod<char_t> & email)	{
	const uint32_t															idBlock						= (uint32_t)(idRecord / Email.BlockConfig.BlockSize);
	const uint32_t															indexRecord					= (uint32_t)(idRecord % Email.BlockConfig.BlockSize);
	for(uint32_t iBlock = 0; iBlock < Email.Id.size(); ++iBlock) {
		if(idBlock == Email.Id[iBlock])
			return Email.Block[iBlock].GetEMail(indexRecord, email);
	}
	::gpk::error_t															indexBlock					= Email.Block.push_back({});
	gpk_necall(Email.Id.push_back(idBlock), "%s", "");
	::gpk::array_pod<char_t>												fileName;
	gpk_necall(::gpk::blockFileName(idBlock, Email.DBName, DBPath, fileName), "%s", "Out of memory?");
	::gpk::array_pod<char_t>												fileBytes;
	gpk_necall(::gpk::fileFromMemory({fileName.begin(), fileName.size()}, fileBytes), "Invalid record id: %llu. Block doesn't exist.", idRecord);
	::ubk::SSMTPMapBlock													& loadedBlock				= Email.Block[indexBlock];
	gpk_necall(loadedBlock.Load(fileBytes), "Failed to load block data from %s", ::gpk::toString(fileName).begin());
	return loadedBlock.GetEMail(indexRecord, email);
}
