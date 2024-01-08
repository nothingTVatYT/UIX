using FlaxEngine.GUI;
using FlaxEngine.Utilities;

namespace UIX;

/// <summary>
/// Class for a static in-game UI style setting
/// </summary>
public class UIXStyle
{
    private static Style _style;
    /// <summary>
    /// The current style for the game UI
    /// </summary>
    public static Style Style {
        get => _style ?? Style.Current;
        private set => _style = value;
    }

    /// <summary>
    /// Clones the default style and sets it as the default
    /// </summary>
    /// <returns>the newly created style</returns>
    public static Style CreateAndSetNewStyle()
    {
        Style = Style.Current.DeepClone();
        return _style;
    }
}