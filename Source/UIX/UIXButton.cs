using System;
using FlaxEngine;
using FlaxEngine.GUI;
using UIX.Event;

namespace UIX;

public class UIXButton : Control
{
    public string Text;
    public Font Font = UIXStyle.Style.FontMedium;
    public IBrush Icon;
    public IBrush BackgroundBrush;
    public bool ChangeBackgroundOnHover;
    [VisibleIf("ChangeBackgroundOnHover")]
    public IBrush BackgroundMouseOverBrush;
    public bool ChangeBackgroundOnPress;
    [VisibleIf("ChangeBackgroundOnPress")]
    public IBrush BackgroundMousePressBrush;
    public TextLayoutOptions TextLayout = TextLayoutOptions.Default;
    [HideInEditor]
    public Action<ButtonEvent> Clicked;

    private bool _buttonPressed;

    /// <inheritdoc />
    public override void Draw()
    {
        var bounds = new Rectangle(Float2.Zero, Size);
        if (IsMouseOver && _buttonPressed && ChangeBackgroundOnPress)
        {
            BackgroundMousePressBrush.Draw(bounds, Color.White);
        }
        else if (IsMouseOver && ChangeBackgroundOnHover)
        {
            BackgroundMouseOverBrush.Draw(bounds, Color.White);
        }
        else if (BackgroundBrush != null)
            BackgroundBrush.Draw(bounds, Color.White);
        else
            Render2D.FillRectangle(bounds, UIXStyle.Style.Background);

        Icon?.Draw(bounds, Color.White);
        if ("" != Text)
        {
            TextLayout.Bounds = bounds;
            TextLayout.HorizontalAlignment = TextAlignment.Center;
            TextLayout.VerticalAlignment = TextAlignment.Center;
            Render2D.DrawText(Font, Text, UIXStyle.Style.Foreground, ref TextLayout);
        }
    }

    /// <inheritdoc />
    public override bool OnMouseDown(Float2 location, MouseButton button)
    {
        if (button == MouseButton.Left)
        {
            _buttonPressed = true;
            return true;
        }

        return false;
    }

    /// <inheritdoc />
    public override bool OnMouseUp(Float2 location, MouseButton button)
    {
        if (button == MouseButton.Left && _buttonPressed)
        {
            Clicked?.Invoke(ButtonEvent.PressedLeftButton(this));
            _buttonPressed = false;
            return true;
        }

        return false;
    }

    /// <inheritdoc />
    public override void OnKeyUp(KeyboardKeys key)
    {
        if (key is KeyboardKeys.Return or KeyboardKeys.Spacebar)
            Clicked?.Invoke(ButtonEvent.PressedKeyboard(this, key));
    }
}