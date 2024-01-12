using FlaxEngine;
using FlaxEngine.GUI;
using UIX.Layout;

namespace UIX;

/// <summary>
/// The base for all UIX panels, like titled panel (<see cref="UIXTitledPanel" />).
/// </summary>
public class UIXPanel : ContainerControl
{
    /// <summary>
    /// The background painter for this panel
    /// </summary>
    [Tooltip("The background painter for this panel")]
    public IBrush Background;

    public Margin Insets;

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
        Insets = Margin.Zero;
    }

    public virtual Rectangle GetUsableClientArea()
    {
        return GetClientArea();
    }

    /// <inheritdoc />
    public override void PerformLayout(bool force = false)
    {
        if (!IsLayoutLocked)
            Layout.PerformLayout(this, force);
    }

    /// <inheritdoc />
    public override void Update(float deltaTime)
    {
        base.Update(deltaTime);
        if (!IsLayoutLocked && !Layout.IsLayoutDone)
            Layout.PerformLayout(this);
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