using FlaxEngine;

namespace UIX.Examples;

public class QuestTextBox : Script
{
    private UIXTextBox _textBox;
    private QuestTextModel _questTextModel;

    public override void OnEnable()
    {
        _textBox = new UIXTextBox();
        // parent it to some panel, left out for simplicity
        _questTextModel = new QuestTextModel();
        _textBox.Model = _questTextModel;
        // imagine you have a quest list, an inventory and maybe more
        // you could subscribe to changes here
        //QuestList.QuestAdded += () => { _questTextModel.Refresh() }
        //QuestList.QuestCanceled += () => { _questTextModel.Refresh() }
        //Inventory.QuestItemAdded += () => { _questTextModel.Refresh() }
    }
}

/// <summary>
/// This is an example implementation of a model that composes a text on demand
/// which takes long enough that you want to run it only when absolutely necessary
/// </summary>
class QuestTextModel : BaseTextBoxModel
{
    private string _text;
    public override string Text => _text;

    public void Refresh()
    {
        _text = ComposeText();
        TextChanged?.Invoke();
    }

    string ComposeText()
    {
        // imagine a case where getting all the details might be expensive enough that you
        // want to run that code only if necessary, like checking quest status, inventory items, etc.
        return "";
    }
}