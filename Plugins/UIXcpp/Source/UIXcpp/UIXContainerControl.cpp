#include "UIXContainerControl.h"
#include "UIXWindowRootControl.h"
#include "Engine/Core/Collections/Sorting.h"
#include "Engine/Render2D/Render2D.h"
#include "Engine/Core/Log.h"


UIXContainerControl::UIXContainerControl() : UIXControl()
{
    _isLayoutLocked = true;
}

UIXContainerControl::UIXContainerControl(float x, float y, float width, float height)
    : UIXControl( x, y, width, height), _isLayoutLocked(true)
{
}

UIXContainerControl::UIXContainerControl(const Float2 &location, const Float2 &size)
    : UIXControl(location, size), _isLayoutLocked(true)
{
}

UIXContainerControl::UIXContainerControl(const Rectangle &bounds)
    : UIXControl(bounds), _isLayoutLocked(true)
{
}


void UIXContainerControl::LockChildrenRecursive()
{
    _isLayoutLocked = true;
    for (int i = 0; i < _children.Count(); i++)
    {
        UIXContainerControl *child = dynamic_cast<UIXContainerControl*>(_children[i]);
        if (child != nullptr)
            child->LockChildrenRecursive();
    }
}

void UIXContainerControl::UnlockChildrenRecursive()
{
    _isLayoutLocked = false;
    for (int i = 0; i < _children.Count(); i++)
    {
        UIXContainerControl *child = dynamic_cast<UIXContainerControl*>(_children[i]);
        if (child != nullptr)
            child->UnlockChildrenRecursive();
    }
}

void UIXContainerControl::RemoveChildren()
{
    bool wasLayoutLocked = _isLayoutLocked;
    _isLayoutLocked = true;

    // Delete children
    while (_children.Count() > 0)
    {
        _children[0]->SetParent(nullptr);
    }

    _isLayoutLocked = wasLayoutLocked;
    PerformLayout();
}

void UIXContainerControl::DisposeChildren()
{
    bool wasLayoutLocked = _isLayoutLocked;
    _isLayoutLocked = true;

    // Delete children
    while (_children.Count() > 0)
    {
        _children[0]->Dispose();
    }

    _isLayoutLocked = wasLayoutLocked;
    PerformLayout();
}

void UIXContainerControl::RemoveChild(UIXControl *child)
{
    if (child == nullptr)
    {
        LOG(Error, "ArgumentNullException: Child to remove from container is null.");
        return;
    }
    if (child->GetParent() != this)
    {
        LOG(Error, "ArgumentNullException: Argument child cannot be removed, if current container is not its parent.");
        return;
    }

    // Unlink
    child->SetParent(nullptr);
}


UIXControl* UIXContainerControl::GetChild(int index)
{
    return _children[index];
}

int UIXContainerControl::GetChildIndex(const UIXControl *child)
{
    return _children.Find(child);
}


int UIXContainerControl::GetChildIndexAt(const Float2 &point) const
{
    int result = -1;
    for (int i = _children.Count() - 1; i >= 0; i--)
    {
        UIXControl *child = _children[i];
        Float2 childLocation;
        if (IntersectsChildContent(child, point, childLocation))
        {
            result = i;
            break;
        }
    }
    return result;
}


UIXControl* UIXContainerControl::GetChildAt(const Float2 &point)
{
    UIXControl *result = nullptr;
    for (int i = _children.Count() - 1; i >= 0; i--)
    {
        UIXControl *child = _children[i];
        Float2 childLocation;
        if (IntersectsChildContent(child, point, childLocation))
        {
            result = child;
            break;
        }
    }
    return result;
}

UIXControl* UIXContainerControl::GetChildAt(Float2 point, Function<bool(UIXControl*)> isValid)
{
    if (!isValid.IsBinded())
    {
        LOG(Error, "ArgumentNullException: Null delegate for getting child validity.");
        return nullptr;
    }
    UIXControl *result = nullptr;
    for (int i = _children.Count() - 1; i >= 0; i--)
    {
        UIXControl *child = _children[i];
        Float2 childLocation;
        if (isValid(child) && IntersectsChildContent(child, point, childLocation))
        {
            result = child;
            break;
        }
    }
    return result;
}

UIXControl* UIXContainerControl::GetChildAtRecursive(Float2 point)
{
    UIXControl *result = nullptr;
    for (int i = _children.Count() - 1; i >= 0; i--)
    {
        UIXControl *child = _children[i];
        Float2 childLocation;
        if (IntersectsChildContent(child, point, childLocation))
        {
            UIXContainerControl* containerControl = dynamic_cast<UIXContainerControl*>(child);
            UIXControl* childAtRecursive = containerControl != nullptr ? containerControl->GetChildAtRecursive(childLocation) : nullptr;
            if (childAtRecursive != nullptr && childAtRecursive->GetVisible())
            {
                child = childAtRecursive;
            }
            result = child;
            break;
        }
    }
    return result;
}

Rectangle UIXContainerControl::GetClientArea() const
{
    Rectangle clientArea;
    GetDesireClientArea(clientArea);
    return clientArea;
}

namespace
{
    template<typename T>
    bool SortArrayPointersAsc(const T &a, const T &b, int *tmp) { return (uintptr_t)a < (uintptr_t)b; }
}

void UIXContainerControl::SortChildren()
{
    
    //std::qsort(_children.Get(), _children.Count(), sizeof(UIXControl*), [](const UIXControl *a, const UIXControl *b) { return (uintptr_t)b - (uintptr_t)a; })
    int tmp;
    Sorting::SortArray<UIXControl*, int>(_children.Get(), _children.Count(), &SortArrayPointersAsc<UIXControl*>, &tmp);
    //_children.Sort();
    PerformLayout();
}


void UIXContainerControl::SortChildrenRecursive()
{
    SortChildren();

    for (int i = 0; i < _children.Count(); i++)
    {
        UIXContainerControl *child = dynamic_cast<UIXContainerControl*>(_children[i]);
        if (child != nullptr)
            child->SortChildrenRecursive();
    }
}

void UIXContainerControl::OnChildrenChanged()
{
    // Check if control isn't during disposing state
    if (!GetIsDisposing())
    {
        // Arrange child controls
        PerformLayout();
    }
}


void UIXContainerControl::CacheRootHandle()
{
    UIXControl::CacheRootHandle();

    for (int i = 0; i < _children.Count(); i++)
    {
        _children[i]->CacheRootHandle();
    }
}

void UIXContainerControl::AddChildInternal(UIXControl *child)
{
    if (child == nullptr)
    {
        //ASSERT(child != nullptr, "Invalid control.");
        LOG(Error, "Assert: child is nullptr.");
        return;
    }
    if (GetParent() == child)
    {
        LOG(Error, "InvalidOperationException: Child to add to container is parent of container");
        return;
    }

    // Add child
    _children.Add(child);

    OnChildrenChanged();
}

void UIXContainerControl::RemoveChildInternal(UIXControl *child)
{
    if (child == nullptr)
    {
        //ASSERT(child != nullptr, "Invalid control.");
        LOG(Error, "Assert: child is nullptr.");
        return;
    }

    // Remove child
    _children.Remove(child);

    OnChildrenChanged();
}

bool UIXContainerControl::CanNavigateChild(UIXControl *child) const
{
    return !child->GetIsFocused() && child->GetEnabled() && child->GetVisible() && CanGetAutoFocus(child);
}

UIXControl* UIXContainerControl::NavigationWrap(UIXNavDirection direction, const Float2 &location, API_PARAM(Ref) Array<UIXControl*> &visited, API_PARAM(Out) Float2 &rightMostLocation)
{
    auto rw = GetRootWindow();
    // This searches form a child that calls this navigation event (see Control.OnNavigate) to determinate the layout wrapping size based on that child size
    UIXControl *currentChild = rw != nullptr ? rw->GetFocusedControl() : nullptr;
    visited.Add(this);
    if (currentChild != nullptr)
    {
        Float2 layoutSize = currentChild->GetSize();
        Float2 predictedLocation = Float2::Minimum;
        switch (direction)
        {
            case UIXNavDirection::Next:
                predictedLocation = Float2(0, location.Y + layoutSize.Y);
                break;
            case UIXNavDirection::Previous:
                predictedLocation = Float2(GetSize().X, location.Y - layoutSize.Y);
                break;
        }
        if (Rectangle(Float2::Zero, GetSize()).Contains(predictedLocation))
        {
            UIXControl *result = NavigationRaycast(direction, predictedLocation, visited);
            if (result != nullptr)
            {
                rightMostLocation = predictedLocation;
                return result;
            }
        }
    }
    rightMostLocation = location;
    UIXContainerControl *p = GetParent();
    return p != nullptr ? p->NavigationWrap(direction, PointToParent(location), visited, rightMostLocation) : nullptr;
}

void UIXContainerControl::UpdateContainsFocus()
{
    // Get current state and update all children
    bool result = UIXControl::GetContainsFocus();

    for (int i = 0; i < _children.Count(); i++)
    {
        UIXControl *control = _children[i];
        UIXContainerControl *child = dynamic_cast<UIXContainerControl*>(control);
        if (child != nullptr)
            child->UpdateContainsFocus();
        if (control->GetContainsFocus())
            result = true;
    }

    // Check if state has been changed
    if (result != _containsFocus)
    {
        _containsFocus = result;
        if (result)
        {
            OnStartContainsFocus();
        }
        else
        {
            OnEndContainsFocus();
        }
    }
}

void UIXContainerControl::UpdateChildrenBounds()
{
    for (int i = 0; i < _children.Count(); i++)
    {
        _children[i]->UpdateBounds();
    }
}

void UIXContainerControl::PerformLayoutBeforeChildren()
{
    UpdateChildrenBounds();
}

void UIXContainerControl::DrawChildren()
{
    // Draw all visible child controls
    auto &children = _children;
    if (_cullChildren)
    {
        Rectangle globalClipping;
        Render2D::PeekClip(globalClipping);
        Matrix3x3 globalTransform;
        Render2D::PeekTransform(globalTransform);
        for (int i = 0; i < children.Count(); i++)
        {
            UIXControl *child = children[i];
            if (child->GetVisible())
            {
                Matrix3x3 globalChildTransform;
                Matrix3x3::Multiply(child->_cachedTransform, globalTransform, globalChildTransform);

                Rectangle childGlobalRect = Rectangle(globalChildTransform.M31, globalChildTransform.M32, child->GetWidth() * globalChildTransform.M11, child->GetHeight() * globalChildTransform.M22);
                if (globalClipping.Intersects(childGlobalRect))
                {
                    Render2D::PushTransform(child->_cachedTransform);
                    child->Draw();
                    Render2D::PopTransform();
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < children.Count(); i++)
        {
            UIXControl *child = children[i];
            if (child->GetVisible())
            {
                Render2D::PushTransform(child->_cachedTransform);
                child->Draw();
                Render2D::PopTransform();
            }
        }
    }
}

void UIXContainerControl::OnSizeChanged()
{
    // Lock updates to prevent additional layout calculations
    bool wasLayoutLocked = _isLayoutLocked;
    _isLayoutLocked = true;

    UIXControl::OnSizeChanged();

    // Fire event
    for (int i = 0; i < _children.Count(); i++)
    {
        _children[i]->OnParentResized();
    }

    // Restore state
    _isLayoutLocked = wasLayoutLocked;

    // Arrange child controls
    PerformLayout();
}


void UIXContainerControl::GetDesireClientArea(API_PARAM(Out) Rectangle &rect) const
{
    rect = Rectangle(Float2::Zero, GetSize());
}

bool UIXContainerControl::IntersectsChildContent(UIXControl *child, Float2 location, API_PARAM(Out) Float2 &childSpaceLocation) const
{
    return child->IntersectsContent(location, childSpaceLocation);
}

UIXControl* UIXContainerControl::OnNavigate(UIXNavDirection direction, Float2 location, UIXControl* caller, API_PARAM(Ref) Array<UIXControl*> &visited)
{
    // Try to focus itself first (only if navigation focus can enter this container)
    if (GetAutoFocus() && !GetContainsFocus())
        return this;

    // Try to focus children
    if (_children.Count() != 0 && !visited.Contains(this))
    {
        visited.Add(this);

        // Perform automatic navigation based on the layout
        UIXControl *result = NavigationRaycast(direction, location, visited);
        Float2 rightMostLocation = location;
        if (result == nullptr && (direction == UIXNavDirection::Next || direction == UIXNavDirection::Previous))
        {
            // Try wrap the navigation over the layout based on the direction
            Array<UIXControl*> visitedWrap(visited);
            result = NavigationWrap(direction, location, visitedWrap, rightMostLocation);
        }
        if (result != nullptr)
        {
            // HACK: only the 'previous' direction needs the rightMostLocation so i used a ternary conditional operator.
            // The rightMostLocation can probably become a 'desired raycast origin' that gets calculated correctly in the NavigationWrap method.
            Float2 useLocation = direction == UIXNavDirection::Previous ? rightMostLocation : location;
            result = result->OnNavigate(direction, result->PointFromParent(useLocation), this, visited);
            if (result != nullptr)
                return result;
        }
    }

    // Try to focus itself
    if (GetAutoFocus() && !GetIsFocused() || caller == this)
        return this;

    // Route navigation to parent
    UIXContainerControl *parent = GetParent();
    if (GetAutoFocus() && GetVisible())
    {
        // Focusable container controls use own nav origin instead of the provided one
        location = GetNavOrigin(direction);
    }
    return parent != nullptr ? parent->OnNavigate(direction, PointToParent(location), caller, visited) : nullptr;
}


void UIXContainerControl::OnDestroy()
{
    // Steal focus from children
    if (GetContainsFocus())
    {
        Focus();
    }

    // Disable layout
    if (!_isLayoutLocked)
    {
        LockChildrenRecursive();
    }

    UIXControl::OnDestroy();

    // Pass event further
    for (int i = 0; i < _children.Count(); i++)
    {
        _children[i]->OnDestroy();
    }
    _children.Clear();
}

bool UIXContainerControl::GetIsTouchOver() const
{
    if (UIXControl::GetIsTouchOver())
        return true;
    for (int i = 0; i < _children.Count() /*&& _children.Count() > 0*/; i++)
    {
        if (_children[i]->GetIsTouchOver())
            return true;
    }
    return false;
}

void UIXContainerControl::Update(float deltaTime)
{
    UIXControl::Update(deltaTime);

    // Update all enabled child controls
    for (int i = 0; i < _children.Count(); i++)
    {
        if (_children[i]->GetEnabled())
        {
            _children[i]->Update(deltaTime);
        }
    }
}

void UIXContainerControl::ClearState()
{
    UIXControl::ClearState();

    // Clear state for any nested controls
    for (int i = 0; i < _children.Count(); i++)
    {
        UIXControl *child = _children[i];
        //if (child.Enabled && child.Enabled) // Already commented out in the original C#
        child->ClearState();
    }
}

void UIXContainerControl::Draw()
{
    DrawSelf();

    if (_clipChildren)
    {
        Rectangle clientArea;
        GetDesireClientArea(clientArea);
        Render2D::PushClip(clientArea);
        DrawChildren();
        Render2D::PopClip();
    }
    else
    {
        DrawChildren();
    }
}

void UIXContainerControl::DrawSelf()
{
    UIXControl::Draw();
}

void UIXContainerControl::PerformLayout(bool force)
{
    if (_isLayoutLocked && !force)
        return;

    bool wasLocked = _isLayoutLocked;
    if (!wasLocked)
        LockChildrenRecursive();

    PerformLayoutBeforeChildren();

    for (int i = 0; i < _children.Count(); i++)
        _children[i]->PerformLayout(true);

    PerformLayoutAfterChildren();

    if (!wasLocked)
        UnlockChildrenRecursive();
}

void UIXContainerControl::OnMouseEnter(Float2 location)
{
    // Check all children collisions with mouse and fire events for them
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                // Enter
                child->OnMouseEnter(childLocation);
            }
        }
    }

    UIXControl::OnMouseEnter(location);
}

void UIXContainerControl::OnMouseMove(Float2 location)
{
    // Check all children collisions with mouse and fire events for them
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                if (child->GetIsMouseOver())
                {
                    // Move
                    child->OnMouseMove(childLocation);
                }
                else
                {
                    // Enter
                    child->OnMouseEnter(childLocation);
                }
            }
            else if (child->GetIsMouseOver())
            {
                // Leave
                child->OnMouseLeave();
            }
        }
    }

    UIXControl::OnMouseMove(location);
}

void UIXContainerControl::OnMouseLeave()
{
    // Check all children collisions with mouse and fire events for them
    for (int i = 0; i < _children.Count() && _children.Count() > 0; i++)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled() && child->GetIsMouseOver())
        {
            // Leave
            child->OnMouseLeave();
        }
    }

    UIXControl::OnMouseLeave();
}

bool UIXContainerControl::OnMouseWheel(Float2 location, float delta)
{
    // Check all children collisions with mouse and fire events for them
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                if (child->OnMouseWheel(childLocation, delta))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool UIXContainerControl::OnMouseDown(Float2 location, MouseButton button)
{
    // Check all children collisions with mouse and fire events for them
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                if (child->OnMouseDown(childLocation, button))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool UIXContainerControl::OnMouseUp(Float2 location, MouseButton button)
{
    // Check all children collisions with mouse and fire events for them
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                if (child->OnMouseUp(childLocation, button))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool UIXContainerControl::OnMouseDoubleClick(Float2 location, MouseButton button)
{
    // Check all children collisions with mouse and fire events for them
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                if (child->OnMouseDoubleClick(childLocation, button))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool UIXContainerControl::IsTouchPointerOver(int pointerId)
{
    if (UIXControl::IsTouchPointerOver(pointerId))
        return true;

    for (int i = 0; i < _children.Count() && _children.Count() > 0; i++)
    {
        if (_children[i]->IsTouchPointerOver(pointerId))
            return true;
    }

    return false;
}

void UIXContainerControl::OnTouchEnter(Float2 location, int pointerId)
{
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled() && !child->IsTouchPointerOver(pointerId))
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                child->OnTouchEnter(childLocation, pointerId);
            }
        }
    }

    UIXControl::OnTouchEnter(location, pointerId);
}

bool UIXContainerControl::OnTouchDown(Float2 location, int pointerId)
{
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                if (!child->IsTouchPointerOver(pointerId))
                {
                    child->OnTouchEnter(location, pointerId);
                }
                if (child->OnTouchDown(childLocation, pointerId))
                {
                    return true;
                }
            }
        }
    }

    return UIXControl::OnTouchDown(location, pointerId);
}

void UIXContainerControl::OnTouchMove(Float2 location, int pointerId)
{
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                if (child->IsTouchPointerOver(pointerId))
                {
                    child->OnTouchMove(childLocation, pointerId);
                }
                else
                {
                    child->OnTouchEnter(childLocation, pointerId);
                }
            }
            else if (child->IsTouchPointerOver(pointerId))
            {
                child->OnTouchLeave(pointerId);
            }
        }
    }

    UIXControl::OnTouchMove(location, pointerId);
}

bool UIXContainerControl::OnTouchUp(Float2 location, int pointerId)
{
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled() && child->IsTouchPointerOver(pointerId))
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                if (child->OnTouchUp(childLocation, pointerId))
                {
                    return true;
                }
            }
        }
    }

    return UIXControl::OnTouchUp(location, pointerId);
}

void UIXContainerControl::OnTouchLeave(int pointerId)
{
    for (int i = 0; i < _children.Count() && _children.Count() > 0; i++)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled() && child->IsTouchPointerOver(pointerId))
        {
            child->OnTouchLeave(pointerId);
        }
    }

    UIXControl::OnTouchLeave(pointerId);
}

bool UIXContainerControl::OnCharInput(char c)
{
    for (int i = 0; i < _children.Count() && _children.Count() > 0; i++)
    {
        UIXControl *child = _children[i];
        if (child->GetEnabled() && child->GetContainsFocus())
        {
            return child->OnCharInput(c);
        }
    }
    return false;
}

bool UIXContainerControl::OnKeyDown(KeyboardKeys key)
{
    for (int i = 0; i < _children.Count() && _children.Count() > 0; i++)
    {
        UIXControl *child = _children[i];
        if (child->GetEnabled() && child->GetContainsFocus())
        {
            return child->OnKeyDown(key);
        }
    }
    return false;
}

void UIXContainerControl::OnKeyUp(KeyboardKeys key)
{
    for (int i = 0; i < _children.Count() && _children.Count() > 0; i++)
    {
        UIXControl *child = _children[i];
        if (child->GetEnabled() && child->GetContainsFocus())
        {
            child->OnKeyUp(key);
            break;
        }
    }
}

DragDropEffect UIXContainerControl::OnDragEnter(Float2 location, const DragData &data)
{
    // Base
    DragDropEffect result = UIXControl::OnDragEnter(location, data);

    // Check all children collisions with mouse and fire events for them
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                // Enter
                result = child->OnDragEnter(childLocation, data);
                if (result != DragDropEffect::None)
                    break;
            }
        }
    }

    return result;
}

DragDropEffect UIXContainerControl::OnDragMove(Float2 location, const DragData &data)
{
    // Base
    DragDropEffect result = UIXControl::OnDragMove(location, data);

    // Check all children collisions with mouse and fire events for them
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                if (child->GetIsDragOver())
                {
                    // Move
                    DragDropEffect tmpResult = child->OnDragMove(childLocation, data);
                    if (tmpResult != DragDropEffect::None)
                        result = tmpResult;
                }
                else
                {
                    // Enter
                    DragDropEffect tmpResult = child->OnDragEnter(childLocation, data);
                    if (tmpResult != DragDropEffect::None)
                        result = tmpResult;
                }
            }
            else if (child->GetIsDragOver())
            {
                // Leave
                child->OnDragLeave();
            }
        }
    }

    return result;
}

void UIXContainerControl::OnDragLeave()
{
    // Base
    UIXControl::OnDragLeave();

    // Check all children collisions with mouse and fire events for them
    for (int i = 0; i < _children.Count() && _children.Count() > 0; i++)
    {
        UIXControl *child = _children[i];
        if (child->GetIsDragOver())
        {
            // Leave
            child->OnDragLeave();
        }
    }
}

DragDropEffect UIXContainerControl::OnDragDrop(Float2 location, const DragData &data)
{
    // Base
    DragDropEffect result = UIXControl::OnDragDrop(location, data);

    // Check all children collisions with mouse and fire events for them
    for (int i = _children.Count() - 1; i >= 0 && _children.Count() > 0; i--)
    {
        UIXControl *child = _children[i];
        if (child->GetVisible() && child->GetEnabled())
        {
            Float2 childLocation;
            if (IntersectsChildContent(child, location, childLocation))
            {
                // Enter
                result = child->OnDragDrop(childLocation, data);
                if (result != DragDropEffect::None)
                    break;
            }
        }
    }

    return result;
}

void UIXContainerControl::ChangeChildIndex(UIXControl *child, int newIndex)
{
    int oldIndex = _children.Find(child);
    if (oldIndex == newIndex || oldIndex == -1)
        return;

    _children.RemoveAt(oldIndex);

    if (newIndex < 0 || newIndex >= _children.Count())
    {
        // Append at the end
        _children.Add(child);
    }
    else
    {
        // Change order
        _children.Insert(newIndex, child);
    }

    PerformLayout();
}

namespace
{
    float RectangleDistance(const Rectangle &a, const Rectangle &b)
    {
        return Float2::Max(Float2::Zero, Float2::Abs(a.GetCenter() - b.GetCenter()) - ((a.Size + b.Size) * 0.5f)).Length();
    }

    float RectangleDistance(const Rectangle &rect, const Float2 &p)
    {
        Float2 max = rect.Location + rect.Size;
        float dx = Math::Max(Math::Max(rect.Location.X - p.X, p.X - max.X), 0.f);
        float dy = Math::Max(Math::Max(rect.Location.Y - p.Y, p.Y - max.Y), 0.f);
        return Math::Sqrt(dx * dx + dy * dy);
    }
}

UIXControl* UIXContainerControl::NavigationRaycast(UIXNavDirection direction, const Float2 &location, const Array<UIXControl*> &visited)
{
    Float2 uiDir1 = Float2::Zero, uiDir2 = Float2::Zero;
    switch (direction)
    {
        case UIXNavDirection::Up:
            uiDir1 = uiDir2 = Float2(0, -1);
            break;
        case UIXNavDirection::Down:
            uiDir1 = uiDir2 = Float2(0, 1);
            break;
        case UIXNavDirection::Left:
            uiDir1 = uiDir2 = Float2(-1, 0);
            break;
        case UIXNavDirection::Right:
            uiDir1 = uiDir2 = Float2(1, 0);
            break;
        case UIXNavDirection::Next:
            uiDir1 = Float2(1, 0);
            uiDir2 = Float2(0, 1);
            break;
        case UIXNavDirection::Previous:
            uiDir1 = Float2(-1, 0);
            uiDir2 = Float2(0, -1);
            break;
    }
    UIXControl *result = nullptr;
    float minDistance = MAX_float;
    for (int i = 0; i < _children.Count(); i++)
    {
        UIXControl *child = _children[i];
        if (!CanNavigateChild(child) || visited.Contains(child))
            continue;
        Float2 childNavLocation = child->GetCenter();
        Rectangle childBounds = child->GetBounds();
        Float2 childNavDirection = Float2::Normalize(childNavLocation - location);
        float childNavCoherence1 = Float2::Dot(uiDir1, childNavDirection);
        float childNavCoherence2 = Float2::Dot(uiDir2, childNavDirection);
        auto distance = RectangleDistance(childBounds, location);
        if (childNavCoherence1 > 1e-6f && childNavCoherence2 > 1e-6f && distance < minDistance)
        {
            minDistance = distance;
            result = child;
        }
    }
    return result;
}

bool UIXContainerControl::CanGetAutoFocus(UIXControl *c)
{
    if (c->GetAutoFocus())
        return true;

    UIXContainerControl *cc = dynamic_cast<UIXContainerControl*>(c);
    if (cc != nullptr)
    {
        auto &chd = cc->GetChildren();
        for (int i = 0; i < chd.Count(); i++)
        {
            if (cc->CanNavigateChild(chd[i]))
                return true;
        }
    }
    return false;
}
