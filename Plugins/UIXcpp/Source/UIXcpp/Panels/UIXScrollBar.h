#pragma once

#include "../UIXControl.h"
#include "../UIXEnums.h"
#include "../UIXRootControl.h"


/// <summary>
/// Scroll Bars base class - allows to scroll contents of the GUI panel.
/// </summary>
/// <seealso cref="FlaxEngine.GUI.Control" />
API_CLASS(abstract, Attributes="HideInEditor")
class UIXCPP_API UIXScrollBar : public UIXControl
{
public:

    /// <summary>
    /// Gets the thumb box thickness.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetThumbThickness() const { return _thumbThickness; }

    /// <summary>
    /// Sets the thumb box thickness.
    /// </summary>
    API_PROPERTY()
    void SetThumbThickness(float value);

    /// <summary>
    /// Gets the track line thickness.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetTrackThickness() const { return _trackThickness; }

    /// <summary>
    /// Gets or sets the track line thickness.
    /// </summary>
    API_PROPERTY()
    void SetTrackThickness(float value);

    /// <summary>
    /// The maximum time it takes to animate from current to target scroll position
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetScrollAnimationDuration() const  { return _scrollAnimationDuration; }

    /// <summary>
    /// The maximum time it takes to animate from current to target scroll position
    /// </summary>
    API_PROPERTY()
    void SetScrollAnimationDuration(float value);

    /// <summary>
    /// Gets a value indicating whether use scroll value smoothing.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE bool GetUseSmoothing() const { return GetEnableSmoothing() && !Math::IsZero(GetScrollAnimationDuration()); }

    /// <summary>
    /// Gets the UIXOrientation::
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE UIXOrientation GetOrientation() const { return _orientation; }

    /// <summary>
    /// Enables scroll smoothing
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE bool GetEnableSmoothing() const { return _enableSmoothing; }

    /// <summary>
    /// Enables scroll smoothing
    /// </summary>
    API_PROPERTY()
    void SetEnableSmoothing(bool value);

    /// <summary>
    /// Gets the minimum value.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetMinimum() const { return _minimum; }

    /// <summary>
    /// Sets the minimum value.
    /// </summary>
    API_PROPERTY()
    void SetMinimum(float value);

    /// <summary>
    /// Gets the maximum value.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetMaximum() const { return _maximum; }
    
    /// <summary>
    /// Sets the maximum value.
    /// </summary>
    API_PROPERTY()
    void SetMaximum(float value);

    /// <summary>
    /// Gets the scroll value (current, smooth).
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetValue() const { return _value; }

    /// <summary>
    /// Sets the scroll value (current, smooth).
    /// </summary>
    API_PROPERTY()
    void SetValue(float value);

    /// <summary>
    /// Gets or sets the target value (target, not smooth).
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetTargetValue() const { return _targetValue;  }

    /// <summary>
    /// Gets or sets the target value (target, not smooth).
    /// </summary>
    API_PROPERTY()
    void SetTargetValue(float value);

    /// <summary>
    /// Gets or sets the speed for the scroll on mouse wheel.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetScrollSpeedWheel() const { return _scrollChange; }

    /// <summary>
    /// Gets or sets the speed for the scroll on mouse wheel.
    /// </summary>
    API_PROPERTY()
    void SetScrollSpeedWheel(float value);

    /// <summary>
    /// Gets or sets the speed for the scroll on mouse click.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetScrollSpeedClick() const { return _clickChange; }

    /// <summary>
    /// Gets or sets the speed for the scroll on mouse click.
    /// </summary>
    API_PROPERTY()
    void SetScrollSpeedClick(float value);

    /// <summary>
    /// Gets the value slow down.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetValueSlowDown() const { return _targetValue - _value; }

    /// <summary>
    /// Gets a value indicating whether thumb is being clicked (scroll bar is in use).
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE bool GetIsThumbClicked() const { return _thumbClicked; }

    /// <summary>
    /// Cuts the scroll bar value smoothing and imminently goes to the target scroll value.
    /// </summary>
    API_FUNCTION() void FastScroll();

    /// <summary>
    /// Scrolls the view to the desire range (favors minimum value if cannot cover whole range in a bounds).
    /// </summary>
    /// <param name="min">The view minimum.</param>
    /// <param name="max">The view maximum.</param>
    /// <param name="fastScroll">True of scroll to the item quickly without smoothing.</param>
    API_FUNCTION() void ScrollViewTo(float min, float max, bool fastScroll = false);

    /// <summary>
        /// Sets the scroll range (min and max at once).
        /// </summary>
        /// <param name="minimum">The minimum scroll range value (see <see cref="Minimum"/>).</param>
        /// <param name="maximum">The maximum scroll range value (see <see cref="Minimum"/>).</param>
    API_FUNCTION() void SetScrollRange(float minimum, float maximum);

    /// <inheritdoc />
    API_FUNCTION() void Draw() override;

    /// <inheritdoc />
    API_FUNCTION() void OnLostFocus() override
    {
        EndTracking();

        UIXControl::OnLostFocus();
    }

    /// <inheritdoc />
    API_FUNCTION() void OnMouseMove(Float2 location) override;

    /// <inheritdoc />
    API_FUNCTION() bool OnMouseWheel(Float2 location, float delta) override;

    /// <inheritdoc />
    API_FUNCTION() bool OnMouseDown(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION() bool OnMouseUp(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION() void OnEndMouseCapture() override;


    /// <inheritdoc />
    API_FUNCTION() void OnMouseEnter(Float2 location) override;

    /// <inheritdoc />
    API_FUNCTION() void OnMouseLeave() override;


    /// <summary>
    /// The default size.
    /// </summary>
    static int DefaultSize;

    /// <summary>
    /// The default minimum opacity.
    /// </summary>
    static float DefaultMinimumOpacity;

    /// <summary>
    /// Occurs when value gets changed.
    /// </summary>
    API_EVENT() Action ValueChanged;

    /// <summary>
    /// Enables/disabled scrolling by user.
    /// </summary>
    API_FIELD(Attributes="NoSerialize, HideInEditor")
    bool ThumbEnabled = true;

protected:
    /// <summary>
    /// Initializes a new instance of the <see cref="ScrollBar"/> class.
    /// </summary>
    /// <param name="orientation">The UIXOrientation::</param>
    UIXScrollBar(const SpawnParams &params, UIXOrientation orientation);

    /// <summary>
    /// Gets the size of the track.
    /// </summary>
    API_FUNCTION()
    virtual float GetTrackSize() const = 0;

    /// <summary>
    /// Called when value gets changed.
    /// </summary>
    API_FUNCTION()
    virtual void OnValueChanged();

    /// <inheritdoc />
    API_FUNCTION()
    void OnSizeChanged() override;

    /// <inheritdoc />
    API_FUNCTION()
    void AddUpdateCallbacks(UIXRootControl *root) override;

    /// <inheritdoc />
    API_FUNCTION()
    void RemoveUpdateCallbacks(UIXRootControl *root) override;
private:
    void UpdateThumb();
    void EndTracking();

    void OnUpdate(float deltaTime);

    /*internal*/ void Reset();

    bool _enableSmoothing = true;

    float _thumbThickness = 8;
    float _trackThickness = 2.0f;
    float _scrollAnimationDuration = 0.18f;

    float _clickChange = 20;
    float _scrollChange = 50;
    float _minimum = 0.0f;
    float _maximum = 100.0f;
    float _startValue = 0.0f;
    float _value = 0.0f;
    float _targetValue = 0.0f;
    const UIXOrientation _orientation = UIXOrientation::Horizontal;
    UIXRootControl::UpdateDelegate _update;

    float _mouseOffset = 0.0f;

    Rectangle _thumbRect;
    Rectangle _trackRect;
    bool _thumbClicked = false;
    float _thumbCenter = 0.0f;
    float _thumbSize = 0.0f;

    float _thumbOpacity = DefaultMinimumOpacity;
    float _scrollAnimationProgress = 0.0f;

    friend class UIXPanel;
};

