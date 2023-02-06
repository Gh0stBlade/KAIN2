#pragma once

#include <typeinfo>
#include <string>
#include <iostream>

#include <boost/mpl/range_c.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/zip.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/mpl.hpp>

#include <boost/type_index.hpp>

#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/lexical_cast.hpp>
#include <stdio.h>
#include <memory>
#include <type_traits>

#include "FILE.H"

using namespace boost::fusion;

template <typename T2> struct UPGRADE_BaseDump;

template <typename S, typename N> struct Comma {
	static inline void comma() { printf(" , "); }
};
template <typename S> struct Comma<S, typename
	boost::mpl::prior<typename boost::fusion::result_of::size<S>::type >::type> {
	static inline void comma() {}
};

template<typename T>
struct remove_pointer
{
	typedef T type;
};

template<typename T>
struct remove_pointer<T*>
{
	typedef typename remove_pointer<T>::type type;
};

template <typename S, typename N> 
struct DecImplSeqItr_s 
{
	typedef typename boost::fusion::result_of::value_at<S, N>::type current_t;
	typedef typename boost::mpl::next<N>::type next_t;
	//typedef typename boost::mpl::first<F>::type first_t;
	typedef boost::fusion::extension::struct_member_name<S, N::value> name_t;

	template <typename C = S>
	static inline void decode(S& s)
	{
		if (boost::is_pointer<current_t>::value)
		{
			union DataPointer
			{
				long* DTP;
				current_t* value;
			};

			DataPointer dtp;
			dtp.value = &boost::fusion::at_c<N::value>(s);

			if (dtp.DTP[0] != NULL)
			{
				g_numPointerTypes++;
			}
		}
		else if (std::is_pointer<std::remove_pointer<current_t>::type>::value)//ptr**
		{
			union DataPointer
			{
				long* DTP;
				current_t* value;
			};

			DataPointer dtp;
			dtp.value = &boost::fusion::at_c<N::value>(s);

			if (dtp.DTP[0] != NULL)
			{
				g_numPointerTypes++;
			}
		}

		DecImplSeqItr_s<S, next_t>::decode(s);
	}

	static inline int UPGRADE_HowManyPointersAfter(S& s)
	{
		static int numPointers = 0;

		if (boost::is_pointer<current_t>::value || std::is_pointer<std::remove_pointer<current_t>::type>::value)
		{
			numPointers++;
		}
		
		numPointers += DecImplSeqItr_s<S, next_t>::UPGRADE_HowManyPointersAfter(s);
		return numPointers;
	}

	template <typename C = S>
	static inline void decodeDTP(S& s, int* offset)
	{
		printf("%d", ftell(g_fileHandle));
		printf(" \"%s\" = ", name_t::call());

		//Dump all pointers now :)
		if (!boost::is_pointer<current_t>::value)//ptr
		{
			memmove(g_pCurr, &boost::fusion::at_c<N::value>(s), sizeof(current_t));
			g_pCurr += sizeof(current_t);
		}
		else if (std::is_pointer<std::remove_pointer<current_t>::type>::value || boost::is_pointer<current_t>::value)//ptr**
		{
			union DataPointer
			{
				long* DTP;
				current_t* value;
			};

			DataPointer dtp;
			dtp.value = &boost::fusion::at_c<N::value>(s);

			UPGRADE_RedirectList* existingrdList = UPGRADE_FindRedirectList((long*)dtp.DTP[0]);

			if (existingrdList == NULL)
			{
				UPGRADE_RedirectList rdList;
				if (dtp.DTP[0] != NULL)
				{
					rdList.incr = (UPGRADE_HowManyPointersAfter(s)) + ((dtp.DTP[0] - (long)g_baseAddr - 0x800) / sizeof(unsigned int));
				}
				else
				{
					rdList.incr = 0;
				}
				rdList.pDTP = (long*)dtp.DTP[0];
				rdList.pDTPConvertedFile = (long*)g_pCurr;
				rdList.pDTPConvertedFile[0] = 0;
				g_redirectList.push_back(rdList);

				if (offset != NULL && rdList.incr > 0 && rdList.incr * sizeof(unsigned int) <= *offset)
				{
					*offset = rdList.incr * sizeof(unsigned int);
				}
			}

			//memmove(g_pCurr, &boost::fusion::at_c<N::value>(s), sizeof(current_t));
			g_pCurr += sizeof(current_t) * 2;
		}

		DecImplSeqItr_s<S, next_t>::decodeDTP(s, offset);
	}
};
template <typename S>
struct DecImplSeqItr_s<S, typename boost::fusion::result_of::size<S>::type > {
	static inline void decode(S& s) { }
	static inline void decodeDTP(S& s, int* offset) { }
	static inline int UPGRADE_HowManyPointersAfter(S& s) { return 0; }
};
template <typename S>
struct DecImplSeqStart_s :DecImplSeqItr_s<S, boost::mpl::int_<0> > {};

template <typename S> struct DecImplSeq_s {
	typedef DecImplSeq_s<S> type;
	static void decode(S& s) {
		printf("  struct  start --- { --- ");
		dec_indents += 4;
		DecImplSeqStart_s<S>::decode(s);
		dec_indents -= 4;
		printf("  struct  done  --- } --- ");
	};
};

template <typename S> struct DecImplSeqDTP_s {
	typedef DecImplSeqDTP_s<S> type;
	static void decode(S& s, int* offset) {
		printf("  struct  start --- { --- ");
		dec_indents += 4;
		DecImplSeqStart_s<S>::decodeDTP(s, offset);
		dec_indents -= 4;
		printf("  struct  done  --- } --- ");
	};
};

template <typename T2> struct DecImplArray_s {
	typedef DecImplArray_s<T2> type;
	typedef typename boost::remove_bounds<T2>::type slice_t;
	static const size_t size = sizeof(T2) / sizeof(slice_t);
	static inline void decode(T2& t) {
		printf("  array start --- [ --- ");
		dec_indents += 4;
		NL::print();
		for (size_t idx = 0; idx < size; idx++) {
			Dec_s<slice_t>::decode(t[idx]);
			if (idx < size - 1) {
				NL::print(); printf(" , ");
			}
		}
		dec_indents -= 4;
		NL::print();
		printf("  array done  --- ] --- \n");
		NL::print();
	}
};

template <typename T2> struct DecImplArrayDTP_s {
	typedef DecImplArrayDTP_s<T2> type;
	typedef typename boost::remove_bounds<T2>::type slice_t;
	static const size_t size = sizeof(T2) / sizeof(slice_t);
	static inline void decode(T2& t) {
		printf("  array start --- [ --- ");
		dec_indents += 4;
		NL::print();
		for (size_t idx = 0; idx < size; idx++) {
			Dec_s<slice_t>::decode(t[idx]);
			if (idx < size - 1) {
				NL::print(); printf(" , ");
			}
		}
		dec_indents -= 4;
		NL::print();
		printf("  array done  --- ] --- \n");
		NL::print();
	}
};

template <typename T2> struct DecImplVoid_s {
	typedef DecImplVoid_s<T2> type;

	static void decode(T2& t) {
		int status = 0;

		if (g_fileHandle != NULL)
		{
			fwrite(&t, sizeof(T2), 1, g_fileHandle);
		}
	};
};

template <typename T2> struct DecImplVoidDTP_s {
	typedef DecImplVoidDTP_s<T2> type;

	static void decode(T2& t) {
		int status = 0;

		if (g_fileHandle != NULL)
		{
			fwrite(&t, sizeof(T2), 1, g_fileHandle);
		}
	};
};

template <typename T2> struct DecCalc_s 
{
	typedef
		typename boost::mpl::eval_if< traits::is_sequence<T2>, DecImplSeq_s<T2>,
		typename boost::mpl::eval_if< boost::is_array<T2>,
		boost::mpl::identity< DecImplArray_s<T2> >,
		DecImplVoid_s<T2>   > >
		::type type;
};

template <typename T2> struct DecCalcDTP_s 
{
	typedef
		typename boost::mpl::eval_if< traits::is_sequence<T2>, DecImplSeqDTP_s<T2>,
		typename boost::mpl::eval_if< boost::is_array<T2>,
		boost::mpl::identity< DecImplArrayDTP_s<T2> >,
		DecImplVoidDTP_s<T2>   > >
		::type type;
};

template <typename T2> struct UPGRADE_BaseDump : public DecCalc_s<T2>::type { };

template <typename T2> struct UPGRADE_DTPDump : public DecCalcDTP_s<T2>::type { };