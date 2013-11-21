#ifndef hSDK_Properties_HeaderPlusPlus
#define hSDK_Properties_HeaderPlusPlus

#include "hSDK.hpp"

#include <iostream>
#include <streambuf>
#include <memory>

namespace hSDK
{
	namespace BinaryStream
	{
		struct In : std::basic_istream<char_t>
		{
			In(std::basic_streambuf<char_t> *sb)
			: std::basic_istream<char_t>(sb)
			{
			}
			In(In const &) = delete;
			In(In &&) = default;
			In &operator=(In const &) = delete;
			In &operator=(In &&) = default;
			virtual ~In() = 0;
		};
		inline In::~In() = default;
		struct Out : std::basic_ostream<char_t>
		{
			Out(std::basic_streambuf<char_t> *sb)
			: std::basic_ostream<char_t>(sb)
			{
			}
			Out(Out const &) = delete;
			Out(Out &&) = default;
			Out &operator=(Out const &) = delete;
			Out &operator=(Out &&) = default;
			virtual ~Out() = 0;
		};
		inline Out::~Out() = default;

		namespace EditData
		{
			using InBase = ::hSDK::BinaryStream::In;
			struct In : InBase
			{
				In(ED *);
				In(In const &) = delete;
				In(In &&) = default;
				In &operator=(In const &) = delete;
				In &operator=(In &&) = default;
				virtual ~In();

			private:
				struct Buf;
				std::unique_ptr<Buf> b;
				In(Buf *, ED *);
			};
			using OutBase = ::hSDK::BinaryStream::Out;
			struct Out : OutBase
			{
				Out(mv *, ED *&);
				Out(Out const &) = delete;
				Out(Out &&) = default;
				Out &operator=(Out const &) = delete;
				Out &operator=(Out &&) = default;
				virtual ~Out();

			private:
				struct Buf;
				std::unique_ptr<Buf> b;
				Out(Buf *, mv *, ED *&);
			};
		}
		namespace RunData
		{
			using InBase = ::hSDK::BinaryStream::In;
			struct In : InBase
			{
				In(void *);
				In(In const &) = delete;
				In(In &&) = default;
				In &operator=(In const &) = delete;
				In &operator=(In &&) = default;
				virtual ~In();

			private:
				struct Buf;
				std::unique_ptr<Buf> b;
				In(Buf *, void *);
			};
			using OutBase = ::hSDK::BinaryStream::Out;
			struct Out : OutBase
			{
				Out(void *);
				Out(Out const &) = delete;
				Out(Out &&) = default;
				Out &operator=(Out const &) = delete;
				Out &operator=(Out &&) = default;
				virtual ~Out();

			private:
				struct Buf;
				std::unique_ptr<Buf> b;
				Out(Buf *, void *);
			};
		}
	}
}

#endif
