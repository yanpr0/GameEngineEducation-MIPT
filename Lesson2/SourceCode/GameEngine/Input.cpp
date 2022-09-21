#include "Input.h"
#include "INIReader.h"

#include <iostream>

InputHandler::InputHandler(std::string_view config_file)
{
	LoadMappingsFromConfig(config_file);
}

void InputHandler::LoadMappingsFromConfig(std::string_view config_file)
{
	INIReader reader(std::string{ config_file });

	if (reader.ParseError() < 0)
	{
        	std::cerr << "Can't load '" << config_file << "'\n";
        	return;
    	}

	for (const auto& f : reader.GetFields("Keyboard"))
	{
		auto act = str2act.at(f);
		auto vk = str2vk.at(reader.Get("Keyboard", f, ""));
		act2vk_[act] = vk;
	}
}

bool InputHandler::HasHappened(Action act)
{
	return was_key_pressed_[act2vk_[act]];
}

void InputHandler::Tick()
{
	for (auto [act, vk] : act2vk_)
	{
		const auto event_mask = 0x01 | (1 << (8 * sizeof(SHORT) - 1));
		was_key_pressed_[vk] = (GetAsyncKeyState(vk) & event_mask) != 0;
	}
}
