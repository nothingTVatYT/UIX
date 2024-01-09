using FlaxEngine;

namespace UIX.Event;

public class ButtonEvent
{
    public UIXButton Source { get; private set; }
    public MouseButton Button { get; private set; }
    public KeyboardKeys Key { get; private set; }

    public static ButtonEvent PressedLeftButton(UIXButton source) => new()
    {
        Source = source,
        Button = MouseButton.Left
    };

    public static ButtonEvent PressedKeyboard(UIXButton source, KeyboardKeys key) => new()
    {
        Source = source,
        Key = key
    };
}