using System;
using FlaxEngine;
using FlaxEngine.GUI;
using UIX.Event;

namespace UIX;

public class UIXButton : Control
{
    [Tooltip("The optional text on the button")]
    public string Text;
    [Tooltip("Font to be used for the button text.")]
    public FontReference Font = new FontReference(UIXStyle.Style.FontMedium);
    [Tooltip("The optional icon on the button")]
    public IBrush Icon;
    [Tooltip("The background painter")]
    public IBrush BackgroundBrush;
    [Tooltip("Set this to true if the background should change when the mouse pointer hovers over this button.")]
    public bool ChangeBackgroundOnHover;
    [VisibleIf("ChangeBackgroundOnHover")]
    [Tooltip("The background painter if the mouse pointer hovers over this button.")]
    public IBrush BackgroundMouseOverBrush;
    [Tooltip("Set this to true if the background should change while the button is pressed.")]
    public bool ChangeBackgroundOnPress;
    [VisibleIf("ChangeBackgroundOnPress")]
    [Tooltip("The background painter for a pressed button")]
    public IBrush BackgroundMousePressBrush;
    [Tooltip("The text layout options used if a button text is set.")]
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
            if (Font == null)
                Font = new FontReference(UIXStyle.Style.FontMedium);
            Render2D.DrawText(Font.GetFont(), Text, UIXStyle.Style.Foreground, ref TextLayout);
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