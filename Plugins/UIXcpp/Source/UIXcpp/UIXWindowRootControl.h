#pragma once

#include "UIXRootControl.h"
#include "Engine/Platform/Window.h"


/// <summary>
/// Root control implementation used by the <see cref="FlaxEngine.Window"/>.
/// </summary>
/// <seealso cref="FlaxEngine.GUI.RootControl" />
API_CLASS(Attributes="HideInEditor") class UIXCPP_API UIXWindowRootControl final : public UIXRootControl
{
public:
    /// <summary>
    /// Gets the native window object.
    /// </summary>
    API_PROPERTY()
    Window* GetWindow() const;

    /// <summary>
    /// Gets the window title.
    /// </summary>
    API_PROPERTY()
    String GetTitle() const;

    /// <summary>
    /// Sets the window title.
    /// </summary>
    API_PROPERTY()
    void SetTitle(String value);

    /// <summary>
    /// Gets a value indicating whether this window is in fullscreen mode.
    /// </summary>
    API_PROPERTY()
    bool GetIsFullscreen() const;

    /// <summary>
    /// Gets a value indicating whether this window is in windowed mode.
    /// </summary>
    API_PROPERTY()
    bool GetIsWindowed() const;

    /// <summary>
    /// Gets a value indicating whether this instance is visible.
    /// </summary>
    API_PROPERTY()
    bool GetIsShown() const;

    /// <summary>
    /// Gets a value indicating whether this window is minimized.
    /// </summary>
    API_PROPERTY()
    bool GetIsMinimized() const;

    /// <summary>
    /// Gets a value indicating whether this window is maximized.
    /// </summary>
    API_PROPERTY()
    bool GetIsMaximized() const;

    /// <summary>
    /// Shows the window.
    /// </summary>
    
    API_FUNCTION() void Show();

    /// <summary>
    /// Hides the window.
    /// </summary>
    API_FUNCTION() void Hide();

    /// <summary>
    /// Minimizes the window.
    /// </summary>
    API_FUNCTION() void Minimize();

    /// <summary>
    /// Maximizes the window.
    /// </summary>
    API_FUNCTION() void Maximize();

    /// <summary>
    /// Restores the window state before minimizing or maximizing.
    /// </summary>
    API_FUNCTION() void Restore();

    /// <summary>
    /// Closes the window.
    /// </summary>
    /// <param name="reason">The closing reason.</param>
    API_FUNCTION() void Close(ClosingReason reason = ClosingReason::CloseEvent);
    
    /// <summary>
    /// Brings window to the front of the Z order.
    /// </summary>
    /// <param name="force">True if move to the front by force, otherwise false.</param>
    API_FUNCTION() void BringToFront(bool force = false);


    /// <summary>
    /// Flashes the window to bring use attention.
    /// </summary>
    API_FUNCTION() void FlashWindow();

    /// <summary>
    /// Gets or sets the current focused control
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE UIXControl* GetFocusedControl() const override { return _focusedControl; }
    API_PROPERTY()
    void SetFocusedControl(UIXControl *value) override;
   

    /// <inheritdoc />
    API_PROPERTY()
    CursorType GetCursor() const override;

    /// <inheritdoc />
    API_PROPERTY()
    void SetCursor(CursorType value) override;

    /// <inheritdoc />
    API_PROPERTY()
    Float2 GetTrackingMouseOffset() const override;

    /// <inheritdoc />
    API_PROPERTY()
    UIXWindowRootControl* GetRootWindow() const override { return const_cast<UIXWindowRootControl*>(this); }

    /// <inheritdoc />
    API_PROPERTY()
    Float2 GetMousePosition() const override;
   
    /// <inheritdoc />
    API_PROPERTY()
    void SetMousePosition(Float2 value) override;


    /// <inheritdoc />
    API_FUNCTION() void StartTrackingMouse(UIXControl *control, bool useMouseScreenOffset) override;

    /// <inheritdoc />
    API_FUNCTION() void EndTrackingMouse() override;

    /// <inheritdoc />
    API_FUNCTION() bool GetKey(KeyboardKeys key) override;

    /// <inheritdoc />
    API_FUNCTION() bool GetKeyDown(KeyboardKeys key) override;

    /// <inheritdoc />
    API_FUNCTION() bool GetKeyUp(KeyboardKeys key) override;

    /// <inheritdoc />
    API_FUNCTION() bool GetMouseButton(MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION() bool GetMouseButtonDown(MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION() bool GetMouseButtonUp(MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION() Float2 PointFromScreen(Float2 location) const override;

    /// <inheritdoc />
    API_FUNCTION() Float2 PointToScreen(Float2 location) const override;

    /// <inheritdoc />
    API_FUNCTION() void Focus() override;

    /// <inheritdoc />
    API_FUNCTION() void DoDragDrop(const DragData &data) override;

    /// <inheritdoc />
    API_FUNCTION() bool OnMouseDown(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION() bool OnMouseUp(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION() bool OnMouseDoubleClick(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION() bool OnMouseWheel(Float2 location, float delta) override;

    /// <inheritdoc />
    API_FUNCTION() void OnMouseMove(Float2 location) override;

protected:

    /// <inheritdoc />
    API_FUNCTION() bool Focus(UIXControl *c) override;

private:

    /*internal*/ UIXWindowRootControl(const SpawnParams &params, Window *window);

    Window *_window;
    UIXControl *_focusedControl;
    UIXControl *_trackingControl;
};
