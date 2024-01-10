using FlaxEngine.GUI;
using UIX;

namespace Game;

public class TestDialog1 : UIXTitledPanel
{
    private Label _label;
    private CheckBox _checkBox;

    /// <inheritdoc />
    public override void PerformLayout(bool force = false)
    {
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
        }
        base.PerformLayout(force);
    }
}