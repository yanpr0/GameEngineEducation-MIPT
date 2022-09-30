#include "InputHandler.h"

#include <windows.h>
#include <algorithm>
#include <cassert>

InputHandler::InputHandler()
{
	m_strMapFilePath = "../../../Assets/Configs/actionmap.ini";
	std::replace(m_strMapFilePath.begin(), m_strMapFilePath.end(), '\\', '/');

	MapSymbol("a", 'A');
	MapSymbol("d", 'D');
	MapSymbol("left", VK_LEFT);
	MapSymbol("right", VK_RIGHT);
	MapSymbol("space", VK_SPACE);

	MapCommandSymbol("GoLeft", eIC_GoLeft, "a");
	MapCommandSymbol("GoRight", eIC_GoRight, "d");
	MapCommandSymbol("Jump", eIC_Jump, "space");

	LoadConfiguration();

	Remap();
}

bool InputHandler::IsKeyDown(size_t vk_key)
{
	if (GetAsyncKeyState(vk_key) & 0x8000)
		return true;

	return false;
}

void InputHandler::MapSymbol(std::string strSymbol, size_t nSymbol)
{
	m_symbolMap[strSymbol] = nSymbol;
}

void InputHandler::MapInputEvent(std::size_t nSymbol, size_t nCommand)
{
	m_inputEventMap[nSymbol] = nCommand;
}

void InputHandler::MapCommandSymbol(std::string strCommand, size_t nCommand, std::string strDefaultSymbol)
{
	m_commandMap[strCommand] = nCommand;
	m_commandSymbolMap[strCommand] = strDefaultSymbol;
}

void InputHandler::Remap()
{
	for (auto& it : m_commandSymbolMap)
	{
		MapInputEvent(m_symbolMap[it.second], m_commandMap[it.first]);
	}
}

// We used int as return type just for demonstration. It should be done another way
void InputHandler::Update()
{
	for (auto& it : m_inputEventMap)
	{
		m_InputState.set(it.second, IsKeyDown(it.first));
	}
}

const std::bitset<eIC_Max>& InputHandler::GetInputState() const
{
	return m_InputState;
}

void InputHandler::LoadConfiguration()
{
	INIReader reader(m_strMapFilePath.c_str());
	assert(reader.ParseError() >= 0);
	auto fields = reader.GetFields("Keyboard");

	for (auto& field : fields)
	{
		std::string strCommand = field;
		std::string strSymbol = reader.Get("Keyboard", field, "");

		m_commandSymbolMap[strCommand] = strSymbol;
	}
}