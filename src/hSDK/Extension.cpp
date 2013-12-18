#include "hSDK/Extension.hpp"

#include "RunData.hpp"

namespace hSDK
{
	void Extension::CallMovement(std::uint32_t action, std::int32_t param)
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_CALLMOVEMENT,
			static_cast<WPARAM>(action),
			static_cast<LPARAM>(param)
		);
	}
	void Extension::Pause()
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_PAUSE,
			0,
			0
		);
	}
	void Extension::Continue()
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_CONTINUE,
			0,
			0
		);
	}
	void Extension::Destroy()
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_DESTROY,
			0,
			0
		);
	}
	bool Extension::DebuggerEditDialog(string const &title, std::int32_t &i)
	{
#ifndef RUN_ONLY
		string temp = title;
		EditDebugInfo edi = {0};
		edi.pTitle = temp.c_str();
		edi.value = i;
		if(callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_EDITINT,
			0,
			reinterpret_cast<LPARAM>(&edi)
		) == IDOK)
		{
			i = edi.value;
			return true;
		}
#endif
		return false;
	}
	bool Extension::DebuggerEditDialog(string const &title, string &s, std::uint32_t maxlen)
	{
#ifndef RUN_ONLY
		string temp = title;
		EditDebugInfo edi = {0};
		edi.pTitle = temp.c_str();
		std::unique_ptr<string::value_type[]> buf {new string::value_type[maxlen]{0}};
		std::copy(std::begin(s), std::end(s), buf.get());
		edi.pText = buf.get();
		edi.lText = maxlen;
		if(callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_EDITINT,
			0,
			reinterpret_cast<LPARAM>(&edi)
		) == IDOK)
		{
			s = buf.get();
			return true;
		}
#endif
		return false;
	}
	void Extension::ExecProgram(string const &path, string const &args, bool hide, bool wait)
	{
		prgParam pp = {0, {0}, {0}};
		if(hide) pp.prgFlags |= PRGFLAGS_HIDE;
		if(wait) pp.prgFlags |= PRGFLAGS_WAIT;
		std::copy(std::begin(path), std::begin(path)+std::min(std::extent<decltype(pp.prgPath   )>::value, path.size()+1), &(pp.prgPath[0]));
		std::copy(std::begin(args), std::begin(args)+std::min(std::extent<decltype(pp.prgCommand)>::value, args.size()+1), &(pp.prgCommand[0]));
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_EXECPROGRAM,
			0,
			reinterpret_cast<LPARAM>(&pp)
		);
	}
	void Extension::TriggerCondition(ACE_ID_t conditionID)
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_GENERATEEVENT,
			static_cast<WPARAM>(conditionID),
			0 //?
		);
	}
	void Extension::QueueCondition(ACE_ID_t conditionID)
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_PUSHEVENT,
			static_cast<WPARAM>(conditionID),
			0 //?
		);
	}
	void Extension::UnqueueCondition(ACE_ID_t conditionID)
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_PUSHEVENTSTOP,
			static_cast<WPARAM>(conditionID),
			0
		);
	}
	string Extension::CurrentDrive()
	{
		string::value_type buf[_MAX_PATH];
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_GETFILEINFOS,
			FILEINFO_DRIVE,
			reinterpret_cast<LPARAM>(&(buf[0]))
		);
		return buf;
	}
	string Extension::CurrentDirectory()
	{
		string::value_type buf[_MAX_PATH];
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_GETFILEINFOS,
			FILEINFO_DIR,
			reinterpret_cast<LPARAM>(&(buf[0]))
		);
		return buf;
	}
	string Extension::CurrentPath()
	{
		string::value_type buf[_MAX_PATH];
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_GETFILEINFOS,
			FILEINFO_PATH,
			reinterpret_cast<LPARAM>(&(buf[0]))
		);
		return buf;
	}
	string Extension::CurrentAppname()
	{
		string::value_type buf[_MAX_PATH];
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_GETFILEINFOS,
			FILEINFO_APPNAME,
			reinterpret_cast<LPARAM>(&(buf[0]))
		);
		return buf;
	}
	void Extension::RedrawEntireFrame()
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_REDISPLAY,
			0,
			0
		);
	}
	void Extension::QueueRedraw()
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_REDRAW,
			0,
			0
		);
	}
	void Extension::QueueTick()
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_REHANDLE,
			0,
			0
		);
	}
	void Extension::SetPosition(std::int32_t x, std::int32_t y)
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_SETPOSITION,
			static_cast<WPARAM>(x),
			static_cast<LPARAM>(y)
		);
	}/*
	void Extension::SubclassWindow(std::uint32_t num, std::int32_t hook_routine)
	{
		callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_SUBCLASSWINDOW,
			static_cast<WPARAM>(num),
			static_cast<LPARAM>(hook_routine)
		);
	}*//*
	void *Extension::GetAddress(std::uint32_t wp, std::int32_t lp)
	{
		return reinterpret_cast<void *>(callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_GETOBJECTADDRESS,
			static_cast<WPARAM>(wp),
			static_cast<LPARAM>(lp)
		));
	}*/
	//RFUNCTION_GETCALLTABLES

	auto Extension::CopyString(string const &s) -> string::const_pointer
	{
		auto p = reinterpret_cast<string::pointer>(callRunTimeFunction
		(
			(&RuntimeInfo::Rd()),
			RFUNCTION_GETSTRINGSPACE_EX,
			0,
			((s.size()+1)*sizeof(string::value_type))
		));
		std::copy(s.c_str(), s.c_str()+s.size()+1, p);
		return p;
	}

	std::int32_t Extension::exp_lparam = 0;

	using ExprT = ExpressionType;

	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::None   >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::None   >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::None   >::GetFirst(){ return CNC_GetFirstExpressionParameter((&RuntimeInfo::Rd()), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::Integer>::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::Integer>::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::Integer>::GetFirst(){ return CNC_GetFirstExpressionParameter((&RuntimeInfo::Rd()), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::Float  >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::Float  >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::Float  >::GetFirst(){ return CNC_GetFirstExpressionParameter((&RuntimeInfo::Rd()), exp_lparam, TYPE_FLOAT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::String >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::String >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::String >::GetFirst(){ return CNC_GetFirstExpressionParameter((&RuntimeInfo::Rd()), exp_lparam, TYPE_STRING); }

	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::None   >::GetNext(){ return CNC_GetParameter((&RuntimeInfo::Rd())); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::None   >::GetNext(){ return CNC_GetParameter((&RuntimeInfo::Rd())); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::None   >::GetNext(){ return CNC_GetNextExpressionParameter((&RuntimeInfo::Rd()), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::Integer>::GetNext(){ return CNC_GetIntParameter((&RuntimeInfo::Rd())); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::Integer>::GetNext(){ return CNC_GetIntParameter((&RuntimeInfo::Rd())); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::Integer>::GetNext(){ return CNC_GetNextExpressionParameter((&RuntimeInfo::Rd()), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::Float  >::GetNext(){ return CNC_GetFloatParameter((&RuntimeInfo::Rd())); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::Float  >::GetNext(){ return CNC_GetFloatParameter((&RuntimeInfo::Rd())); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::Float  >::GetNext(){ return CNC_GetNextExpressionParameter((&RuntimeInfo::Rd()), exp_lparam, TYPE_FLOAT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::String >::GetNext(){ return CNC_GetStringParameter((&RuntimeInfo::Rd())); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::String >::GetNext(){ return CNC_GetStringParameter((&RuntimeInfo::Rd())); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::String >::GetNext(){ return CNC_GetNextExpressionParameter((&RuntimeInfo::Rd()), exp_lparam, TYPE_STRING); }
}
