#ifndef hSDK_Template_TestExtension_HeaderPlusPlus
#define hSDK_Template_TestExtension_HeaderPlusPlus
#include "hSDK/Extension.hpp"

#include "TestProperties.hpp"

#include <map>
#include <typeinfo>
#include <typeindex>
#include <memory>

struct TestExtension : hSDK::Extension
{
	struct VarHolder
	{
		template<typename T>
		T &get()
		{
			return dynamic_cast<TVarHolder<T> &>(*this).v;
		}
		virtual ~VarHolder() = default;
	};
	template<typename T>
	struct TVarHolder : VarHolder
	{
		T v;
		TVarHolder()
		: v(T()) 
		{
		}
	};
	std::map<std::type_index, std::unique_ptr<VarHolder>> vars;
	template<typename T>
	T &get()
	{
		if(vars.find(typeid(T)) == vars.end())
		{
			vars.emplace(typeid(T), std::unique_ptr<VarHolder>(new TVarHolder<T>));
		}
		return vars.at(typeid(T)).get()->get<T>();
	}

	//Actions
	template<typename T>
	void SetVar(T t)
	{
		get<T>() = t;
	}

	//Conditions
	template<typename T>
	bool VarsEqual(T t)
	{
		return get<T>() == t;
	}

	//Expressions
	template<typename T>
	T GetVar()
	{
		return get<T>();
	}

	//Constructors
	TestExtension(TestProperties const &props)
	: hSDK::Extension(props)
	{
		using E = TestExtension;
		registerActions(
		{
			{0, &E::SetVar<short>},
			{1, &E::SetVar<int>},
			{2, &E::SetVar<long long>},
			{3, &E::SetVar<float>},
			{4, &E::SetVar<double>},
			{5, &E::SetVar<long double>},
			{6, &E::SetVar<string>},
			{7, null_returning_void} //do nothing for this action
		});
		registerConditions(
		{
			{0, &E::VarsEqual<short>},
			{1, &E::VarsEqual<int>},
			{2, &E::VarsEqual<long long>},
			{3, &E::VarsEqual<float>},
			{4, &E::VarsEqual<double>},
			{5, &E::VarsEqual<long double>},
			{6, &E::VarsEqual<string>},
			{7, null_returning_bool} //condition always returns false
		});
		registerExpressions(
		{
			{0, &E::GetVar<short>},
			{1, &E::GetVar<int>},
			{2, &E::GetVar<long long>},
			{3, &E::GetVar<float>},
			{4, &E::GetVar<double>},
			{5, &E::GetVar<long double>},
			{6, &E::GetVar<string>},
			{7, null_returning_string} //expression always returns empty string
		});
	}
};

#endif