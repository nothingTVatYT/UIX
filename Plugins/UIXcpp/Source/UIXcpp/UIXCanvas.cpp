#include "UIXCanvas.h"

UIXCanvas::UIXCanvas(const SpawnParams& params)
    : Script(params)
{
    // Enable ticking OnUpdate function
    _tickUpdate = true;
}

void UIXCanvas::OnEnable()
{
    // Here you can add code that needs to be called when script is enabled (eg. register for events)
}

void UIXCanvas::OnDisable()
{
    // Here you can add code that needs to be called when script is disabled (eg. unregister from events)
}

void UIXCanvas::OnUpdate()
{
    // Here you can add code that needs to be called every frame
}
