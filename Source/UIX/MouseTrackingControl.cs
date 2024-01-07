using System;
using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX;

/// <summary>
/// Helper control that tracks mouse dragging for resizing and moving controls
/// </summary>
public class MouseTrackingControl : ContainerControl
{
    private Float2 _buttonPressedLocation;
    private bool _buttonPressed;

    /// <summary>
    /// Event fired when this control is dragged.
    /// The parameter holds the delta to the position last reported.
    /// </summary>
    public event Action<Float2> ControlMoved;

    /// <summary>
    /// Creates an empty mouse tracking control
    /// </summary>
    public MouseTrackingControl()
    {
    }

    /// <summary>
    /// Create a new mouse drag tracking control,
    /// captured delta is available via the ControlMoved event
    /// </summary>
    public MouseTrackingControl(Control child)
    {
        Width = child.Width;
        Height = child.Height;
        AddChild(child);
        X = child.X;
        Y = child.Y;
        child.X = 0;
        child.Y = 0;
    }

    /// <inheritdoc />
    public override void PerformLayout(bool force = false)
    {
        // by default it stretches a single child
        if (ChildrenCount == 1)
        {
            var child = Children[0];
            child.Width = Width;
            child.Height = Height;
            child.X = 0;
            child.Y = 0;
        }
        else
        {
            base.PerformLayout(force);
        }
    }

    /// <inheritdoc />
    public override bool OnMouseDown(Float2 location, MouseButton button)
    {
        _buttonPressedLocation = location;
        _buttonPressed = true;
        StartMouseCapture();
        return true;
    }

    /// <inheritdoc />
    public override bool OnMouseUp(Float2 location, MouseButton button)
    {
        _buttonPressed = false;
        EndMouseCapture();
        return true;
    }

    /// <inheritdoc />
    public override void OnMouseMove(Float2 location)
    {
        if (_buttonPressed)
        {
            ControlMoved?.Invoke(location-_buttonPressedLocation);
        }
    }

}