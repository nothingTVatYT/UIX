using FlaxEngine.GUI;

namespace UIX;

/// <summary>
/// UIXTextBox Script.
/// </summary>
public class UIXTextBox : Control
{
    private BaseTextBoxModel _model;
    private string _text;

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
            _text = _model.Text;
        }
    }

    protected virtual void OnTextChanged()
    {
        _text = _model.Text;
    }

    public override void Draw()
    {
        //base.Draw();
        // draw the text cached in _text
    }
}
