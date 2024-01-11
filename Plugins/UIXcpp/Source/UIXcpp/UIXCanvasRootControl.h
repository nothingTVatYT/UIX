#pragma once

#include "Engine/Scripting/Script.h"

API_CLASS() class UIXCPP_API UIXCanvasRootControl : public Script
{
API_AUTO_SERIALIZATION();
DECLARE_SCRIPTING_TYPE(UIXCanvasRootControl);

    // [Script]
    void OnEnable() override;
    void OnDisable() override;
    void OnUpdate() override;
};
