using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX;

public interface ILayout
{
    /// <summary>
    /// gets and sets the minimum size of this container
    /// </summary>
    public Float2 MinimumSize { get; set; }

    /// <summary>
    /// gets and sets the maximum size this container can get
    /// </summary>
    public Float2 MaximumSize { get; set; }

    /// <summary>
    /// Adds a child to the layout and also to the container
    /// </summary>
    /// <param name="child"></param>
    public void AddChild(Control child);

    /// <summary>
    /// Perform the layout for this container with all children added via this layout.
    /// </summary>
    /// <param name="container"></param>
    /// <param name="force">Force a layout even if it's marked as locked</param>
    public void PerformLayout(ContainerControl container, bool force = false);
}