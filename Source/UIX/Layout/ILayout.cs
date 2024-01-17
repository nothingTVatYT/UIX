using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX;

/// <summary>
/// Interface for layout classes used in UIXPanel
/// </summary>
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
    /// Returns true if the layout job is finished otherwise false for e.g. an animated layout
    /// </summary>
    public bool IsLayoutDone { get; set; }

    /// <summary>
    /// Number of children this layout currently knows
    /// </summary>
    public int NumberChildren { get; }

    /// <summary>
    /// Adds a child to the layout and also to the container
    /// </summary>
    /// <param name="child"></param>
    public void AddChild(Control child);

    /// <summary>
    /// Remove the child at index from this layout as well as from the container
    /// </summary>
    /// <param name="index"></param>
    public void RemoveChild(int index);
    /// <summary>
    /// Get the child at that index
    /// </summary>
    /// <param name="index"></param>
    /// <returns></returns>
    public Control GetChild(int index);

    /// <summary>
    /// Perform the layout for this container with all children added via this layout.
    /// </summary>
    /// <param name="container"></param>
    /// <param name="force">Force a layout even if it's marked as locked</param>
    public void PerformLayout(ContainerControl container, bool force = false);
}