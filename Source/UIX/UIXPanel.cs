using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX;

public class UIXPanel : ContainerControl
{
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
        Render2D.FillRectangle(new Rectangle(Float2.Zero, Size), UIXStyle.Style.BackgroundNormal);
    }
}