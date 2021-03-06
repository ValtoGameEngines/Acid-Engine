#include "Joysticks.hpp"

#include <GLFW/glfw3.h>

namespace acid {
void CallbackJoystick(int32_t id, int32_t event) {
	if (event == GLFW_CONNECTED) {
		Log::Out("Joystick connected: '", glfwGetJoystickName(id), "' to ", id, '\n');
		Joysticks::JoystickImpl joystick = {};
		joystick.name = glfwGetJoystickName(id);
		Joysticks::Get()->connected.emplace(id, joystick);
		Joysticks::Get()->onConnect(id, true);
	} else if (event == GLFW_DISCONNECTED) {
		Log::Out("Joystick disconnected from ", id, '\n');
		Joysticks::Get()->connected.erase(id);
		Joysticks::Get()->onConnect(id, false);
	}
}

Joysticks::Joysticks() {
	glfwSetJoystickCallback(CallbackJoystick);

	for (uint32_t i = 0; i < GLFW_JOYSTICK_LAST; i++) {
		if (glfwJoystickPresent(i)) {
			JoystickImpl joystick = {};
			joystick.name = glfwGetJoystickName(i);
			connected.emplace(i, joystick);
			onConnect(i, true);
		}
	}
}

void Joysticks::Update() {
	for (auto &[port, joystick] : connected) {
		int32_t axeCount = 0;
		auto axes = glfwGetJoystickAxes(port, &axeCount);
		joystick.axes.resize(static_cast<std::size_t>(axeCount));

		for (uint32_t i = 0; i < static_cast<uint32_t>(axeCount); i++) {
			if (joystick.axes[i] != axes[i]) {
				joystick.axes[i] = axes[i];
				onAxis(port, i, joystick.axes[i]);
			}
		}

		int32_t buttonCount = 0;
		auto buttons = glfwGetJoystickButtons(port, &buttonCount);
		joystick.buttons.resize(static_cast<std::size_t>(buttonCount));

		for (uint32_t i = 0; i < static_cast<uint32_t>(buttonCount); i++) {
			if (buttons[i] != GLFW_RELEASE && joystick.buttons[i] != InputAction::Release) {
				joystick.buttons[i] = InputAction::Repeat;
			} else if (joystick.buttons[i] != static_cast<InputAction>(buttons[i])) {
				joystick.buttons[i] = static_cast<InputAction>(buttons[i]);
				onButton(port, i, joystick.buttons[i]);
			}
		}

		int32_t hatCount = 0;
		auto hats = glfwGetJoystickHats(port, &hatCount);
		joystick.hats.resize(static_cast<std::size_t>(hatCount));

		for (uint32_t i = 0; i < static_cast<uint32_t>(hatCount); i++) {
			if (joystick.hats[i] != MakeBitMask<JoystickHatValue>(hats[i])) {
				joystick.hats[i] = MakeBitMask<JoystickHatValue>(hats[i]);
				onHat(port, i, joystick.hats[i]);
			}
		}
	}
}

bool Joysticks::IsConnected(JoystickPort port) const {
	return GetJoystick(port).has_value();
}

std::string Joysticks::GetName(JoystickPort port) const {
	auto joystick = GetJoystick(port);
	return joystick ? joystick->name : "";
}

std::size_t Joysticks::GetAxisCount(JoystickPort port) const {
	auto joystick = GetJoystick(port);
	return joystick ? static_cast<std::size_t>(joystick->axes.size()) : 0;
}

std::size_t Joysticks::GetButtonCount(JoystickPort port) const {
	auto joystick = GetJoystick(port);
	return joystick ? static_cast<std::size_t>(joystick->buttons.size()) : 0;
}

std::size_t Joysticks::GetHatCount(JoystickPort port) const {
	auto joystick = GetJoystick(port);
	return joystick ? static_cast<std::size_t>(joystick->hats.size()) : 0;
}

float Joysticks::GetAxis(JoystickPort port, JoystickAxis axis) const {
	auto joystick = GetJoystick(port);

	if (!joystick || axis > joystick->axes.size()) {
		return 0.0f;
	}

	return joystick->axes[axis];
}

InputAction Joysticks::GetButton(JoystickPort port, JoystickButton button) const {
	auto joystick = GetJoystick(port);

	if (!joystick || button > joystick->buttons.size()) {
		return InputAction::Release;
	}

	return joystick->buttons[button];
}

BitMask<JoystickHatValue> Joysticks::GetHat(JoystickPort port, JoystickHat hat) const {
	auto joystick = GetJoystick(port);

	if (!joystick || hat > joystick->hats.size()) {
		return JoystickHatValue::Centered;
	}

	return joystick->hats[hat];
}

std::optional<Joysticks::JoystickImpl> Joysticks::GetJoystick(JoystickPort port) const {
	if (auto it = connected.find(port); it != connected.end())
		return it->second;
	return std::nullopt;
}
}
