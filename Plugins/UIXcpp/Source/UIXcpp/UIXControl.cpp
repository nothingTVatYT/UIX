#include "UIXControl.h"
#include "UIXContainerControl.h"
#include "UIXRootControl.h"
#include "UIXWindowRootControl.h"
#include "UIXTooltip.h"
#include "UIXStyle.h"
#include "Panels/UIXPanel.h"
#include "Engine/Render2D/Render2D.h"
#include "Engine/Core/Log.h"


UIXControl::UIXControl(/*const SpawnParams &params*/) : ScriptingObject(SpawnParams(Guid::New(), TypeInitializer))
{
    _bounds = Rectangle(_offsets.Left, _offsets.Top, _offsets.Right, _offsets.Bottom);
    UpdateTransform();
}



/// <summary>
/// Initializes a new instance of the <see cref="UIXControl"/> class.
/// </summary>
/// <param name="x">X coordinate</param>
/// <param name="y">Y coordinate</param>
/// <param name="width">Width</param>
/// <param name="height">Height</param>
UIXControl::UIXControl(/*const SpawnParams &params,*/ float x, float y, float width, float height) : UIXControl(Rectangle(x, y, width, height))
{
}

/// <summary>
/// Initializes a new instance of the <see cref="UIXControl"/> class.
/// </summary>
/// <param name="location">Upper left corner location.</param>
/// <param name="size">Bounds size.</param>
UIXControl::UIXControl(/*const SpawnParams &params,*/ Float2 location, Float2 size) : UIXControl(Rectangle(location, size))
{
}

UIXControl::UIXControl(/*const SpawnParams &params, */Rectangle bounds) : ScriptingObject(SpawnParams(Guid::New(), TypeInitializer))
{
    _bounds = bounds;
    _offsets = UIXMargin(bounds.GetX(), bounds.GetWidth(), bounds.GetY(), bounds.GetHeight());
    UpdateTransform();
}


//void UIXControl::OnEnable()
//{
//    // Here you can add code that needs to be called when script is enabled (eg. register for events)
//}
//
//void UIXControl::OnDisable()
//{
//    // Here you can add code that needs to be called when script is disabled (eg. unregister from events)
//}
//
//void UIXControl::OnUpdate()
//{
//    // Here you can add code that needs to be called every frame
//}


const UIXControl::AnchorPresetData UIXControl::AnchorPresetsData[] = {
            AnchorPresetData(UIXAnchorPresets::TopLeft, Float2(0, 0), Float2(0, 0)),
            AnchorPresetData(UIXAnchorPresets::TopCenter, Float2(0.5f, 0), Float2(0.5f, 0)),
            AnchorPresetData(UIXAnchorPresets::TopRight, Float2(1, 0), Float2(1, 0)),

            AnchorPresetData(UIXAnchorPresets::MiddleLeft, Float2(0, 0.5f), Float2(0, 0.5f)),
            AnchorPresetData(UIXAnchorPresets::MiddleCenter, Float2(0.5f, 0.5f), Float2(0.5f, 0.5f)),
            AnchorPresetData(UIXAnchorPresets::MiddleRight, Float2(1, 0.5f), Float2(1, 0.5f)),

            AnchorPresetData(UIXAnchorPresets::BottomLeft, Float2(0, 1), Float2(0, 1)),
            AnchorPresetData(UIXAnchorPresets::BottomCenter, Float2(0.5f, 1), Float2(0.5f, 1)),
            AnchorPresetData(UIXAnchorPresets::BottomRight, Float2(1, 1), Float2(1, 1)),

            AnchorPresetData(UIXAnchorPresets::HorizontalStretchTop, Float2(0, 0), Float2(1, 0)),
            AnchorPresetData(UIXAnchorPresets::HorizontalStretchMiddle, Float2(0, 0.5f), Float2(1, 0.5f)),
            AnchorPresetData(UIXAnchorPresets::HorizontalStretchBottom, Float2(0, 1), Float2(1, 1)),

            AnchorPresetData(UIXAnchorPresets::VerticalStretchLeft, Float2(0, 0), Float2(0, 1)),
            AnchorPresetData(UIXAnchorPresets::VerticalStretchCenter, Float2(0.5f, 0), Float2(0.5f, 1)),
            AnchorPresetData(UIXAnchorPresets::VerticalStretchRight, Float2(1, 0), Float2(1, 1)),

            AnchorPresetData(UIXAnchorPresets::StretchAll, Float2(0, 0), Float2(1, 1)),
};
// WARNING: If number of elements change, change this value as well:
#define ANCHOR_PRESET_DATA_COUNT    16

void UIXControl::SetParent(UIXContainerControl *value)
{
    if (_parent == value)
        return;

    Defocus();

    Float2 oldParentSize;
    if (_parent != nullptr)
    {
        oldParentSize = _parent->GetSize();
        _parent->RemoveChildInternal(this);
    }
    else
    {
        oldParentSize = Float2::Zero;
        ClearState();
    }

    _parent = value;
    if (_parent != nullptr)
        _parent->AddChildInternal(this);

    CacheRootHandle();
    OnParentChangedInternal();

    // Check if parent size has been changed
    if (_parent != nullptr && !Math::NearEqual(_parent->GetSize(), oldParentSize))
    {
        OnParentResized();
    }
}


int UIXControl::GetIndexInParent() const
{
    return _parent != nullptr ? _parent->GetChildIndex(this) : -1;
}

void UIXControl::SetIndexInParent(int value)
{
    if (_parent != nullptr)
        _parent->ChangeChildIndex(this, value);
}


void UIXControl::SetBackgroundColor(const Color &value)
{
    _backgroundColor = value;
}

UIXAnchorPresets UIXControl::GetAnchorPreset() const
{
    UIXAnchorPresets result = UIXAnchorPresets::Custom;
    for (int i = 0, siz = ANCHOR_PRESET_DATA_COUNT; i < siz; i++)
    {
        if (Float2::NearEqual(_anchorMin, AnchorPresetsData[i].Min) &&
            Float2::NearEqual(_anchorMax, AnchorPresetsData[i].Max))
        {
            result = AnchorPresetsData[i].Preset;
            break;
        }
    }
    return result;
}

void UIXControl::SetAnchorPreset(UIXAnchorPresets value)
{
    SetAnchorPreset(value, false);
}


void UIXControl::SetEnabled(bool value)
{
    if (_isEnabled != value)
    {
        _isEnabled = value;
        if (!_isEnabled)
            ClearState();
    }
}

bool UIXControl::GetEnabledInHierarchy() const
{
    if (!_isEnabled)
        return false;
    if (_parent != nullptr)
        return _parent->GetEnabledInHierarchy();
    return true;
}

void UIXControl::SetVisible(bool value)
{
    if (_isVisible != value)
    {
        _isVisible = value;
        if (!_isVisible)
            ClearState();

        OnVisibleChanged();
        if (_parent != nullptr)
            _parent->PerformLayout();
    }
}

bool UIXControl::GetVisibleInHierarchy() const
{
    if (!_isVisible)
        return false;
    if (_parent != nullptr)
        return _parent->GetVisibleInHierarchy();
    return true;
}

UIXWindowRootControl* UIXControl::GetRootWindow() const
{
    return _root != nullptr ? _root->GetRootWindow() : nullptr;
}


/// <summary>
/// Gets the control DPI scale factor (1 is default). Includes custom DPI scale.
/// </summary>
float UIXControl::GetDpiScale() const
{
    UIXWindowRootControl *root = GetRootWindow();
    if (root == nullptr)
        return Platform::GetDpiScale();
    auto w = root->GetWindow();
    if (w == nullptr)
        return Platform::GetDpiScale();
    return w->GetDpiScale();
}

Float2 UIXControl::GetScreenPos() const
{
    const UIXRootControl *parentWin = _root;
    if (parentWin == nullptr)
    {
        LOG(Error, "ArgumentNullException: Parent window is null!");
        return Float2::Zero;
    }

    auto clientPos = PointToWindow(Float2::Zero);
    return parentWin->PointToScreen(clientPos);
}


CursorType UIXControl::GetCursor() const
{
    return _parent != nullptr ? _parent->GetCursor() : CursorType::Default;
}

void UIXControl::SetCursor(CursorType value)
{
    if (_parent != nullptr)
        _parent->SetCursor(value);
}


void UIXControl::Dispose()
{
    if (_isDisposing)
        return;

    // Call event
    OnDestroy();

    // Unlink
    SetParent(nullptr);
}

void UIXControl::OnUpdateTooltip(float deltaTime)
{
    _tooltip->OnMouseOverControl(this, deltaTime);
}


void UIXControl::Draw()
{
    // Paint Background
    if (_backgroundColor.A > 0.0f)
    {
        Render2D::FillRectangle(Rectangle(Float2::Zero, GetSize()), _backgroundColor);
    }
}

void UIXControl::ClearState()
{
    Defocus();
    if (_isMouseOver)
        OnMouseLeave();
    /*if (_isDragOver)
        OnDragLeave();*/
    while (/*_touchOvers != nullptr && */_touchOvers.Count() != 0)
        OnTouchLeave(_touchOvers[0]);
}

void UIXControl::SetAutoFocus(bool value)
{
    _autoFocus = value;
}


/// <summary>
/// Sets input focus to the control
/// </summary>
void UIXControl::Focus()
{
    if (!GetIsFocused())
    {
        Focus(this);
    }
}

/// <summary>
/// Removes input focus from the control
/// </summary>
void UIXControl::Defocus()
{
    if (GetContainsFocus())
    {
        Focus(nullptr);
    }
}


void UIXControl::OnGotFocus()
{
    // Cache flag
    _isFocused = true;
    _isNavFocused = false;
}

void UIXControl::OnLostFocus()
{
    // Clear flag
    _isFocused = false;
    _isNavFocused = false;
}


bool UIXControl::Focus(UIXControl *c)
{
    return _parent != nullptr && _parent->Focus(c);
}

void UIXControl::StartMouseCapture(bool useMouseScreenOffset)
{
    auto parent = GetRoot();
    if (parent != nullptr)
        parent->StartTrackingMouse(this, useMouseScreenOffset);
}

void UIXControl::EndMouseCapture()
{
    auto parent = GetRoot();
    if (parent != nullptr)
        parent->EndTrackingMouse();
}


UIXControl* UIXControl::GetNavTarget(UIXNavDirection direction)
{
    switch (direction)
    {
        case UIXNavDirection::Up: return NavTargetUp;
        case UIXNavDirection::Down: return NavTargetDown;
        case UIXNavDirection::Left: return NavTargetLeft;
        case UIXNavDirection::Right: return NavTargetRight;
        default: return nullptr;
    }
}

Float2 UIXControl::GetNavOrigin(UIXNavDirection direction)
{
    auto size = GetSize();
    switch (direction)
    {
        case UIXNavDirection::Up: return Float2(size.X * 0.5f, 0);
        case UIXNavDirection::Down: return Float2(size.X * 0.5f, size.Y);
        case UIXNavDirection::Left: return Float2(0, size.Y * 0.5f);
        case UIXNavDirection::Right: return Float2(size.X, size.Y * 0.5f);
        case UIXNavDirection::Next: return Float2::Zero;
        case UIXNavDirection::Previous: return size;
        default: return size * 0.5f;
    }
}

UIXControl* UIXControl::OnNavigate(UIXNavDirection direction, Float2 location, UIXControl* caller, API_PARAM(Ref) Array<UIXControl*> &visited)
{
    if (caller == _parent && GetAutoFocus() && GetVisible())
        return this;
    if (_parent != nullptr)
        return _parent->OnNavigate(direction, PointToParent(GetNavOrigin(direction)), caller, visited);
    return nullptr;
}

void UIXControl::NavigationFocus()
{
    Focus();
    if (GetIsFocused())
    {
        _isNavFocused = true;

        // Ensure to be in a view
        auto parent = GetParent();
        while (parent != nullptr)
        {
            auto panel = dynamic_cast<UIXPanel*>(parent);
            if (panel != nullptr && ((panel->VScrollBar != nullptr && panel->VScrollBar->GetEnabled()) || (panel->HScrollBar != nullptr && panel->HScrollBar->GetEnabled())))
            {
                panel->ScrollViewTo(this);
                break;
            }
            parent = parent->GetParent();
        }
    }
}

void UIXControl::OnMouseEnter(Float2 location)
{
    // Set flag
    _isMouseOver = true;

    // Update tooltip
    if (GetShowTooltip() && OnTestTooltipOverControl(location))
    {
        GetTooltip()->OnMouseEnterControl(this);

        UpdateDelegate tooltip_delegate;
        tooltip_delegate.Bind<UIXControl, &UIXControl::OnUpdateTooltip>(this);

        SetUpdate(_tooltipUpdate, tooltip_delegate);
    }
}

/// <summary>
/// When mouse moves over control's area
/// </summary>
/// <param name="location">Mouse location in UIXControl Space</param>
API_FUNCTION(NoAnimate)
void UIXControl::OnMouseMove(Float2 location)
{
    // Update tooltip
    if (GetShowTooltip() && OnTestTooltipOverControl(location))
    {
        GetTooltip()->OnMouseEnterControl(this);
        if (_tooltipUpdate.IsBinded())
        {
            UpdateDelegate tooltip_delegate;
            tooltip_delegate.Bind<UIXControl, &UIXControl::OnUpdateTooltip>(this);

            SetUpdate(_tooltipUpdate, tooltip_delegate);
        }
    }
    else if (_tooltipUpdate.IsBinded())
    //{
        SetUpdate(_tooltipUpdate, UpdateDelegate());
    GetTooltip()->OnMouseLeaveControl(this);
    //}
}

/// <summary>
/// When mouse leaves control's area
/// </summary>
API_FUNCTION(NoAnimate)
void UIXControl::OnMouseLeave()
{
    // Clear flag
    _isMouseOver = false;

    // Update tooltip
    if (_tooltipUpdate.IsBinded())
    //{
        SetUpdate(_tooltipUpdate, UpdateDelegate());
    GetTooltip()->OnMouseLeaveControl(this);
    //}
}

/// <summary>
/// When mouse wheel moves
/// </summary>
/// <param name="location">Mouse location in UIXControl Space</param>
/// <param name="delta">Mouse wheel move delta. A positive value indicates that the wheel was rotated forward, away from the user; a negative value indicates that the wheel was rotated backward, toward the user. Normalized to [-1;1] range.</param>
/// <returns>True if event has been handled</returns>
API_FUNCTION(NoAnimate)
bool UIXControl::OnMouseWheel(Float2 location, float delta)
{
    return false;
}

/// <summary>
/// When mouse goes down over control's area
/// </summary>
/// <param name="location">Mouse location in UIXControl Space</param>
/// <param name="button">Mouse buttons state (flags)</param>
/// <returns>True if event has been handled, otherwise false</returns>
API_FUNCTION(NoAnimate)
bool UIXControl::OnMouseDown(Float2 location, MouseButton button)
{
    return false;
}

/// <summary>
/// When mouse goes up over control's area
/// </summary>
/// <param name="location">Mouse location in UIXControl Space</param>
/// <param name="button">Mouse buttons state (flags)</param>
/// <returns>True if event has been handled, otherwise false</returns>
API_FUNCTION(NoAnimate)
bool UIXControl::OnMouseUp(Float2 location, MouseButton button)
{
    return false;
}

/// <summary>
/// When mouse double clicks over control's area
/// </summary>
/// <param name="location">Mouse location in UIXControl Space</param>
/// <param name="button">Mouse buttons state (flags)</param>
/// <returns>True if event has been handled, otherwise false</returns>
API_FUNCTION(NoAnimate)
bool UIXControl::OnMouseDoubleClick(Float2 location, MouseButton button)
{
    return false;
}


/*void UIXControl::DoDragDrop(const DragData& data)
{
    // Hide tooltip
    auto tt = GetTooltip();
    if (tt != nullptr)
        tt->Hide();
    auto r = GetRoot();
    if (r != nullptr)
        r->DoDragDrop(data);
}*/

UIXTooltip* UIXControl::GetTooltip() const
{
    return _tooltip != nullptr ? _tooltip : UIXStyle::GetCurrent()->SharedTooltip;
}

UIXControl* UIXControl::LinkTooltip(String text, UIXTooltip *customTooltip)
{
    _tooltipText = text;
    _tooltip = customTooltip;
    return this;
}


void UIXControl::UnlinkTooltip()
{
    _tooltipText.Clear();
    _tooltip = nullptr;
}

bool UIXControl::OnShowTooltip(API_PARAM(Out) String &text, API_PARAM(Out) Float2 &location, API_PARAM(Out) Rectangle &area)
{
    text = _tooltipText;
    location = GetSize() * Float2(0.5f, 1.0f);
    area = Rectangle(Float2::Zero, GetSize());
    return GetShowTooltip();
}

bool UIXControl::OnTestTooltipOverControl(Float2 location)
{
    return ContainsPoint(location) && GetShowTooltip();
}

bool UIXControl::IntersectsContent(Float2 locationParent, API_PARAM(Out) Float2 &location) const
{
    location = PointFromParent(locationParent);
    return ContainsPoint(location);
}

bool UIXControl::ContainsPoint(Float2 location) const
{
    return location.X >= 0 &&
        location.Y >= 0 &&
        location.X <= _bounds.Size.X &&
        location.Y <= _bounds.Size.Y;
}

Float2 UIXControl::PointToParent(UIXContainerControl *parent, Float2 location) const
{
    if (parent == nullptr)
    {
        LOG(Error, "ArgumentNullException: Parent control is null!");
        return Float2::Zero;
    }

    const UIXControl *c = this;
    while (c != nullptr)
    {
        location = c->PointToParent(location);

        c = c->GetParent();
        if (c == parent)
            break;
    }
    return location;
}

//Float2 UIXControl::PointToParent(Float2 location) const
//{
//    return PointToParent(location);
//}
//
//Float2 UIXControl::PointToParent(API_PARAM(Ref) Float2 &location) const
//{
//    Float2 result;
//    Matrix3x3::Transform2DPoint(location, _cachedTransform, result);
//    return result;
//}

Float2 UIXControl::PointToParent(Float2 location) const
{
    Float2 result;
    Matrix3x3::Transform2DPoint(location, _cachedTransform, result);
    return result;
}


//Float2 UIXControl::PointFromParent(Float2 locationParent) const
//{
//    return PointFromParent(locationParent);
//}
//
//Float2 UIXControl::PointFromParent(API_PARAM(Ref) Float2 &locationParent) const
//{
//    Float2 result;
//    Matrix3x3::Transform2DPoint(locationParent, _cachedTransformInv, result);
//    return result;
//}

Float2 UIXControl::PointFromParent(Float2 locationParent) const
{
    Float2 result;
    Matrix3x3::Transform2DPoint(locationParent, _cachedTransformInv, result);
    return result;
}

Float2 UIXControl::PointFromParent(UIXContainerControl *parent, Float2 location) const
{
    if (parent == nullptr)
    {
        LOG(Error, "ArgumentNullException: Parent control is null!");
        return Float2::Zero;
    }

    Array<const UIXControl*> path;

    const UIXControl *c = this;
    while (c != nullptr && c != parent)
    {
        path.Add(c);
        c = c->GetParent();
    }
    for (int i = path.Count() - 1; i >= 0; i--)
    {
        location = path[i]->PointFromParent(location);
    }
    return location;
}

Float2 UIXControl::PointToWindow(Float2 location) const
{
    location = PointToParent(location);
    if (_parent != nullptr)
    {
        location = _parent->PointToWindow(location);
    }
    return location;
}

Float2 UIXControl::PointFromWindow(Float2 location) const
{
    if (_parent != nullptr)
    {
        location = _parent->PointFromWindow(location);
    }
    return PointFromParent(location);
}

Float2 UIXControl::PointToScreen(Float2 location) const
{
    location = PointToParent(location);
    if (_parent != nullptr)
    {
        location = _parent->PointToScreen(location);
    }
    return location;
}

Float2 UIXControl::PointFromScreen(Float2 location) const
{
    if (_parent != nullptr)
    {
        location = _parent->PointFromScreen(location);
    }
    return PointFromParent(location);
}


void UIXControl::OnLocationChanged()
{
    if (LocationChanged.IsBinded())
        LocationChanged(this);
}

void UIXControl::OnSizeChanged()
{
    if (SizeChanged.IsBinded())
        SizeChanged(this);
    if (_parent != nullptr)
        _parent->OnChildResized(this);
}

void UIXControl::SetScaleInternal(API_PARAM(Ref) const Float2 &scale)
{
    _scale = scale;
    UpdateTransform();
    if (_parent != nullptr)
        _parent->OnChildResized(this);
}

void UIXControl::SetPivotInternal(API_PARAM(Ref) const Float2 &pivot)
{
    _pivot = pivot;
    UpdateTransform();
    if (_parent != nullptr)
        _parent->OnChildResized(this);
}

void UIXControl::SetShearInternal(API_PARAM(Ref) const Float2 &shear)
{
    _shear = shear;
    UpdateTransform();
    if (_parent != nullptr)
        _parent->OnChildResized(this);
}

void UIXControl::SetRotationInternal(float rotation)
{
    _rotation = rotation;
    UpdateTransform();
    if (_parent != nullptr)
        _parent->OnChildResized(this);
}

void UIXControl::OnVisibleChanged()
{
    // Clear state when control gets hidden
    if (!_isVisible && _isMouseOver)
    {
        OnMouseLeave();
    }

    if (VisibleChanged.IsBinded())
        VisibleChanged(this);
}

void UIXControl::OnParentChangedInternal()
{
    if (ParentChanged.IsBinded())
        ParentChanged(this);
}

void UIXControl::CacheRootHandle()
{
    if (_root != nullptr)
        RemoveUpdateCallbacks(_root);

    _root = _parent != nullptr ? _parent->GetRoot() : nullptr;
    if (_root != nullptr)
        AddUpdateCallbacks(_root);
}

void UIXControl::AddUpdateCallbacks(UIXRootControl *root)
{
    if (_tooltipUpdate.IsBinded())
        root->UpdateCallbacksToAdd.Bind(_tooltipUpdate);
}

void UIXControl::RemoveUpdateCallbacks(UIXRootControl *root)
{
    if (_tooltipUpdate.IsBinded())
        root->UpdateCallbacksToRemove.Bind(_tooltipUpdate);
}

void UIXControl::SetUpdate(API_PARAM(ref) UpdateDelegate &onUpdate, const UpdateDelegate &value)
{
// TODO: if it's possible, change this to a check in contents?
    if (onUpdate == value)
        return;
    if (_root != nullptr && onUpdate.IsBinded())
        _root->UpdateCallbacksToRemove.Bind(onUpdate);
    onUpdate = value;
    if (_root != nullptr && onUpdate.IsBinded())
        _root->UpdateCallbacksToAdd.Bind(onUpdate);
}

void UIXControl::OnParentResized()
{
    if (!_anchorMin.IsZero() || !_anchorMax.IsZero())
    {
        UpdateBounds();
    }
}

void UIXControl::OnDestroy()
{
    // Set disposing flag
    _isDisposing = true;
    Defocus();
    UnlinkTooltip();
    Tag = nullptr;
}


int UIXControl::CompareTo(Object *obj) const
{
    UIXControl *c = dynamic_cast<UIXControl*>(obj);
    if (c)
        return Compare(c);
    return 0;
}

int UIXControl::Compare(UIXControl *other) const
{
    return (int)(GetY() - other->GetY());
}

void UIXControl::SetSize(const Float2 &value)
{
    if (Math::NearEqual(_bounds.Size, value))
        return;

    Rectangle bounds = Rectangle(_bounds.Location, value);
    SetBoundsInner(bounds);
}

void UIXControl::SetX(float value)
{
    SetBounds(Rectangle(value, GetY(), _bounds.Size.X, _bounds.Size.Y));
}

void UIXControl::SetY(float value)
{
    SetBounds(Rectangle(GetX(), value, _bounds.Size.X, _bounds.Size.Y));
}

void UIXControl::SetLocalX(float value)
{
    SetLocalLocation(Float2(value, GetLocalLocation().Y));
}

void UIXControl::SetLocalY(float value)
{
    SetLocalLocation(Float2(GetLocalLocation().X, value));
}

void UIXControl::SetAnchorMin(const Float2 &value)
{
    if (!Math::NearEqual(_anchorMin, value))
    {
        Rectangle bounds = GetBounds();
        _anchorMin = value;
        UpdateBounds();
        SetBounds(bounds);
    }
}

void UIXControl::SetAnchorMax(const Float2 &value)
{
    if (!Math::NearEqual(_anchorMax, value))
    {
        Rectangle bounds = GetBounds();
        _anchorMax = value;
        UpdateBounds();
        SetBounds(bounds);
    }
}

void UIXControl::SetOffsets(const UIXMargin* value)
{
    if (!_offsets.Equals(*value))
    {
        _offsets = *value;
        UpdateBounds();
    }
}

void UIXControl::SetLocation(const Float2 &value)
{
    if (Math::NearEqual(_bounds.Location, value))
        return;

    Rectangle bounds = Rectangle(value, _bounds.Size);
    SetBoundsInner(bounds);
}

Float2 UIXControl::GetLocalLocation() const
{
    return _bounds.Location - (_parent != nullptr ? _parent->_bounds.Size * (_anchorMax + _anchorMin) * 0.5f : Float2::Zero) + _bounds.Size * _pivot;
}
void UIXControl::SetLocalLocation(const Float2 &value)
{
    SetBounds(Rectangle(value + (_parent != nullptr ? _parent->GetBounds().Size * (_anchorMax + _anchorMin) * 0.5f : Float2::Zero) - _bounds.Size * _pivot, _bounds.Size));
}

void UIXControl::SetWidth(float value)
{
    if (Math::NearEqual(_bounds.Size.X, value))
        return;
    Rectangle bounds = Rectangle(_bounds.Location.X, _bounds.Location.Y, value, _bounds.Size.Y);
    if (_pivotRelativeSizing)
    {
        float delta = _bounds.Size.X - value;
        bounds.Location.X += delta * GetPivot().X;
    }
    SetBoundsInner(bounds);
}

void UIXControl::SetHeight(float value)
{
    if (Math::NearEqual(_bounds.Size.Y, value))
        return;
    Rectangle bounds = Rectangle(_bounds.Location.X, _bounds.Location.Y, _bounds.Size.X, value);
    if (_pivotRelativeSizing)
    {
        float delta = _bounds.Size.Y - value;
        bounds.Location.Y += delta * GetPivot().Y;
    }
    SetBoundsInner(bounds);
}

void UIXControl::SetCenter(Float2 value)
{
    SetLocation(value - GetSize() * 0.5f);
}

void UIXControl::SetBounds(const Rectangle &value)
{
    if (!_bounds.NearEqual(_bounds, value))
        SetBoundsInner(value);
}

void UIXControl::SetBoundsInner(const Rectangle &value)
{
    // Calculate anchors based on the parent container client area
    UIXMargin anchors;
    if (_parent != nullptr)
    {
        Rectangle parentBounds;
        _parent->GetDesireClientArea(parentBounds);
        anchors = UIXMargin
        (
            _anchorMin.X * parentBounds.Size.X + parentBounds.Location.X,
            _anchorMax.X * parentBounds.Size.X,
            _anchorMin.Y * parentBounds.Size.Y + parentBounds.Location.Y,
            _anchorMax.Y * parentBounds.Size.Y
        );
    }

    // Calculate offsets on X axis
    _offsets.Left = value.Location.X - anchors.Left;
    if (_anchorMin.X != _anchorMax.X)
    {
        _offsets.Right = anchors.Right - value.Location.X - value.Size.X;
    }
    else
    {
        _offsets.Right = value.Size.X;
    }

    // Calculate offsets on Y axis
    _offsets.Top = value.Location.Y - anchors.Top;
    if (_anchorMin.Y != _anchorMax.Y)
    {
        _offsets.Bottom = anchors.Bottom - value.Location.Y - value.Size.Y;
    }
    else
    {
        _offsets.Bottom = value.Size.Y;
    }

    // Flush the control bounds
    UpdateBounds();
}


void UIXControl::SetScale(const Float2 &value)
{
    if (!Math::NearEqual(_scale, value))
        SetScaleInternal(value);
}

void UIXControl::SetPivot(const Float2 &value)
{
    if (!Math::NearEqual(_pivot, value))
    {
        SetPivotInternal(value);
    }
}

void UIXControl::SetShear(const Float2 &value)
{
    if (!Math::NearEqual(_shear, value))
    {
        SetShearInternal(value);
    }
}

void UIXControl::SetRotation(float value)
{
    if (!Math::NearEqual(_rotation, value))
    {
        SetRotationInternal(value);
    }
}

void UIXControl::UpdateBounds()
{
    Rectangle prevBounds = _bounds;

    // Calculate anchors based on the parent container client area
    UIXMargin anchors;
    Float2 offset;
    if (_parent != nullptr)
    {
        Rectangle parentBounds;
        _parent->GetDesireClientArea(parentBounds);
        anchors = UIXMargin
        (
            _anchorMin.X * parentBounds.Size.X,
            _anchorMax.X * parentBounds.Size.X,
            _anchorMin.Y * parentBounds.Size.Y,
            _anchorMax.Y * parentBounds.Size.Y
        );
        offset = parentBounds.Location;
    }
    else
    {
        anchors = UIXMargin::Zero;
        offset = Float2::Zero;
    }

    // Calculate position and size on X axis
    _bounds.Location.X = anchors.Left + _offsets.Left;
    if (_anchorMin.X != _anchorMax.X)
    {
        _bounds.Size.X = anchors.Right - _bounds.Location.X - _offsets.Right;
    }
    else
    {
        _bounds.Size.X = _offsets.Right;
    }

    // Calculate position and size on Y axis
    _bounds.Location.Y = anchors.Top + _offsets.Top;
    if (_anchorMin.Y != _anchorMax.Y)
    {
        _bounds.Size.Y = anchors.Bottom - _bounds.Location.Y - _offsets.Bottom;
    }
    else
    {
        _bounds.Size.Y = _offsets.Bottom;
    }

    // Apply the offset
    _bounds.Location += offset;

    // Update cached transformation matrix
    UpdateTransform();

    // Handle location/size changes
    if (!Math::NearEqual(_bounds.Location, prevBounds.Location))
    {
        OnLocationChanged();
    }

    if (!Math::NearEqual(_bounds.Size, prevBounds.Size))
    {
        OnSizeChanged();
    }
}


void UIXControl::UpdateTransform()
{
    // Actual pivot and negative pivot
    //Float2.Multiply(ref _pivot, ref _bounds.Size, out var v1);
    //Float2.Negate(ref v1, out var v2);
    //Float2.Add(ref v1, ref _bounds.Location, out v1);
    Float2 v1 = _pivot * _bounds.Size;
    Float2 v2 = -v1;
    v1 += _bounds.Location;

    // ------ Matrix3x3 based version:

    /*
    // Negative pivot
    Matrix3x3 m1, m2;
    Matrix3x3.Translation2D(ref v2, out m1);

    // Scale
    Matrix3x3.Scaling(_scale.X, _scale.Y, 1, out m2);
    Matrix3x3.Multiply(ref m1, ref m2, out m1);

    // Shear
    Matrix3x3.Shear(ref _shear, out m2);
    Matrix3x3.Multiply(ref m1, ref m2, out m1);

    // Rotation
    Matrix3x3.RotationZ(_rotation * Mathf.DegreesToRadians, out m2);
    Matrix3x3.Multiply(ref m1, ref m2, out m1);

    // Pivot + Location
    Matrix3x3.Translation2D(ref v1, out m2);
    Matrix3x3.Multiply(ref m1, ref m2, out _cachedTransform);
    */

    // ------ Matrix2x2 based version:

    // 2D transformation

    //Matrix2x2.Scale(ref _scale, out Matrix2x2 m1);
    //Matrix2x2.Shear(ref _shear, out Matrix2x2 m2);
    //Matrix2x2.Multiply(ref m1, ref m2, out m1);

    // Scale and Shear
    Matrix3x3 m1 = Matrix3x3
    (
        _scale.X,
        _scale.X * (_shear.Y == 0 ? 0 : (1.0f / Math::Tan(DegreesToRadians * (90 - Math::Clamp(_shear.Y, -89.0f, 89.0f))))),
        0,
        _scale.Y * (_shear.X == 0 ? 0 : (1.0f / Math::Tan(DegreesToRadians * (90 - Math::Clamp(_shear.X, -89.0f, 89.0f))))),
        _scale.Y,
        0, 0, 0, 1
    );


    //Matrix2x2.Rotation(Mathf.DegreesToRadians * _rotation, out m2);
    float sin = Math::Sin(DegreesToRadians * _rotation);
    float cos = Math::Cos(DegreesToRadians * _rotation);

    //Matrix2x2.Multiply(ref m1, ref m2, out m1);
    m1.M11 = (_scale.X * cos) + (m1.M12 * -sin);
    m1.M12 = (_scale.X * sin) + (m1.M12 * cos);
    float m21 = (m1.M21 * cos) + (_scale.Y * -sin);
    m1.M22 = (m1.M21 * sin) + (_scale.Y * cos);
    m1.M21 = m21;
    // Mix all the stuff
    //Matrix3x3.Translation2D(ref v2, out Matrix3x3 m3);
    //Matrix3x3 m4 = (Matrix3x3)m1;
    //Matrix3x3.Multiply(ref m3, ref m4, out m3);
    //Matrix3x3.Translation2D(ref v1, out m4);
    //Matrix3x3.Multiply(ref m3, ref m4, out _cachedTransform);
    m1.M31 = (v2.X * m1.M11) + (v2.Y * m1.M21) + v1.X;
    m1.M32 = (v2.X * m1.M12) + (v2.Y * m1.M22) + v1.Y;
    _cachedTransform = m1;

    // Cache inverted transform
    Matrix3x3::Invert(_cachedTransform, _cachedTransformInv);
}

void UIXControl::SetAnchorPreset(const UIXAnchorPresets &anchorPreset, bool preserveBounds, bool setPivotToo)
{
    for (int i = 0, siz = ANCHOR_PRESET_DATA_COUNT; i < siz; i++)
    {
        if (AnchorPresetsData[i].Preset == anchorPreset)
        {
            Float2 anchorMin = AnchorPresetsData[i].Min;
            Float2 anchorMax = AnchorPresetsData[i].Max;
            Rectangle bounds = _bounds;
            if (!Float2::NearEqual(_anchorMin, anchorMin) ||
                !Float2::NearEqual(_anchorMax, anchorMax))
            {
                // Disable scrolling for anchored controls (by default but can be manually restored)
                if (!anchorMin.IsZero() || !anchorMax.IsZero())
                    _isScrollable = false;

                _anchorMin = anchorMin;
                _anchorMax = anchorMax;
                if (preserveBounds)
                {
                    UpdateBounds();
                    SetBounds(bounds);
                }
            }
            if (!preserveBounds)
            {
                if (_parent != nullptr)
                {
                    Rectangle parentBounds;
                    _parent->GetDesireClientArea(parentBounds);
                    switch (anchorPreset)
                    {
                        case UIXAnchorPresets::TopLeft:
                            bounds.Location = Float2::Zero;
                            break;
                        case UIXAnchorPresets::TopCenter:
                            bounds.Location = Float2(parentBounds.GetWidth() * 0.5f - bounds.GetWidth() * 0.5f, 0);
                            break;
                        case UIXAnchorPresets::TopRight:
                            bounds.Location = Float2(parentBounds.GetWidth() - bounds.GetWidth(), 0);
                            break;
                        case UIXAnchorPresets::MiddleLeft:
                            bounds.Location = Float2(0, parentBounds.GetHeight() * 0.5f - bounds.GetHeight() * 0.5f);
                            break;
                        case UIXAnchorPresets::MiddleCenter:
                            bounds.Location = Float2(parentBounds.GetWidth() * 0.5f - bounds.GetWidth() * 0.5f, parentBounds.GetHeight() * 0.5f - bounds.GetHeight() * 0.5f);
                            break;
                        case UIXAnchorPresets::MiddleRight:
                            bounds.Location = Float2(parentBounds.GetWidth() - bounds.GetWidth(), parentBounds.GetHeight() * 0.5f - bounds.GetHeight() * 0.5f);
                            break;
                        case UIXAnchorPresets::BottomLeft:
                            bounds.Location = Float2(0, parentBounds.GetHeight() - bounds.GetHeight());
                            break;
                        case UIXAnchorPresets::BottomCenter:
                            bounds.Location = Float2(parentBounds.GetWidth() * 0.5f - bounds.GetWidth() * 0.5f, parentBounds.GetHeight() - bounds.GetHeight());
                            break;
                        case UIXAnchorPresets::BottomRight:
                            bounds.Location = Float2(parentBounds.GetWidth() - bounds.GetWidth(), parentBounds.GetHeight() - bounds.GetHeight());
                            break;
                        case UIXAnchorPresets::VerticalStretchLeft:
                            bounds.Location = Float2::Zero;
                            bounds.Size = Float2(bounds.GetWidth(), parentBounds.GetHeight());
                            break;
                        case UIXAnchorPresets::VerticalStretchCenter:
                            bounds.Location = Float2(parentBounds.GetWidth() * 0.5f - bounds.GetWidth() * 0.5f, 0);
                            bounds.Size = Float2(bounds.GetWidth(), parentBounds.GetHeight());
                            break;
                        case UIXAnchorPresets::VerticalStretchRight:
                            bounds.Location = Float2(parentBounds.GetWidth() - bounds.GetWidth(), 0);
                            bounds.Size = Float2(bounds.GetWidth(), parentBounds.GetHeight());
                            break;
                        case UIXAnchorPresets::HorizontalStretchTop:
                            bounds.Location = Float2::Zero;
                            bounds.Size = Float2(parentBounds.GetWidth(), bounds.GetHeight());
                            break;
                        case UIXAnchorPresets::HorizontalStretchMiddle:
                            bounds.Location = Float2(0, parentBounds.GetHeight() * 0.5f - bounds.GetHeight() * 0.5f);
                            bounds.Size = Float2(parentBounds.GetWidth(), bounds.GetHeight());
                            break;
                        case UIXAnchorPresets::HorizontalStretchBottom:
                            bounds.Location = Float2(0, parentBounds.GetHeight() - bounds.GetHeight());
                            bounds.Size = Float2(parentBounds.GetWidth(), bounds.GetHeight());
                            break;
                        case UIXAnchorPresets::StretchAll:
                            bounds.Location = Float2::Zero;
                            bounds.Size = parentBounds.Size;
                            break;
                        default:
                            LOG(Error, "ArguementOutOfRangeException: anchorPreset has incorrect value {0}!", (int)anchorPreset);
                            return;
                    }
                    bounds.Location += parentBounds.Location;
                }
                SetBounds(bounds);
            }
            if (setPivotToo)
            {
                SetPivot((anchorMin + anchorMax) / 2.f);
            }
            if (_parent != nullptr)
                _parent->PerformLayout();
            return;
        }
    }
}

// Hack to check equality between bindings.
bool operator==(const UIXControl::UpdateDelegate &a, const UIXControl::UpdateDelegate &b)
{
    if (a.Count() != b.Count())
        return false;

    Array<UIXControl::UpdateDelegate::FunctionType> abindings;
    abindings.Resize(a.Count());
    int tmp = a.GetBindings(abindings.Get(), abindings.Count());
    Array<UIXControl::UpdateDelegate::FunctionType> bbindings;
    bbindings.Resize(b.Count());
    int tmp2 = b.GetBindings(bbindings.Get(), bbindings.Count());

    if (tmp != tmp2)
        return false;

    for (int ax = 0, asiz = abindings.Count(); ax < asiz; ++ax)
    {
        bool found = false;
        for (int bx = 0, bsiz = bbindings.Count(); bx < bsiz && !found; ++bx)
        {
            if (abindings[ax] == bbindings[bx])
                found = true;
        }
        if (!found)
            return false;
    }

    return true;
}

