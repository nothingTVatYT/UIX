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
    public int NumberChildren => Controls.Count;

    [NoSerialize]
    protected ContainerControl Container;
    protected List<Control> Controls;
    protected Dictionary<Control, ControlDetails> ControlDetails;

    /// <summary>
    /// Create a new layout for the container
    /// </summary>
    /// <param name="container"></param>
    public BaseLayout(ContainerControl container)
    {
        Container = container;
        Controls = new List<Control>();
        ControlDetails = new Dictionary<Control, ControlDetails>();
    }

    /// <inheritdoc />
    public virtual void AddChild(Control child)
    {
        Controls.Add(child);
        // ideally the controls would set min/preferred/max size
        ControlDetails[child] = new ControlDetails
        {
            PreferredSize = child.Size,
            MinimumSize = LayoutTools.GetMinimumSize(child)
        };
        Container.AddChild(child);
    }

    public void RemoveChild(int index)
    {
        var control = GetChild(index);
        if (control == null) return;
        Container.RemoveChild(control);
        Controls.Remove(control);
        control.Dispose();
    }

    /// <inheritdoc />
    public Control GetChild(int index)
    {
        return (index >= 0 && index < Controls.Count) ? Controls[index] : null;
    }

    /// <inheritdoc />
    public abstract void PerformLayout(ContainerControl container, bool force = false);
}