#include "UIXScrollableControl.h"
#include "Engine/Render2D/Render2D.h"


UIXScrollableControl::UIXScrollableControl(const SpawnParams &params) : UIXContainerControl(params)
{}

void UIXScrollableControl::SetViewOffset(Float2 value)
{
	_viewOffset = value;
	OnViewOffsetChanged();
}

void UIXScrollableControl::DrawChildren()
{
    // Draw all visible child controls
    bool hasViewOffset = !_viewOffset.IsZero();
    for (int i = 0; i < _children.Count(); i++)
    {
        UIXControl *child = _children[i];

        if (child->GetVisible())
        {
            Matrix3x3 transform = child->_cachedTransform;
            if (hasViewOffset && child->GetIsScrollable())
            {
                transform.M31 += _viewOffset.X;
                transform.M32 += _viewOffset.Y;
            }

            Render2D::PushTransform(transform);
            child->Draw();
            Render2D::PopTransform();
        }
    }
}

bool UIXScrollableControl::IntersectsChildContent(UIXControl *child, Float2 location, API_PARAM(Out) Float2 &childSpaceLocation) const
{
    // Apply offset on scrollable controls
    if (child->GetIsScrollable())
        location -= _viewOffset;

    return child->IntersectsContent(location, childSpaceLocation);
}

 bool UIXScrollableControl::IntersectsContent(Float2 locationParent, API_PARAM(Out) Float2 &location) const
{
    // Little workaround to prevent applying offset when performing intersection test with this scrollable control.
    // Note that overriden PointFromParent applies view offset.
    location = UIXContainerControl::PointFromParent(locationParent);
    return ContainsPoint(location);
}

Float2 UIXScrollableControl::PointToParent(Float2 location) const
{
    return UIXContainerControl::PointToParent(location) + _viewOffset;
}


Float2 UIXScrollableControl::PointFromParent(Float2 location) const
{
    return UIXContainerControl::PointFromParent(location) - _viewOffset;
}
