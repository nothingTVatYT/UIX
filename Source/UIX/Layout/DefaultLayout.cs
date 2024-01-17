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
    public bool FillX = true;
    [Tooltip("Set to true to stretch child height to fill all the space.")]
    public bool FillY = true;

    /// <inheritdoc />
    public DefaultLayout(ContainerControl container) : base(container)
    {
    }

    private float PerformLayoutChildren(bool layoutChildren, out float maxWidth)
    {
        var clientArea = LayoutTools.GetClientArea(Container);
        var pos = clientArea.UpperLeft;
        var requestedHeight = 0f;
        maxWidth = 0;
        foreach (var child in Controls)
        {
            child.Pivot = Float2.Zero;
            var detail = ControlDetails.GetValueOrDefault(child, new ControlDetails());
            if (layoutChildren)
                child.PerformLayout();
            child.X = pos.X;
            child.Y = pos.Y;
            pos.Y += child.Height;
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
        if (container.IsLayoutLocked)
        {
            return;
        }
        Container = container;
        var clientArea = LayoutTools.GetClientArea(container);
        var requestedHeight = PerformLayoutChildren(true, out var maxWidth);

        if (requestedHeight > MaximumSize.Y)
        {
            var saveSpace = requestedHeight - MaximumSize.Y;
            foreach (var child in Controls)
            {
                var detail = ControlDetails[child];
                var possibleSavings = child.Height - detail.MinimumSize.Y;
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

        if (FillY && Controls.Count == 1)
        {
            Controls[0].Height = clientArea.Height;
        }

        IsLayoutDone = true;
    }

}