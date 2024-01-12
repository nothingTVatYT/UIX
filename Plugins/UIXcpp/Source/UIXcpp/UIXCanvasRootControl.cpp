#include "UIXCanvasRootControl.h"
#include "Engine/Core/Math/OrientedBoundingBox.h"
#include "Engine/Core/Math/Matrix.h"
#include "Engine/Input/Input.h"
#include "Engine/Level/Actors/Camera.h"


UIXCanvasRootControl::UIXCanvasRootControl(UIXCanvas *canvas)
{
    _canvas = canvas;
}

bool UIXCanvasRootControl::Is2D() const
{
    return _canvas->RenderMode == CanvasRenderMode::ScreenSpace;
}

bool UIXCanvasRootControl::Is3D() const
{
    return _canvas->RenderMode != CanvasRenderMode::ScreenSpace;
}

UIXCanvas* UIXCanvasRootControl::GetCanvas() const
{
    return _canvas;
}

bool UIXCanvasRootControl::Intersects3D(API_PARAM(Ref) Ray &ray, API_PARAM(Out) Float2 &canvasLocation) const
{
    // Inline bounds calculations (it will reuse world matrix)
    OrientedBoundingBox bounds = OrientedBoundingBox();

    //TODO: replace 0.1e-6 everywhere with an epsilon where the original code used it
    bounds.Extents = Vector3(GetSize() * 0.5f, 0.1e-6);

    Matrix world;
    _canvas->GetWorldMatrix( world);

    Matrix offset;
    Matrix::Translation((float)bounds.Extents.X, (float)bounds.Extents.Y, 0, offset);
    Matrix boxWorld;
    Matrix::Multiply(offset, world, boxWorld);
    boxWorld.Decompose(bounds.Transformation);

    // Hit test
    Vector3 hitPoint;
    if (bounds.Intersects(ray, hitPoint))
    {
        // Transform world-space hit point to canvas local-space
        world.Invert();
        Vector3 localHitPoint;
        Vector3::Transform(hitPoint, world, localHitPoint);

        canvasLocation = Float2(localHitPoint);
        return ContainsPoint(canvasLocation);
    }

    canvasLocation = Float2::Zero;
    return false;
}

UIXControl* UIXCanvasRootControl::GetFocusedControl() const
{
    UIXContainerControl *p = GetParent();
    return p != nullptr ? p->GetRoot()->GetFocusedControl() : nullptr;
}

void UIXCanvasRootControl::SetFocusedControl(UIXControl *value)
{
    UIXContainerControl *p = GetParent();
    if (p != nullptr)
        p->GetRoot()->SetFocusedControl(value);
}

void UIXCanvasRootControl::StartTrackingMouse(UIXControl *control, bool useMouseScreenOffset)
{
    UIXContainerControl *p = GetParent();
    if (p == nullptr)
        return;
    UIXRootControl *r = p != nullptr ? p->GetRoot() : nullptr;
 
    if (r != nullptr)
        r->StartTrackingMouse(control, useMouseScreenOffset);
}


void UIXCanvasRootControl::EndTrackingMouse()
{
    UIXContainerControl *p = GetParent();
    if (p == nullptr)
        return;
    auto r = p->GetRoot();
    if (r != nullptr)
        r->EndTrackingMouse();
}

bool UIXCanvasRootControl::GetKey(KeyboardKeys key)
{
    return Input::GetKey(key);
}

/// <inheritdoc />
bool UIXCanvasRootControl::GetKeyDown(KeyboardKeys key)
{
    return Input::GetKeyDown(key);
}

/// <inheritdoc />
bool UIXCanvasRootControl::GetKeyUp(KeyboardKeys key)
{
    return Input::GetKeyUp(key);
}

/// <inheritdoc />
bool UIXCanvasRootControl::GetMouseButton(MouseButton button)
{
    return Input::GetMouseButton(button);
}

/// <inheritdoc />
bool UIXCanvasRootControl::GetMouseButtonDown(MouseButton button)
{
    return Input::GetMouseButtonDown(button);
}

/// <inheritdoc />
bool UIXCanvasRootControl::GetMouseButtonUp(MouseButton button)
{
    return Input::GetMouseButtonUp(button);
}

Float2 UIXCanvasRootControl::PointToParent(Float2 location) const
{
    if (Is2D())
        return UIXRootControl::PointToParent(location);

    Camera *camera = Camera::GetMainCamera();
    if (camera == nullptr)
        return location;

    // Transform canvas local-space point to the game root location
    Matrix world
    _canvas->GetWorldMatrix(world);
    Vector3 locationCanvasSpace = Vector3(location, 0.0f);
    Vector3 locationWorldSpace;
    Vector3::Transform(locationCanvasSpace, world, locationWorldSpace);
    camera->ProjectPoint(locationWorldSpace, location);
    return location;
}

/// <inheritdoc />
Float2 UIXCanvasRootControl::PointFromParent(Float2 locationParent) const
{
    if (Is2D())
        return UIXRootControl::PointFromParent(locationParent);

    Camera *camera = Camera::GetMainCamera();
    if (camera == nullptr)
        return locationParent;

    // Use world-space ray to convert it to the local-space of the canvas
    Ray ray;
    UIXCanvas::CalculateRay(locationParent, ray);
    Float2 location;
    Intersects3D(ray, location);
    return location;
}

bool UIXCanvasRootControl::ContainsPoint(Float2 location) const
{
    return UIXRootControl::ContainsPoint(location) && (_canvas->TestCanvasIntersection == nullptr || _canvas->TestCanvasIntersection(location));
}

void UIXCanvasRootControl::Update(float deltaTime)
{
    // Update navigation
    if (SkipEvents)
    {
        _navigationHeldTimeUp = _navigationHeldTimeDown = _navigationHeldTimeLeft = _navigationHeldTimeRight = 0;
        _navigationRateTimeUp = _navigationRateTimeDown = _navigationRateTimeLeft = _navigationRateTimeRight = 0;
    }

    UpdateNavigation(deltaTime, _canvas->NavigateUp.Name, UIXNavDirection::Up, _navigationHeldTimeUp, _navigationRateTimeUp);
    UpdateNavigation(deltaTime, _canvas->NavigateDown.Name, UIXNavDirection::Down, _navigationHeldTimeDown, _navigationRateTimeDown);
    UpdateNavigation(deltaTime, _canvas->NavigateLeft.Name, UIXNavDirection::Left, _navigationHeldTimeLeft, _navigationRateTimeLeft);
    UpdateNavigation(deltaTime, _canvas->NavigateRight.Name, UIXNavDirection::Right, _navigationHeldTimeRight, _navigationRateTimeRight);
    UpdateNavigation(deltaTime, _canvas->NavigateSubmit.Name, _navigationHeldTimeSubmit, _navigationRateTimeSubmit);

    UIXRootControl::Update(deltaTime);
}


void UIXCanvasRootControl::UpdateNavigation(float deltaTime, String actionName, UIXNavDirection direction, API_PARAM(Ref) float &heldTime, API_PARAM(Ref) float &rateTime)
{
    if (Input::GetAction(actionName))
    {
        // TODO: fix epsilon and make it into a const somewhere
        if (heldTime <= 0.1e-6)
        {
            Navigate(direction);
        }
        if (heldTime > _canvas->NavigationInputRepeatDelay)
        {
            rateTime += deltaTime;
        }
        if (rateTime > _canvas->NavigationInputRepeatRate)
        {
            Navigate(direction);
            rateTime = 0;
        }
        heldTime += deltaTime;
    }
    else
    {
        heldTime = rateTime = 0;
    }
}

void UIXCanvasRootControl::UpdateNavigation(float deltaTime, String actionName, API_PARAM(Ref) float &heldTime, API_PARAM(Ref) float &rateTime/*, Action action*/)
{
    if (Input::GetAction(actionName))
    {
        // TODO: fix epsilon and make it into a const somewhere
        if (heldTime <= 0.1e-6)
        {
            SubmitFocused();
        }
        if (heldTime > _canvas->NavigationInputRepeatDelay)
        {
            rateTime += deltaTime;
        }
        if (rateTime > _canvas->NavigationInputRepeatRate)
        {
            SubmitFocused();
            rateTime = 0;
        }
        heldTime += deltaTime;
    }
    else
    {
        heldTime = rateTime = 0;
    }
}

bool UIXCanvasRootControl::OnCharInput(char c)
{
    if (SkipEvents)
        return false;

    return UIXRootControl::OnCharInput(c);
}


DragDropEffect UIXCanvasRootControl::OnDragDrop(Float2 location, const DragData &data)
{
    if (SkipEvents)
        return DragDropEffect::None;

    return UIXRootControl::OnDragDrop(location, data);
}


DragDropEffect UIXCanvasRootControl::OnDragEnter(Float2 location, const DragData &data)
{
    if (SkipEvents)
        return DragDropEffect::None;

    return UIXRootControl::OnDragEnter(location, data);
}


void UIXCanvasRootControl::OnDragLeave()
{
    if (SkipEvents)
        return;

    UIXRootControl::OnDragLeave();
}


DragDropEffect UIXCanvasRootControl::OnDragMove(Float2 location, const DragData &data)
{
    if (SkipEvents)
        return DragDropEffect::None;

    return UIXRootControl::OnDragMove(location, data);
}


bool UIXCanvasRootControl::OnKeyDown(KeyboardKeys key)
{
    if (SkipEvents)
        return false;

    return UIXRootControl::OnKeyDown(key);
}


void UIXCanvasRootControl::OnKeyUp(KeyboardKeys key)
{
    if (SkipEvents)
        return;

    UIXRootControl::OnKeyUp(key);
}


bool UIXCanvasRootControl::OnMouseDoubleClick(Float2 location, MouseButton button)
{
    if (SkipEvents)
        return false;

    return UIXRootControl::OnMouseDoubleClick(location, button);
}


bool UIXCanvasRootControl::OnMouseDown(Float2 location, MouseButton button)
{
    if (SkipEvents)
        return false;

    return UIXRootControl::OnMouseDown(location, button);
}


void UIXCanvasRootControl::OnMouseEnter(Float2 location)
{
    if (SkipEvents)
        return;

    _mousePosition = location;
    UIXRootControl::OnMouseEnter(location);
}


void UIXCanvasRootControl::OnMouseLeave()
{
    _mousePosition = Float2::Zero;
    if (SkipEvents)
        return;

    UIXRootControl::OnMouseLeave();
}


void UIXCanvasRootControl::OnMouseMove(Float2 location)
{
    if (SkipEvents)
        return;

    _mousePosition = location;
    UIXRootControl::OnMouseMove(location);
}


bool UIXCanvasRootControl::OnMouseUp(Float2 location, MouseButton button)
{
    if (SkipEvents)
        return false;

    return UIXRootControl::OnMouseUp(location, button);
}


bool UIXCanvasRootControl::OnMouseWheel(Float2 location, float delta)
{
    if (SkipEvents)
        return false;

    return UIXRootControl::OnMouseWheel(location, delta);
}


void UIXCanvasRootControl::OnTouchEnter(Float2 location, int pointerId)
{
    if (SkipEvents)
        return;

    UIXRootControl::OnTouchEnter(location, pointerId);
}


bool UIXCanvasRootControl::OnTouchDown(Float2 location, int pointerId)
{
    if (SkipEvents)
        return false;

    return UIXRootControl::OnTouchDown(location, pointerId);
}


void UIXCanvasRootControl::OnTouchMove(Float2 location, int pointerId)
{
    if (SkipEvents)
        return;

    UIXRootControl::OnTouchMove(location, pointerId);
}


bool UIXCanvasRootControl::OnTouchUp(Float2 location, int pointerId)
{
    if (SkipEvents)
        return false;

    return UIXRootControl::OnTouchUp(location, pointerId);
}


void UIXCanvasRootControl::OnTouchLeave(int pointerId)
{
    if (SkipEvents)
        return;

    UIXRootControl::OnTouchLeave(pointerId);
}


void UIXCanvasRootControl::Focus()
{
}


void UIXCanvasRootControl::DoDragDrop(const DragData &data)
{
}

