using System.Collections.Generic;
using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX.Layout;

public class DocLayoutOptions
{
    public bool Wrap;
    public static DocLayoutOptions Default = new() { Wrap = false };
    public static DocLayoutOptions WrapAfter = new() { Wrap = true };
    public override string ToString()
    {
        return base.ToString() + "(" + (Wrap ? "Wrap" : "") + ")";
    }
}

/// <summary>
/// A layout that arranges children in lines and columns of variable sizes. 
/// </summary>
public class DocLayout : BaseLayout
{
    protected Dictionary<Control, DocLayoutOptions> ControlOptions = new();

    private List<DocLayoutLine> _lines = new();

    /// <inheritdoc />
    public DocLayout(ContainerControl container) : base(container)
    {
    }

    public void AddChild(Control child, DocLayoutOptions options)
    {
        ControlOptions[child] = options;
        base.AddChild(child);
    }

    private DocLayoutLine LineAt(int index)
    {
        while (index >= _lines.Count)
            _lines.Add(new DocLayoutLine());
        return _lines[index];
    }

    /// <inheritdoc />
    public override void PerformLayout(ContainerControl container, bool force = false)
    {
        if (Controls.Count == 0 || container.IsLayoutLocked)
            return;
        var layoutChildren = true;
        // arrange children in lines of cells
        var lineIndex = 0;
        var cellIndex = 0;
        _lines.Clear();
        Debug.Log($"There are {Controls.Count} controls.");
        foreach (var child in Controls)
        {
            if (layoutChildren)
                child.PerformLayout(force);
            var detail = ControlDetails.GetValueOrDefault(child, new ControlDetails());
            if (detail.MinimumSize.IsZero)
                detail.MinimumSize = LayoutTools.GetMinimumSize(child);
            var line = LineAt(lineIndex);
            line.Controls.Add(child);
            var options = ControlOptions.GetValueOrDefault(child, DocLayoutOptions.Default);
            line.MinimumSize.X += detail.MinimumSize.X;
            line.MinimumSize.Y = Mathf.Max(line.MinimumSize.Y, detail.MinimumSize.Y);
            line.RequestedSize.X += child.Width;
            line.RequestedSize.Y = Mathf.Max(line.RequestedSize.Y, child.Height);
            //Debug.Log($"layout options for {child}: {options}");
            if (options.Wrap)
            {
                lineIndex++;
                cellIndex = 0;
            }
            else
            {
                cellIndex++;
            }
        }
        // position children
        var clientArea = LayoutTools.GetClientArea(container);
        float minClientWidth = 0;
        float requestedWidth = 0; 
        var y = clientArea.Y;
        foreach (var layoutLine in _lines)
        {
            float x = clientArea.X;
            var minLineHeight = 0f;
            foreach (var child in layoutLine.Controls)
            {
                child.X = x;
                child.Y = y;
                x += child.Width;
            }

            y += layoutLine.RequestedSize.Y;
            minClientWidth = Mathf.Max(minClientWidth, layoutLine.MinimumSize.X);
            requestedWidth = Mathf.Max(requestedWidth, layoutLine.RequestedSize.X);
        }

        if (requestedWidth > clientArea.Width)
        {
            var neededAdditionalWidth = requestedWidth - clientArea.Width;
            if (Container.Width + neededAdditionalWidth > MaximumSize.X)
            {
                // redo layout with minimum sizes
                Debug.Log("need to redo layout");
            }

            container.Width += neededAdditionalWidth;
            MinimumSize = new Float2(Mathf.Max(MinimumSize.X, container.Width), MinimumSize.Y);
        }
        if (y > clientArea.Height + clientArea.Top)
        {
            var neededAdditionalHeight = y - clientArea.Height - clientArea.Top;
            if (container.Height + neededAdditionalHeight > MaximumSize.Y)
            {
                // redo layout with minimum sizes
                Debug.Log("need to redo layout");
            }

            container.Height += neededAdditionalHeight;
            MinimumSize = new Float2(MinimumSize.X, Mathf.Max(MinimumSize.Y, container.Height));
        }
        MinimumSize = new Float2(minClientWidth, MinimumSize.Y);
        if (container.Width < MinimumSize.X)
            container.Width = MinimumSize.X;
        IsLayoutDone = true;
    }

    private class DocLayoutLine
    {
        public Float2 MinimumSize;
        public Float2 RequestedSize;
        public List<Control> Controls = new();
    }
}