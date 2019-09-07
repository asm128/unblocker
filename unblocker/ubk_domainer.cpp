#include "ubk_domainer.h"

		::gpk::error_t												ubk::SDomainer::GetEMail			(const uint64_t idRecord, ::gpk::array_pod<char_t> & email)		{
	::gpk::SRecordMap														recordMap;
	::gpk::array_pod<char_t>												fileBytes;
	::gpk::error_t															indexBlock							= ::gpk::blockMapLoad(fileBytes, recordMap, Email, Email.DBName, DBPath, idRecord);
	gpk_necall(indexBlock, "Invalid record id: %llu.", idRecord);
	::ubk::SSMTPMapBlock													& loadedBlock						= Email.Block[indexBlock];
	if(fileBytes.size())
		gpk_necall(loadedBlock.Load(fileBytes), "Failed to load block data for record %llu.", idRecord);

	return loadedBlock.GetEMail(recordMap.IndexRecord, email);
}

		::gpk::error_t												ubk::SDomainer::GetURL				(const uint64_t idRecord, ::gpk::array_pod<char_t> & url)		{
	::gpk::SRecordMap														recordMap;
	::gpk::array_pod<char_t>												fileBytes;
	::gpk::error_t															indexBlock							= ::gpk::blockMapLoad(fileBytes, recordMap, URL, URL.DBName, DBPath, idRecord);
	gpk_necall(indexBlock, "Invalid record id: %llu.", idRecord);
	::ubk::SURLMapBlock														& loadedBlock						= URL.Block[indexBlock];
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
	for(uint32_t iBlock = 0; iBlock < Email.Block.size(); ++iBlock) {
		if(Email.IdContainer[iBlock] != container)
			continue;
		const ::ubk::SSMTPMapBlock												& block								= Email.Block[iBlock];
		int32_t																	idEmail								= block.GetSMTPMapId(email);
		if(0 <= idEmail) {
			::gpk::SRecordMap														indices;
			indices.IndexContainer												= Email.IdContainer[iBlock];
			indices.IdBlock														= Email.Id[iBlock];
			indices.IndexRecord													= idEmail;
			uint64_t																result								= (uint64_t)-1LL;
			::gpk::blockRecordId(indices, Email.BlockConfig.BlockSize, result);
			return result;
		}
		(void)block;
	}
	::gpk::array_pod<char_t>												containerPath						= {};
	::gpk::blockFilePath(containerPath, Email.DBName, DBPath, Email.BlockConfig.Containers, (uint8_t)container);
	return 0;
}


		int64_t														ubk::SDomainer::GetIdURL			(const ::gpk::view_const_char & url)	{
	(void)url;
	return 0;
}

