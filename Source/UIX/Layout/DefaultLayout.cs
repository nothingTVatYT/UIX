using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX.Layout;

/// <summary>
/// A default layout in a vertical box
/// </summary>
public class DefaultLayout : BaseLayout
{
    /// <inheritdoc />
    public DefaultLayout(ContainerControl container) : base(container)
    {
    }

    /// <inheritdoc />
    public override void PerformLayout(ContainerControl container, bool force = false)
    {
        Container = container;
        var clientArea = container.GetClientArea();
        var pos = clientArea.UpperLeft;
        var maxWidth = 0f;
        foreach (var child in Controls)
        {
            child.Pivot = Float2.Zero;
            child.PerformLayout(force);
            child.X = pos.X;
            child.Y = pos.Y;
            pos.Y += child.Height;
            if (pos.Y > MaximumSize.Y)
                pos.Y = MaximumSize.Y;
            maxWidth = Mathf.Min(Mathf.Max(maxWidth, pos.X + child.Width), MaximumSize.X);
        }

        if (container.Width < maxWidth)
            container.Width = maxWidth;
        if (container.Height < pos.Y)
            container.Height = pos.Y;
        IsLayoutDone = true;
    }

}