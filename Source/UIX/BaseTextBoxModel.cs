using System;

namespace UIX;

public class BaseTextBoxModel
{
    /// <summary>
    /// Gets or sets the text this model represents
    /// </summary>
    public virtual string Text { get; set; }

    /// <summary>
    /// Action to be fired when the text has changed
    /// </summary>
    public Action TextChanged;
}