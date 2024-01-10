using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX.Layout;

public class LayoutTools
{
    /// <summary>
    /// Get or estimate the minimum size of a control.
    /// </summary>
    /// <param name="control"></param>
    /// <returns></returns>
    public static Float2 GetMinimumSize(Control control)
    {
        if (control is UIXPanel panel)
            return panel.Layout.MinimumSize;
        // built-in controls don't have a minimum size setting so we need to guess
        if (control is Label label)
            return label.Font.GetFont().MeasureText(label.Text) + label.Margin.Size;
        if (control is Button button)
            return button.Font.GetFont().MeasureText(button.Text) + new Float2(button.BorderThickness * 2, button.BorderThickness * 2);
        if (control is TextBox textBox)
        {
            var estimation = textBox.Font.GetFont().MeasureText(textBox.Text) +
                   new Float2(textBox.BorderThickness * 2, textBox.BorderThickness * 2) + textBox.Offsets.Size;
            estimation.Y = Mathf.Max(estimation.Y, TextBoxBase.DefaultHeight);
            return estimation;
        }

        return control.Size;
    }
}