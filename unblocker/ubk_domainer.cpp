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
	(void)email;
	for(uint32_t iBlock = 0; iBlock < Email.Block.size(); ++iBlock) {
		const ::ubk::SSMTPMapBlock												& block								= Email.Block[iBlock];
		(void)block;
	}
	return 0;
}


		int64_t														ubk::SDomainer::GetIdURL			(const ::gpk::view_const_char & url)	{
	(void)url;
	return 0;
}

