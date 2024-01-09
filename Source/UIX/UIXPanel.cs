using FlaxEngine;
using FlaxEngine.GUI;
using UIX.Layout;

namespace UIX;

public class UIXPanel : ContainerControl
{
    public IBrush Background;
    [Serialize]
    private ILayout _layout;
    /// <summary>
    /// The layout class, it defaults to a simple vertical box layout
    /// </summary>
    [NoSerialize]
    public ILayout Layout
    {
        get
        {
            _layout ??= new DefaultLayout(this);
            return _layout;
        }
        set => _layout = value;
    }

    /// <summary>
    /// Create a new UIXPanel
    /// </summary>
    public UIXPanel()
    {
        Layout.MinimumSize = new Float2(60, 60);
        Layout.MaximumSize = Screen.Size;
        Pivot = Float2.Zero;
    }

    /// <inheritdoc />
    public override void PerformLayout(bool force = false)
    {
        Layout.PerformLayout(this, force);
    }

    /// <inheritdoc />
    public override void DrawSelf()
    {
        var bounds = new Rectangle(Float2.Zero, Size);
        if (Background != null)
            Background.Draw(bounds, Color.Transparent);
        else
            Render2D.FillRectangle(bounds, UIXStyle.Style.BackgroundNormal);
    }
}