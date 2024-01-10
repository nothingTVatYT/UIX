using System.Threading;
using FlaxEngine;
using FlaxEngine.GUI;
using UIX;
using UIX.Layout;

namespace UIX;

/// <summary>
/// AddButtons Script.
/// </summary>
public class AddButtons : Script
{
    [Tooltip("The background painter for the panel.")]
    public IBrush Background;
    [Tooltip("The background painter for the buttons.")]
    public IBrush ButtonBackground;
    private UIXPanel _panel;

    /// <inheritdoc/>
    public override void OnStart()
    {
        _panel = Actor.As<UIControl>().Control as UIXPanel;
        if (_panel != null)
        {
            _panel.Layout = new CircularLayout(_panel)
            {
                AngleOffset = 90,
                AngleMinMax = new Float2(45, 315),
                Animate = true
            };
            JobSystem.Dispatch(_ => CreateButtons());
            _panel.Background = Background;
        }
    }

    private void CreateButtons()
    {
        for (var i = 0; i < 6; i++)
        {
            Thread.Sleep(500);
            var button = new UIXButton
            {
                Text = $"Button {i}",
                BackgroundBrush = ButtonBackground,
                Size = new Float2(60, 60),
            };
            Scripting.InvokeOnUpdate(() => _panel.Layout.AddChild(button));
        }
    }
}
