using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX;

/// <summary>
/// A default layout in a vertical box
/// </summary>
public class DefaultLayout : ILayout
{
    public ContainerControl Container;
    private List<Control> _controls = new();

    /// <inheritdoc />
    public Float2 MinimumSize { get; set; }

    /// <inheritdoc />
    public Float2 MaximumSize { get; set; }

    public DefaultLayout(ContainerControl container)
    {
        Container = container;
    }

    public void AddChild(Control child)
    {
        _controls.Add(child);
        Container.AddChild(child);
    }

    /// <inheritdoc />
    public void PerformLayout(ContainerControl container, bool force = false)
    {
        var clientArea = container.GetClientArea();
        var pos = clientArea.UpperLeft;
        var maxWidth = 0f;
        foreach (var child in _controls)
        {
            child.PerformLayout(force);
            child.LocalLocation = pos;
            pos.Y += child.Height;
            if (pos.Y > MaximumSize.Y)
                pos.Y = MaximumSize.Y;
            maxWidth = Mathf.Min(Mathf.Max(maxWidth, pos.X + child.Width), MaximumSize.X);
        }

        if (container.Width < maxWidth)
            container.Width = maxWidth;
        if (container.Height < pos.Y)
            container.Height = pos.Y;
    }
}