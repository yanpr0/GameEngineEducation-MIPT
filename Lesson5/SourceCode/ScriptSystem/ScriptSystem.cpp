#include "ScriptSystem.h"
#include "ScriptProxy.h"


CScriptSystem::CScriptSystem()
{
    return;
}

CScriptSystem::~CScriptSystem()
{
    for (auto proxy : script_proxies_)
    {
        delete proxy;
    }
}

void CScriptSystem::Update()
{
    for (auto proxy : script_proxies_)
    {
        proxy->ProcessScript();
    }
}

IScriptProxy* CScriptSystem::CreateProxy(const char* filename)
{
    IScriptProxy* proxy = new CScriptProxy(filename);
    script_proxies_.push_back(proxy);
    return proxy;
}

void CScriptSystem::ProcessScript(IScriptProxy* scriptProxy)
{
    scriptProxy->ProcessScript();
}
