using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX;

/// <summary>
/// A table widget for text representation only
/// </summary>
public class UIXTable : UIXPanel
{
    private AbstractTableModel _model;
    private int _startIndex = 0;
    private int _endIndex = int.MaxValue;
    private float _childHeight = TextBoxBase.DefaultHeight;

    /// <summary>
    /// The model for this table widget
    /// </summary>
    public AbstractTableModel Model
    {
        get => _model;
        set { _model = value;
            _model.DataChanged = () => { Layout.IsLayoutDone = false; };
        }
    }

    /// <inheritdoc />
    public override void PerformLayout(bool force = false)
    {
        if (IsLayoutLocked && !force)
            return;
        if (_model == null)
            return;
        // calculate how many children we need and make sure the layout has enough
        var linesToShow = Mathf.FloorToInt(Height / _childHeight);
        var linesLeft = Model.NumberItems - _startIndex;
        var childrenNeeded = Mathf.Min(linesLeft, linesToShow);
        IsLayoutLocked = true;
        while (Layout.NumberChildren < childrenNeeded)
        {
            Layout.AddChild(new Label
            {
                Pivot = Float2.Zero,
                Height = 18
            });
            Debug.Log("child added, now we have " + Layout.NumberChildren);
        }

        // make this more convenient
        while (Layout.NumberChildren > childrenNeeded)
        {
            Layout.RemoveChild(Layout.NumberChildren-1);
        }

        IsLayoutLocked = false;

        // set the children's value
        for (var i = 0; i < childrenNeeded; i++)
        {
            var childControl = Layout.GetChild(i);
            if (childControl is Label label)
            {
                label.Text = _model.GetTextForItem(i + _startIndex);
                label.Height = 18;
            }
        }
        Layout.PerformLayout(this, force);
    }

    /// <inheritdoc />
    public override bool OnMouseWheel(Float2 location, float delta)
    {
        var scrollLines = Mathf.RoundToInt(delta);
        _startIndex = Mathf.Clamp(_startIndex - scrollLines, 0, _model.NumberItems);
        PerformLayout();
        return true;
        //return base.OnMouseWheel(location, delta);
    }
}