#include "UIXCanvasContainer.h"
#include "UIXCanvas.h"
#include "UIXCanvasRootControl.h"
#include "Engine/Graphics/RenderTask.h"
#include "Engine/Core/Collections/Sorting.h"
#include "Engine/Core/Math/Ray.h"
#include "Engine/Graphics/RenderTask.h"
#include "Engine/Graphics/RenderView.h"

UIXCanvasContainer::UIXCanvasContainer()
    : UIXContainerControl()
{
    // Enable ticking OnUpdate function
    //_tickUpdate = true;

    SetAnchorPreset(UIXAnchorPresets::StretchAll);
    SetOffsets(New<UIXMargin>(0.0f)); // TODO: Not having UIXMargin::Zero is very very annoying.
    SetAutoFocus(false);
}

namespace
{
    template<typename T>
    bool SortArrayPointersAsc(const T &a, const T &b, int *tmp) { return (uintptr_t)a < (uintptr_t)b; }
}

void UIXCanvasContainer::SortCanvases()
{
    int tmp;
    Array<UIXControl*> &children = GetChildren();
    Sorting::SortArray<UIXControl*, int>(children.Get(), children.Count(), &SortArrayPointersAsc<UIXControl*>, &tmp);
    //GetChildren.Sort(SortCanvas);
}

int UIXCanvasContainer::SortCanvas(UIXControl *a, UIXControl *b)
{
    return ((UIXCanvasRootControl*)a)->GetCanvas()->GetOrder() - ((UIXCanvasRootControl*)b)->GetCanvas()->GetOrder();
}

void UIXCanvasContainer::OnChildrenChanged()
{
    SortCanvases();
    UIXContainerControl::OnChildrenChanged();
}

void UIXCanvasContainer::DrawChildren()
{
    // Draw all screen space canvases
    auto layerMask = MainRenderTask::Instance != nullptr ? MainRenderTask::Instance->View.RenderLayersMask /*ViewLayersMask*/ : LayersMask();
    Array<UIXControl*> &children = GetChildren();
    for (int i = 0; i < children.Count(); i++)
    {
        UIXCanvasRootControl *child = (UIXCanvasRootControl*)children[i];

        if (child->GetVisible() && child->Is2D() && layerMask.HasLayer(child->GetCanvas()->GetLayer()))
        {
            child->Draw();
        }
    }
}

bool UIXCanvasContainer::IntersectsChildContent(UIXControl *child, Float2 location, API_PARAM(Out) Float2 &childSpaceLocation) const
{
    childSpaceLocation = Float2::Zero;
    return ((UIXCanvasRootControl*)child)->Is2D() && UIXContainerControl::IntersectsChildContent(child, location, childSpaceLocation);
}

void UIXCanvasContainer::OnMouseEnter(Float2 location)
{
    // 2D GUI first
    UIXContainerControl::OnMouseEnter(location);

    // Calculate 3D mouse ray
    Ray ray;
    UIXCanvas::CalculateRay(location, ray);

    // Test 3D
    LayersMask layerMask = MainRenderTask::Instance != nullptr ? MainRenderTask::Instance->View.RenderLayersMask /*ViewLayersMask*/ : LayersMask();
    Array<UIXControl*> &children = GetChildren();
    for (int i = children.Count() - 1; i >= 0 && children.Count() > 0; i--)
    {
        UIXCanvasRootControl* child = (UIXCanvasRootControl*)children[i];
        if (child->GetVisible() && child->GetEnabled() && child->Is3D() && layerMask.HasLayer(child->GetCanvas()->GetLayer()))
        {
            Float2 childLocation;
            if (child->Intersects3D(ray, childLocation))
            {
                child->OnMouseEnter(childLocation);
                return;
            }
        }
    }
}

void UIXCanvasContainer::OnMouseMove(Float2 location)
{
    // Calculate 3D mouse ray
    Ray ray;
    UIXCanvas::CalculateRay(location, ray);

    // Check all children collisions with mouse and fire events for them
    bool isFirst3DHandled = false;
    LayersMask layerMask = MainRenderTask::Instance != nullptr ? MainRenderTask::Instance->View.RenderLayersMask /*ViewLayersMask*/ : LayersMask();
    Array<UIXControl*> &children = GetChildren();
    for (int i = children.Count() - 1; i >= 0 && children.Count() > 0; i--)
    {
        UIXCanvasRootControl* child = (UIXCanvasRootControl*)children[i];
        if (child->GetVisible() && child->GetEnabled() && layerMask.HasLayer(child->GetCanvas()->GetLayer()))
        {
            // Fire events
            if (child->Is2D())
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
            else
            {
                Float2 childLocation;
                if (!isFirst3DHandled && child->Intersects3D(ray, childLocation))
                {
                    isFirst3DHandled = true;

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
    }
}

bool UIXCanvasContainer::OnMouseWheel(Float2 location, float delta)
{
    // 2D GUI first
    if (UIXContainerControl::OnMouseWheel(location, delta))
        return true;

    // Calculate 3D mouse ray
    Ray ray;
    UIXCanvas::CalculateRay(location, ray);

    // Test 3D
    LayersMask layerMask = MainRenderTask::Instance != nullptr ? MainRenderTask::Instance->View.RenderLayersMask /*ViewLayersMask*/ : LayersMask();
    Array<UIXControl*> &children = GetChildren();
    for (int i = children.Count()- 1; i >= 0 && children.Count()> 0; i--)
    {
        UIXCanvasRootControl* child = (UIXCanvasRootControl*)children[i];
        if (child->GetVisible() && child->GetEnabled() && child->Is3D() && layerMask.HasLayer(child->GetCanvas()->GetLayer()))
        {
            Float2 childLocation;
            if (child->Intersects3D(ray, childLocation))
            {
                child->OnMouseWheel(childLocation, delta);
                return true;
            }
        }
    }

    return false;
}

bool UIXCanvasContainer::OnMouseDown(Float2 location, MouseButton button)
{
    // 2D GUI first
    if (UIXContainerControl::OnMouseDown(location, button))
        return true;

    // Calculate 3D mouse ray
    Ray ray;
    UIXCanvas::CalculateRay(location, ray);

    // Test 3D
    LayersMask layerMask = MainRenderTask::Instance != nullptr ? MainRenderTask::Instance->View.RenderLayersMask /*ViewLayersMask*/ : LayersMask();
    Array<UIXControl*> &children = GetChildren();
    for (int i = children.Count() - 1; i >= 0 && children.Count() > 0; i--)
    {
        UIXCanvasRootControl* child = (UIXCanvasRootControl*)children[i];
        if (child->GetVisible() && child->GetEnabled() && child->Is3D() && layerMask.HasLayer(child->GetCanvas()->GetLayer()))
        {
            Float2 childLocation;
            if (child->Intersects3D(ray, childLocation))
            {
                child->OnMouseDown(childLocation, button);
                return true;
            }
        }
    }

    return false;
}

bool UIXCanvasContainer::OnMouseUp(Float2 location, MouseButton button)
{
    // 2D GUI first
    if (UIXContainerControl::OnMouseUp(location, button))
        return true;

    // Calculate 3D mouse ray
    Ray ray;
    UIXCanvas::CalculateRay(location, ray);

    // Test 3D
    LayersMask layerMask = MainRenderTask::Instance != nullptr ? MainRenderTask::Instance->View.RenderLayersMask /*ViewLayersMask*/ : LayersMask();
    Array<UIXControl*> &children = GetChildren();
    for (int i = children.Count() - 1; i >= 0 && children.Count() > 0; i--)
    {
        UIXCanvasRootControl* child = (UIXCanvasRootControl*)children[i];
        if (child->GetVisible() && child->GetEnabled() && child->Is3D() && layerMask.HasLayer(child->GetCanvas()->GetLayer()))
        {
            Float2 childLocation;
            if (child->Intersects3D(ray, childLocation))
            {
                child->OnMouseUp(childLocation, button);
                return true;
            }
        }
    }

    return false;
}

bool UIXCanvasContainer::OnMouseDoubleClick(Float2 location, MouseButton button)
{
    // 2D GUI first
    if (UIXContainerControl::OnMouseDoubleClick(location, button))
        return true;

    // Calculate 3D mouse ray
    Ray ray;
    UIXCanvas::CalculateRay(location, ray);

    // Test 3D
    LayersMask layerMask = MainRenderTask::Instance != nullptr ? MainRenderTask::Instance->View.RenderLayersMask /*ViewLayersMask*/ : LayersMask();
    Array<UIXControl*> &children = GetChildren();
    for (int i = children.Count() - 1; i >= 0 && children.Count() > 0; i--)
    {
        UIXCanvasRootControl* child = (UIXCanvasRootControl*)children[i];
        if (child->GetVisible() && child->GetEnabled() && child->Is3D() && layerMask.HasLayer(child->GetCanvas()->GetLayer()))
        {
            Float2 childLocation;
            if (child->Intersects3D(ray, childLocation))
            {
                child->OnMouseDoubleClick(childLocation, button);
                return true;
            }
        }
    }

    return false;
}

