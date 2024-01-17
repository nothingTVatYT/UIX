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
            Size = 10
        };
        _table = new UIXTable
        {
            Model = _model,
            
        };
        _titledPanel?.Layout.AddChild(_table);
    }
    
    /// <inheritdoc/>
    public override void OnEnable()
    {
        // Here you can add code that needs to be called when script is enabled (eg. register for events)
    }

    /// <inheritdoc/>
    public override void OnDisable()
    {
        // Here you can add code that needs to be called when script is disabled (eg. unregister from events)
    }

    /// <inheritdoc/>
    public override void OnUpdate()
    {
        // Here you can add code that needs to be called every frame
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