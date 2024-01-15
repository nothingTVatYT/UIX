#pragma once

#include "Engine/Platform/Base/WindowBase.h"
#include "Engine/Scripting/Script.h"
#include "Engine/Core/Config.h"
#include "Engine/Core/Math/Vector2.h"
#include "Engine/Core/Math/Matrix3x3.h"
#include "Engine/Core/Math/Rectangle.h"
#include "Engine/Core/Math/Color.h"
#include "Engine/Core/Delegate.h"

#include "UIXEnums.h"
#include "UIXMargin.h"


class UIXContainerControl;
class UIXRootControl;
class UIXWindowRootControl;
class UIXTooltip;
//class DragData;

/// <summary>
/// Base class for all GUI controls
/// </summary>
API_CLASS(NoSpawn)
class UIXCPP_API UIXControl : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIXControl)
//API_AUTO_SERIALIZATION();
//DECLARE_SCENE_OBJECT(UIXControl);

//// [Script]
//void OnEnable() override;
//void OnDisable() override;
//void OnUpdate() override;

public:
    /// <summary>
    /// Performs control logic update.
    /// </summary>
    /// <param name="deltaTime">The delta time in seconds (since last update).</param>
    typedef Delegate<float> UpdateDelegate;

    /// <summary>
    /// Initializes a new instance of the <see cref="Control"/> class.
    /// </summary>
    UIXControl();

    /// <summary>
    /// Initializes a new instance of the <see cref="Control"/> class.
    /// </summary>
    /// <param name="x">X coordinate</param>
    /// <param name="y">Y coordinate</param>
    /// <param name="width">Width</param>
    /// <param name="height">Height</param>
    UIXControl(float x, float y, float width, float height);
        
    /// <summary>
    /// Initializes a new instance of the <see cref="Control"/> class.
    /// </summary>
    /// <param name="location">Upper left corner location.</param>
    /// <param name="size">Bounds size.</param>
    UIXControl(Float2 location, Float2 size);

    /// <summary>
    /// Init
    /// </summary>
    /// <param name="bounds">Window bounds</param>
    UIXControl(Rectangle bounds);

    /// <summary>
    /// Action is invoked, when location is changed
    /// </summary>
    API_EVENT() Delegate<UIXControl*> LocationChanged;

    /// <summary>
    /// Action is invoked, when size is changed
    /// </summary>
    API_EVENT() Delegate<UIXControl*> SizeChanged;

    /// <summary>
    /// Action is invoked, when parent is changed
    /// </summary>
    API_EVENT() Delegate<UIXControl*> ParentChanged;

    /// <summary>
    /// Action is invoked, when visibility is changed
    /// </summary>
    API_EVENT() Delegate<UIXControl*> VisibleChanged;

    /// <summary>
    /// Parent control (the one above in the tree hierarchy)
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor, NoSerialize")
    FORCE_INLINE UIXContainerControl* GetParent() const
    {
        return _parent;
    }

    API_PROPERTY(Attributes="HideInEditor, NoSerialize")
    void SetParent(UIXContainerControl* value);

    /// <summary>
    /// Parent control (the one above in the tree hierarchy)
    /// </summary>
    /// <summary>
    /// Checks if control has parent container control
    /// </summary>
    API_PROPERTY() FORCE_INLINE bool GetHasParent() const
    {
        return _parent != nullptr;
    }

    /// <summary>
    /// Gets the zero-based index of the control inside the parent container list.
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor, NoSerialize") int GetIndexInParent() const;

    /// <summary>
    /// Sets the zero-based index of the control inside the parent container list.
    /// </summary>
    API_PROPERTY() void SetIndexInParent(int value);

    /// <summary>
    /// Gets the control background color (transparent color (alpha=0) means no background rendering)
    /// </summary>
    API_PROPERTY(Attributes="ExpandGroups, EditorDisplay(\"Background Style\"), EditorOrder(2000), Tooltip(\"The control background color. Use transparent color (alpha=0) to hide background.\")")
    FORCE_INLINE Color GetBackgroundColor() const
    {
        return _backgroundColor;
    }

    /// <summary>
    /// Sets the control background color (transparent color (alpha=0) means no background rendering)
    /// </summary>
    API_PROPERTY() void SetBackgroundColor(const Color &value);

    /// <summary>
    /// Gets the anchor preset used by the control anchors (based on <see cref="AnchorMin"/> and <see cref="AnchorMax"/>).
    /// </summary>
    /// <remarks>To change anchor preset with current control bounds preservation use <see cref="SetAnchorPreset"/>.</remarks>
    API_PROPERTY(Attributes="NoSerialize, EditorDisplay(\"Transform\"), HideInEditor, EditorOrder(980), Tooltip(\"The anchor preset used by the control anchors.\")")
    UIXAnchorPresets GetAnchorPreset() const;

    /// <summary>
    /// Sets the anchor preset used by the control anchors (based on <see cref="AnchorMin"/> and <see cref="AnchorMax"/>).
    /// </summary>
    /// <remarks>To change anchor preset with current control bounds preservation use <see cref="SetAnchorPreset"/>.</remarks>
    API_PROPERTY() void SetAnchorPreset(UIXAnchorPresets value);

    /// <summary>
    /// Gets a value indicating whether this control is scrollable (scroll bars affect it).
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor, NoSerialize")
    FORCE_INLINE bool GetIsScrollable() const { return _isScrollable; }
    /// <summary>
    /// Sets a value indicating whether this control is scrollable (scroll bars affect it).
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE void SetIsScrollable(bool value) { _isScrollable = value; }

    /// <summary>
    /// Gets a value indicating whether the control can respond to user interaction
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(520), Tooltip(\"If checked, control will receive input events of the user interaction.\")")
    FORCE_INLINE bool GetEnabled() const { return _isEnabled; }
    /// <summary>
    /// Sets a value indicating whether the control can respond to user interaction
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE void SetEnabled(bool value);

    /// <summary>
    /// Gets a value indicating whether the control is enabled in the hierarchy (it's enabled and all it's parents are enabled as well).
    /// </summary>
    API_PROPERTY()
    bool GetEnabledInHierarchy() const;

    /// <summary>
    /// Gets a value indicating whether the control is visible
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(510), Tooltip(\"If checked, control will be visible.\")")
    FORCE_INLINE bool GetVisible() const { return _isVisible; }

    /// <summary>
    /// Sets a value indicating whether the control is visible
    /// </summary>
    API_PROPERTY()
    void SetVisible(bool value);

    /// <summary>
    /// Gets a value indicating whether the control is visible in the hierarchy (it's visible and all it's parents are visible as well).
    /// </summary>
    API_PROPERTY()
    bool GetVisibleInHierarchy() const;

    /// <summary>
    /// Returns true if control is during disposing state (on destroy)
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE bool GetIsDisposing() { return _isDisposing; }

    /// <summary>
    /// Gets the GUI tree root control which contains that control (or null if not linked to any)
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE virtual UIXRootControl* GetRoot() const
    {
        return _root;
    }

    /// <summary>
    /// Gets the GUI window root control which contains that control (or null if not linked to any).
    /// </summary>
    API_PROPERTY()
    virtual UIXWindowRootControl* GetRootWindow() const;

    /// <summary>
    /// Gets the control DPI scale factor (1 is default). Includes custom DPI scale.
    /// </summary>
    API_PROPERTY()
    float GetDpiScale() const;

    /// <summary>
    /// Gets screen position of the control (upper left corner).
    /// </summary>
    API_PROPERTY()
    Float2 GetScreenPos() const;

    /// <summary>
    /// Gets the cursor (per window). Control should restore cursor to the default value eg. when mouse leaves it's area.
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor, NoSerialize")
    virtual CursorType GetCursor() const;

    /// <summary>
    /// Sets the cursor (per window). Control should restore cursor to the default value eg. when mouse leaves it's area.
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor, NoSerialize")
    virtual void SetCursor(CursorType value);

    API_FIELD(Attributes="HideInEditor, NoSerialize")
    Object *Tag;

    /// <summary>
    /// Delete control (will unlink from the parent and start to dispose)
    /// </summary>
    API_FUNCTION() void Dispose();

    /// <summary>
    /// Perform control update and all its children
    /// </summary>
    /// <param name="deltaTime">Delta time in seconds</param>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void Update(float deltaTime)
    {
        // TODO: move all controls to use UpdateDelegate and remove this generic Update
    }

    /// <summary>
    /// Draw control
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void Draw();

    /// <summary>
    /// Update control layout
    /// </summary>
    /// <param name="force">True if perform layout by force even if cached state wants to skip it due to optimization.</param>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void PerformLayout(bool force=false) { }

    /// <summary>
    /// Called to clear UI state. For example, removes mouse over state or drag and drop when control gets disabled or hidden (including hierarchy).
    /// </summary>
    API_FUNCTION()
    virtual void ClearState();

    /// <summary>
    /// If checked, the control can receive automatic focus (eg. on user click or UI navigation).
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(512), Tooltip(\"If checked, the control can receive automatic focus (eg. on user click or UI navigation).\")")
    FORCE_INLINE bool GetAutoFocus() const { return _autoFocus; }

    API_PROPERTY() void SetAutoFocus(bool value);

    /// <summary>
    /// Gets a value indicating whether the control, currently has the input focus
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE virtual bool GetContainsFocus() const { return _isFocused; }

    /// <summary>
    /// Gets a value indicating whether the control has input focus
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE bool GetIsFocused() const { return _isFocused; }

    /// <summary>
    /// Gets a value indicating whether the control has UI navigation focus.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE bool GetIsNavFocused() { return _isNavFocused; }

    /// <summary>
    /// Sets input focus to the control
    /// </summary>
    API_FUNCTION() virtual void Focus();

    /// <summary>
    /// Removes input focus from the control
    /// </summary>
    API_FUNCTION() void Defocus();

    /// <summary>
    /// When control gets input focus
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnGotFocus();

    /// <summary>
    /// When control losts input focus
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnLostFocus();

    /// <summary>
    /// Action fired when control gets 'Contains Focus' state
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnStartContainsFocus() {}

    /// <summary>
    /// Action fired when control lost 'Contains Focus' state
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnEndContainsFocus() {}

    /// <summary>
    /// Starts the mouse tracking. Used by the scrollbars, splitters, etc.
    /// </summary>
    /// <param name="useMouseScreenOffset">If set to <c>true</c> will use mouse screen offset.</param>
    API_FUNCTION(Attributes="NoAnimate")
    void StartMouseCapture(bool useMouseScreenOffset = false);

    /// <summary>
    /// Ends the mouse tracking.
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    void EndMouseCapture();

    /// <summary>
    /// When mouse goes up/down not over the control but it has user focus so remove that focus from it (used by scroll
    /// bars, sliders etc.)
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnEndMouseCapture() {}


    /// <summary>
    /// Gets the next navigation control to focus for the given direction. Returns null for automated direction resolving.
    /// </summary>
    /// <param name="direction">The navigation direction.</param>
    /// <returns>The target navigation control or null to use automatic navigation.</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual UIXControl* GetNavTarget(UIXNavDirection direction);


    /// <summary>
    /// Gets the navigation origin location for this control. It's the starting anchor point for searching navigable controls in the nearby area. By default the origin points are located on the control bounds edges.
    /// </summary>
    /// <param name="direction">The navigation direction.</param>
    /// <returns>The navigation origin for the automatic navigation.</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual Float2 GetNavOrigin(UIXNavDirection direction);

    /// <summary>
    /// Performs the UI navigation for this control.
    /// </summary>
    /// <param name="direction">The navigation direction.</param>
    /// <param name="location">The navigation start location (in the control-space).</param>
    /// <param name="caller">The control that calls the event.</param>
    /// <param name="visited">The list with visited controls. Used to skip recursive navigation calls when doing traversal across the UI hierarchy.</param>
    /// <returns>The target navigation control or nullptr if didn't performed any navigation.</returns>
    API_FUNCTION()
    virtual UIXControl* OnNavigate(UIXNavDirection direction, Float2 location, UIXControl* caller, API_PARAM(Ref)  Array<UIXControl*> &visited);

    /// <summary>
    /// Focuses the control by the UI navigation system. Called during navigating around UI with gamepad/keyboard navigation. Focuses the control and sets the <see cref="IsNavFocused"/> flag.
    /// </summary>
    API_FUNCTION()
    virtual void NavigationFocus();

    /// <summary>
    /// Generic user interaction event for a control used by UI navigation (eg. user submits on the currently focused control).
    /// </summary>
    API_FUNCTION()
    virtual void OnSubmit() {}

    /// <summary>
    /// Check if mouse is over that item or its child items
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE virtual bool GetIsMouseOver() { return _isMouseOver; }


    /// <summary>
    /// When mouse enters control's area
    /// </summary>
    /// <param name="location">Mouse location in UIXControl Space</param>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnMouseEnter(Float2 location);

    /// <summary>
    /// When mouse moves over control's area
    /// </summary>
    /// <param name="location">Mouse location in UIXControl Space</param>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnMouseMove(Float2 location);

    /// <summary>
    /// When mouse leaves control's area
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnMouseLeave();

    /// <summary>
    /// When mouse wheel moves
    /// </summary>
    /// <param name="location">Mouse location in UIXControl Space</param>
    /// <param name="delta">Mouse wheel move delta. A positive value indicates that the wheel was rotated forward, away from the user; a negative value indicates that the wheel was rotated backward, toward the user. Normalized to [-1;1] range.</param>
    /// <returns>True if event has been handled</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual bool OnMouseWheel(Float2 location, float delta);

    /// <summary>
    /// When mouse goes down over control's area
    /// </summary>
    /// <param name="location">Mouse location in UIXControl Space</param>
    /// <param name="button">Mouse buttons state (flags)</param>
    /// <returns>True if event has been handled, otherwise false</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual bool OnMouseDown(Float2 location, MouseButton button);

    /// <summary>
    /// When mouse goes up over control's area
    /// </summary>
    /// <param name="location">Mouse location in UIXControl Space</param>
    /// <param name="button">Mouse buttons state (flags)</param>
    /// <returns>True if event has been handled, otherwise false</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual bool OnMouseUp(Float2 location, MouseButton button);

    /// <summary>
    /// When mouse double clicks over control's area
    /// </summary>
    /// <param name="location">Mouse location in UIXControl Space</param>
    /// <param name="button">Mouse buttons state (flags)</param>
    /// <returns>True if event has been handled, otherwise false</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual bool OnMouseDoubleClick(Float2 location, MouseButton button);

    /// <summary>
    /// On input character
    /// </summary>
    /// <param name="c">Input character</param>
    /// <returns>True if event has been handled, otherwise false</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual bool OnCharInput(char c)
    {
        return false;
    }

    /// <summary>
    /// When key goes down
    /// </summary>
    /// <param name="key">Key value</param>
    /// <returns>True if event has been handled, otherwise false</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual bool OnKeyDown(KeyboardKeys key)
    {
        return false;
    }

    /// <summary>
    /// When key goes up
    /// </summary>
    /// <param name="key">Key value</param>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnKeyUp(KeyboardKeys key) {}

    /// <summary>
    /// Check if touch is over that item or its child items
    /// </summary>
    API_PROPERTY()
    virtual bool GetIsTouchOver() const { return _touchOvers.Count() != 0; }

    /// <summary>
    /// Determines whether the given touch pointer is over the control.
    /// </summary>
    /// <param name="pointerId">The touch pointer identifier. Stable for the whole touch gesture/interaction.</param>
    /// <returns>True if given touch pointer is over the control, otherwise false.</returns>
    API_FUNCTION()
    virtual bool IsTouchPointerOver(int pointerId)
    {
        return _touchOvers.Contains(pointerId);
    }

    /// <summary>
    /// When touch enters control's area
    /// </summary>
    /// <param name="location">Touch location in UIXControl Space</param>
    /// <param name="pointerId">The touch pointer identifier. Stable for the whole touch gesture/interaction.</param>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnTouchEnter(Float2 location, int pointerId)
    {
        _touchOvers.Add(pointerId);
    }

    /// <summary>
    /// When touch enters control's area.
    /// </summary>
    /// <param name="location">Touch location in UIXControl Space.</param>
    /// <param name="pointerId">The touch pointer identifier. Stable for the whole touch gesture/interaction.</param>
    /// <returns>True if event has been handled, otherwise false.</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual bool OnTouchDown(Float2 location, int pointerId)
    {
        return false;
    }

    /// <summary>
    /// When touch moves over control's area.
    /// </summary>
    /// <param name="location">Touch location in UIXControl Space.</param>
    /// <param name="pointerId">The touch pointer identifier. Stable for the whole touch gesture/interaction.</param>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnTouchMove(Float2 location, int pointerId)
    {
    }

    /// <summary>
    /// When touch goes up over control's area.
    /// </summary>
    /// <param name="location">Touch location in UIXControl Space</param>
    /// <param name="pointerId">The touch pointer identifier. Stable for the whole touch gesture/interaction.</param>
    /// <returns>True if event has been handled, otherwise false.</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual bool OnTouchUp(Float2 location, int pointerId)
    {
        return false;
    }

    /// <summary>
    /// When touch leaves control's area
    /// </summary>
    /// <param name="pointerId">The touch pointer identifier. Stable for the whole touch gesture/interaction.</param>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnTouchLeave(int pointerId)
    {
        _touchOvers.Remove(pointerId);
        if (_touchOvers.Count() == 0)
            OnTouchLeave();
    }

    /// <summary>
    /// When all touch leaves control's area
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnTouchLeave()
    {
    }


    /*/// <summary>
    /// Check if mouse dragging is over that item or its child items.
    /// </summary>
    API_PROPERTY()
    virtual bool GetIsDragOver() { return _isDragOver; }*/

    /*/// <summary>
    /// When mouse dragging enters control's area
    /// </summary>
    /// <param name="location">Mouse location in UIXControl Space</param>
    /// <param name="data">The data. See <see cref="DragDataText"/> and <see cref="DragDataFiles"/>.</param>
    /// <returns>The drag event result effect.</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual DragDropEffect OnDragEnter(Float2 location, const DragData &data)
    {
        // Set flag
        _isDragOver = true;
        return DragDropEffect::None;
    }*/

    /*/// <summary>
    /// When mouse dragging moves over control's area
    /// </summary>
    /// <param name="location">Mouse location in UIXControl Space</param>
    /// <param name="data">The data. See <see cref="DragDataText"/> and <see cref="DragDataFiles"/>.</param>
    /// <returns>The drag event result effect.</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual DragDropEffect OnDragMove(Float2 location, const DragData &data)
    {
        return DragDropEffect::None;
    }*/

    /*/// <summary>
    /// When mouse dragging drops on control's area
    /// </summary>
    /// <param name="location">Mouse location in UIXControl Space</param>
    /// <param name="data">The data. See <see cref="DragDataText"/> and <see cref="DragDataFiles"/>.</param>
    /// <returns>The drag event result effect.</returns>
    API_FUNCTION(Attributes="NoAnimate")
    virtual DragDropEffect OnDragDrop(Float2 location, const DragData &data)
    {
        // Clear flag
        _isDragOver = false;
        return DragDropEffect::None;
    }*/

    /*/// <summary>
    /// When mouse dragging leaves control's area
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnDragLeave()
    {
        // Clear flag
        _isDragOver = false;
    }*/

    /*/// <summary>
    /// Starts the drag and drop operation.
    /// </summary>
    /// <param name="data">The data.</param>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void DoDragDrop(const DragData &data);*/

    /// <summary>
    /// Gets the tooltip text.
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor,NoSerialize")
    FORCE_INLINE String GetTooltipText() const
    {
        return _tooltipText;
    }

    /// <summary>
    /// Sets the tooltip text.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE void SetTooltipText(const String &value)
    {
         _tooltipText = value;
    }

    /// <summary>
    /// Gets or sets the custom tooltip control linked. Use nullptr to show default shared tooltip from the current <see cref="Style"/>.
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor,NoSerialize")
    FORCE_INLINE UIXTooltip* GetCustomTooltip() const { return _tooltip; }

    /// <summary>
    /// Sets the custom tooltip control linked. Use nullptr to show default shared tooltip from the current <see cref="Style"/>.
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor, NoSerialize")
    FORCE_INLINE void SetCustomTooltip(UIXTooltip *value)
    {
        _tooltip = value;
    }

    /// <summary>
    /// Gets the tooltip used by this control (custom or shared one).
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE UIXTooltip* GetTooltip() const;


    /// <summary>
    /// Links the tooltip.
    /// </summary>
    /// <param name="text">The text.</param>
    /// <param name="customTooltip">The custom tooltip.</param>
    /// <returns>This control pointer. Useful for creating controls in code.</returns>
    API_FUNCTION(Attributes="NoAnimate")
    UIXControl* LinkTooltip(String text, UIXTooltip *customTooltip = nullptr);

    /// <summary>
    /// Unlinks the tooltip.
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    void UnlinkTooltip();

    /// <summary>
    /// Called when tooltip wants to be shown. Allows modifying its appearance.
    /// </summary>
    /// <param name="text">The tooltip text to show.</param>
    /// <param name="location">The popup start location (in this control local space).</param>
    /// <param name="area">The allowed area of mouse movement to show tooltip (in this control local space).</param>
    /// <returns>True if can show tooltip, otherwise false to skip.</returns>
    API_FUNCTION()
    virtual bool OnShowTooltip(API_PARAM(Out) String &text, API_PARAM(Out) Float2 &location, API_PARAM(Out) Rectangle &area);

    /// <summary>
    /// Called when tooltip gets created and shown for this control. Can be used to customize tooltip UI.
    /// </summary>
    /// <param name="tooltip">The tooltip.</param>
    API_FUNCTION()
    virtual void OnTooltipShown(UIXTooltip *tooltip)
    {
    }

    /// <summary>
    /// Called when tooltip is visible and tests if the given mouse location (in control space) is valid (is over the content).
    /// </summary>
    /// <param name="location">The location.</param>
    /// <returns>True if tooltip can be still visible, otherwise false.</returns>
    API_FUNCTION()
    virtual bool OnTestTooltipOverControl(Float2 location);

    /// <summary>
     /// Checks if given location point in Parent Space intersects with the control content and calculates local position.
     /// </summary>
     /// <param name="locationParent">The location in Parent Space.</param>
     /// <param name="location">The location of intersection in UIXControl Space.</param>
     /// <returns>True if given point in Parent Space intersects with this control content, otherwise false.</returns>
    API_FUNCTION() virtual bool IntersectsContent(const Float2 locationParent, API_PARAM(Out) Float2 &location) const;
        
    /// <summary>
    /// Checks if control contains given point in local UIXControl Space.
    /// </summary>
    /// <param name="location">Point location in UIXControl Space to check</param>
    /// <returns>True if point is inside control's area, otherwise false.</returns>
    API_FUNCTION() virtual bool ContainsPoint(Float2 location) const;

    /// <summary>
    /// Converts point in local control's space into one of the parent control coordinates
    /// </summary>
    /// <param name="parent">This control parent of any other parent.</param>
    /// <param name="location">Input location of the point to convert</param>
    /// <returns>Converted point location in parent control coordinates</returns>
    API_FUNCTION() Float2 PointToParent(UIXContainerControl* parent, Float2 location) const;

    /// <summary>
    /// Converts point in local control's space into parent control coordinates.
    /// </summary>
    /// <param name="location">The input location of the point to convert.</param>
    /// <returns>The converted point location in parent control coordinates.</returns>
    API_FUNCTION() virtual Float2 PointToParent(Float2 location) const;

    ///// <summary>
    ///// Converts point in local control's space into parent control coordinates.
    ///// </summary>
    ///// <param name="location">The input location of the point to convert.</param>
    ///// <returns>The converted point location in parent control coordinates.</returns>
    //API_FUNCTION() virtual Float2 PointToParent(API_PARAM(Ref) Float2 &location) const;

    /// <summary>
    /// Converts point in parent control coordinates into local control's space.
    /// </summary>
    /// <param name="locationParent">The input location of the point to convert.</param>
    /// <returns>The converted point location in control's space.</returns>
    API_FUNCTION() virtual Float2 PointFromParent(Float2 locationParent) const;

    ///// <summary>
    ///// Converts point in parent control coordinates into local control's space.
    ///// </summary>
    ///// <param name="locationParent">The input location of the point to convert.</param>
    ///// <returns>The converted point location in control's space.</returns>
    //API_FUNCTION() virtual Float2 PointFromParent(API_PARAM(Ref) Float2 &locationParent) const;

    /// <summary>
    /// Converts point in one of the parent control coordinates into local control's space.
    /// </summary>
    /// <param name="parent">This control parent of any other parent.</param>
    /// <param name="location">Input location of the point to convert</param>
    /// <returns>The converted point location in control's space.</returns>
    API_FUNCTION() Float2 PointFromParent(UIXContainerControl *parent, Float2 location) const;

    /// <summary>
    /// Converts point in local control's space into window coordinates
    /// </summary>
    /// <param name="location">Input location of the point to convert</param>
    /// <returns>Converted point location in window coordinates</returns>
    API_FUNCTION() Float2 PointToWindow(Float2 location) const;

    /// <summary>
    /// Converts point in the window coordinates into control's space
    /// </summary>
    /// <param name="location">Input location of the point to convert</param>
    /// <returns>Converted point location in control's space</returns>
    API_FUNCTION() Float2 PointFromWindow(Float2 location) const;

    /// <summary>
    /// Converts point in the local control's space into screen coordinates
    /// </summary>
    /// <param name="location">Input location of the point to convert</param>
    /// <returns>Converted point location in screen coordinates</returns>
    API_FUNCTION() virtual Float2 PointToScreen(Float2 location) const;

    /// <summary>
    /// Converts point in screen coordinates into the local control's space
    /// </summary>
    /// <param name="location">Input location of the point to convert</param>
    /// <returns>Converted point location in local control's space</returns>
    API_FUNCTION() virtual Float2 PointFromScreen(Float2 location) const;

#if FLAX_EDITOR
    /// <summary>
    /// Bounds rectangle for editor UI.
    /// </summary>
    API_PROPERTY()
    virtual Rectangle GetEditorBounds() { return Rectangle(Float2::Zero, _bounds.Size); }
#endif


    /// <summary>
    /// Action fired when parent control gets resized (also when control gets non-nullptr parent).
    /// </summary>
    API_FUNCTION()
    virtual void OnParentResized();

    /// <summary>
    /// Method called when managed instance should be destroyed
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnDestroy();


    /// <inheritdoc />
    API_FUNCTION()
    int CompareTo(Object *obj) const;

    /// <summary>
    /// Compares this control with the other control.
    /// </summary>
    /// <param name="other">The other.</param>
    /// <returns>Comparision result.</returns>
    API_FUNCTION()
    virtual int Compare(UIXControl *other) const;


    /// <summary>
    /// Gets the control's size.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor")
    FORCE_INLINE Float2 GetSize() const { return _bounds.Size;  }

    /// <summary>
    /// Sets the control's size.
    /// </summary>
    API_PROPERTY()
    void SetSize(const Float2 &value);

    /// <summary>
    /// Gets the X coordinate of the upper-left corner of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor")
    FORCE_INLINE float GetX() const { return _bounds.GetX(); }

    /// <summary>
    /// Sets the X coordinate of the upper-left corner of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    void SetX(float value);

    /// <summary>
    /// Gets the Y coordinate of the upper-left corner of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor")
    FORCE_INLINE float GetY() const { return _bounds.GetY(); }

    /// <summary>
    /// Sets the Y coordinate of the upper-left corner of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    void SetY(float value);

    /// <summary>
    /// Gets the local X coordinate of the pivot of the control relative to the anchor in parent of its container.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor")
    FORCE_INLINE float GetLocalX() const { return GetLocalLocation().X; }

    /// <summary>
    /// Sets the local X coordinate of the pivot of the control relative to the anchor in parent of its container.
    /// </summary>
    API_PROPERTY()
    void SetLocalX(float value);

    /// <summary>
    /// Gets the local Y coordinate of the pivot of the control relative to the anchor in parent of its container.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor")
    float GetLocalY() const { return GetLocalLocation().Y; }

    /// <summary>
    /// Sets the local Y coordinate of the pivot of the control relative to the anchor in parent of its container.
    /// </summary>
    API_PROPERTY()
    void SetLocalY(float value);

    /// <summary>
    /// Gets the normalized position in the parent control that the upper left corner is anchored to (range 0-1).
    /// </summary>
    API_PROPERTY(Attributes="Serialize, HideInEditor")
    FORCE_INLINE Float2 GetAnchorMin() const
    {
        return _anchorMin;
    }

    /// <summary>
    /// Sets the normalized position in the parent control that the upper left corner is anchored to (range 0-1).
    /// </summary>
    API_PROPERTY()
    void SetAnchorMin(const Float2 &value);

    /// <summary>
    /// Gets the normalized position in the parent control that the bottom right corner is anchored to (range 0-1).
    /// </summary>
    API_PROPERTY(Attributes="Serialize, HideInEditor")
    FORCE_INLINE Float2 GetAnchorMax() const
    {
        return _anchorMax;
    }

    /// <summary>
    /// Sets the normalized position in the parent control that the bottom right corner is anchored to (range 0-1).
    /// </summary>
    API_PROPERTY()
    void SetAnchorMax(const Float2 &value);

    /// <summary>
    /// Gets the offsets of the corners of the control relative to its anchors.
    /// </summary>
    API_PROPERTY(Attributes="Serialize, HideInEditor")
    UIXMargin GetOffsets() { return _offsets; }

    /// <summary>
    /// Sets the offsets of the corners of the control relative to its anchors.
    /// </summary>
    API_PROPERTY()
    void SetOffsets(const UIXMargin &value);

#if FLAX_EDITOR
    /// <summary>
    /// Helper for Editor UI (see UIControlControlEditor).
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor, NoAnimate")
    FORCE_INLINE float GetProxy_Offset_Left() const { return _offsets.Left; }

    /// <summary>
    /// Helper for Editor UI (see UIControlControlEditor).
    /// </summary>
    API_PROPERTY()
    void SetProxy_Offset_Left(float value)
    {
        SetOffsets(UIXMargin(value, _offsets.Right, _offsets.Top, _offsets.Bottom));
    }

    /// <summary>
    /// Helper for Editor UI (see UIControlControlEditor).
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor, NoAnimate")
    FORCE_INLINE float GetProxy_Offset_Right() const { return _offsets.Right; }

    /// <summary>
    /// Helper for Editor UI (see UIControlControlEditor).
    /// </summary>
    API_PROPERTY()
    void SetProxy_Offset_Right(float value)
    {
        SetOffsets(UIXMargin(_offsets.Left, value, _offsets.Top, _offsets.Bottom));
    }

    /// <summary>
    /// Helper for Editor UI (see UIControlControlEditor).
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor, NoAnimate")
    FORCE_INLINE float GetProxy_Offset_Top() const { return _offsets.Top; }

    /// <summary>
    /// Helper for Editor UI (see UIControlControlEditor).
    /// </summary>
    API_PROPERTY()
    float SetProxy_Offset_Top(float value)
    {
        SetOffsets(UIXMargin(_offsets.Left, _offsets.Right, value, _offsets.Bottom));
    }

    /// <summary>
    /// Helper for Editor UI (see UIControlControlEditor).
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor, NoAnimate")
    FORCE_INLINE float GetProxy_Offset_Bottom() const { return _offsets.Bottom; }

    /// <summary>
    /// Helper for Editor UI (see UIControlControlEditor).
    /// </summary>
    API_PROPERTY()
    float SetProxy_Offset_Bottom(float value)
    {
        SetOffsets(UIXMargin(_offsets.Left, _offsets.Right, _offsets.Top, value));
    }
#endif

    /// <summary>
    /// Gets the coordinates of the upper-left corner of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor")
    FORCE_INLINE Float2 GetLocation() const { return _bounds.Location; }

    /// <summary>
    /// Sets the coordinates of the upper-left corner of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    void SetLocation(const Float2 &value);

    /// <summary>
    /// Gets the local position of the pivot of the control relative to the anchor in parent of its container.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor")
    Float2 GetLocalLocation() const;

    /// <summary>
    /// Sets the local position of the pivot of the control relative to the anchor in parent of its container.
    /// </summary>
    API_PROPERTY()
    void SetLocalLocation(const Float2 &value);

    /// <summary>
    /// Whether to resize the UI Control based on where the pivot is rather than just the top-left.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor")
    FORCE_INLINE bool GetPivotRelative() const { return _pivotRelativeSizing; }

    /// <summary>
    /// Whether to resize the UI Control based on where the pivot is rather than just the top-left.
    /// </summary>
    API_PROPERTY()
    void SetPivotRelative(bool value) { _pivotRelativeSizing = value; }

    /// <summary>
    /// Gets the width of the control.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor")
    FORCE_INLINE float GetWidth() const { return _bounds.Size.X; }

    /// <summary>
    /// Sets the width of the control.
    /// </summary>
    API_PROPERTY()
    void SetWidth(float value);

    /// <summary>
    /// Gets the width of the control.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, HideInEditor")
    FORCE_INLINE float GetHeight() const { return _bounds.Size.Y; }

    /// <summary>
    /// Sets the width of the control.
    /// </summary>
    API_PROPERTY()
    void SetHeight(float value);

    /// <summary>
    /// Gets Y coordinate of the top edge of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetTop() const { return _bounds.GetTop(); }

    /// <summary>
    /// Gets Y coordinate of the bottom edge of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetBottom() const{ return _bounds.GetBottom(); }

    /// <summary>
    /// Gets X coordinate of the left edge of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetLeft() const { return _bounds.GetLeft(); }

    /// <summary>
    /// Gets X coordinate of the right edge of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE float GetRight() const { return _bounds.GetRight(); }

    /// <summary>
    /// Gets position of the upper left corner of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE Float2 GetUpperLeft() const { return _bounds.GetUpperLeft(); }

    /// <summary>
    /// Gets position of the upper right corner of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE Float2 GetUpperRight() const { return _bounds.GetUpperRight(); }

    /// <summary>
    /// Gets position of the bottom right corner of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE Float2 GetBottomRight() const { return _bounds.GetBottomRight(); }

    /// <summary>
    /// Gets position of the bottom left of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE Float2 GetBottomLeft() const { return _bounds.GetBottomLeft(); }

    /// <summary>
    /// Gets the center position of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor, NoSerialize")
    FORCE_INLINE Float2 GetCenter() const { return _bounds.GetCenter(); }

    /// <summary>
    /// Sets the center position of the control relative to the upper-left corner of its container.
    /// </summary>
    API_PROPERTY()
    void SetCenter(Float2 value);

    /// <summary>
    /// Gets the control's bounds rectangle.
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor, NoSerialize")
    FORCE_INLINE Rectangle GetBounds() const { return _bounds; }

    /// <summary>
    /// Sets control's bounds rectangle.
    /// </summary>
    API_PROPERTY()
    void SetBounds(const Rectangle &value);


    /// <summary>
    /// Gets the scale. Scales control according to its Pivot which by default is (0.5,0.5) (middle of the control). If you set pivot to (0,0) it will scale the control based on it's upper-left corner.
    /// </summary>
    API_PROPERTY(Attributes="ExpandGroups, EditorDisplay(\"Transform\"), Limit(float.MinValue, float.MaxValue, 0.1f), EditorOrder(1020), Tooltip(\"The control scale parameter. Scales control according to its Pivot which by default is (0.5,0.5) (middle of the control). If you set pivot to (0,0) it will scale the control based on it's upper-left corner.\")")
    FORCE_INLINE Float2 GetScale() const { return _scale; }

    /// <summary>
    /// Sets the scale. Scales control according to its Pivot which by default is (0.5,0.5) (middle of the control). If you set pivot to (0,0) it will scale the control based on it's upper-left corner.
    /// </summary>
    API_PROPERTY()
    void SetScale(const Float2 &value);

    /// <summary>
    /// Gets the normalized pivot location (used to transform control around it). Point (0,0) is upper left corner, (0.5,0.5) is center, (1,1) is bottom right corner.
    /// </summary>
    API_PROPERTY(Attributes="ExpandGroups, EditorDisplay(\"Transform\"), Limit(0.0f, 1.0f, 0.1f), EditorOrder(1030), Tooltip(\"The control rotation pivot location in normalized control size. Point (0,0) is upper left corner, (0.5,0.5) is center, (1,1) is bottom right corner.\")")
    FORCE_INLINE Float2 GetPivot() const { return _pivot; }

    /// <summary>
    /// Sets the normalized pivot location (used to transform control around it). Point (0,0) is upper left corner, (0.5,0.5) is center, (1,1) is bottom right corner.
    /// </summary>
    API_PROPERTY()
    void SetPivot(const Float2 &value);

    /// <summary>
    /// Gets the shear transform angles (x, y). Defined in degrees. Shearing happens relative to the control pivot point.
    /// </summary>
    API_PROPERTY(Attributes="ExpandGroups, EditorDisplay(\"Transform\"), EditorOrder(1040), Tooltip(\"The shear transform angles (x, y). Defined in degrees. Shearing happens relative to the control pivot point.\")")
    FORCE_INLINE Float2 GetShear() const { return  _shear; }

    /// <summary>
    /// Sets the shear transform angles (x, y). Defined in degrees. Shearing happens relative to the control pivot point.
    /// </summary>
    API_PROPERTY()
    void SetShear(const Float2 &value);

    /// <summary>
    /// Gets the rotation angle (in degrees). Control is rotated around it's pivot point (middle of the control by default).
    /// </summary>
    API_PROPERTY(Attributes="ExpandGroups, EditorDisplay(\"Transform\"), EditorOrder(1050), Tooltip(\"The control rotation angle (in degrees). Control is rotated around it's pivot point (middle of the control by default).\")")
    FORCE_INLINE float GetRotation() const { return _rotation; }

    /// <summary>
    /// Sets the rotation angle (in degrees). Control is rotated around it's pivot point (middle of the control by default).
    /// </summary>
    API_PROPERTY()
    void SetRotation(float value);

    /// <summary>
    /// Updates the control cached bounds (based on anchors and offsets).
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    void UpdateBounds();

    /// <summary>
    /// Updates the control cached transformation matrix (based on bounds).
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    void UpdateTransform();

    /// <summary>
    /// Sets the anchor preset for the control. Can be use to auto-place the control for a given preset or can preserve the current control bounds.
    /// </summary>
    /// <param name="anchorPreset">The anchor preset to set.</param>
    /// <param name="preserveBounds">True if preserve current control bounds, otherwise will align control position accordingly to the anchor location.</param>
    /// <param name="setPivotToo">Whether or not we should set the pivot too, eg left-top 0,0, bottom-right 1,1</param>
    API_FUNCTION()
    void SetAnchorPreset(const UIXAnchorPresets &anchorPreset, bool preserveBounds, bool setPivotToo = false);


    /// <summary>
    /// The explicitly specified target navigation control for <see cref="NavDirection.Up"/> direction.
    /// </summary>
    API_FIELD(Attributes="HideInEditor, NoSerialize")
    UIXControl* NavTargetUp;

    /// <summary>
    /// The explicitly specified target navigation control for <see cref="NavDirection.Down"/> direction.
    /// </summary>
    API_FIELD(Attributes="HideInEditor, NoSerialize")
    UIXControl* NavTargetDown;

    /// <summary>
    /// The explicitly specified target navigation control for <see cref="NavDirection.Left"/> direction.
    /// </summary>
    API_FIELD(Attributes="HideInEditor, NoSerialize")
    UIXControl* NavTargetLeft;

    /// <summary>
    /// The explicitly specified target navigation control for <see cref="NavDirection.Right"/> direction.
    /// </summary>
    API_FIELD(Attributes="HideInEditor, NoSerialize")
    UIXControl* NavTargetRight;

protected:
    /// <summary>
    /// Focus that control
    /// </summary>
    /// <param name="c">Control to focus</param>
    /// <returns>True if control got a focus</returns>
    API_FUNCTION()
    virtual bool Focus(UIXControl *c);

    /// <summary>
    /// Gets a value indicating whether show control tooltip (control is in a proper state, tooltip text is valid, etc.). Can be used to implement custom conditions for showing tooltips (eg. based on current mouse location within the control bounds).
    /// </summary>
    /// <remarks>Tooltip can be only visible if mouse is over the control area (see <see cref="IsMouseOver"/>).</remarks>
    API_PROPERTY()
    virtual bool GetShowTooltip() { return !_tooltipText.IsEmpty(); }


    /// <summary>
    /// Called when control location gets changed.
    /// </summary>
    API_FUNCTION()
    virtual void OnLocationChanged();

    /// <summary>
    /// Called when control size gets changed.
    /// </summary>
    API_FUNCTION()
    virtual void OnSizeChanged();

    /// <summary>
    /// Sets the scale and updates the transform.
    /// </summary>
    /// <param name="scale">The scale.</param>
    API_FUNCTION()
    virtual void SetScaleInternal(API_PARAM(Ref) const Float2 &scale);

    /// <summary>
    /// Sets the pivot and updates the transform.
    /// </summary>
    /// <param name="pivot">The pivot.</param>
    API_FUNCTION()
    virtual void SetPivotInternal(API_PARAM(Ref) const Float2 &pivot);

    /// <summary>
    /// Sets the shear and updates the transform.
    /// </summary>
    /// <param name="shear">The shear.</param>
    API_FUNCTION()
    virtual void SetShearInternal(API_PARAM(Ref) const Float2 &shear);

    /// <summary>
    /// Sets the rotation angle and updates the transform.
    /// </summary>
    /// <param name="rotation">The rotation (in degrees).</param>
    API_FUNCTION()
    virtual void SetRotationInternal(float rotation);

    /// <summary>
    /// Called when visible state gets changed.
    /// </summary>
    API_FUNCTION()
    virtual void OnVisibleChanged();

    /// <summary>
    /// Action fired when parent control gets changed.
    /// </summary>
    API_FUNCTION()
    virtual void OnParentChangedInternal();

    /// <summary>
    /// Caches the root control handle.
    /// </summary>
    API_FUNCTION()
    virtual void CacheRootHandle();

    /// <summary>
    /// Adds the custom control logic update callbacks to the root.
    /// </summary>
    /// <param name="root">The root.</param>
    API_FUNCTION()
    virtual void AddUpdateCallbacks(UIXRootControl *root);

    /// <summary>
    /// Removes the custom control logic update callbacks from the root.
    /// </summary>
    /// <param name="root">The root.</param>
    API_FUNCTION()
    virtual void RemoveUpdateCallbacks(UIXRootControl *root);

    /// <summary>
    /// Helper utility function to sets the update callback to the root. Does nothing if value has not been modified. Handles if control has no root or parent.
    /// </summary>
    /// <param name="onUpdate">The cached update callback delegate (field in the custom control implementation).</param>
    /// <param name="value">The value to assign.</param>
    void SetUpdate(API_PARAM(ref) UpdateDelegate &onUpdate, const UpdateDelegate &value);

private:

    struct AnchorPresetData
    {
        UIXAnchorPresets Preset;
        Float2 Min;
        Float2 Max;

        AnchorPresetData(UIXAnchorPresets preset, Float2 min, Float2 max)
        {
            Preset = preset;
            Min = min;
            Max = max;
        }
    };

    void OnUpdateTooltip(float deltaTime);
    void SetBoundsInner(const Rectangle &value);

    static const AnchorPresetData AnchorPresetsData[];

    UIXContainerControl *_parent = nullptr;
    UIXRootControl *_root = nullptr;
    bool _isDisposing = false;
    bool _isFocused = false;
    bool _isNavFocused = false;

    // State

    // TODO: convert to flags

    bool _isMouseOver = false;//, _isDragOver;
    bool _isVisible = true;
    bool _isEnabled = true;
    bool _autoFocus = true;
    bool _pivotRelativeSizing = false;
    bool _isScrollable = true;
    /*List*/ Array<int> _touchOvers;

    /*UIXRootControl::*/UpdateDelegate _tooltipUpdate;

    // Transform

    Rectangle _bounds = Rectangle(Float2::Zero, Float2::Zero);
    UIXMargin _offsets { 0.0f, 100.0f, 0.0f, 30.0f };
    Float2 _anchorMin = Float2::Zero;
    Float2 _anchorMax = Float2::Zero;
    Float2 _scale = Float2(1.0f);
    Float2 _pivot = Float2(0.5f);
    Float2 _shear = Float2::Zero;
    float _rotation = 0.0f;
    /*internal*/ Matrix3x3 _cachedTransform;
    /*internal*/ Matrix3x3 _cachedTransformInv;


    // Style

    Color _backgroundColor = Color::Transparent;

    // Tooltip

    String _tooltipText;
    UIXTooltip *_tooltip = nullptr;


    // To access internal functions
    friend class UIXContainerControl;
    friend class UIXScrollableControl;
    friend class UIXPanel;
};

bool operator==(const UIXControl::UpdateDelegate &a, const UIXControl::UpdateDelegate &b);
