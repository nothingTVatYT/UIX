#pragma once

#include "Engine/Scripting/Script.h"

API_CLASS() class UIXCPP_API UIXCanvasContainer : public Script
{
API_AUTO_SERIALIZATION();
DECLARE_SCRIPTING_TYPE(UIXCanvasContainer);

    // [Script]
    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
};
