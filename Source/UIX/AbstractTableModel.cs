using System;

namespace UIX;

public abstract class AbstractTableModel
{
    /// <summary>
    /// Get a text representation for the item in question
    /// </summary>
    /// <param name="index">zero-based index into the table</param>
    /// <returns></returns>
    // for an even more flexible model this could return an object that is rendered
    // in a more complicated way than just a text, e.g. an icon plus a label
    public abstract string GetTextForItem(int index);

    /// <summary>
    /// The number of items in the model
    /// </summary>
    public abstract int NumberItems { get; }

    /// <summary>
    /// Action to be fire when the data for the table changed
    /// </summary>
    public Action DataChanged;
}
