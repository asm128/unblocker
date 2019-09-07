#include "ubk_domainer.h"
#include "gpk_find.h"
#include "gpk_parse.h"

		::gpk::error_t												ubk::SDomainer::GetEMail			(const uint64_t idRecord, ::gpk::array_pod<char_t> & email)		{
	::gpk::SRecordMap														recordMap;
	::gpk::array_pod<char_t>												fileBytes;
	::gpk::error_t															indexBlock							= ::gpk::blockMapLoad(fileBytes, recordMap, Email, Email.DBName, DBPath, idRecord);
	gpk_necall(indexBlock, "Invalid record id: %llu.", idRecord);
	::ubk::SSMTPMapBlock													& loadedBlock						= *Email.Block[indexBlock];
	if(fileBytes.size())
		gpk_necall(loadedBlock.Load(fileBytes), "Failed to load block data for record %llu.", idRecord);

	return loadedBlock.GetEMail(recordMap.IndexRecord, email);
}

		::gpk::error_t												ubk::SDomainer::GetURL				(const uint64_t idRecord, ::gpk::array_pod<char_t> & url)		{
	::gpk::SRecordMap														recordMap;
	::gpk::array_pod<char_t>												fileBytes;
	::gpk::error_t															indexBlock							= ::gpk::blockMapLoad(fileBytes, recordMap, URL, URL.DBName, DBPath, idRecord);
	gpk_necall(indexBlock, "Invalid record id: %llu.", idRecord);
	::ubk::SURLMapBlock														& loadedBlock						= *URL.Block[indexBlock];
	if(fileBytes.size())
		gpk_necall(loadedBlock.Load(fileBytes), "Failed to load block data for record %llu.", idRecord);

	return loadedBlock.GetURL(recordMap.IndexRecord, url);
}

		int64_t														ubk::SDomainer::AddEMail			(const ::gpk::view_const_char & email)	{
	(void)email;
	return 0;
}

		int64_t														ubk::SDomainer::AddURL				(const ::gpk::view_const_char & url)	{
	(void)url;
	return 0;
}

		int64_t														ubk::SDomainer::GetIdEMail			(const ::gpk::view_const_char & email)	{
	uint64_t																container							= 0;
	::gpk::crcGenerate(email, container);
	container															= container % Email.BlockConfig.Containers;
	::gpk::array_pod<uint32_t>												idBlocks							= Email.Id;
	::gpk::array_pod<uint32_t>												blocksToSkip;
	for(uint32_t iBlock = 0; iBlock < Email.Block.size(); ++iBlock) {
		if(Email.IdContainer[iBlock] != container)
			continue;
		const ::ubk::SSMTPMapBlock												& block								= *Email.Block[iBlock];
		const int32_t															idEmail								= block.GetSMTPMapId(email);
		const uint32_t															idBlock								= Email.Id[iBlock];
		if(0 <= idEmail) {
			::gpk::SRecordMap														indices;
			indices.IndexContainer												= Email.IdContainer[iBlock];
			indices.IdBlock														= idBlock;
			indices.IndexRecord													= idEmail;
			uint64_t																result								= (uint64_t)-1LL;
			::gpk::blockRecordId(indices, Email.BlockConfig.BlockSize, result);
			return result;
		}
		blocksToSkip.push_back(idBlock);
		(void)block;
	}
	::gpk::array_pod<char_t>												containerPath						= {};
	::gpk::blockFilePath(containerPath, Email.DBName, DBPath, Email.BlockConfig.Containers, (uint8_t)container);
	::gpk::array_obj<::gpk::array_pod<char_t>>								paths;
	::gpk::pathList({containerPath.begin(), containerPath.size()}, paths, false);
	const ::gpk::view_const_string											extension							= "ubk";
	::gpk::array_pod<char_t>												loadedBytes;
	for(uint32_t iFile = 0; iFile < paths.size(); ++iFile) {
		const ::gpk::array_pod<char_t>											fileNameCurrent						= paths[iFile];
		if(5 >= fileNameCurrent.size())
			continue;
		::gpk::view_const_char													filePart							= {&fileNameCurrent[fileNameCurrent.size() - 3], 3};
		if(filePart != extension)
			continue;
		filePart															= {fileNameCurrent.begin(), fileNameCurrent.size()-4};
		::gpk::error_t															indexOfPrevDot						= ::gpk::rfind('.', filePart);
		if(-1 == indexOfPrevDot)
			continue;
		filePart															= {&filePart[indexOfPrevDot], filePart.size() - indexOfPrevDot};
		uint32_t																idBlock								= (uint32_t)-1;
		::gpk::parseIntegerDecimal(filePart, &idBlock);
		if(0 <= ::gpk::find(idBlock, ::gpk::view_array<const uint32_t>{blocksToSkip.begin(), blocksToSkip.size()}))
			continue;
		::gpk::SRecordMap														indexMap							= {};
		indexMap.IdBlock													= idBlock;
		indexMap.IndexContainer											= (uint8_t)container;
		indexMap.IndexRecord												= -1;
		loadedBytes.clear();
		const ::gpk::error_t													indexBlock							= ::gpk::blockMapLoad(loadedBytes, Email, fileNameCurrent, indexMap);
		::ubk::SSMTPMapBlock													& block								= *Email.Block[indexBlock];
		Email.Block[indexBlock]->Load(loadedBytes);
		const int32_t															idEmail								= block.GetSMTPMapId(email);
		if(0 <= idEmail) {
			indexMap.IndexRecord												= idEmail;
			uint64_t																result								= (uint64_t)-1LL;
			::gpk::blockRecordId(indexMap, Email.BlockConfig.BlockSize, result);
			return result;
		}
	}
	return -1;
}

		int64_t														ubk::SDomainer::GetIdURL			(const ::gpk::view_const_char & url)	{
	(void)url;
	return 0;
}

