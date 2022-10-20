#include "ScriptProxy.h"


CScriptProxy::CScriptProxy(const char* script_filename) : script_filename_{ script_filename }
{
    lua_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
}

void CScriptProxy::ProcessScript()
{
    lua_.script_file(script_filename_);
}