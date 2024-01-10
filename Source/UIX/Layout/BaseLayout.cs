using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX.Layout;

/// <summary>
/// Base for layout classes used in UIXPanels
/// </summary>
public abstract class BaseLayout : ILayout
{
    public Float2 MinimumSize { get; set; }
    public Float2 MaximumSize { get; set; }

    public bool IsLayoutDone { get; set; }

    [NoSerialize]
    protected ContainerControl Container;
    protected List<Control> Controls = new();

    /// <summary>
    /// Create a new layout for the container
    /// </summary>
    /// <param name="container"></param>
    public BaseLayout(ContainerControl container)
    {
        Container = container;
    }

    /// <inheritdoc />
    public void AddChild(Control child)
    {
        Controls.Add(child);
        Container.AddChild(child);
    }

    /// <inheritdoc />
    public abstract void PerformLayout(ContainerControl container, bool force = false);
}