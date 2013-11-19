#ifndef hSDK_Properties_HeaderPlusPlus
#define hSDK_Properties_HeaderPlusPlus

#include "hSDK.hpp"

#include <cstdint>
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
			virtual ~In() = 0;
		};
		inline In::~In() = default;
		struct Out : std::basic_ostream<char_t>
		{
			Out(std::basic_streambuf<char_t> *sb)
			: std::basic_ostream<char_t>(sb)
			{
			}
			virtual ~Out() = 0;
		};
		inline Out::~Out() = default;
		namespace EditData
		{
			struct In : ::hSDK::BinaryStream::In
			{
				In(ED *);
			private:
				struct Buf;
				std::unique_ptr<Buf> b;
			};
			struct Out : ::hSDK::BinaryStream::Out
			{
				Out(mv *, ED *&);
			private:
				struct Buf;
				std::unique_ptr<Buf> b;
			};
		}
		namespace RunData
		{
			struct In : ::hSDK::BinaryStream::In
			{
				In(void *);
			private:
				struct Buf;
				std::unique_ptr<Buf> b;
			};
			struct Out : ::hSDK::BinaryStream::Out
			{
				Out(void *);
			private:
				struct Buf;
				std::unique_ptr<Buf> b;
			};
		}
	}
}

#endif
