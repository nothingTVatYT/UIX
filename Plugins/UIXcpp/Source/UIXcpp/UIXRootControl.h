#pragma once

#include "UIXContainerControl.h"
#include "Engine/Core/Delegate.h"

class UIXCanvasContainer;

/// <summary>
/// GUI root control that is represented by a window or an canvas and can contain children but has no parent at all. It's a source of the input events.
/// </summary>
API_CLASS(NoSpawn, Abstract) class UIXCPP_API UIXRootControl : public UIXContainerControl
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIXRootControl)
//API_AUTO_SERIALIZATION();
//DECLARE_SCRIPTING_TYPE(UIXRootControl);

public:

    /// <summary>
    /// Gets the main GUI control (it can be window or editor overriden control). Use it to plug-in custom GUI controls.
    /// </summary>
    API_PROPERTY()
    static UIXContainerControl* GetGameRoot() { return _gameRoot;  }

    /// <summary>
    /// Gets the current focused control
    /// </summary>
    API_PROPERTY()
    virtual UIXControl* GetFocusedControl() const = 0;

    /// <summary>
    /// Sets the current focused control
    /// </summary>
    API_PROPERTY()
    virtual void SetFocusedControl(UIXControl *value) = 0;

    /// <summary>
    /// Gets the tracking mouse offset.
    /// </summary>
    API_PROPERTY()
    virtual Float2 GetTrackingMouseOffset() const = 0;

    /// <summary>
    /// Gets the position of the mouse in the window space coordinates.
    /// </summary>
    API_PROPERTY()
    virtual Float2 GetMousePosition() const = 0;

    /// <summary>
    /// Sets the position of the mouse in the window space coordinates.
    /// </summary>
    API_PROPERTY()
    virtual void SetMousePosition(Float2 value) = 0;

    /// <summary>
    /// The update callbacks collection. Controls can register for this to get the update event for logic handling.
    /// </summary>
    API_EVENT() Delegate<float> UpdateCallbacks;

    /// <summary>
    /// The update callbacks to add before invoking the update.
    /// </summary>
    API_EVENT() Delegate<float> UpdateCallbacksToAdd;

    /// <summary>
    /// The update callbacks to remove before invoking the update.
    /// </summary>
    API_EVENT() Delegate<float> UpdateCallbacksToRemove;

    /// <summary>
    /// The custom callback function for UI navigation. Can be used to override the default behaviour.
    /// </summary>
    API_EVENT() Delegate<UIXNavDirection> CustomNavigation;

    /// <summary>
    /// Performs the UI navigation.
    /// </summary>
    /// <param name="direction">The navigation direction.</param>
    API_FUNCTION()
    void Navigate(UIXNavDirection direction);

    /// <summary>
    /// Submits the currently focused control.
    /// </summary>
    API_FUNCTION()
    void SubmitFocused();

    /// <inheritdoc />
    void Update(float deltaTime) override;

    /// <summary>
    /// Starts the mouse tracking. Used by the scrollbars, splitters, etc.
    /// </summary>
    /// <param name="control">The target control that want to track mouse. It will receive OnMouseMove event.</param>
    /// <param name="useMouseScreenOffset">If set to <c>true</c> will use mouse screen offset.</param>
    API_FUNCTION()
    virtual void StartTrackingMouse(UIXControl *control, bool useMouseScreenOffset) = 0;

    /// <summary>
    /// Ends the mouse tracking.
    /// </summary>
    API_FUNCTION()
    virtual void EndTrackingMouse() = 0;

    /// <summary>
    /// Gets keyboard key state.
    /// </summary>
    /// <param name="key">Key to check.</param>
    /// <returns>True while the user holds down the key identified by id.</returns>
    API_FUNCTION()
    virtual bool GetKey(KeyboardKeys key) = 0;

    /// <summary>
    /// Gets keyboard key down state.
    /// </summary>
    /// <param name="key">Key to check.</param>
    /// <returns>True during the frame the user starts pressing down the key.</returns>
    API_FUNCTION()
    virtual bool GetKeyDown(KeyboardKeys key) = 0;

    /// <summary>
    /// Gets keyboard key up state.
    /// </summary>
    /// <param name="key">Key to check.</param>
    /// <returns>True during the frame the user releases the button.</returns>
    API_FUNCTION()
    virtual bool GetKeyUp(KeyboardKeys key) = 0;

    /// <summary>
    /// Gets mouse button state.
    /// </summary>
    /// <param name="button">Mouse button to check.</param>
    /// <returns>True while the user holds down the button.</returns>
    API_FUNCTION()
    virtual bool GetMouseButton(MouseButton button) = 0;

    /// <summary>
    /// Gets mouse button down state.
    /// </summary>
    /// <param name="button">Mouse button to check.</param>
    /// <returns>True during the frame the user starts pressing down the button.</returns>
    API_FUNCTION()
    virtual bool GetMouseButtonDown(MouseButton button) = 0;

    /// <summary>
    /// Gets mouse button up state.
    /// </summary>
    /// <param name="button">Mouse button to check.</param>
    /// <returns>True during the frame the user releases the button.</returns>
    API_FUNCTION()
    virtual bool GetMouseButtonUp(MouseButton button) = 0;

    /// <inheritdoc />
    virtual UIXRootControl* GetRoot() const override { return const_cast<UIXRootControl*>(this); }

protected:

    /// <summary>
    /// Initializes a new instance of the <see cref="RootControl"/> class.
    /// </summary>
    UIXRootControl();
        
private:
    /// <summary>
    /// Gets the main GUI control (it can be window or editor overriden control). Use it to plug-in custom GUI controls.
    /// </summary>
    API_PROPERTY()
    /*internal*/ static void SetGameRoot(UIXContainerControl *value);

    /// <summary>
    /// Gets the canvas controls root container.
    /// </summary>
    /*internal*/ static UIXCanvasContainer* GetCanvasRoot();

    static UIXContainerControl *_gameRoot;
    static UIXCanvasContainer *_canvasContainer;

    friend class UIXCanvas;
};

