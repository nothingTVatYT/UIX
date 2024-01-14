#pragma once

#include "../UIXScrollableControl.h"
#include "../UIXMargin.h"
#include "../UIXEnums.h"
#include "UIXVScrollBar.h"
#include "UIXHScrollBar.h"
#include "Engine/Scripting/ScriptingObjectReference.h"
#include "Engine/Core/Config.h"

/// <summary>
/// Panel UI control.
/// </summary>
/// <seealso cref="FlaxEngine.GUI.ScrollableControl" />
API_CLASS(NoSpawn)
class UIXCPP_API UIXPanel : public UIXScrollableControl
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIXPanel)
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="Panel"/> class.
    /// </summary>
    UIXPanel();

    /// <summary>
    /// Initializes a new instance of the <see cref="Panel"/> class.
    /// </summary>
    /// <param name="scrollBars">The scroll bars.</param>
    /// <param name="autoFocus">True if control can accept user focus</param>
    UIXPanel(UIXScrollBars scrollBars, bool autoFocus = false);


    // TODO: check everywhere where a control field is public and add ScriptingObjectReference<> to them.

    /// <summary>
    /// The vertical scroll bar.
    /// </summary>
    API_FIELD(Attributes="HideInEditor, NoSerialize")
    ScriptingObjectReference<UIXVScrollBar> VScrollBar;

    /// <summary>
    /// The horizontal scroll bar.
    /// </summary>
    API_FIELD(Attributes = "HideInEditor, NoSerialize")
    ScriptingObjectReference<UIXHScrollBar> HScrollBar;

    /// <summary>
    /// Gets the view bottom.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE Float2 GetViewBottom() const { return GetSize() + _viewOffset; }

    /// <summary>
    /// Gets the cached scroll area bounds. Used to scroll contents of the panel control. Cached during performing layout.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE Rectangle GetControlsBounds() const { return _controlsBounds; }

    /// <summary>
    /// Gets the scroll bars usage by this panel.
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(0), Tooltip(\"The scroll bars usage.\")")
    FORCE_INLINE UIXScrollBars GetScrollBars() const { return _scrollBars; }

    /// <summary>
    /// Sets the scroll bars usage by this panel.
    /// </summary>
    API_PROPERTY()
    void SetScrollBars(UIXScrollBars value);

    /// <summary>
    /// Gets the size of the scroll bars.
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(5), Tooltip(\"Scroll bars size.\")")
    FORCE_INLINE float GetScrollBarsSize() const { return _scrollBarsSize; }

    /// <summary>
    /// Sets the size of the scroll bars.
    /// </summary>
    API_PROPERTY()
    void SetScrollBarsSize(float value);

    /// <summary>
    /// Gets a value indicating whether always show scrollbars. Otherwise show them only if scrolling is available.
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(10), Tooltip(\"Whether always show scrollbars. Otherwise show them only if scrolling is available.\")")
    FORCE_INLINE bool GetAlwaysShowScrollbars() const { return _alwaysShowScrollbars; }

    /// <summary>
    /// Sets a value indicating whether always show scrollbars. Otherwise show them only if scrolling is available.
    /// </summary>
    API_PROPERTY()
    void SetAlwaysShowScrollbars(bool value);

    /// <summary>
    /// Gets or sets the scroll margin applies to the child controls area. Can be used to expand the scroll area bounds by adding a margin.
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(20), Tooltip(\"Scroll margin applies to the child controls area. Can be used to expand the scroll area bounds by adding a margin.\")")
    UIXMargin* GetScrollMargin() { return &_scrollMargin; }

    /// <summary>
    /// Sets the scroll margin applies to the child controls area. Can be used to expand the scroll area bounds by adding a margin.
    /// </summary>
    API_PROPERTY()
    void SetScrollMargin(UIXMargin* value);

    /// <summary>
    /// Cuts the scroll bars value smoothing and imminently goes to the target scroll value.
    /// </summary>
    API_FUNCTION() void FastScroll();

    /// <summary>
    /// Scrolls the view to the given control area.
    /// </summary>
    /// <param name="c">The control.</param>
    /// <param name="fastScroll">True of scroll to the item quickly without smoothing.</param>
    API_FUNCTION() void ScrollViewTo(UIXControl *c, bool fastScroll = false);

    /// <summary>
    /// Scrolls the view to the given location.
    /// </summary>
    /// <param name="location">The location.</param>
    /// <param name="fastScroll">True of scroll to the item quickly without smoothing.</param>
    API_FUNCTION() void ScrollViewTo(Float2 location, bool fastScroll = false);

    /// <summary>
    /// Scrolls the view to the given area.
    /// </summary>
    /// <param name="bounds">The bounds.</param>
    /// <param name="fastScroll">True of scroll to the item quickly without smoothing.</param>
    API_FUNCTION() void ScrollViewTo(Rectangle bounds, bool fastScroll = false);

    /// <inheritdoc />
    bool OnMouseDown(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    bool OnMouseWheel(Float2 location, float delta) override;

    /// <inheritdoc />
    void RemoveChildren() override;

    /// <inheritdoc />
    void DisposeChildren() override;

    /// <inheritdoc />
    void OnChildResized(UIXControl *control) override;

    /// <inheritdoc />
    void Draw() override;

    /// <inheritdoc />
    bool IntersectsChildContent(UIXControl *child, Float2 location, API_PARAM(Out) Float2 &childSpaceLocation) const override;

    /// <inheritdoc />
    void PerformLayout(bool force = false) override;
protected:
    /// <inheritdoc />
    void SetViewOffset(Float2 value) override;

    /// <inheritdoc />
    void PerformLayoutBeforeChildren() override;

    /// <summary>
    /// Arranges the child controls and gets their bounds.
    /// </summary>
    API_FUNCTION() virtual void ArrangeAndGetBounds();

    /// <summary>
    /// Arranges the child controls.
    /// </summary>
    API_FUNCTION() virtual void Arrange();

    /// <summary>
    /// The cached scroll area bounds. Used to scroll contents of the panel control. Cached during performing layout.
    /// </summary>
    API_FIELD(Attributes="HideInEditor, NoSerialize")
    Rectangle _controlsBounds;

    /// <inheritdoc />
    void GetDesireClientArea(API_PARAM(Out) Rectangle &rect) const override;

    /// <inheritdoc />
    // API_FUNCTION() DragDropEffect OnDragMove(Float2 location, const DragData &data) override;
private:
    /*internal*/ void SetViewOffset(UIXOrientation orientation, float value);

    /// <inheritdoc />
    /*internal*/ void AddChildInternal(UIXControl *child) override;

    // For binding to VScrollBar->ValueChanged
    void VScrollBarSetViewOffset();
    // For binding to HScrollBar->ValueChanged
    void HScrollBarSetViewOffset();


    bool _layoutChanged;
    bool _alwaysShowScrollbars;
    int _layoutUpdateLock;
    UIXScrollBars _scrollBars;
    float _scrollBarsSize;
    UIXMargin _scrollMargin;
};
