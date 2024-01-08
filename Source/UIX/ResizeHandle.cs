using FlaxEngine;

namespace UIX;

/// <summary>
/// A resize handle that tracks mouse dragging
/// </summary>
public class ResizeHandle : MouseTrackingControl
{
    /// <summary>
    /// Create a new resize handle
    /// </summary>
    public ResizeHandle()
    {
        Width = 12;
        Height = 12;
    }

    /// <inheritdoc />
    public override void Draw()
    {
        Render2D.DrawSprite(UIXStyle.Style.StatusBarSizeGrip, new Rectangle(Width - 12, Height - 12, 12, 12), UIXStyle.Style.Foreground);
    }

}