#pragma once

#include "UIXScrollBar.h"
#include "../UIXEnums.h"


class UIXContainerControl;

/// <summary>
/// Horizontal scroll bar control.
/// </summary>
/// <seealso cref="FlaxEngine.GUI.ScrollBar" />
API_CLASS(NoSpawn, Attributes = "HideInEditor")
class UIXCPP_API UIXHScrollBar : public UIXScrollBar
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIXHScrollBar)
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="HScrollBar"/> class.
    /// </summary>
    /// <param name="parent">The parent control.</param>
    /// <param name="y">The y position.</param>
    /// <param name="width">The width.</param>
    /// <param name="height">The height.</param>
    UIXHScrollBar(/*const SpawnParams &params,*/ UIXContainerControl *parent, float y, float width, float height = UIXScrollBar::DefaultSize);

protected:
    /// <inheritdoc />
    FORCE_INLINE float GetTrackSize() const override { return GetWidth(); }
};
