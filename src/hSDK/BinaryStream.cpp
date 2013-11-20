#include "hSDK/BinaryStream.hpp"

#include "EditData.hpp"

#include <vector>

namespace hSDK
{
	namespace BinaryStream
	{
		namespace EditData
		{
			struct In::Buf : std::basic_streambuf<char_t>
			{
				Buf(ED const &sed, std::size_t putback = 8)
				: pb(putback), b(putback)
				{
					b.reserve(pb + sed.h.extSize);
					for(std::size_t i = 0; i < sed.h.extSize; ++i)
					{
						b.push_back(sed.data[i]);
					}
					setg(&b.front(), &b.front()+pb, &b.front()+b.size());
				}
				Buf(Buf const &) = delete;
				Buf(Buf &&) = default;
				Buf &operator=(Buf const &) = delete;
				Buf &operator=(Buf &&) = default;

			private:
				std::size_t pb;
				std::vector<char_type> b;

				virtual int_type underflow() override
				{
					if(gptr() < egptr())
					{
						return traits_type::to_int_type(*gptr());
					}
					return traits_type::eof();
				}
			};

			In::In(ED *sed)
			: InBase(temp = new Buf(*sed))
			, b(temp)
			{
				temp = nullptr;
			}
			In::~In() = default;

			struct Out::Buf : std::basic_streambuf<char_t>
			{
				Buf(mv *mV_, ED *&sed_)
				: mV(mV_)
				, sed(sed_)
				{
					setp(0, 0);
				}
				Buf(Buf const &) = delete;
				Buf(Buf &&) = default;
				Buf &operator=(Buf const &) = delete;
				Buf &operator=(Buf &&) = default;

			private:
				mv *mV;
				ED *&sed;
				std::vector<char_type> b;

				virtual int_type overflow(int_type ch) override
				{
					if(ch != traits_type::eof())
					{
						b.push_back(char_type(ch));
						setp(&b.front(), /*&b.back()+1, */&b.back()+1);
						pbump((&b.back()+1) - &b.front());
					}
					return traits_type::eof();
				}
				virtual int sync() override
				{
					std::size_t size = (&b.back()+1) - &b.front();
					ED *t = reinterpret_cast<ED *>(mvReAllocEditData(mV, sed, sizeof(ED)+size));
					if(t)
					{
						sed = t;
						memcpy(&sed->data, &b.front(), size);
						return 0;
					}
					return -1;
				}
			};

			Out::Out(mv *mV, ED *&sed)
			: OutBase(temp = new Buf(mV, sed))
			, b(temp)
			{
				temp = nullptr;
			}
			Out::~Out() = default;
		}
		namespace RunData
		{
		}
	}
}
