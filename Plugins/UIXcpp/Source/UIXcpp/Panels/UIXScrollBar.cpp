#include "UIXScrollBar.h"
#include "../UIXContainerControl.h"
#include "../UIXScrollableControl.h"
#include "../UIXRootControl.h"
#include "../UIXStyle.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Math/Math.h"
#include "Engine/Render2D/Render2D.h"



float UIXScrollBar::DefaultSize = 14;
float UIXScrollBar::DefaultMinimumOpacity = 0.75f;

UIXScrollBar::UIXScrollBar(/*const SpawnParams &params,*/ UIXOrientation orientation) : UIXControl(), _orientation(orientation)
{
    SetAutoFocus(false);   
}

void UIXScrollBar::SetThumbThickness(float value)
{
    _thumbThickness = value;
}

void UIXScrollBar::SetTrackThickness(float value)
{
    _trackThickness = value;
}

void UIXScrollBar::SetScrollAnimationDuration(float value)
{
    _scrollAnimationDuration = value;
}

void UIXScrollBar::SetEnableSmoothing(bool value)
{
    _enableSmoothing = value;
}

void UIXScrollBar::SetMinimum(float value)
{
    if (value > _maximum)
    {
        LOG(Error, "ArgumentOutOfRangeException: New minimum scrollbar value outside range.");
        value = _maximum;
    }
    _minimum = value;
    if (GetValue() < _minimum)
        SetValue(_minimum);
}

void UIXScrollBar::SetMaximum(float value)
{
    if (value < _minimum)
    {
        LOG(Error, "ArgumentOutOfRangeException: New maximum scrollbar value outside range.");
        value = _minimum;
    }

    _maximum = value;
    if (GetValue() > _maximum)
        SetValue(_maximum);
}

void UIXScrollBar::SetValue(float value)
{
    value = Math::Clamp(value, _minimum, _maximum);
    if (!Math::NearEqual(value, _targetValue))
    {
        _targetValue = value;
        _startValue = _value;
        _scrollAnimationProgress = 0.0f;

        // Check if skip smoothing
        if (!GetUseSmoothing())
        {
            _value = value;
            _startValue = value;
            _scrollAnimationProgress = 1.0f;
            OnValueChanged();
        }
        else
        {
            UpdateDelegate update_delegate;
            update_delegate.Bind<UIXScrollBar, &UIXScrollBar::OnUpdate>(this);
            SetUpdate(_update, update_delegate);
        }
    }
}

void UIXScrollBar::SetTargetValue(float value)
{
    value = Math::Clamp(value, _minimum, _maximum);
    if (!Math::NearEqual(value, _targetValue))
    {
        _targetValue = value;
        _value = value;
        SetUpdate(_update, UpdateDelegate());
        OnValueChanged();
    }
}

void UIXScrollBar::SetScrollSpeedWheel(float value)
{
    _scrollChange = value;
}

void UIXScrollBar::SetScrollSpeedClick(float value)
{
    _clickChange = value;
}

void UIXScrollBar::FastScroll()
{
    if (!Math::NearEqual(_value, _targetValue))
    {
        _value = _targetValue = _startValue;
        _scrollAnimationProgress = 0.0f;
        SetUpdate(_update, UpdateDelegate());
        OnValueChanged();
    }
}

void UIXScrollBar::ScrollViewTo(float min, float max, bool fastScroll)
{
    // Check if we need to change view
    float viewMin = _value;
    float viewSize = GetTrackSize();
    float viewMax = viewMin + viewSize;
    if (Math::IsNotInRange(min, viewMin, viewMax))
    {
        if (fastScroll)
            SetTargetValue(min);
        else
            SetValue(min);
    }

    // Commented out in original C# code
    /*else if (Mathf.IsNotInRange(max, viewMin, viewMax))
    {
        Value = max - viewSize;
    }*/
}

void UIXScrollBar::UpdateThumb()
{
    // Cache data
    float width = GetWidth();
    float height = GetHeight();
    float trackSize = GetTrackSize();
    float range = _maximum - _minimum;

    _thumbSize = Math::Min(trackSize - 10, Math::Max(trackSize / range * 100.0f, 50.0f));

    float pixelRange = trackSize - _thumbSize;
    float percentage = (_value - _minimum) / range;
    int thumbPosition = (int)(percentage * pixelRange);

    _thumbCenter = thumbPosition + _thumbSize / 2;

    _thumbRect = _orientation == UIXOrientation::Vertical
        ? Rectangle((width - GetThumbThickness()) / 2.0f, thumbPosition + 4.0f, GetThumbThickness(), _thumbSize - 8.0f)
        : Rectangle(thumbPosition + 4.0f, (height - GetThumbThickness()) / 2.0f, _thumbSize - 8.0f, GetThumbThickness());

    _trackRect = _orientation == UIXOrientation::Vertical
        ? Rectangle((width - GetTrackThickness()) / 2.0f, 4.0f, GetTrackThickness(), height - 8.0f)
        : Rectangle(4.0f, (height - GetTrackThickness()) / 2.0f, width - 8.0f, GetTrackThickness());
}

void UIXScrollBar::EndTracking()
{
    // Check flag
    if (_thumbClicked)
    {
        // Clear flag
        _thumbClicked = false;

        // End capturing mouse
        EndMouseCapture();
    }
}

void UIXScrollBar::Reset()
{
    _value = _targetValue = _startValue = 0;
    _scrollAnimationProgress = 0.0f;
}

void UIXScrollBar::OnValueChanged()
{
    UpdateThumb();

    if (ValueChanged.IsBinded())
        ValueChanged();
}

void UIXScrollBar::OnUpdate(float deltaTime)
{
    bool isDeltaSlow = deltaTime > (1 / 20.0f);

    // Opacity smoothing
    float targetOpacity = GetIsMouseOver() ? 1.0f : DefaultMinimumOpacity;
    _thumbOpacity = isDeltaSlow ? targetOpacity : Math::Lerp(_thumbOpacity, targetOpacity, deltaTime * 10.0f);
    bool needUpdate = Math::Abs(_thumbOpacity - targetOpacity) > 0.001f;

    // Ensure scroll bar is visible and smoothing is required
    if (GetVisible() && Math::Abs(_targetValue - _value) > 0.01f)
    {
        // Interpolate or not if running slow
        float value;
        if (!isDeltaSlow && GetUseSmoothing())
        {
            // percentage of scroll from 0 to _scrollChange, ex. 0.5 at _scrollChange / 2
            float minScrollChangeRatio = Math::Clamp(Math::Abs(_targetValue - _startValue) / _scrollChange, 0.0f, 1.f);

            // shorten the duration if we scrolled less than _scrollChange
            float actualDuration = GetScrollAnimationDuration() * minScrollChangeRatio;
            float step = deltaTime / actualDuration;

            float progress = _scrollAnimationProgress;
            progress = Math::Clamp(progress + step, 0.f, 1.f);

            // https://easings.net/#easeOutSine
            float easedProgress = Math::Sin((progress * PI) / 2.f);
            value = Math::Lerp(_startValue, _targetValue, easedProgress);

            _scrollAnimationProgress = progress;
        }
        else
        {
            value = _targetValue;
            _startValue = _targetValue;
            _scrollAnimationProgress = 0.f;
        }

        _value = value;
        OnValueChanged();
        needUpdate = true;
    }

    // End updating if all animations are done
    if (!needUpdate)
    {
        SetUpdate(_update, UpdateDelegate());
    }
}

void UIXScrollBar::SetScrollRange(float minimum, float maximum)
{
    if (minimum > maximum)
    {
        LOG(Error, "ArgumentOutOfRangeException: minimum range over maximum.");
        minimum = maximum;
    }

    _minimum = minimum;
    _maximum = maximum;

    if (GetValue() < minimum)
        SetValue(minimum);
    else if (GetValue() > maximum)
        SetValue(maximum);

    UpdateThumb();
}

void UIXScrollBar::Draw()
{
    UIXControl::Draw();

    const UIXStyle *style = UIXStyle::GetCurrent();
    Render2D::FillRectangle(_trackRect, style->BackgroundHighlighted * _thumbOpacity);
    Render2D::FillRectangle(_thumbRect, (_thumbClicked ? style->BackgroundSelected : style->BackgroundNormal) * _thumbOpacity);
}

void UIXScrollBar::OnMouseMove(Float2 location)
{
    if (_thumbClicked)
    {
        Float2 slidePosition = location + GetRoot()->GetTrackingMouseOffset();

        // TODO: fix all dynamic casts with scripting object api
        UIXScrollableControl *p = dynamic_cast<UIXScrollableControl*>(GetParent());
        if (p != nullptr)
            slidePosition += p->GetViewOffset(); // Hardcoded fix

        float mousePosition = _orientation == UIXOrientation::Vertical ? slidePosition.Y : slidePosition.X;

        float percentage = (mousePosition - _mouseOffset - _thumbSize / 2) / (GetTrackSize() - _thumbSize);
        SetTargetValue(_minimum + percentage * (_maximum - _minimum));
    }
}

bool UIXScrollBar::OnMouseWheel(Float2 location, float delta)
{
    if (ThumbEnabled)
    {
        // Scroll
        SetValue(_targetValue - delta * _scrollChange);
    }
    return true;
}

bool UIXScrollBar::OnMouseDown(Float2 location, MouseButton button)
{
    if (button == MouseButton::Left && ThumbEnabled)
    {
        // Remove focus
        UIXRootControl *root = GetRoot();
        auto f = root->GetFocusedControl();
        if (f != nullptr)
            f->Defocus();

        float mousePosition = _orientation == UIXOrientation::Vertical ? location.Y : location.X;

        if (_thumbRect.Contains(location))
        {
            // Start moving thumb
            _thumbClicked = true;
            _mouseOffset = mousePosition - _thumbCenter;

            // Start capturing mouse
            StartMouseCapture();
        }
        else
        {
            // Click change
            SetValue(_value + (mousePosition < _thumbCenter ? -1 : 1) * _clickChange);
        }
    }

    return UIXControl::OnMouseDown(location, button);
}


bool UIXScrollBar::OnMouseUp(Float2 location, MouseButton button)
{
    EndTracking();

    return UIXControl::OnMouseUp(location, button);
}

void UIXScrollBar::OnEndMouseCapture()
{
    EndTracking();
}

void UIXScrollBar::OnMouseEnter(Float2 location)
{
    UIXControl::OnMouseEnter(location);

    UpdateDelegate update_delegate;
    update_delegate.Bind<UIXScrollBar, &UIXScrollBar::OnUpdate>(this);

    SetUpdate(_update, update_delegate);
}

void UIXScrollBar::OnMouseLeave()
{
    UIXControl::OnMouseLeave();

    UpdateDelegate update_delegate;
    update_delegate.Bind<UIXScrollBar, &UIXScrollBar::OnUpdate>(this);

    SetUpdate(_update, update_delegate);
}

void UIXScrollBar::OnSizeChanged()
{
    UIXControl::OnSizeChanged();

    UpdateThumb();
}

void UIXScrollBar::AddUpdateCallbacks(UIXRootControl *root)
{
    UIXControl::AddUpdateCallbacks(root);

    if (_update.IsBinded())
        GetRoot()->UpdateCallbacksToAdd.Add(_update);
}


void UIXScrollBar::RemoveUpdateCallbacks(UIXRootControl *root)
{
    UIXControl::RemoveUpdateCallbacks(root);

    if (_update.IsBinded())
        GetRoot()->UpdateCallbacksToRemove.Add(_update);
}
