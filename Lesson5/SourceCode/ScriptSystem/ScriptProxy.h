#pragma once

#include <sol/sol.hpp>
#include <string>
#include <utility>

#include "IScriptProxy.h"


class SCRIPTSYSTEM_API CScriptProxy final : public IScriptProxy
{
public:
	CScriptProxy(const char* script_filename);

	virtual void ProcessScript() override;

	template <class T, class... MemberDescrPart>
	void AddType(const char* class_name, MemberDescrPart... descr)
	{
		lua_.new_usertype<T>(class_name, descr...);
	}

	template <class T>
	void AddVar(const char* var_name, T var)
	{
		lua_[var_name] = var;
	}

	template <class F>
	void AddFunc(const char* var_name, F func)
	{
		lua_.set_function(var_name, func);
	}

private:
	sol::state lua_;
	std::string script_filename_;
};

