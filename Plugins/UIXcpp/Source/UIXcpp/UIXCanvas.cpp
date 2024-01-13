#include "UIXCanvas.h"

void UIXCanvas::SetRenderMode(UIXCanvasRenderMode value)
{
    if (_renderMode != value)
    {
        auto previous = _renderMode;

        _renderMode = value;

        Setup();

        // Reset size
        if (previous == UIXCanvasRenderMode::ScreenSpace || (_renderMode == UIXCanvasRenderMode::WorldSpace || _renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera))
            Size = new Float2(500, 500);
    }
}
