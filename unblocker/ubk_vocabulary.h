#include "gpk_array.h"

#include "unblocker.h"

#ifndef VOCABULARY_H_89023749283742
#define VOCABULARY_H_89023749283742

namespace ubk
{
	//-------------------------
	struct SParagraph		{
		::gpk::array_obj<::gpk::view_const_string>							B64Text;
		::gpk::array_obj<::gpk::array_pod<uint32_t>>						Words;
	};

	struct STags			{
		::gpk::array_obj<::gpk::view_const_string>							Word;
		::gpk::array_obj<::gpk::array_pod<::ubk::SReferral>>				Referrals;
	};

	struct SDictionary		{
		::gpk::array_obj<::gpk::view_const_string>							Word;
		::gpk::array_obj<::gpk::array_pod<::ubk::SReferral>>				Referrals;
	};

	struct SLanguages		{
		::gpk::array_obj<::gpk::view_const_string>							Name;
		::gpk::array_obj<::gpk::view_const_string>							CodeISO;
		::gpk::array_obj<::gpk::view_const_string>							Code1;
	};

	struct SVocabulary		{
		::gpk::array_obj<::ubk::STags>										Tag;		// Tags are common for all languages.
		::gpk::array_obj<::ubk::SLanguages>									Language;
		::gpk::array_obj<::gpk::array_obj<::ubk::SParagraph>>				Paragraph;	// for each language, an array of paragraphs.
		::gpk::array_obj<::ubk::SDictionary>								Dictionary;	// for each language, a dictionary.
	};
} // namespace

#endif // VOCABULARY_H_89023749283742
