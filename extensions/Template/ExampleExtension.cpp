#include "hSDK/Extension.hpp"

#include <map>
#include <typeinfo>
#include <typeindex>
#include <memory>

struct ExampleExtension : hSDK::Extension
{
	struct VarHolder
	{
		template<typename T>
		T &get()
		{
			return dynamic_cast<TVarHolder<T> &>(*this).v;
		}
	};
	template<typename T>
	struct TVarHolder : VarHolder
	{
		T v = T();
	};
	std::map<std::type_index, std::unique_ptr<VarHolder>> vars;
	T &get()
	{
		if(vars.find(typeid(T)) == vars.end())
		{
			vars.emplace(typeid(T), std::unique_ptr<VarHolder>(new TVarHolder<T>));
		}
		return vars.at(typeid(T));
	}

	//example actions
	template<typename T>
	void SetVar(T t)
	{
		get<T>() = t;
	}

	//example conditions
	template<typename T>
	bool VarsEqual(T t)
	{
		return get<T>() == t;
	}

	//example expressions
	template<typename T>
	T GetVar()
	{
		return get<T>();
	}

	//constructor
	ExampleExtension()
	: hSDK::Extension(
	{ //register actions
		{0, &SetVar<short>},
		{1, &SetVar<int>},
		{2, &SetVar<long long>},
		{3, &SetVar<float>},
		{4, &SetVar<double>},
		{5, &SetVar<long double>},
		//hSDK::Extension provides the 'string' type, don't use std::string
		{6, &SetVar<string>} //changes depending on unicode/non-unicode
	},
	{ //register conditions
		{0, &VarsEqual<short>},
		{1, &VarsEqual<int>},
		{2, &VarsEqual<long long>},
		{3, &VarsEqual<float>},
		{4, &VarsEqual<double>},
		{5, &VarsEqual<long double>},
		{6, &VarsEqual<string>}
	},
	{ //register expressions
		{0, &GetVar<short>},
		{1, &GetVar<int>},
		{2, &GetVar<long long>},
		{3, &GetVar<float>},
		{4, &GetVar<double>},
		{5, &GetVar<long double>},
		{6, &GetVar<string>}
	})
//	, other(meh)
//	, vars(blah)
	{
		//perform any initialization
	}
};
