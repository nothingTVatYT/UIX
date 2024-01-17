using FlaxEngine.GUI;

namespace UIX;

/// <summary>
/// UIXTextBox Script.
/// </summary>
public class UIXTextBox : Control
{
    private BaseTextBoxModel _model;

    public string Text
    {
        get => Model?.Text;
        set
        {
            Model ??= new BaseTextBoxModel();
            Model.Text = value;
            OnTextChanged();
        }
    }

    /// <summary>
    /// Gets or sets the model for this text box
    /// </summary>
    public BaseTextBoxModel Model { get => _model;
        set {
            _model = value;
            _model.TextChanged += OnTextChanged;
        }
    }

    protected virtual void OnTextChanged()
    {
        // you could flash it or do whatever with this information
    }

    public override void Draw()
    {
        //base.Draw();
        // draw the text from _model.Text
    }
}
