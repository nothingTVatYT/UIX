using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX;

/// <summary>
/// A container control with a title, a close button and a resize handle
/// </summary>
public class UIXTitledPanel : UIXPanel
{
    private MouseTrackingControl _title;
    private Label _titleLabel;
    private UIXButton _closeButton;
    private ResizeHandle _resizeHandle;
    private Rectangle _clientArea;

    /// <summary>
    /// The text for the title label
    /// </summary>
    [Tooltip("Text for the title")]
    public string Title;

    /// <summary>
    /// Set this to true if the panel can be resized by the user
    /// </summary>
    [Tooltip("Set this to true if the panel can be resized by the user")]
    public bool Resizable = true;
    /// <summary>
    /// Set this to true if the panel can be closed by the user
    /// </summary>
    [Tooltip("Set this to true if the panel can be closed by the user")]
    public bool Closeable = true;
    /// <summary>
    /// Set this to true if the panel can be dragged by the user
    /// </summary>
    [Tooltip("Set this to true if the panel can be dragged by the user")]
    public bool Movable = true;

    public UIXTitledPanel()
    {
        Insets = new Margin(10);
    }

    private void CreateUI()
    {
        ClipChildren = false;
        CullChildren = false;
        Layout.MinimumSize = new Float2(60, 60);
        _titleLabel = new Label
        {
            Text = Title,
            TextColor = UIXStyle.Style.Foreground,
            BackgroundColor = UIXStyle.Style.Background,
            Font = new FontReference(UIXStyle.Style.FontMedium)
        };
        if (Movable)
        {
            _title = new MouseTrackingControl(_titleLabel);
            AddChild(_title);
            _title.ControlMoved += MoveDialog;
        }
        else
        {
            AddChild(_titleLabel);
        }

        if (Closeable)
        {
            var titleHeight = _titleLabel.Height;
            // make this squared, Width = titleHeight is intentional
            _closeButton = new UIXButton
            {
                Icon = new SpriteBrush(UIXStyle.Style.Cross),
                Width = titleHeight,
                Height = titleHeight
            };
            AddChild(_closeButton);
            _closeButton.Clicked += _ =>
            {
                 Dispose();
            };
        }

        if (Resizable)
        {
            _resizeHandle = new ResizeHandle();
            _resizeHandle.ControlMoved += Resize;
            AddChild(_resizeHandle);
        }
    }

    /// <inheritdoc />
    public override void PerformLayout(bool force = false)
    {
        if (_titleLabel == null)
            CreateUI();
        if (IsLayoutLocked)
            return;
        _titleLabel.Text = Title;
        var shortenTitleWidth = 0f;
        var titleHeight = 16f;
        if (_closeButton != null)
        {
            _closeButton.Width = _closeButton.Height;
            _closeButton.X = Width - _closeButton.Width;
            shortenTitleWidth = _closeButton.Width;
            titleHeight = Mathf.Max(titleHeight, _closeButton.Height);
        }

        Control titleControl = Movable ? _title : _titleLabel;
        titleControl.Width = Width - shortenTitleWidth;
        titleControl.Height = titleHeight;
        titleControl.X = 0;
        titleControl.Y = 0;
        var bottomSpace = 0f;
        if (_resizeHandle != null)
        {
            _resizeHandle.X = Width - _resizeHandle.Width;
            _resizeHandle.Y = Height - _resizeHandle.Height;
            bottomSpace = _resizeHandle.Height;
        }

        _clientArea = new Rectangle(Insets.Left, titleHeight + Insets.Top,
            Width - Insets.Left - Insets.Right, Height - titleHeight - bottomSpace - Insets.Top - Insets.Bottom);
        base.PerformLayout(force);
    }

    /// <inheritdoc />
    public override Rectangle GetUsableClientArea()
    {
        return _clientArea;
    }

    private void MoveDialog(Float2 delta)
    {
        X = Mathf.Min(Mathf.Max(0, X + delta.X), Screen.Size.X - Width);
        Y = Mathf.Min(Mathf.Max(0, Y + delta.Y), Screen.Size.Y - Height);
    }

    private void Resize(Float2 delta)
    {
        Width = Mathf.Max(Layout.MinimumSize.X, Mathf.Min(Width + delta.X, Screen.Size.X - X));
        Height = Mathf.Max(Layout.MinimumSize.Y, Mathf.Min(Height + delta.Y, Screen.Size.Y - Y));
    }
}