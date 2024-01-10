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
        }
        base.PerformLayout(force);
    }
}