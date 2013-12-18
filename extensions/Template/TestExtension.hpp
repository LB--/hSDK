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

	template<typename T>
	void SetVar(T t)
	{
		get<T>() = t;
	}

	template<typename T>
	bool VarsEqual(T t)
	{
		return get<T>() == t;
	}

	template<typename T>
	T GetVar()
	{
		return get<T>();
	}

	void A0(){}
	void A2(int, int){}
	void A3(int, int, int){}
	void A4(int, int, int, int){}
	bool C0(){ return true; }
	bool C2(int, int){ return true; }
	bool C3(int, int, int){ return true; }
	bool C4(int, int, int, int){ return true; }
	int E1(int){ return 0; }
	int E2(int, int){ return 0; }
	int E3(int, int, int){ return 0; }
	int E4(int, int, int, int){ return 0; }

	bool OnTrigger(bool r)
	{
		return r;
	}
	bool OnQueue()
	{
		return true;
	}
	void Trigger()
	{
		TriggerCondition(11);
		TriggerCondition(13);
	}
	void Queue()
	{
		QueueCondition(12);
		QueueCondition(14);
	}
	void Unqueue()
	{
		UnqueueCondition(12);
		UnqueueCondition(14);
	}

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
			{7, &E::A0},
			{8, &E::A2},
			{9, &E::A3},
			{10,&E::A4},
			{11,&E::Trigger},
			{12,&E::Queue},
			{13,&E::Unqueue}
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
			{7, &E::C0},
			{8, &E::C2},
			{9, &E::C3},
			{10,&E::C4},
			{11,{&E::OnTrigger, true}},
			{12,{&E::OnQueue, true}},
			{13,&E::OnTrigger},
			{14,&E::OnQueue}
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
			{7, &E::E1},
			{8, &E::E2},
			{9, &E::E3},
			{10,&E::E4}
		});
	}
};

#endif
