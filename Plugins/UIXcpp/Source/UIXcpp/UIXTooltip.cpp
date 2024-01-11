﻿#include "UIXTooltip.h"

UIXTooltip::UIXTooltip(const SpawnParams& params)
    : Script(params)
{
    // Enable ticking OnUpdate function
    _tickUpdate = true;
}

void UIXTooltip::OnEnable()
{
    // Here you can add code that needs to be called when script is enabled (eg. register for events)
}

void UIXTooltip::OnDisable()
{
    // Here you can add code that needs to be called when script is disabled (eg. unregister from events)
}

void UIXTooltip::OnUpdate()
{
    // Here you can add code that needs to be called every frame
}