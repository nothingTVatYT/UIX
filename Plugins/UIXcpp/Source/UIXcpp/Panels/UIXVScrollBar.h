#pragma once

#include "UIXScrollBar.h"
#include "../UIXEnums.h"


class UIXContainerControl;

/// <summary>
/// Vertical scroll bar control.
/// </summary>
/// <seealso cref="FlaxEngine.GUI.ScrollBar" />
API_CLASS(NoSpawn, Attributes = "HideInEditor")
class UIXCPP_API UIXVScrollBar : public UIXScrollBar
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIXVScrollBar)
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="VScrollBar"/> class.
    /// </summary>
    /// <param name="parent">The parent control.</param>
    /// <param name="x">The x position.</param>
    /// <param name="height">The height.</param>
    /// <param name="width">The width.</param>
    UIXVScrollBar(/*const SpawnParams &params, */UIXContainerControl *parent, float x, float height, float width = UIXScrollBar::DefaultSize);

protected:
    /// <inheritdoc />
    FORCE_INLINE float GetTrackSize() const override { return GetHeight(); }
};
