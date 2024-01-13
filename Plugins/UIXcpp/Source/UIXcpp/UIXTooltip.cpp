#include "UIXTooltip.h"
#include "UIXControl.h"
#include "UIXWindowRootControl.h"
#include "UIXStyle.h"
#include "Engine/Input/Input.h"
#include "Engine/Render2D/Render2D.h"
#include "Engine/Render2D/Font.h"
#include "Engine/Render2D/TextLayoutOptions.h"
#include "Engine/Core/Log.h"


UIXTooltip::UIXTooltip(const SpawnParams &params) : UIXContainerControl(params, 0, 0, 300, 24), _timeToShow(0.3f), _maxWidth(500.0f), _timeToPopupLeft(0.0f)
{
    SetVisible(false);
    SetAutoFocus(false);
}


void UIXTooltip::Show(UIXControl *target, const Float2 &location, const Rectangle &targetArea)
{
    if (target == nullptr)
    {
        LOG(Error, "ArgumentNullException: Target for showing tooltip is null.");
        return;
    }

    // Ensure to be closed
    Hide();

    // Block showing tooltips when application is not focused
    if (!Platform::GetHasFocus())
        return;

    // Unlock and perform controls update
    UnlockChildrenRecursive();
    PerformLayout();

    // Calculate popup direction and initial location
    UIXRootControl *parentWin = target->GetRoot();
    if (parentWin == nullptr)
        return;
    float dpiScale = target->GetRootWindow()->GetDpiScale();
    Float2 dpiSize = GetSize() * dpiScale;
    Float2 locationWS = target->PointToWindow(location);
    Float2 locationSS = parentWin->PointToScreen(locationWS);
    _showTarget = target;
    WrapPosition(locationSS);

    // Create window
    CreateWindowSettings desc;
    desc.StartPosition = WindowStartPosition::Manual;
    desc.Position = locationSS;
    desc.Size = dpiSize;
    desc.Fullscreen = false;
    desc.HasBorder = false;
    desc.SupportsTransparency = false;
    desc.ShowInTaskbar = false;
    desc.ActivateWhenFirstShown = false;
    desc.AllowInput = false;
    desc.AllowMinimize = false;
    desc.AllowMaximize = false;
    desc.AllowDragAndDrop = false;
    desc.IsTopmost = true;
    desc.IsRegularWindow = false;
    desc.HasSizingFrame = false;
    desc.ShowAfterFirstPaint = true;
    _window = Platform::CreateWindow(desc);
    if (_window == nullptr)
    {
        LOG(Error, "InvalidOperationException: Failed to create tooltip window.");
        return;
    }

    // Attach to the window and focus
    // TODO: this is used with platform windows, not game window. It's hardcoded to
    // one of the original UI classes and cannot be changed, unless we rewrite the engine.
    // We will have to change this to a parent we can control that's limited to inside the game.
    SetParent(_window->GUI);

    SetVisible(true);
    _window->Show();
    _showTarget->OnTooltipShown(this);
}

void UIXTooltip::Hide()
{
    if (!GetVisible())
        return;

    // Unlink
    SetIsLayoutLocked(true);
    SetParent(nullptr);

    // Close window
    if (_window)
    {
        Window *win = _window;
        _window = nullptr;
        win->Close();
    }

    // Hide
    SetVisible(false);
}


void UIXTooltip::OnMouseEnterControl(UIXControl *target)
{
    _lastTarget = target;
    _timeToPopupLeft = _timeToShow;
}

void UIXTooltip::OnMouseOverControl(UIXControl *target, float dt)
{
    if (!GetVisible() && _timeToPopupLeft > 0.0f)
    {
        _lastTarget = target;
        _timeToPopupLeft -= dt;

        if (_timeToPopupLeft <= 0.0f)
        {
            Float2 location;
            Rectangle area;
            if (_lastTarget->OnShowTooltip(_currentText, location, area))
            {
                Show(_lastTarget, location, area);
            }
        }
    }
}

void UIXTooltip::OnMouseLeaveControl(UIXControl *target)
{
    if (GetVisible())
        Hide();
    _lastTarget = nullptr;
}


void UIXTooltip::UpdateWindowSize()
{
    if (_window)
    {
        _window->GetClientSize() = GetSize() * _window->GetDpiScale();
    }
}

void UIXTooltip::WrapPosition(Float2 &locationSS, float flipOffset)
{
    if (!_showTarget || _showTarget->GetRootWindow() == nullptr)
        return;

    // Calculate popup direction
    float dpiScale = _showTarget->GetRootWindow()->GetDpiScale();
    Float2 dpiSize = GetSize() * dpiScale;
    Rectangle monitorBounds = Platform::GetMonitorBounds(locationSS);
    Float2 rightBottomMonitorBounds = monitorBounds.GetBottomRight();
    Float2 rightBottomLocationSS = locationSS + dpiSize;

    // Prioritize tooltip placement within parent window, fall back to virtual desktop
    if (rightBottomMonitorBounds.Y < rightBottomLocationSS.Y)
    {
        // Direction: up
        locationSS.Y -= dpiSize.Y + flipOffset;
    }
    if (rightBottomMonitorBounds.X < rightBottomLocationSS.X)
    {
        // Direction: left
        locationSS.X -= dpiSize.X + flipOffset * 2;
    }
}


void UIXTooltip::Update(float deltaTime)
{
    Float2 mousePos = Input::GetMouseScreenPosition();
    Float2 location = _showTarget->PointFromScreen(mousePos);
    if (!_showTarget->OnTestTooltipOverControl(location))
    {
        // Auto hide if mouse leaves control area
        Hide();
    }
    else
    {
        // Position tooltip when mouse moves
        WrapPosition(mousePos, 10);
        if (_window)
            _window->GetPosition() = mousePos + Float2(15, 10);
    }

    UIXContainerControl::Update(deltaTime);
}

void UIXTooltip::Draw()
{
    const UIXStyle *style = UIXStyle::GetCurrent();

    // Background
    Render2D::FillRectangle(Rectangle(Float2::Zero, GetSize()), Color::Lerp(style->BackgroundSelected, style->Background, 0.6f));
    Render2D::FillRectangle(Rectangle(1.1f, 1.1f, GetWidth() - 2, GetHeight() - 2), style->Background);

    // Tooltip text
    TextLayoutOptions layout;
    layout.HorizontalAlignment = TextAlignment::Center;
    layout.VerticalAlignment = TextAlignment::Center;
    layout.TextWrapping = TextWrapping::WrapWords;
    layout.Bounds = GetClientArea();

    Render2D::DrawText(
        style->GetFontMedium(),
        _currentText,
        style->Foreground,
        layout
    );
}


void UIXTooltip::PerformLayoutBeforeChildren()
{
    UIXContainerControl::PerformLayoutBeforeChildren();

    Float2 prevSize = GetSize();
    const UIXStyle *style = UIXStyle::GetCurrent();

    // Calculate size of the tooltip
    Float2 size = Float2::Zero;
    if (style != nullptr && style->GetFontMedium() && !_currentText.IsEmpty())
    {
        TextLayoutOptions layout = TextLayoutOptions();
        layout.Bounds = Rectangle(0, 0, _maxWidth, 10000000);
        layout.HorizontalAlignment = TextAlignment::Center;
        layout.VerticalAlignment = TextAlignment::Center;
        layout.TextWrapping = TextWrapping::WrapWords;
        Array<FontLineCache> items = style->GetFontMedium()->ProcessText(_currentText, layout);
        for (int i = 0; i < items.Count(); i++)
        {
            FontLineCache &item = items[i];
            size.X = size.X > item.Size.X + 8.0f ? size.X : item.Size.X + 8.0f;
            size.Y += item.Size.Y;
        }
        //size.X += style->FontMedium.MeasureText(_currentText).X;
    }
    GetSize() = size + Float2(24.0f);

    // Check if is visible size get changed
    if (GetVisible() && prevSize != GetSize())
    {
        UpdateWindowSize();
    }
}

bool UIXTooltip::OnShowTooltip(API_PARAM(Out) String &text, API_PARAM(Out) Float2 &location, API_PARAM(Out) Rectangle &area)
{
    UIXContainerControl::OnShowTooltip(text, location, area);

    // It's better not to show tooltip for a tooltip.
    // It would be kind of tooltipness.
    return false;
}

void UIXTooltip::OnDestroy()
{
    Hide();

    UIXContainerControl::OnDestroy();
}
