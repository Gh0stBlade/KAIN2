#pragma once

#include <typeinfo>
#include <string>
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

extern int dec_indents;
struct NL {
	static void print() {
		printf("\n");
		for (int i = 0; i < dec_indents; i++) printf(" ");
	}
};

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
		printf("%d", ftell(g_fileHandle));
		printf(" \"%s\" = ", name_t::call());

		//Skip all pointers, we manually dump them since they're size based :(
		if (!boost::is_pointer<current_t>::value)
		{
			UPGRADE_BaseDump<current_t>::decode(boost::fusion::at<N>(s));
			Comma<S, N>::comma();  // Insert comma or not
		}
		else if (boost::is_pointer<current_t>::value)
		{
			FILE_SeekCur(sizeof(unsigned long long));
		}

		DecImplSeqItr_s<S, next_t>::decode(s);
	}

	template <typename C = S>
	static inline void decodeDTP(S& s)
	{
		printf("%d", ftell(g_fileHandle));
		printf(" \"%s\" = ", name_t::call());

		//Dump all pointers now :)
		if (!boost::is_pointer<current_t>::value)//ptr
		{
			fseek(g_fileHandle, sizeof(current_t), SEEK_CUR);
		}
		else if (std::is_pointer<std::remove_pointer<current_t>::type>::value)//ptr**
		{
#if 0
			union DTPContainer
			{
				void* DTP;
				current_t value;
			};

			DTPContainer DTP;
			DTP.value = boost::fusion::at_c<N::value>(s);

			unsigned int NULL_PTR = 0;
			bool bIsNotNullPointer = memcmp(&DTP, &NULL_PTR, sizeof(unsigned int));
			unsigned long long p = 0;
			long savedOffset = ftell(g_fileHandle);
			p = bIsNotNullPointer ? savedOffset - 0x800 : 0;
			if (bIsNotNullPointer)
			{
				fseek(g_fileHandle, (g_lastRelocIndex++ * sizeof(unsigned int)) + sizeof(unsigned int), SEEK_SET);
				fwrite(&p, sizeof(unsigned int), 1, g_fileHandle);
			}
			fseek(g_fileHandle, 0, SEEK_END);
			p = ftell(g_fileHandle) - 0x800;

			if (bIsNotNullPointer)
			{
				unsigned long long offsetDTP = ftell(g_fileHandle) - 0x800 + 0x8;
				fwrite(&offsetDTP, sizeof(unsigned long long), 1, g_fileHandle);
			}

			fseek(g_fileHandle, savedOffset, SEEK_SET);
			fwrite(&p, sizeof(p), 1, g_fileHandle);
#endif
		}
		else if (boost::is_pointer<current_t>::value)//ptr*
		{
			union DTPContainer
			{
				char* DTP;
				current_t value;
			};

			DTPContainer DTP;
			DTP.value = boost::fusion::at_c<N::value>(s);

			unsigned int NULL_PTR = 0;
			bool bIsNotNullPointer = DTP.DTP != NULL;
			unsigned long long p = 0;
			long savedOffset = ftell(g_fileHandle);
			p = bIsNotNullPointer ? savedOffset - 0x800 : 0;
			if (bIsNotNullPointer)
			{
				fseek(g_fileHandle, (g_lastRelocIndex++ * sizeof(unsigned int)) + sizeof(unsigned int), SEEK_SET);
				fwrite(&p, sizeof(unsigned int), 1, g_fileHandle);
			}
			fseek(g_fileHandle, 0, SEEK_END);
			p = ftell(g_fileHandle) - 0x800;

			if (!bIsNotNullPointer)
			{
				unsigned long long dummy = 0;
				fwrite(&dummy, sizeof(dummy), 1, g_fileHandle);
			}
			else
			{
				fseek(g_fileHandle, savedOffset, SEEK_SET);
				fwrite(&p, sizeof(p), 1, g_fileHandle);
			}
		}

		DecImplSeqItr_s<S, next_t>::decodeDTP(s);
	}
};
template <typename S>
struct DecImplSeqItr_s<S, typename boost::fusion::result_of::size<S>::type > {
	static inline void decode(S& s) { }
	static inline void decodeDTP(S& s) { }
};
template <typename S>
struct DecImplSeqStart_s :DecImplSeqItr_s<S, boost::mpl::int_<0> > {};

template <typename S> struct DecImplSeq_s {
	typedef DecImplSeq_s<S> type;
	static void decode(S& s) {
		printf("  struct  start --- { --- ");
		dec_indents += 4;
		NL::print();
		DecImplSeqStart_s<S>::decode(s);
		dec_indents -= 4;
		NL::print();
		printf("  struct  done  --- } --- ");
		NL::print();
	};
};

template <typename S> struct DecImplSeqDTP_s {
	typedef DecImplSeqDTP_s<S> type;
	static void decode(S& s) {
		printf("  struct  start --- { --- ");
		dec_indents += 4;
		NL::print();
		DecImplSeqStart_s<S>::decodeDTP(s);
		dec_indents -= 4;
		NL::print();
		printf("  struct  done  --- } --- ");
		NL::print();
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