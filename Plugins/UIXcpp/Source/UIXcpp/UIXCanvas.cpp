#include "UIXCanvas.h"

UIXCanvas::UIXCanvas(const SpawnParams& params) : Actor(params)
{
    _guiRoot = new UIXCanvasRootControl(this);
    _guiRoot->SetIsLayoutLocked(false);
}

UIXCanvas::~UIXCanvas()
{
    if (_isRegisteredForTick)
    {
        // TODO: Figure out how to register to Scripting Events.
        _isRegisteredForTick = false;
        Scripting::Update -= OnUpdate;
    }
}

void UIXCanvas::SetRenderMode(UIXCanvasRenderMode value)
{
    if (_renderMode != value)
    {
        auto previous = _renderMode;

        _renderMode = value;

        Setup();

        // Reset size
        if (previous == UIXCanvasRenderMode::ScreenSpace || (_renderMode == UIXCanvasRenderMode::WorldSpace || _renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera))
            SetSize(Float2(500, 500));
    }
}

void UIXCanvas::SetOrder(int value)
{
    if (_order != value)
    {
        _order = value;
        RootControl.CanvasRoot.SortCanvases();
    }
}

void UIXCanvas::SetSize(Float2 value)
{
    if (_renderMode == UIXCanvasRenderMode::WorldSpace || _renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera || _isLoading)
    {
        _guiRoot->SetSize(value);
    }
}

void UIXCanvas::DefaultCalculateRay(Float2 location, Ray& ray)
{
    auto camera = Camera::GetMainCamera();
    if (camera)
    {
        ray = camera->ConvertMouseToRay(location * Platform::GetDpiScale());
    }
    else
    {
        ray = Ray(Vector3::Zero, Vector3::Forward);
    }
}
