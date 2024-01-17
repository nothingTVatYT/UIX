using System.Collections.Generic;

namespace UIX;

public class BaseTableModel : AbstractTableModel
{
    private List<string> _data;

    public BaseTableModel(List<string> data)
    {
        _data = data;
    }

    /// <inheritdoc />
    public override string GetTextForItem(int index)
    {
        return (index >= 0 && index < _data.Count) ? _data[index] : "";
    }

    /// <inheritdoc />
    public override int NumberItems => _data.Count;
}