#include "UIXCanvasRootControl.h"

UIXCanvasRootControl::UIXCanvasRootControl(const SpawnParams& params)
    : Script(params)
{
    // Enable ticking OnUpdate function
    _tickUpdate = true;
}

void UIXCanvasRootControl::OnEnable()
{
    // Here you can add code that needs to be called when script is enabled (eg. register for events)
}

void UIXCanvasRootControl::OnDisable()
{
    // Here you can add code that needs to be called when script is disabled (eg. unregister from events)
}

void UIXCanvasRootControl::OnUpdate()
{
    // Here you can add code that needs to be called every frame
}
