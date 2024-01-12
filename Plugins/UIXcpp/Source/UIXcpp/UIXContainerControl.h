#pragma once

#include "UIXControl.h"

API_CLASS() class UIXCPP_API UIXContainerControl : public UIXControl
{
//API_AUTO_SERIALIZATION();
//DECLARE_SCRIPTING_TYPE(UIXContainerControl);

    //// [Script]
    //void OnEnable() override;
    //void OnDisable() override;
    //void OnUpdate() override;
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="UIXContainerControl"/> class.
    /// </summary>
    UIXContainerControl();

    /// <summary>
    /// Initializes a new instance of the <see cref="UIXContainerControl"/> class.
    /// </summary>
    UIXContainerControl(float x, float y, float width, float height);

    /// <summary>
    /// Initializes a new instance of the <see cref="UIXContainerControl"/> class.
    /// </summary>
    UIXContainerControl(const Float2 &location, const Float2 &size);

    /// <inheritdoc />
    UIXContainerControl(const Rectangle &bounds);

    /// <summary>
    /// Gets child controls list
    /// </summary>
    API_PROPERTY()
    Array<UIXControl*>& GetChildren() { return _children; }

    /// <summary>
    /// Gets amount of the children controls
    /// </summary>
    API_PROPERTY()
    int GetChildrenCount() const { return _children.Count(); }

    /// <summary>
    /// Checks if container has any child controls
    /// </summary>
    API_PROPERTY()
    bool GetHasChildren() const { return _children.Count() > 0; }

    /// <summary>
    /// Gets a value indicating whether the control, or one of its child controls, currently has the input focus.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE virtual bool GetContainsFocus() const { return _containsFocus; }

    /// <summary>
    /// True if automatic updates for control layout are locked (useful when creating a lot of GUI control to prevent lags).
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor, NoSerialize, NoAnimate")
    FORCE_INLINE bool GetIsLayoutLocked() const { return _isLayoutLocked; }

    /// <summary>
    /// True if automatic updates for control layout are locked (useful when creating a lot of GUI control to prevent lags).
    /// </summary>
    API_PROPERTY()
    void SetIsLayoutLocked(bool value) { _isLayoutLocked = value; }
    
    /// <summary>
    /// Gets a value indicating whether apply clipping mask on children during rendering.
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(530), Tooltip(\"If checked, control will apply clipping mask on children during rendering.\")")
    FORCE_INLINE bool GetClipChildren() const { return _clipChildren; }

    /// <summary>
    /// Sets a value indicating whether apply clipping mask on children during rendering.
    /// </summary>
    API_PROPERTY()
    void SetClipChildren(bool value) { _clipChildren = value; }
    
    /// <summary>
    /// Gets a value indicating whether perform view culling on children during rendering.
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(540), Tooltip(\"If checked, control will perform view culling on children during rendering.\")")
    FORCE_INLINE bool GetCullChildren() { return _cullChildren; }

    API_PROPERTY()
    void SetCullChildren(bool value) { _cullChildren = value; }

    /// <summary>
    /// Locks all child controls layout and itself.
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    void LockChildrenRecursive();


    /// <summary>
    /// Unlocks all the child controls layout and itself.
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    void UnlockChildrenRecursive();


    /// <summary>
    /// Unlinks all the child controls.
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void RemoveChildren();


    /// <summary>
    /// Removes and disposes all the child controls
    /// </summary>
    API_FUNCTION()
    virtual void DisposeChildren();

    /// <summary>
    /// Creates a new control and adds it to the container.
    /// </summary>
    /// <returns>The added control.</returns>
    template<typename T>
    FORCE_INLINE T* AddChild()
    {
        //TODO: what is an Activator?
        T* child = (T*)Activator::CreateInstance(typeof(T));
        child->Parent = this;
        return child;
    }

    /// <summary>
    /// Adds the control to the container.
    /// </summary>
    /// <param name="child">The control to add.</param>
    /// <returns>The added control.</returns>
    template<typename T>
    FORCE_INLINE T* AddChild(T *child)
    {
        if (child == nullptr)
            throw new ArgumentNullException();
        if (child->GetParent() == this && _children.Contains(child))
            throw new InvalidOperationException("Argument child cannot be added, if current container is already its parent.");

        child->SetParent(this);

        return child;
    }

    /// <summary>
    /// Removes control from the container.
    /// </summary>
    /// <param name="child">The control to remove.</param>
    API_FUNCTION() void RemoveChild(UIXControl *child);
   
    /// <summary>
    /// Gets child control at given index.
    /// </summary>
    /// <param name="index">The control index.</param>
    /// <returns>The child control.</returns>
    API_FUNCTION()
    UIXControl* GetChild(int index);

    /// <summary>
    /// Searches for a child control of a specific type. If there are multiple controls matching the type, only the first one found is returned.
    /// </summary>
    /// <typeparam name="T">The type of the control to search for. Includes any controls derived from the type.</typeparam>
    /// <returns>The control instance if found, otherwise null.</returns>
    template<typename T>
    FORCE_INLINE T* GetChild()
    {
        var type = typeof(T);
        for (int i = 0; i < _children.Count; i++)
        {
            //var ct = _children[i].GetType();
            //if (type.IsAssignableFrom(ct))
            //    return (T)_children[i];
            T *c = dynamic_cast<T*>(_children[i]);
            if (c != nullptr)
                return (T*)c;
        }
        return nullptr;
    }


    /// <summary>
    /// Gets zero-based index in the list of control children.
    /// </summary>
    /// <param name="child">The child control.</param>
    /// <returns>The zero-based index in the list of control children.</returns>
    API_FUNCTION()
    int GetChildIndex(const UIXControl *child);

    /// <summary>
    /// Tries to find any child control at given point in control local coordinates.
    /// </summary>
    /// <param name="point">The local point to check.</param>
    /// <returns>The found control index or -1 if failed.</returns>
    API_FUNCTION()
    int GetChildIndexAt(const Float2 &point) const;

    /// <summary>
    /// Tries to find any child control at given point in control local coordinates
    /// </summary>
    /// <param name="point">The local point to check.</param>
    /// <returns>The found control or null.</returns>
    API_FUNCTION()
    UIXControl* GetChildAt(const Float2 &point);


    /// <summary>
    /// Tries to find valid child control at given point in control local coordinates. Uses custom callback method to test controls to pick.
    /// </summary>
    /// <param name="point">The local point to check.</param>
    /// <param name="isValid">The control validation callback.</param>
    /// <returns>The found control or null.</returns>
    API_FUNCTION()
    UIXControl* GetChildAt(Float2 point, Function<bool (UIXControl*)> isValid);

    /// <summary>
    /// Tries to find lowest child control at given point in control local coordinates.
    /// </summary>
    /// <param name="point">The local point to check.</param>
    /// <returns>The found control or null.</returns>
    API_FUNCTION()
    UIXControl* GetChildAtRecursive(Float2 point);

    /// <summary>
    /// Gets rectangle in local control coordinates with area for controls (without scroll bars, anchored controls, etc.).
    /// </summary>
    /// <returns>The rectangle in local control coordinates with area for controls (without scroll bars etc.).</returns>
    API_FUNCTION()
    Rectangle GetClientArea() const;

    /// <summary>
    /// Sort child controls list
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    void SortChildren();

    /// <summary>
    /// Sort children using recursion
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    void SortChildrenRecursive();

    /// <summary>
    /// Called when child control gets resized.
    /// </summary>
    /// <param name="control">The resized control.</param>
    API_FUNCTION()
    virtual void OnChildResized(UIXControl *control) {}

    /// <summary>
    /// Called when children collection gets changed (child added or removed).
    /// </summary>
    API_FUNCTION(Attributes="NoAnimate")
    virtual void OnChildrenChanged();

    /// <summary>
    /// Gets the desire client area rectangle for all the controls.
    /// </summary>
    /// <param name="rect">The client area rectangle for child controls.</param>
    API_FUNCTION()
    virtual void GetDesireClientArea(API_PARAM(Out) Rectangle &rect) const;

    /// <summary>
    /// Checks if given point in this container control space intersects with the child control content.
    /// Also calculates result location in child control space which can be used to feed control with event at that point.
    /// </summary>
    /// <param name="child">The child control to check.</param>
    /// <param name="location">The location in this container control space.</param>
    /// <param name="childSpaceLocation">The output location in child control space.</param>
    /// <returns>True if point is over the control content, otherwise false.</returns>
    API_FUNCTION()
    virtual bool IntersectsChildContent(UIXControl *child, const Float2 &location, API_PARAM(Out) Float2 &childSpaceLocation) const;

    /// <inheritdoc />
    API_FUNCTION()
    UIXControl* OnNavigate(UIXNavDirection direction, Float2 location, UIXControl* caller, Array<UIXControl*> &visited) override;

    /// <inheritdoc />
    API_FUNCTION()
    void OnDestroy() override;

    /// <inheritdoc />
    API_PROPERTY()
    bool GetIsTouchOver() const override;

    /// <inheritdoc />
    API_FUNCTION()
    void Update(float deltaTime) override;

    /// <inheritdoc />
    API_FUNCTION()
    void ClearState() override;

    /// <summary>
    /// Draw the control and the children.
    /// </summary>
    API_FUNCTION()
    void Draw() override;

    /// <summary>
    /// Draws the control.
    /// </summary>
    API_FUNCTION()
    virtual void DrawSelf();

    /// <inheritdoc />
    API_FUNCTION()
    void PerformLayout(bool force = false) override;

    /// <inheritdoc />
    API_FUNCTION()
    void OnMouseEnter(Float2 location) override;


    /// <inheritdoc />
    API_FUNCTION()
    void OnMouseMove(Float2 location) override;

    /// <inheritdoc />
    API_FUNCTION()
    void OnMouseLeave() override;

    /// <inheritdoc />
    API_FUNCTION()
    bool OnMouseWheel(Float2 location, float delta) override;

    /// <inheritdoc />
    API_FUNCTION()
    bool OnMouseDown(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION()
    bool OnMouseUp(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION()
    bool OnMouseDoubleClick(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    API_FUNCTION()
    bool IsTouchPointerOver(int pointerId) override;

    /// <inheritdoc />
    API_FUNCTION()
    void OnTouchEnter(Float2 location, int pointerId) override;

    /// <inheritdoc />
    API_FUNCTION()
    bool OnTouchDown(Float2 location, int pointerId) override;

    /// <inheritdoc />
    API_FUNCTION()
    void OnTouchMove(Float2 location, int pointerId) override;

    /// <inheritdoc />
    API_FUNCTION()
    bool OnTouchUp(Float2 location, int pointerId) override;

    /// <inheritdoc />
    API_FUNCTION()
    void OnTouchLeave(int pointerId) override;

    /// <inheritdoc />
    API_FUNCTION()
    bool OnCharInput(char c) override;

    /// <inheritdoc />
    API_FUNCTION()
    bool OnKeyDown(KeyboardKeys key) override;

    /// <inheritdoc />
    API_FUNCTION()
    void OnKeyUp(KeyboardKeys key) override;

    /// <inheritdoc />
    API_FUNCTION()
    DragDropEffect OnDragEnter(API_PARAM(Ref) Float2 &location, const DragData &data) override;

    /// <inheritdoc />
    API_FUNCTION()
    DragDropEffect OnDragMove(API_PARAM(Ref) Float2 &location, const DragData &data) override;

    /// <inheritdoc />
    API_FUNCTION()
    void OnDragLeave() override;

    /// <inheritdoc />
    API_FUNCTION()
    DragDropEffect OnDragDrop(API_PARAM(Ref) Float2 &location, const DragData &data) override;


protected:
    /// <inheritdoc />
    /*internal*/ void CacheRootHandle() override;

    /// <summary>
    /// Adds a child control to the container.
    /// </summary>
    /// <param name="child">The control to add.</param>
    API_FUNCTION()
    /*internal*/ virtual void AddChildInternal(UIXControl *child);

    /// <summary>
    /// Removes a child control from this container.
    /// </summary>
    /// <param name="child">The control to remove.</param>
    API_FUNCTION()
    /*internal*/ virtual void RemoveChildInternal(UIXControl *child);

    /// <summary>
    /// Checks if this container control can more with focus navigation into the given child control.
    /// </summary>
    /// <param name="child">The child.</param>
    /// <returns>True if can navigate to it, otherwise false.</returns>
    API_FUNCTION()
    virtual bool CanNavigateChild(UIXControl *child) const;

    /// <summary>
    /// Wraps the navigation over the layout.
    /// </summary>
    /// <param name="direction">The navigation direction.</param>
    /// <param name="location">The navigation start location (in the control-space).</param>
    /// <param name="visited">The list with visited controls. Used to skip recursive navigation calls when doing traversal across the UI hierarchy.</param>
    /// <param name="rightMostLocation">Returns the rightmost location of the parent container for the raycast used by the child container</param>
    /// <returns>The target navigation control or null if didn't performed any navigation.</returns>
    API_FUNCTION()
    virtual UIXControl* NavigationWrap(UIXNavDirection direction, const Float2 &location, Array<UIXControl*> &visited, API_PARAM(Out) Float2 &rightMostLocation);
   

    /// <summary>
     /// Update contain focus state and all it's children
     /// </summary>
    void UpdateContainsFocus();

    /// <summary>
    /// Updates child controls bounds.
    /// </summary>
    void UpdateChildrenBounds();

    /// <summary>
    /// Perform layout for that container control before performing it for child controls.
    /// </summary>
    API_FUNCTION()
    virtual void PerformLayoutBeforeChildren();

    /// <summary>
    /// Perform layout for that container control after performing it for child controls.
    /// </summary>
    API_FUNCTION()
    virtual void PerformLayoutAfterChildren() { }

    /// <summary>
    /// Draws the children. Can be overridden to provide some customizations. Draw is performed with applied clipping mask for the client area.
    /// </summary>
    API_FUNCTION()
    virtual void DrawChildren();

    /// <inheritdoc />
    API_FUNCTION()
    void OnSizeChanged() override;

    /// <summary>
    /// The children collection.
    /// </summary>
    API_FIELD(Attributes="NoSerialize,Readonly")
    Array<UIXControl*> _children;

    /// <summary>
    /// The contains focus cached flag.
    /// </summary>
    API_FIELD(Attributes="NoSerialize")
    bool _containsFocus;

    /// <summary>
    /// The layout locking flag.
    /// </summary>
    API_FIELD(Attributes="NoSerialize")
    bool _isLayoutLocked;

private:

    /*internal*/ void ChangeChildIndex(UIXControl *child, int newIndex);

    static bool CanGetAutoFocus(UIXControl *c);

    UIXControl* NavigationRaycast(UIXNavDirection direction, const Float2 &location, const Array<UIXControl*> &visited);



    bool _clipChildren = true;
    bool _cullChildren = true;

    friend class UIXControl;
};
