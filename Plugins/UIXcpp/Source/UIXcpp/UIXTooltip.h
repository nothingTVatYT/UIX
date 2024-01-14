#pragma once

#include "UIXContainerControl.h"
#include "Engine/Platform/Window.h"


/// <summary>
/// The tooltip popup.
/// </summary>
/// <seealso cref="FlaxEngine.GUI.ContainerControl" />
API_CLASS(NoSpawn, Attributes="HideInEditor") class UIXCPP_API UIXTooltip : public UIXContainerControl
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIXTooltip);
//API_AUTO_SERIALIZATION();
//DECLARE_SCRIPTING_TYPE(UIXTooltip);

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="Tooltip"/> class.
    /// </summary>
    UIXTooltip();

    /// <summary>
    /// Gets the time in seconds that mouse have to be over the target to show the tooltip.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetTimeToShow() const { return _timeToShow; }
    /// <summary>
    /// Sets the time in seconds that mouse have to be over the target to show the tooltip.
    /// </summary>
    API_PROPERTY()
    void SetTimeToShow(float value) { _timeToShow = value; }

    /// <summary>
    /// Gets or sets the maximum width of the tooltip. Used to wrap text that overflows and ensure that tooltip stays readable.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetMaxWidth() const { return _maxWidth; }
    /// <summary>
    /// Gets or sets the maximum width of the tooltip. Used to wrap text that overflows and ensure that tooltip stays readable.
    /// </summary>
    API_PROPERTY()
    void SetMaxWidth(float value) { _maxWidth = 500.0f; }

    /// <summary>
    /// Gets the tooltip window.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE Window *GetWindow() const { return _window; }

    /// <summary>
    /// Shows tooltip over given control.
    /// </summary>
    /// <param name="target">The parent control to attach to it.</param>
    /// <param name="location">Popup menu origin location in parent control coordinates.</param>
    /// <param name="targetArea">Tooltip target area of interest.</param>
    API_FUNCTION()
    void Show(UIXControl *target, const Float2 &location, const Rectangle &targetArea);

    /// <summary>
    /// Hides the popup.
    /// </summary>
    API_FUNCTION()
    void Hide();

    /// <summary>
    /// Called when mouse enters a control.
    /// </summary>
    /// <param name="target">The target-></param>
    API_FUNCTION()
    void OnMouseEnterControl(UIXControl *target);

    /// <summary>
    /// Called when mouse is over a control.
    /// </summary>
    /// <param name="target">The target-></param>
    /// <param name="dt">The delta time.</param>
    API_FUNCTION()
    void OnMouseOverControl(UIXControl *target, float dt);

    /// <summary>
    /// Called when mouse leaves a control.
    /// </summary>
    /// <param name="target">The target-></param>
    API_FUNCTION()
    void OnMouseLeaveControl(UIXControl *target);

    /// <inheritdoc />
    API_FUNCTION()
    void Update(float deltaTime) override;

    /// <inheritdoc />
    API_FUNCTION()
    void Draw() override;


    /// <inheritdoc />
    API_FUNCTION()
    bool OnShowTooltip(API_PARAM(Out) String &text, API_PARAM(Out) Float2 &location, API_PARAM(Out) Rectangle &area) override;

    /// <inheritdoc />
    API_FUNCTION()
    void OnDestroy() override;
protected:
    /// <inheritdoc />
    API_FUNCTION()
    void PerformLayoutBeforeChildren() override;

private:
    void UpdateWindowSize();

    void WrapPosition(Float2 &locationSS, float flipOffset = 0.0f);

    float _timeToShow;
    float _maxWidth;

    float _timeToPopupLeft;
    UIXControl *_lastTarget;
    UIXControl *_showTarget;
    String _currentText;
    Window *_window;

};
