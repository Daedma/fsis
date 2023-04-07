#pragma once
#include <EASTL/functional.h>
#include <EASTL/hash_map.h>
#include <EASTL/string_view.h>
#include <EASTL/string.h>

class Character;
struct ActionBinding;

class Controller
{
public:
	template<typename T>
	using InputAxisHandlerSignature = void(T::*)(float);

	template<typename T>
	using InputActionHandlerSignature = void(T::*)(void);

	using  ActionIDType = uint64_t;

	enum class InputEvent: uint8_t
	{
		PRESSED, RELEASED
	};

	struct ActionBinding
	{
		eastl::function<void(void)> released = nullptr;

		eastl::function<void(void)> pressed = nullptr;

		inline void resetAction(InputEvent event);

		inline void resetAction();

		inline void setAction(eastl::function<void(void)> action, InputEvent event);

		template<typename T>
		inline void setAction(T* instigator, InputActionHandlerSignature<T> action, InputEvent event);

		inline void execute(InputEvent event);

		inline void operator()(InputEvent event);
	};

	struct AxisBinding
	{
		eastl::function<void(float)> action;

		template<typename T>
		inline void setAction(T* instigator, InputAxisHandlerSignature<T> action);

		inline void setAction(eastl::function<void(float)> action);

		inline void resetAction();

		inline void execute(float val);

		inline void operator()(float val);
	};

	Controller() = default;

	Controller(Character* character);

	void possess(Character* character);

	template<typename UserObject>
	inline void bindAxis(ActionIDType axisID, UserObject* executor, InputAxisHandlerSignature<UserObject> func);

	template<typename UserObject>
	inline void bindAction(ActionIDType actionID, InputEvent keyEvent, UserObject* executor, InputActionHandlerSignature<UserObject> func);

	void execute(ActionIDType actionID, InputEvent keyEvent);

	void execute(ActionIDType actionID, float value);

	Character* getMarionette() const { return m_marionette; }

	// Note: unbindAxis and unbindAction on request, f u ;)

private:
	Character* m_marionette = nullptr;

	eastl::hash_map<ActionIDType, AxisBinding> m_axisMapping;

	eastl::hash_map<ActionIDType, ActionBinding> m_actionMapping;

};

void Controller::ActionBinding::resetAction(Controller::InputEvent event)
{
	switch (event)
	{
	case InputEvent::PRESSED:
		pressed = nullptr;
		break;
	case InputEvent::RELEASED:
		released = nullptr;
		break;
	default:
		break;
	}
}

void Controller::ActionBinding::resetAction()
{
	pressed = released = nullptr;
}

void Controller::ActionBinding::setAction(eastl::function<void(void)> action, InputEvent event)
{
	switch (event)
	{
	case InputEvent::PRESSED:
		pressed = action;
		break;
	case InputEvent::RELEASED:
		released = action;
		break;
	default:
		break;
	}
}

template<typename T>
void Controller::ActionBinding::setAction(T* instigator, InputActionHandlerSignature<T> action, InputEvent event)
{
	switch (event)
	{
	case InputEvent::PRESSED:
		pressed = [instigator, action]() {instigator->*action();};
		break;
	case InputEvent::RELEASED:
		released = [instigator, action]() {instigator->*action();};
		break;
	default:
		break;
	}
}

void Controller::ActionBinding::execute(InputEvent event)
{
	switch (event)
	{
	case InputEvent::PRESSED:
		pressed();
		break;
	case InputEvent::RELEASED:
		released();
		break;
	default:
		break;
	}
}

void Controller::ActionBinding::operator()(InputEvent event)
{
	execute(event);
}

template<typename T>
void Controller::AxisBinding::setAction(T* instigator, InputAxisHandlerSignature<T> action)
{
	this->action = [instigator, action](float val) {instigator->*action(val);};
}

void Controller::AxisBinding::setAction(eastl::function<void(float)> action)
{
	this->action = action;
}

void Controller::AxisBinding::resetAction()
{
	action = nullptr;
}


void Controller::AxisBinding::execute(float val)
{
	action(val);
}

void Controller::AxisBinding::operator()(float val)
{
	action(val);
}

template<typename UserObject>
void Controller::bindAxis(ActionIDType axisID, UserObject* executor, InputAxisHandlerSignature<UserObject> func)
{
	m_axisMapping[axisID].setAction(executor, func);
}

template<typename UserObject>
void Controller::bindAction(ActionIDType actionID, InputEvent keyEvent, UserObject* executor, InputActionHandlerSignature<UserObject> func)
{
	m_actionMapping[actionID].setAction(executor, func);
}