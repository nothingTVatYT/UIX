using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX.Layout;

/// <summary>
/// A default layout in a vertical box
/// </summary>
public class DefaultLayout : BaseLayout
{
    [Tooltip("Set to true to stretch child width to fill all the space.")]
    public bool FillX;

    private Dictionary<Control, ControlDetails> _details = new();

    /// <inheritdoc />
    public DefaultLayout(ContainerControl container) : base(container)
    {
    }

    private float PerformLayoutChildren(bool layoutChildren, out float maxWidth)
    {
        var clientArea = Container.GetClientArea();
        var pos = clientArea.UpperLeft;
        var requestedHeight = 0f;
        maxWidth = 0;
        foreach (var child in Controls)
        {
            child.Pivot = Float2.Zero;
            var detail = _details.GetValueOrDefault(child, new ControlDetails());
            if (layoutChildren)
                child.PerformLayout();
            child.X = pos.X;
            child.Y = pos.Y;
            pos.Y += child.Height;
            detail.ActualSize = child.Size;
            if (detail.MinimumSize.IsZero)
                detail.MinimumSize = LayoutTools.GetMinimumSize(child);
            _details[child] = detail;
            requestedHeight += child.Height;
            if (pos.Y > MaximumSize.Y)
            {
                pos.Y = MaximumSize.Y;
            }
            maxWidth = Mathf.Min(Mathf.Max(maxWidth, pos.X + child.Width), MaximumSize.X);
        }

        return requestedHeight;
    }

    /// <inheritdoc />
    public override void PerformLayout(ContainerControl container, bool force = false)
    {
        Container = container;
        var clientArea = container.GetClientArea();
        var pos = clientArea.UpperLeft;
        var requestedHeight = PerformLayoutChildren(true, out var maxWidth);

        if (requestedHeight > MaximumSize.Y)
        {
            var saveSpace = requestedHeight - MaximumSize.Y;
            foreach (var child in Controls)
            {
                var detail = _details[child];
                var possibleSavings = detail.ActualSize.Y - detail.MinimumSize.Y;
                if (possibleSavings > 0)
                {
                    var requestHeightReduction = Mathf.Min(saveSpace, possibleSavings);
                    child.Height -= requestHeightReduction;
                    saveSpace -= requestHeightReduction;
                    if (saveSpace < Mathf.Epsilon)
                        break;
                }
            }

            requestedHeight = PerformLayoutChildren(false, out maxWidth);
        }

        if (container.Width < maxWidth)
            container.Width = maxWidth;
        var heightDiff = requestedHeight - clientArea.Height;
        if (heightDiff > 0)
            container.Height += heightDiff;

        if (FillX)
        {
            foreach (var child in Controls)
            {
                child.Width = clientArea.Width;
            }
        }

        IsLayoutDone = true;
    }

}