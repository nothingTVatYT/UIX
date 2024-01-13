#include "UIXHScrollBar.h"

UIXHScrollBar::UIXHScrollBar(/*const SpawnParams &params,*/ UIXContainerControl *parent, float y, float height, float width)
    : UIXScrollBar(/*params,*/ UIXOrientation::Vertical)
{
    SetAnchorPreset(UIXAnchorPresets::VerticalStretchRight);
    SetParent(parent);
    SetBounds(Rectangle(0, y, width, height));
}
