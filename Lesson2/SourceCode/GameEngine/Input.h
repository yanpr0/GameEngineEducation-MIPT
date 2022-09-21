#pragma once

#include <Windows.h>

#include <string_view>
#include <unordered_map>

enum class Action
{
	GoLeft,
	GoRight,
	GoUp,
	GoDown
};
	
class InputHandler
{
public:
	InputHandler(std::string_view config_file);

	void LoadMappingsFromConfig(std::string_view config_file);

	bool HasHappened(Action act);

	void Tick();
private:
	using VK = decltype(VK_RETURN);

	inline static const std::unordered_map<std::string_view, Action> str2act =
	{
		{"GoLeft", Action::GoLeft},
		{"GoRight", Action::GoRight},
		{"GoUp", Action::GoUp},
		{"GoDown", Action::GoDown}
	};

	inline static const std::unordered_map<std::string_view, VK> str2vk =
	{
		{"", VK{}},
		{"a", 'A'},
		{"d", 'D'},
		{"w", 'W'},
		{"s", 'S'}
		// ...
	};

	std::unordered_map<Action, VK> act2vk_;
	std::unordered_map<VK, bool> was_key_pressed_;
};
