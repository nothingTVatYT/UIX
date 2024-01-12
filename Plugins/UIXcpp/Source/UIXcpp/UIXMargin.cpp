#include "UIXMargin.h"
#include "Engine/Core/Math/Rectangle.h"


const String UIXMargin::_formatString = TEXT("Left:{0:F2} Right:{1:F2} Top:{2:F2} Bottom:{3:F2}");
const UIXMargin UIXMargin::Zero = UIXMargin();


UIXMargin::UIXMargin() : ScriptingObject(SpawnParams(Guid::New(), TypeInitializer))
{
    Left = 0;
    Right = 0;
    Top = 0;
    Bottom = 0;
}

UIXMargin::UIXMargin(float value) : ScriptingObject(SpawnParams(Guid::New(), TypeInitializer))
{
    Left = value;
    Right = value;
    Top = value;
    Bottom = value;
}

UIXMargin::UIXMargin(float left, float right, float top, float bottom) : ScriptingObject(SpawnParams(Guid::New(), TypeInitializer))
{
    Left = left;
    Right = right;
    Top = top;
    Bottom = bottom;
}

UIXMargin::UIXMargin(const UIXMargin &other) : ScriptingObject(SpawnParams(Guid::New(), TypeInitializer))
{
    Left = other.Left;
    Right = other.Right;
    Top = other.Top;
    Bottom = other.Bottom;
}

bool UIXMargin::IsZero() const
{
    return Math::IsZero(Left) && Math::IsZero(Right) && Math::IsZero(Top) && Math::IsZero(Bottom);
}


void UIXMargin::ShrinkRectangle(API_PARAM(Ref) Rectangle &rect) const
{
    rect.Location.X += Left;
    rect.Location.Y += Top;
    rect.Size.X -= Left + Right;
    rect.Size.Y -= Top + Bottom;
}

void UIXMargin::ExpandRectangle(API_PARAM(Ref) Rectangle &rect) const
{
    rect.Location.X -= Left;
    rect.Location.Y -= Top;
    rect.Size.X += Left + Right;
    rect.Size.Y += Top + Bottom;
}
