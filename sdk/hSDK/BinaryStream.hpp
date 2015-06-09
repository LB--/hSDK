#ifndef hSDK_BinaryStream_HeaderPlusPlus
#define hSDK_BinaryStream_HeaderPlusPlus

#include "hSDK.hpp"

#include <iostream>
#include <streambuf>
#include <memory>

namespace hSDK
{
	namespace BinaryStream
	{
		enum struct Endianness
		{
			Little, Big
		};
		namespace impl
		{
			static inline int geti()
			{
				static int i = std::ios_base::xalloc();
				return i;
			}

			struct AsBase
			{
				virtual ~AsBase() = default;

				using oit_t = std::num_put<char_t>::iter_type;
				using iit_t = std::num_get<char_t>::iter_type;

				virtual oit_t Put(oit_t it, std::uint64_t v) = 0;
				virtual iit_t Get(iit_t it, std::uint64_t &v) = 0;
				virtual oit_t Put(oit_t it, long double v) = 0;
				virtual iit_t Get(iit_t it, long double &v) = 0;
			};
			template<typename T, Endianness E>
			struct AsT final : AsBase
			{
				virtual oit_t Put(oit_t it, std::uint64_t v) override
				{
					std::uint8_t *pv = reinterpret_cast<std::uint8_t *>(&v);
					if(E == Endianness::Little)
					{
						for(auto p = pv; p != pv + sizeof(T); ++p)
						{
							*it = *p;
							++it;
						}
					}
					else
					{
						for(auto p = pv + sizeof(T) - 1; p != pv - 1; --p)
						{
							*it = *p;
							++it;
						}
					}
					return it;
				}
				virtual iit_t Get(iit_t it, std::uint64_t &v) override
				{
					std::uint8_t *pv = reinterpret_cast<std::uint8_t *>(&v);
					if(E == Endianness::Little)
					{
						for(auto p = pv; p != pv + sizeof(T); ++p)
						{
							*p = *it;
							++it;
						}
					}
					else
					{
						for(auto p = pv + sizeof(T) - 1; p != pv - 1; --p)
						{
							*p = *it;
							++it;
						}
					}
					return it;
				}
				virtual oit_t Put(oit_t it, long double v) override
				{
					std::uint8_t *pv = reinterpret_cast<std::uint8_t *>(&v);
					if(E == Endianness::Little)
					{
						for(auto p = pv; p != pv + sizeof(T); ++p)
						{
							*it = *p;
							++it;
						}
					}
					else
					{
						for(auto p = pv + sizeof(T) - 1; p != pv - 1; --p)
						{
							*it = *p;
							++it;
						}
					}
					return it;
				}
				virtual iit_t Get(iit_t it, long double &v) override
				{
					std::uint8_t *pv = reinterpret_cast<std::uint8_t *>(&v);
					if(E == Endianness::Little)
					{
						for(auto p = pv; p != pv + sizeof(T); ++p)
						{
							*p = *it;
							++it;
						}
					}
					else
					{
						for(auto p = pv + sizeof(T) - 1; p != pv - 1; --p)
						{
							*p = *it;
							++it;
						}
					}
					return it;
				}
			};

			struct NumPut : std::num_put<char_t>
			{
				virtual iter_type do_put(iter_type it, std::ios_base &ios, char_type fill, bool v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						iter_type r = as->Put(it, v ? 1ull : 0ull);
						delete as, as = nullptr;
						return r;
					}
					return std::num_put<char_t>::do_put(it, ios, fill, v);
				}
				virtual iter_type do_put(iter_type it, std::ios_base &ios, char_type fill, long v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						iter_type r = as->Put(it, static_cast<std::uint64_t>(v));
						delete as, as = nullptr;
						return r;
					}
					return std::num_put<char_t>::do_put(it, ios, fill, v);
				}
				virtual iter_type do_put(iter_type it, std::ios_base &ios, char_type fill, unsigned long v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						iter_type r = as->Put(it, static_cast<std::uint64_t>(v));
						delete as, as = nullptr;
						return r;
					}
					return std::num_put<char_t>::do_put(it, ios, fill, v);
				}
				virtual iter_type do_put(iter_type it, std::ios_base &ios, char_type fill, long long v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						iter_type r = as->Put(it, static_cast<std::uint64_t>(v));
						delete as, as = nullptr;
						return r;
					}
					return std::num_put<char_t>::do_put(it, ios, fill, v);
				}
				virtual iter_type do_put(iter_type it, std::ios_base &ios, char_type fill, unsigned long long v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						iter_type r = as->Put(it, v);
						delete as, as = nullptr;
						return r;
					}
					return std::num_put<char_t>::do_put(it, ios, fill, v);
				}
				virtual iter_type do_put(iter_type it, std::ios_base &ios, char_type fill, double v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						iter_type r = as->Put(it, static_cast<long double>(v));
						delete as, as = nullptr;
						return r;
					}
					return std::num_put<char_t>::do_put(it, ios, fill, v);
				}
				virtual iter_type do_put(iter_type it, std::ios_base &ios, char_type fill, long double v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						iter_type r = as->Put(it, v);
						delete as, as = nullptr;
						return r;
					}
					return std::num_put<char_t>::do_put(it, ios, fill, v);
				}
			};

			struct NumGet : std::num_get<char_t>
			{
				virtual iter_type do_get(iter_type it, iter_type end, std::ios_base &ios, std::ios_base::iostate &err, bool &v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						std::uint64_t temp;
						iter_type r = as->Get(it, temp);
						v = (temp ? true : false);
						delete as, as = nullptr;
						return r;
					}
					return std::num_get<char_t>::do_get(it, end, ios, err, v);
				}
				virtual iter_type do_get(iter_type it, iter_type end, std::ios_base &ios, std::ios_base::iostate &err, long &v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						std::uint64_t temp;
						iter_type r = as->Get(it, temp);
						v = static_cast<std::remove_reference<decltype(v)>::type>(temp);
						delete as, as = nullptr;
						return r;
					}
					return std::num_get<char_t>::do_get(it, end, ios, err, v);
				}
				virtual iter_type do_get(iter_type it, iter_type end, std::ios_base &ios, std::ios_base::iostate &err, long long &v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						std::uint64_t temp;
						iter_type r = as->Get(it, temp);
						v = static_cast<std::remove_reference<decltype(v)>::type>(temp);
						delete as, as = nullptr;
						return r;
					}
					return std::num_get<char_t>::do_get(it, end, ios, err, v);
				}
				virtual iter_type do_get(iter_type it, iter_type end, std::ios_base &ios, std::ios_base::iostate &err, unsigned short &v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						std::uint64_t temp;
						iter_type r = as->Get(it, temp);
						v = static_cast<std::remove_reference<decltype(v)>::type>(temp);
						delete as, as = nullptr;
						return r;
					}
					return std::num_get<char_t>::do_get(it, end, ios, err, v);
				}
				virtual iter_type do_get(iter_type it, iter_type end, std::ios_base &ios, std::ios_base::iostate &err, unsigned int &v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						std::uint64_t temp;
						iter_type r = as->Get(it, temp);
						v = static_cast<std::remove_reference<decltype(v)>::type>(temp);
						delete as, as = nullptr;
						return r;
					}
					return std::num_get<char_t>::do_get(it, end, ios, err, v);
				}
				virtual iter_type do_get(iter_type it, iter_type end, std::ios_base &ios, std::ios_base::iostate &err, unsigned long &v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						std::uint64_t temp;
						iter_type r = as->Get(it, temp);
						v = static_cast<std::remove_reference<decltype(v)>::type>(temp);
						delete as, as = nullptr;
						return r;
					}
					return std::num_get<char_t>::do_get(it, end, ios, err, v);
				}
				virtual iter_type do_get(iter_type it, iter_type end, std::ios_base &ios, std::ios_base::iostate &err, unsigned long long &v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						iter_type r = as->Get(it, v);
						delete as, as = nullptr;
						return r;
					}
					return std::num_get<char_t>::do_get(it, end, ios, err, v);
				}
				virtual iter_type do_get(iter_type it, iter_type end, std::ios_base &ios, std::ios_base::iostate &err, float &v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						long double temp;
						iter_type r = as->Get(it, temp);
						v = static_cast<std::remove_reference<decltype(v)>::type>(temp);
						delete as, as = nullptr;
						return r;
					}
					return std::num_get<char_t>::do_get(it, end, ios, err, v);
				}
				virtual iter_type do_get(iter_type it, iter_type end, std::ios_base &ios, std::ios_base::iostate &err, double &v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						long double temp;
						iter_type r = as->Get(it, temp);
						v = static_cast<std::remove_reference<decltype(v)>::type>(temp);
						delete as, as = nullptr;
						return r;
					}
					return std::num_get<char_t>::do_get(it, end, ios, err, v);
				}
				virtual iter_type do_get(iter_type it, iter_type end, std::ios_base &ios, std::ios_base::iostate &err, long double &v) const override
				{
					AsBase *&as = *reinterpret_cast<AsBase **>(&ios.pword(geti()));
					if(as != nullptr)
					{
						iter_type r = as->Get(it, v);
						delete as, as = nullptr;
						return r;
					}
					return std::num_get<char_t>::do_get(it, end, ios, err, v);
				}
			};
		}

		template<typename T, Endianness E = Endianness::Little, typename = void>
		auto As(std::basic_ostream<char_t> &os)
		-> typename std::enable_if
			<
				std::is_integral<T>::value
			 || std::is_floating_point<T>::value,
				std::basic_ostream<char_t> &
			>::type
		{
			auto &imbued = os.iword(impl::geti());
			if(imbued != 1)
			{
				os.imbue(std::locale(std::locale(), new impl::NumPut));
				imbued = 1;
			}
			impl::AsBase *&as = *reinterpret_cast<impl::AsBase **>(&os.pword(impl::geti()));
			if(as != nullptr)
			{
				delete as, as = nullptr;
			}
			as = new impl::AsT<T, E>;
			return os;
		}

		template<typename T, Endianness E = Endianness::Little, typename = void>
		auto As(std::basic_istream<char_t> &is)
		-> typename std::enable_if
			<
				std::is_integral<T>::value
			 || std::is_floating_point<T>::value,
				std::basic_istream<char_t> &
			>::type
		{
			auto &imbued = is.iword(impl::geti());
			if(imbued != 1)
			{
				is.imbue(std::locale(std::locale(), new impl::NumGet));
				imbued = 1;
			}
			impl::AsBase *&as = *reinterpret_cast<impl::AsBase **>(&is.pword(impl::geti()));
			if(as != nullptr)
			{
				delete as, as = nullptr;
			}
			as = new impl::AsT<T, E>;
			return is;
		}

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
				Out(void *, ED *&);
				Out(Out const &) = delete;
				Out(Out &&) = default;
				Out &operator=(Out const &) = delete;
				Out &operator=(Out &&) = default;
				virtual ~Out();

			private:
				struct Buf;
				std::unique_ptr<Buf> b;
				Out(Buf *, void *, ED *&);
			};
		}
		namespace ParamData
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
