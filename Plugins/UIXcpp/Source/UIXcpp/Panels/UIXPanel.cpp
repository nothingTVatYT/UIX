#include "UIXPanel.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Math/Matrix3x3.h"
#include "Engine/Render2D/Render2D.h"


UIXPanel::UIXPanel(const SpawnParams &params) : UIXPanel(params, UIXScrollBars::None)
{
}

/// <summary>
/// Initializes a new instance of the <see cref="Panel"/> class.
/// </summary>
/// <param name="scrollBars">The scroll bars.</param>
/// <param name="autoFocus">True if control can accept user focus</param>
UIXPanel::UIXPanel(const SpawnParams &params, UIXScrollBars scrollBars, bool autoFocus) : UIXScrollableControl(params),
        _layoutChanged(false), _alwaysShowScrollbars(false), _layoutUpdateLock(false), _scrollBarsSize(UIXScrollBar::DefaultSize)
{
    SetAutoFocus(autoFocus);
    SetScrollBars(scrollBars);
}

void UIXPanel::SetScrollBars(UIXScrollBars value)
{
    if (_scrollBars == value)
        return;

    _scrollBars = value;

    if (((int)value & (int)UIXScrollBars::Vertical) == (int)UIXScrollBars::Vertical)
    {
        if (VScrollBar == nullptr)
            VScrollBar = GetChild<VScrollBar>();
        if (VScrollBar == nullptr)
        {
            VScrollBar = New<VScrollBar>(this, GetWidth() - _scrollBarsSize, GetHeight());
            VScrollBar->SetAnchorPreset(UIXAnchorPresets::TopLeft);

            // Was already commented out: //VScrollBar.X += VScrollBar.Width;

            // TODO: Change to bind when class is converted.
            VScrollBar.ValueChanged += () = > SetViewOffset(Orientation.Vertical, VScrollBar.Value);
        }
    }
    else if (VScrollBar != nullptr)
    {
        VScrollBar.Dispose();
        VScrollBar = nullptr;
    }

    if (((int)value & (int)UIXScrollBars::Horizontal) == (int)UIXScrollBars::Horizontal)
    {
        if (HScrollBar == nullptr)
            HScrollBar = GetChild<HScrollBar>();
        if (HScrollBar == nullptr)
        {
            HScrollBar = New<HScrollBar>(this, GetHeight() - _scrollBarsSize, GetWidth());
            HScrollBar->SetAnchorPreset(UIXAnchorPresets::TopLeft);

            // Was already commented out: //HScrollBar.Y += HScrollBar.Height;
            // Was already commented out: //HScrollBar.Offsets += new Margin(0, 0, HScrollBar.Height * 0.5f, 0);

            // TODO: Change to bind when class is converted.
            HScrollBar.ValueChanged += () = > SetViewOffset(Orientation.Horizontal, HScrollBar.Value);
        }
    }
    else if (HScrollBar != nullptr)
    {
        HScrollBar.Dispose();
        HScrollBar = nullptr;
    }

    PerformLayout();
}

void UIXPanel::SetScrollBarsSize(float value)
{
    if (Math::NearEqual(_scrollBarsSize, value))
        return;

    _scrollBarsSize = value;
    PerformLayout();
}
     

void UIXPanel::SetAlwaysShowScrollbars(bool value)
{
        if (_alwaysShowScrollbars != value)
        {
            _alwaysShowScrollbars = value;
            PerformLayout();
        }
}

void UIXPanel::SetScrollMargin(UIXMargin value)
{
    if (_scrollMargin != value)
    {
        _scrollMargin = value;
        PerformLayout();
    }
}

void UIXPanel::SetViewOffset(Float2 value)
{
    bool wasLocked = _isLayoutLocked;
    _isLayoutLocked = true;

    if (HScrollBar != nullptr)
        HScrollBar.Value = -value.X;
    if (VScrollBar != nullptr)
        VScrollBar.Value = -value.Y;

    _isLayoutLocked = wasLocked;
    UIXScrollableControl::SetViewOffset(value);
}

void UIXPanel::FastScroll()
{
    if (HScrollBar != nullptr)
        HScrollBar->FastScroll();
    if (VScrollBar != nullptr)
        VScrollBar->FastScroll();
}

void UIXPanel::ScrollViewTo(UIXControl *c, bool fastScroll = false)
{
    if (c == nullptr)
    {
        LOG(Error, "ArgumentNullException: The control to scroll to is null.");
        return;
    }

    Float2 location = c->GetLocation();
    Float2 size = c->GetSize();
    while (c->GetHasParent() && c->GetParent() != this)
    {
        c = c->GetParent();
        location = c->PointToParent(location);
    }

    if (c->GetHasParent())
    {
        ScrollViewTo(Rectangle(location, size), fastScroll);
    }
}

void UIXPanel::ScrollViewTo(Float2 location, bool fastScroll = false)
{
    ScrollViewTo(Rectangle(location, Float2::Zero), fastScroll);
}

void UIXPanel::ScrollViewTo(Rectangle bounds, bool fastScroll = false)
{
    bool wasLocked = _isLayoutLocked;
    _isLayoutLocked = true;

    if (HScrollBar != nullptr && HScrollBar.Enabled)
        HScrollBar.ScrollViewTo(bounds.GetLeft(), bounds.GetRight(), fastScroll);
    if (VScrollBar != nullptr && VScrollBar.Enabled)
        VScrollBar.ScrollViewTo(bounds.GetTop(), bounds.GetBottom(), fastScroll);

    _isLayoutLocked = wasLocked;
    PerformLayout();
}

void UIXPanel::SetViewOffset(UIXOrientation orientation, float value)
{
    if (orientation == UIXOrientation::Vertical)
        _viewOffset.Y = -value;
    else
        _viewOffset.X = -value;
    OnViewOffsetChanged();
    PerformLayout();
}

void UIXPanel::AddChildInternal(UIXControl *child)
{
    UIXScrollableControl::AddChildInternal(child);

    if (child->GetIsScrollable())
    {
        PerformLayout();
    }
}

bool UIXPanel::OnMouseDown(Float2 location, MouseButton button)
{
    if (UIXScrollableControl::OnMouseDown(location, button))
        return true;
    return GetAutoFocus() && Focus(this);
}

bool UIXPanel::OnMouseWheel(Float2 location, float delta)
{
    // Base
    if (UIXScrollableControl::OnMouseWheel(location, delta))
        return true;

    // Roll back to scroll bars
    if (VScrollBar != nullptr && VScrollBar.Enabled && VScrollBar.OnMouseWheel(VScrollBar.PointFromParent(location), delta))
        return true;
    if (HScrollBar != nullptr && HScrollBar.Enabled && HScrollBar.OnMouseWheel(HScrollBar.PointFromParent(location), delta))
        return true;

    // No event handled
    return false;
}


void UIXPanel::RemoveChildren()
{
    // Keep scroll bars alive
    if (VScrollBar != nullptr)
        _children.Remove(VScrollBar);
    if (HScrollBar != nullptr)
        _children.Remove(HScrollBar);

    UIXScrollableControl::RemoveChildren();

    // Restore scrollbars
    if (VScrollBar != nullptr)
        _children.Add(VScrollBar);
    if (HScrollBar != nullptr)
        _children.Add(HScrollBar);
    PerformLayout();
}


void UIXPanel::DisposeChildren()
{
    // Keep scrollbars alive
    if (VScrollBar != nullptr)
        _children.Remove(VScrollBar);
    if (HScrollBar != nullptr)
        _children.Remove(HScrollBar);

    UIXScrollableControl::DisposeChildren();

    // Restore scrollbars
    if (VScrollBar != nullptr)
        _children.Add(VScrollBar);
    if (HScrollBar != nullptr)
        _children.Add(HScrollBar);
    PerformLayout();
}


void UIXPanel::OnChildResized(UIXControl *control)
{
    UIXScrollableControl::OnChildResized(control);

    if (control->GetIsScrollable())
    {
        PerformLayout();
    }
}


void UIXPanel::Draw()
{
    UIXScrollableControl::Draw();

    // Draw scrollbars manually (they are outside the clipping bounds)
    if (VScrollBar != nullptr && VScrollBar->Visible)
    {
        Render2D::PushTransform(VScrollBar->_cachedTransform);
        VScrollBar.Draw();
        Render2D::PopTransform();
    }

    if (HScrollBar != nullptr && HScrollBar.Visible)
    {
        Render2D::PushTransform(HScrollBar->_cachedTransform);
        HScrollBar.Draw();
        Render2D::PopTransform();
    }
}


bool UIXPanel::IntersectsChildContent(UIXControl *child, Float2 location, API_PARAM(Out) Float2 &childSpaceLocation) const
{
    // For not scroll bars we want to reject any collisions
    if (child != VScrollBar && child != HScrollBar)
    {
        // Check if has v scroll bar to reject points on it
        if (VScrollBar != nullptr && VScrollBar.Enabled)
        {
            Float2 pos = VScrollBar->PointFromParent(location);
            if (VScrollBar.ContainsPoint(pos))
            {
                childSpaceLocation = Float2::Zero;
                return false;
            }
        }

        // Check if has h scroll bar to reject points on it
        if (HScrollBar != nullptr && HScrollBar.Enabled)
        {
            Float2 pos = HScrollBar->PointFromParent(location);
            if (HScrollBar.ContainsPoint(pos))
            {
                childSpaceLocation = Float2::Zero;
                return false;
            }
        }
    }

    return UIXScrollableControl::IntersectsChildContent(child, location, childSpaceLocation);
}

void UIXPanel::PerformLayout(bool force)
{
    if (_layoutUpdateLock > 2)
        return;
    _layoutUpdateLock++;

    if (!_isLayoutLocked)
    {
        _layoutChanged = false;
    }

    UIXScrollableControl::PerformLayout(force);

    if (!_isLayoutLocked && _layoutChanged)
    {
        _layoutChanged = false;
        PerformLayout(true);
    }

    _layoutUpdateLock--;
}

void UIXPanel::PerformLayoutBeforeChildren()
{
    // Arrange controls and get scroll bounds
    ArrangeAndGetBounds();

    // Update scroll bars
    Rectangle controlsBounds = _controlsBounds;
    Rectangle scrollBounds = controlsBounds;
    _scrollMargin.ExpandRectangle(scrollBounds);
    if (VScrollBar != nullptr)
    {
        float height = GetHeight();
        bool vScrollEnabled = (controlsBounds.GetBottom() > height + 0.01f || controlsBounds.Y < 0.0f) && height > _scrollBarsSize;

        if (VScrollBar.Enabled != vScrollEnabled)
        {
            // Set scroll bar visibility
            VScrollBar.Enabled = vScrollEnabled;
            VScrollBar.Visible = vScrollEnabled || _alwaysShowScrollbars;
            _layoutChanged = true;

            // Clear scroll state
            VScrollBar.Reset();
            _viewOffset.Y = 0;
            OnViewOffsetChanged();

            // Get the new bounds after changing scroll
            ArrangeAndGetBounds();
        }

        if (vScrollEnabled)
        {
            VScrollBar.SetScrollRange(scrollBounds.GetTop(), Math::Max(Math::Max(0.f, scrollBounds.GetTop()), scrollBounds.GetHeight() - height));
        }
        VScrollBar.Bounds = new Rectangle(GetWidth() - _scrollBarsSize, 0, _scrollBarsSize, GetHeight());
    }
    if (HScrollBar != nullptr)
    {
        float width = GetWidth();
        bool hScrollEnabled = (controlsBounds.GetRight() > width + 0.01f || controlsBounds.GetX() < 0.0f) && width > _scrollBarsSize;

        if (HScrollBar.Enabled != hScrollEnabled)
        {
            // Set scroll bar visibility
            HScrollBar.Enabled = hScrollEnabled;
            HScrollBar.Visible = hScrollEnabled || _alwaysShowScrollbars;
            _layoutChanged = true;

            // Clear scroll state
            HScrollBar.Reset();
            _viewOffset.X = 0;
            OnViewOffsetChanged();

            // Get the new bounds after changing scroll
            ArrangeAndGetBounds();
        }

        if (hScrollEnabled)
        {
            HScrollBar.SetScrollRange(scrollBounds.GetLeft(), Math::Max(Math::Max(0.f, scrollBounds.GetLeft()), scrollBounds.GetWidth() - width));
        }
        HScrollBar.Bounds = new Rectangle(0, GetHeight() - _scrollBarsSize, GetWidth() - (VScrollBar != nullptr && VScrollBar.Visible ? VScrollBar.GetWidth() : 0), _scrollBarsSize);
    }
}

void UIXPanel::ArrangeAndGetBounds()
{
    Arrange();

    // Calculate scroll area bounds
    Float2 totalMin = Float2::Zero;
    Float2 totalMax = Float2::Zero;
    for (int i = 0; i < _children.Count(); i++)
    {
        UIXControl *c = _children[i];
        if (c->GetVisible() && c->GetIsScrollable())
        {
            Float2 min = Float2::Zero;
            Float2 max = c->GetSize();
            Matrix3x3::Transform2DPoint(min, c->_cachedTransform, min);
            Matrix3x3::Transform2DPoint(max, c->_cachedTransform, max);
            Float2::Min(min, totalMin, totalMin);
            Float2::Max(max, totalMax, totalMax);
        }
    }

    // Cache result
    _controlsBounds = Rectangle(totalMin, totalMax - totalMin);
}

void UIXPanel::Arrange()
{
    UIXScrollableControl::PerformLayoutBeforeChildren();
}

void UIXPanel::GetDesireClientArea(API_PARAM(Out) Rectangle &rect) const
{
    rect = Rectangle(Float2::Zero, GetSize());

    if (VScrollBar != nullptr && VScrollBar.Visible)
    {
        //TODO: check if there's a better way to do this: rect.Width -= VScrollBar.Width;
        rect = Rectangle(0, 0, GetSize().X - VScrollBar.Width, GetSize().Y);
    }

    if (HScrollBar != nullptr && HScrollBar.Visible)
    {
        //TODO: check if there's a better way to do this: rect.Height -= HScrollBar.Height;
        rect = Rectangle(0, 0, GetSize().X, GetSize().Y - VScrollBar.Height);
    }
}

DragDropEffect UIXPanel::OnDragMove(Float2 location, const DragData &data)
{
    DragDropEffect result = UIXScrollableControl::OnDragMove(location, data);

    float width = GetWidth();
    float height = GetHeight();
    float MinSize = 70;
    float AreaSize = 25;
    float MoveScale = 4.0f;
    Float2 viewOffset = -_viewOffset;

    if (VScrollBar != nullptr && VScrollBar.Enabled && height > MinSize)
    {
        if (Rectangle(0, 0, width, AreaSize).Contains(location))
        {
            viewOffset.Y -= MoveScale;
        }
        else if (Rectangle(0, height - AreaSize, width, AreaSize).Contains(location))
        {
            viewOffset.Y += MoveScale;
        }

        viewOffset.Y = Math::Clamp(viewOffset.Y, VScrollBar.Minimum, VScrollBar.Maximum);
        VScrollBar.Value = viewOffset.Y;
    }

    if (HScrollBar != nullptr && HScrollBar.Enabled && width > MinSize)
    {
        if (Rectangle(0, 0, AreaSize, height).Contains(location))
        {
            viewOffset.X -= MoveScale;
        }
        else if (Rectangle(width - AreaSize, 0, AreaSize, height).Contains(location))
        {
            viewOffset.X += MoveScale;
        }

        viewOffset.X = Math::Clamp(viewOffset.X, HScrollBar.Minimum, HScrollBar.Maximum);
        HScrollBar.Value = viewOffset.X;
    }

    viewOffset *= -1;

    if (viewOffset != _viewOffset)
    {
        _viewOffset = viewOffset;
        OnViewOffsetChanged();
        PerformLayout();
    }

    return result;
}
