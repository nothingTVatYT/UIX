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

    /// <inheritdoc />
    public override void PerformLayout(bool force = false)
    {
        (Layout as DefaultLayout).FillX = true;
        if (_label == null)
        {
            _label = new Label
            {
                Text = "Content",
                BackgroundColor = UIXStyle.Style.TextBoxBackground
            };
            Layout.AddChild(_label);
            _checkBox = new CheckBox
            {
                TooltipText = "A checkbox for testing"
            };
            Layout.AddChild(_checkBox);
            _loginLabel = new Label
            {
                Text = "Login"
            };
            Layout.AddChild(_loginLabel);
            _loginTextBox = new TextBox();
            Layout.AddChild(_loginTextBox);
            _passwordLabel = new Label
            {
                Text = "Password",
                Font = new FontReference(UIXStyle.Style.FontMedium)
            };
            Layout.AddChild(_passwordLabel);
            _passwordTextBox = new UIXPasswordBox();
            Layout.AddChild(_passwordTextBox);
        }
        base.PerformLayout(force);
    }
}