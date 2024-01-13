#include "UIXInputEvent.h"
#include "Engine/Input/Input.h"


UIXInputEvent::UIXInputEvent() : ScriptingObject(SpawnParams(Guid::New(), TypeInitializer)), bound(false)
{
    Input::ActionTriggered.Bind<UIXInputEvent, &UIXInputEvent::Handler>(this);
    bound = true;
}


UIXInputEvent::UIXInputEvent(String name) : ScriptingObject(SpawnParams(Guid::New(), TypeInitializer)), Name(name), bound(false)
{
    Input::ActionTriggered.Bind<UIXInputEvent, &UIXInputEvent::Handler>(this);
    bound = true;
}

UIXInputEvent::~UIXInputEvent()
{
    if (bound)
        Input::ActionTriggered.Unbind<UIXInputEvent, &UIXInputEvent::Handler>(this);
    bound = false;
}


bool UIXInputEvent::Active()
{
    return Input::GetAction(Name);
}

InputActionState UIXInputEvent::GetState() const
{
    return Input::GetActionState(Name);
}

void UIXInputEvent::Dispose()
{
    if (bound)
        Input::ActionTriggered.Unbind<UIXInputEvent, &UIXInputEvent::Handler>(this);
    bound = false;
    DeleteObject();
    //GC.SuppressFinalize(this);
}

void UIXInputEvent::Handler(StringView name, InputActionState state)
{
    if (!StringUtils::CompareIgnoreCase(*name, *Name))
        return;
    switch (state)
    {
        case InputActionState::None: break;
        case InputActionState::Waiting: break;
        case InputActionState::Pressing:
        {
            if (Pressing.IsBinded())
                Pressing();
            break;
        }
        case InputActionState::Press:
        {
            if (Pressed.IsBinded())
                Pressed();
            break;
        }
        case InputActionState::Release:
        {
            if (Released.IsBinded())
                Released();
            break;
        }
        default: break;
    }
}

