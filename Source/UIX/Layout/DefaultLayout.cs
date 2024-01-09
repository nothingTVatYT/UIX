using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX.Layout;

/// <summary>
/// A default layout in a vertical box
/// </summary>
public class DefaultLayout : ILayout
{
    private ContainerControl _container;
    private List<Control> _controls = new();

    /// <inheritdoc />
    public Float2 MinimumSize { get; set; }

    /// <inheritdoc />
    public Float2 MaximumSize { get; set; }

    /// <summary>
    /// Creates a new default layout, a vertical layout that will expand the parent if necessary.
    /// </summary>
    /// <param name="container"></param>
    public DefaultLayout(ContainerControl container)
    {
        _container = container;
    }

    /// <inheritdoc />
    public void AddChild(Control child)
    {
        _controls.Add(child);
        _container.AddChild(child);
    }

    /// <inheritdoc />
    public void PerformLayout(ContainerControl container, bool force = false)
    {
        _container = container;
        var clientArea = container.GetClientArea();
        var pos = clientArea.UpperLeft;
        var maxWidth = 0f;
        foreach (var child in _controls)
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
    }
}