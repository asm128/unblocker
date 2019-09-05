#include "gpk_array.h"

#include "unblocker.h"

#ifndef INFINIREPLY_H_2983749827432
#define INFINIREPLY_H_2983749827432

namespace ubk
{
	struct SComment				{ int64_t Author; int64_t Text; int64_t Parent; };
	struct SQOrA				{ int64_t Author; int64_t Text; };
	struct SQuestion			{ SQOrA Question; ::gpk::array_pod<SQOrA> Answers; };
	struct SComments			{
		::gpk::array_pod<::ubk::SReferral>		Referral;
		::gpk::array_pod<::ubk::SComment>		Comment;
		::gpk::array_obj<::ubk::SQuestion>		Question;
	};
} // namespace

#endif // INFINIREPLY_H_2983749827432
