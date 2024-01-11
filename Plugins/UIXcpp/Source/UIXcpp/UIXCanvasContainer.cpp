#include "UIXCanvasContainer.h"

UIXCanvasContainer::UIXCanvasContainer(const SpawnParams& params)
    : Script(params)
{
    // Enable ticking OnUpdate function
    _tickUpdate = true;
}

void UIXCanvasContainer::OnEnable()
{
    // Here you can add code that needs to be called when script is enabled (eg. register for events)
}

void UIXCanvasContainer::OnDisable()
{
    // Here you can add code that needs to be called when script is disabled (eg. unregister from events)
}

void UIXCanvasContainer::OnUpdate()
{
    // Here you can add code that needs to be called every frame
}
