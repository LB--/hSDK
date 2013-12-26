#include "hSDK.hpp"

#include <fstream>
#include <streambuf>

#include <Windows.h>

namespace hSDK
{
	string8 str_to8fr16(string16 const &s)
	{
		if(s.size() == 0) return string8();
		int r1 = WideCharToMultiByte
		(
			CP_UTF8,
			0,
			s.c_str(),
			s.size(),
			NULL,
			0,
			NULL,
			NULL
		);
		if(r1 == 0) return string8();
		std::unique_ptr<string8::value_type[]> buf {new string8::value_type[r1]};
		int r2 = WideCharToMultiByte
		(
			CP_UTF8,
			0,
			s.c_str(),
			s.size(),
			buf.get(),
			r1,
			NULL,
			NULL
		);
		if(r2 == 0) return string8();
		return string8(buf.get(), r2);
	}
	string16 str_to16fr8(string8 const &s)
	{
		if(s.size() == 0) return string16();
		int r1 = MultiByteToWideChar
		(
			CP_UTF8,
			0,
			s.c_str(),
			s.size(),
			NULL,
			0
		);
		if(r1 == 0) return string16();
		std::unique_ptr<string16::value_type[]> buf {new string16::value_type[r1]};
		int r2 = MultiByteToWideChar
		(
			CP_UTF8,
			0,
			s.c_str(),
			s.size(),
			buf.get(),
			r1
		);
		if(r2 == 0) return string16();
		return string16(buf.get(), r2);
	}

	string8 EntireFile(string const &filename)
	{
		std::ifstream t (str_to8(filename));
		string8 str;

		t.seekg(0, std::ios::end);
		str.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign
		(
			std::istreambuf_iterator<string8::value_type>(t),
			std::istreambuf_iterator<string8::value_type>()
		);
		return str;
	}
}
