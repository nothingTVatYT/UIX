using System;
using System.Collections.Generic;
using FlaxEngine;

namespace UIX;

/// <summary>
/// InterestingText Script.
/// </summary>
public class InterestingText : Script
{
    private UIXTitledPanel _titledPanel;
    private UIXTable _table;
    private InterestingTextTableModel _model;

    public int NumberOfNumbers
    {
        get => _model?.Size ?? 0;
        set
        {
            if (_model != null) _model.Size = value;
        }
    }

    /// <inheritdoc/>
    public override void OnStart()
    {
        _titledPanel = Actor.As<UIControl>().Control as UIXTitledPanel;
        _model = new InterestingTextTableModel
        {
            Size = 1000000
        };
        _table = new UIXTable
        {
            Model = _model
        };
        _titledPanel?.Layout.AddChild(_table);
        _table.PerformLayout(true);
    }
}

/// <inheritdoc />
public class InterestingTextTableModel : AbstractTableModel
{
    /// <summary>
    /// The size of this virtual table of interesting text
    /// </summary>
    public int Size;

    /// <inheritdoc />
    public override string GetTextForItem(int index)
    {
        return $"{index} in hexadecimal is {index:x8}";
    }

    /// <inheritdoc />
    public override int NumberItems => Size;
}