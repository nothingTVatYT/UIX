#include "UIXVScrollBar.h"

UIXVScrollBar::UIXVScrollBar(const SpawnParams &params, UIXContainerControl *parent, float x, float height, float width)
    : UIXScrollBar(params, UIXOrientation::Vertical)
{
    SetAnchorPreset(UIXAnchorPresets::VerticalStretchRight);
    SetParent(parent);
    SetBounds(Rectangle(x, 0, width, height));
}

