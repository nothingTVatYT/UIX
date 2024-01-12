#include "UIXCanvasRootControl.h"
#include "Engine/Core/Math/OrientedBoundingBox.h"
#include "Engine/Core/Math/Matrix.h"

UIXCanvasRootControl::UIXCanvasRootControl(UIXCanvas *canvas)
{
    _canvas = canvas;
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
    if (!p)
        return;
    UIXRootControl *r = p != nullptr ? p->GetRoot() : nullptr;
 
    if (r != nullptr)
        r->StartTrackingMouse(control, useMouseScreenOffset);
}


