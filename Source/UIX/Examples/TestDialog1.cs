using FlaxEngine;
using FlaxEngine.GUI;
using UIX;
using UIX.Layout;

namespace Game;

public class TestDialog1 : UIXTitledPanel
{
    private Label _label;
    private CheckBox _checkBox;
    private Label _loginLabel;
    private TextBox _loginTextBox;
    private Label _passwordLabel;
    private UIXPasswordBox _passwordTextBox;

    public TestDialog1()
    {
        CreateUI();
    }

    /// <inheritdoc />
    public void CreateUI()
    {
        var layout = new DocLayout(this);
        Layout = layout;
        _label = new Label
        {
            Text = "Content",
            BackgroundColor = UIXStyle.Style.TextBoxBackground
        };
        layout.AddChild(_label, DocLayoutOptions.WrapAfter);
        _checkBox = new CheckBox
        {
            TooltipText = "A checkbox for testing"
        };
        layout.AddChild(_checkBox, DocLayoutOptions.WrapAfter);
        _loginLabel = new Label
        {
            Text = "Login"
        };
        layout.AddChild(_loginLabel);
        _loginTextBox = new TextBox
        {
            Width = 80
        };
        layout.AddChild(_loginTextBox, DocLayoutOptions.WrapAfter);
        _passwordLabel = new Label
        {
            Text = "Password",
            Font = new FontReference(UIXStyle.Style.FontMedium)
        };
        layout.AddChild(_passwordLabel);
        _passwordTextBox = new UIXPasswordBox();
        layout.AddChild(_passwordTextBox, DocLayoutOptions.WrapAfter);
    }
}