#include "ubk_domainer.h"

		::gpk::error_t							ubk::SMapBlockPath::AddMap						(const ::gpk::view_const_char & textToAdd)	{
	::gpk::array_obj<::gpk::view_const_char>			parts;
	::gpk::split(textToAdd, '/', parts);
	::gpk::array_pod<_tIndex>							mapParts;
	for(uint32_t iPart = 0, countParts = parts.size(); iPart < countParts; ++iPart) {
		const ::gpk::view_const_char						& part											= parts[iPart];
		gpk_necall(mapParts.push_back(part.size() ? AllocatorChars.View(part.begin(), (uint16_t)part.size()) : -1), "%s", "Out of memory?");
	}
	return mapParts.size() ? AllocatorMaps.View(mapParts.begin(), (uint16_t)mapParts.size()) : -1;
}

		::gpk::error_t							ubk::SMapBlockPath::GetMap						(int32_t index, ::gpk::array_pod<char_t> & path)				const	{
	const ::gpk::view_array<const _tIndex>				mapParts										= {AllocatorMaps.Views[index], AllocatorMaps.Counts[index]};
	const uint32_t										last											= (mapParts.size() - 1);
	if(1 == mapParts.size() && -1 == mapParts[0])
		path.push_back('/');
	else {
		for(uint32_t iPart = 0; iPart < mapParts.size(); ++iPart) {
			const _tIndex										indexPart										= mapParts[iPart];
			if(0 <= indexPart)
				path.append(::gpk::view_const_char{AllocatorChars.Views[indexPart], AllocatorChars.Counts[indexPart]});
			if(iPart < last)
				path.push_back('/');
		}
	}
	return 0;
}

template<typename _tIndex>
static	::gpk::error_t							partCompare										(::gpk::CViewManager<char_t> allocatorChars, ::gpk::view_array<const _tIndex> viewMap, ::gpk::view_array<const ::gpk::view_const_char> parts, const uint32_t iPart)						{
	if(iPart == viewMap.size())
		return iPart;
	const _tIndex										mapPart											= viewMap[iPart];
	const ::gpk::view_const_char						partInput										= parts[iPart];
	if(-1 == mapPart && 0 == partInput.size()) {
		if(0 <= ::partCompare(allocatorChars, viewMap, parts, iPart + 1))
			return iPart;
	}
	else {
		const ::gpk::view_const_char						partMapped										= {allocatorChars.Views[mapPart], allocatorChars.Counts[mapPart]};
		if(0 == memcmp(partInput.begin(), partMapped.begin(), partMapped.size())) {
			if(0 <= ::partCompare(allocatorChars, viewMap, parts, iPart + 1))
				return iPart;
		}
	}
	return -1;
}

		::gpk::error_t							ubk::SMapBlockPath::GetMapId					(const ::gpk::view_const_char & textToAdd)						const	{
	::gpk::array_obj<::gpk::view_const_char>			parts;
	::gpk::split(textToAdd, '/', parts);
	const uint32_t										countParts										= parts.size();
	for(uint32_t iMap = 0; iMap < AllocatorMaps.Counts.size(); ++iMap) {
		const uint32_t										lenCurrentMap									= AllocatorMaps.Counts[iMap];
		if(countParts != lenCurrentMap)
			continue;
		const ::gpk::view_array<const _tIndex>				viewMap											= {AllocatorMaps.Views[iMap], lenCurrentMap};
		if(0 <= ::partCompare(AllocatorChars, viewMap, parts, 0))
			return iMap;
	}
	return -1;
}

		::gpk::error_t							ubk::SMapBlockPath::Save					(::gpk::array_pod<byte_t> & output)								const	{
	gpk_necall(::gpk::viewManagerSave(AllocatorChars, output), "Failed to load path map allocator: %s", "Out of memory?");
	gpk_necall(::gpk::viewManagerSave(AllocatorMaps	, output), "Failed to load path map allocator: %s", "Out of memory?");
	return 0;
}

		::gpk::error_t							ubk::SMapBlockPath::Load					(const ::gpk::view_const_byte & input)	{
	int32_t												offsetMaps									= ::gpk::viewManagerLoad(AllocatorChars, input);
	gpk_necall(offsetMaps, "Failed to load path character allocator: %s", "Out of memory?");
	if(offsetMaps < (int32_t)input.size())
		gpk_necall(::gpk::viewManagerLoad(AllocatorMaps, {&input[offsetMaps], input.size() - offsetMaps}), "Failed to load path map allocator: %s", "Out of memory?");
	return 0;
}

