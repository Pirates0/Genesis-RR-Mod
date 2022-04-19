#pragma once

//-------------------------------------------------------------//
// "Malware related compile-time hacks with C++11" by LeFF   //
// You can use this code however you like, I just don't really //
// give a shit, but if you feel some respect for me, please //
// don't cut off this comment when copy-pasting... ;-)       //
//-------------------------------------------------------------//

namespace utils {


	template <int X> struct EnsureCompileTime {
		enum : int {
			Value = X
		};
	};

	//Use Compile-Time as seed
#define compile_seed (	(__TIME__[7] - '0') * 1  + (__TIME__[6] - '0') * 10		+	\
    						(__TIME__[4] - '0') * 60   + (__TIME__[3] - '0') * 600	+	\
    						(__TIME__[1] - '0') * 3600 + (__TIME__[0] - '0') * 36000	)

	constexpr int LinearCongruentGenerator(int Rounds) {
		return 1013904223 + 1664525 * ((Rounds > 0) ? LinearCongruentGenerator(Rounds - 1) : compile_seed & 0xFFFFFFFF);
	}

#define Random() EnsureCompileTime<LinearCongruentGenerator(10)>::Value //10 Rounds
#define RandomNumber(Min, Max) (Min + (Random() % (Max - Min + 1)))

	template <int... Pack> struct IndexList {};

	template <typename IndexList, int Right> struct Append;
	template <int... Left, int Right> struct Append<IndexList<Left...>, Right> {
		typedef IndexList<Left..., Right> Result;
	};

	template <int N> struct ConstructIndexList {
		typedef typename Append<typename ConstructIndexList<N - 1>::Result, N - 1>::Result Result;
	};

	template <> struct ConstructIndexList<0> {
		typedef IndexList<> Result;
	};

	template <typename Char, typename IndexList> class XorStringT;
	template <typename Char, int... Index> class XorStringT<Char, IndexList<Index...> > {

	private:
		Char Value[sizeof...(Index) + 1];

		static const Char XORKEY = static_cast<Char>(RandomNumber(0, 0xFFFF));

		template <typename CHar>
		constexpr CHar EncryptCharacterT(const CHar Character, int INdex) {
			return Character ^ (XORKEY + INdex);
		}

	public:
		__forceinline constexpr XorStringT(const Char * const String)
			: Value{ EncryptCharacterT(String[Index], Index)... } {}

		const Char* decrypt() {
			for (uint32_t t = 0; t < sizeof...(Index); t++) {
				Value[t] = Value[t] ^ (XORKEY + t);
			}

			Value[sizeof...(Index)] = static_cast<Char>(0);

			return Value;
		}

		const Char* get() {
			return Value;
		}
	};
	//#define XORSTR(String) String
#define XS( String ) ( utils::XorStringT<char, utils::ConstructIndexList<sizeof( String ) - 1>::Result>( String ).decrypt() )
#define XWS( String ) ( utils::XorStringT<wchar_t, utils::ConstructIndexList<(sizeof( String ) - 1) / 2>::Result>( String ).decrypt() )

}

