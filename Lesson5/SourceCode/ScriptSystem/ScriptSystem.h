#pragma once

#include "IScriptSystem.h"

#include <vector>


class SCRIPTSYSTEM_API CScriptSystem final : public IScriptSystem {
public:
	CScriptSystem();

	virtual ~CScriptSystem();

	virtual void Update() override;
	virtual IScriptProxy* CreateProxy(const char* filename) override;
	virtual void ProcessScript(IScriptProxy* scriptProxy) override;

private:
	std::vector<IScriptProxy*> script_proxies_;
};