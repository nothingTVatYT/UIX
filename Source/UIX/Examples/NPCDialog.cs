using FlaxEngine;
using FlaxEngine.GUI;
using UIX;

namespace Game;

public class NPCDialog : UIXPanel
{
    public string NpcName;
    public string InitialText;
    public float BorderThickness = 4;

    private Float2 _minimumSize;
    private MouseTrackingControl _title;
    private Label _titleLabel;
    private Button _closeButton;
    private TextBox _textBox;
    private Label _status;
    private ResizeHandle _resizeHandle;

    public override void PerformLayout(bool force = false)
    {
        if (_title == null)
            CreateUI();
        UpdateUI();
        //base.PerformLayout(force);
        _closeButton.Width = _closeButton.Height;
        _closeButton.X = Width - _closeButton.Width;
        _title.Width = Width - _closeButton.Width;
        _title.Height = _closeButton.Height;
        _title.X = 0;
        _title.Y = 0;
        _status.Width = Width;
        _status.Y = Height - _status.Height;
        _status.X = 0;
        _textBox.Width = Width;
        _textBox.Y = _closeButton.Height;
        _textBox.Height = Height - _closeButton.Height - _status.Height;
        _resizeHandle.X = Width - _resizeHandle.Width;
        _resizeHandle.Y = Height - _resizeHandle.Height;
    }

    public void UpdateUI()
    {
        CullChildren = false;
        ClipChildren = false;
        _titleLabel.Text = NpcName;
        _textBox.Text = InitialText;
        _status.Text = "Talking";
    }

    public void CreateUI()
    {
        _titleLabel = new Label
        {
            Text = NpcName,
            BackgroundColor = Color.Teal,
            Font = new FontReference(UIXStyle.Style.FontMedium)
        };
        _title = new MouseTrackingControl(_titleLabel);
        _closeButton = new Button
        {
            Text = "X"
        };
        _textBox = new TextBox
        {
            Text = InitialText,
            IsReadOnly = true,
            IsMultiline = true,
            IsScrollable = true,
            IsMultilineScrollable = true,
            Height = 300,
            Wrapping = TextWrapping.WrapWords
        };
        _status = new Label
        {
            Text = " ",
            BackgroundColor = Color.Teal,
            TextColor = Color.White,
            Font = new FontReference(UIXStyle.Style.FontMedium),
            HorizontalAlignment = TextAlignment.Near
        };
        _resizeHandle = new ResizeHandle();
        _resizeHandle.ControlMoved += Resize;
        _title.ControlMoved += MoveDialog;
        _closeButton.Clicked += Dispose;

        AddChild(_title);
        AddChild(_closeButton);
        AddChild(_textBox);
        AddChild(_status);
        AddChild(_resizeHandle);

        _minimumSize = new Float2(60, 60);
    }

    public override void DrawSelf()
    {
        base.DrawSelf();
        if (BorderThickness > 0)
        {
            var topLeft = new Float2(-BorderThickness+4, -BorderThickness+4);
            var borderSize = new Float2(Width + 2 * BorderThickness-8, Height + 2 * BorderThickness-8);
            Render2D.DrawRectangle(new Rectangle(topLeft, borderSize), Color.Black, BorderThickness);
        }
    }

    private void MoveDialog(Float2 delta)
    {
        X = Mathf.Min(Mathf.Max(0, X + delta.X), Screen.Size.X - Width);
        Y = Mathf.Min(Mathf.Max(0, Y + delta.Y), Screen.Size.Y - Height);
    }

    private void Resize(Float2 delta)
    {
        Width = Mathf.Max(_minimumSize.X, Mathf.Min(Width + delta.X, Screen.Size.X - X));
        Height = Mathf.Max(_minimumSize.Y, Mathf.Min(Height + delta.Y, Screen.Size.Y - Y));
    }
}
