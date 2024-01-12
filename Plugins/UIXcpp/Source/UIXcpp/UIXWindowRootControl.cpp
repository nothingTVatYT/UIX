#include "UIXWindowRootControl.h"
#include "UIXStyle.h"

UIXWindowRootControl::UIXWindowRootControl(Window *window) : UIXRootControl()
{
    _window = window;
    SetClipChildren(false);
    if (UIXStyle::GetCurrent() != nullptr)
        SetBackgroundColor(UIXStyle::GetCurrent()->Background);
}

Window* UIXWindowRootControl::GetWindow() const
{
    return _window;
}

String UIXWindowRootControl::GetTitle() const
{
    return _window->GetTitle();
}

void UIXWindowRootControl::SetTitle(String value)
{
    _window->SetTitle(value);
}

bool UIXWindowRootControl::GetIsFullscreen() const
{
    return _window->IsFullscreen();
}

bool UIXWindowRootControl::GetIsWindowed() const
{
    return _window->IsWindowed();
}

bool UIXWindowRootControl::GetIsShown() const
{
    return _window->IsVisible();
}

bool UIXWindowRootControl::GetIsMinimized() const
{
    return _window->IsMinimized();
}

bool UIXWindowRootControl::GetIsMaximized() const
{
    return _window->IsMaximized();
}


void UIXWindowRootControl::Show()
{
    _window->Show();
}


void UIXWindowRootControl::Hide()
{
    _window->Show();
}

void UIXWindowRootControl::Minimize()
{
    _window->Minimize();
}

void UIXWindowRootControl::Maximize()
{
    _window->Maximize();
}

void UIXWindowRootControl::Restore()
{
    _window->Restore();
}

void UIXWindowRootControl::Close(ClosingReason reason)
{
    _window->Close(reason);
}

void UIXWindowRootControl::BringToFront(bool force)
{
    _window->BringToFront(force);
}

 void UIXWindowRootControl::FlashWindow()
{
    _window->FlashWindow();
}

void UIXWindowRootControl::SetFocusedControl(UIXControl *value)
{
    ASSERT(_focusedControl == nullptr || _focusedControl->GetRoot() == this, "Invalid control to focus");
    Focus(value);
}

CursorType UIXWindowRootControl::GetCursor() const
{
    return _window->GetCursor();
}

void UIXWindowRootControl::SetCursor(CursorType value)
{
    _window->SetCursor(value);
}

Float2 UIXWindowRootControl::GetTrackingMouseOffset() const
{
    return _window->GetTrackingMouseOffset() / _window->GetDpiScale();
}


Float2 UIXWindowRootControl::GetMousePosition() const
{
    return _window->GetMousePosition() / _window->GetDpiScale();
}

void UIXWindowRootControl::SetMousePosition(Float2 value)
{
    _window->SetMousePosition(value * _window->GetDpiScale());
}

void UIXWindowRootControl::StartTrackingMouse(UIXControl *control, bool useMouseScreenOffset)
{
    if (control == nullptr)
        throw new ArgumentNullException();
    if (_trackingControl == control)
        return;

    if (_trackingControl != nullptr)
        EndTrackingMouse();

    if (control->GetAutoFocus())
        Focus(control);

    _trackingControl = control;

    _window->StartTrackingMouse(useMouseScreenOffset);
}

void UIXWindowRootControl::EndTrackingMouse()
{
    if (_trackingControl != nullptr)
    {
        UIXControl *control = _trackingControl;
        _trackingControl = nullptr;
        control->OnEndMouseCapture();

        _window->EndTrackingMouse();
    }
}

bool UIXWindowRootControl::GetKey(KeyboardKeys key)
{
    return _window->GetKey(key);
}


bool UIXWindowRootControl::GetKeyDown(KeyboardKeys key)
{
    return _window->GetKeyDown(key);
}

/// <inheritdoc />
bool UIXWindowRootControl::GetKeyUp(KeyboardKeys key)
{
    return _window->GetKeyUp(key);
}

/// <inheritdoc />
bool UIXWindowRootControl::GetMouseButton(MouseButton button)
{
    return _window->GetMouseButton(button);
}

/// <inheritdoc />
bool UIXWindowRootControl::GetMouseButtonDown(MouseButton button)
{
    return _window->GetMouseButtonDown(button);
}

/// <inheritdoc />
bool UIXWindowRootControl::GetMouseButtonUp(MouseButton button)
{
    return _window->GetMouseButtonUp(button);
}

/// <inheritdoc />
Float2 UIXWindowRootControl::PointFromScreen(Float2 location) const
{
    return _window->ScreenToClient(location) / _window->GetDpiScale();
}

/// <inheritdoc />
Float2 UIXWindowRootControl::PointToScreen(Float2 location) const
{
    return _window->ClientToScreen(location * _window->GetDpiScale());
}

/// <inheritdoc />
void UIXWindowRootControl::Focus()
{
    _window->Focus();
}

/// <inheritdoc />
void UIXWindowRootControl::DoDragDrop(const DragData &data)
{
    _window->DoDragDrop(data);
}


/// <inheritdoc />
bool UIXWindowRootControl::OnMouseDown(Float2 location, MouseButton button)
{
    if (_trackingControl != nullptr)
    {
        return _trackingControl->OnMouseDown(_trackingControl->PointFromWindow(location), button);
    }

    return UIXRootControl::OnMouseDown(location, button);
}

/// <inheritdoc />
bool UIXWindowRootControl::OnMouseUp(Float2 location, MouseButton button)
{
    if (_trackingControl != nullptr)
    {
        return _trackingControl->OnMouseUp(_trackingControl->PointFromWindow(location), button);
    }

    return UIXRootControl::OnMouseUp(location, button);
}

/// <inheritdoc />
bool UIXWindowRootControl::OnMouseDoubleClick(Float2 location, MouseButton button)
{
    if (_trackingControl != nullptr)
    {
        return _trackingControl->OnMouseDoubleClick(_trackingControl->PointFromWindow(location), button);
    }

    return UIXRootControl::OnMouseDoubleClick(location, button);
}

/// <inheritdoc />
bool UIXWindowRootControl::OnMouseWheel(Float2 location, float delta)
{
    if (_trackingControl != nullptr)
    {
        return _trackingControl->OnMouseWheel(_trackingControl->PointFromWindow(location), delta);
    }

    return UIXRootControl::OnMouseWheel(location, delta);
}

/// <inheritdoc />
void UIXWindowRootControl::OnMouseMove(Float2 location)
{
    if (_trackingControl != nullptr)
    {
        _trackingControl->OnMouseMove(_trackingControl->PointFromWindow(location));
        return;
    }

    UIXRootControl::OnMouseMove(location);
}

bool UIXWindowRootControl::Focus(UIXControl *c)
{
    if (GetIsDisposing() || _focusedControl == c)
        return false;

    // Change focused control
    UIXControl *previous = _focusedControl;
    _focusedControl = c;

    // Fire events
    if (previous != nullptr)
    {
        previous->OnLostFocus();
        ASSERT(previous->GetIsFocused());
    }
    if (_focusedControl != nullptr)
    {
        _focusedControl->OnGotFocus();
        ASSERT(!_focusedControl->GetIsFocused());
    }

    // Update flags
    UpdateContainsFocus();

    return true;
}
