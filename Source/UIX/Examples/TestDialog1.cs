using FlaxEngine.GUI;
using UIX;
using UIX.Layout;

namespace Game;

public class TestDialog1 : UIXTitledPanel
{
    private Label _label;
    private CheckBox _checkBox;

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
        }
        base.PerformLayout(force);
    }
}