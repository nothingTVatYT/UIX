#include "UIXMargin.h"
#include "Engine/Core/Math/Rectangle.h"


const String UIXMargin::_formatString = TEXT("Left:{0:F2} Right:{1:F2} Top:{2:F2} Bottom:{3:F2}");
const UIXMargin UIXMargin::Zero = UIXMargin();


bool UIXMargin::IsZero() const
{
    return Math::IsZero(Left) && Math::IsZero(Right) && Math::IsZero(Top) && Math::IsZero(Bottom);
}


void UIXMargin::ShrinkRectangle(Rectangle &rect) const
{
    rect.Location.X += Left;
    rect.Location.Y += Top;
    rect.Size.X -= Left + Right;
    rect.Size.Y -= Top + Bottom;
}

void UIXMargin::ExpandRectangle(Rectangle &rect) const
{
    rect.Location.X -= Left;
    rect.Location.Y -= Top;
    rect.Size.X += Left + Right;
    rect.Size.Y += Top + Bottom;
}
