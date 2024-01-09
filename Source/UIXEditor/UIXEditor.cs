using System;
using FlaxEditor;
using FlaxEditor.GUI.ContextMenu;

namespace UIXEditor;

/// <summary>
/// The editor part of the UIX plugin
/// </summary>
public class UIXEditor : EditorPlugin
{
    /// <inheritdoc />
    public override Type GamePluginType => typeof(UIX.UIX);
    private ContextMenuChildMenu _menu;

    /// <inheritdoc />
    public override void InitializeEditor()
    {
        base.InitializeEditor();
        _menu = Editor.UI.MenuTools.ContextMenu.AddChildMenu("UI Extensions");
        var button = _menu.ContextMenu.AddButton("UI Explorer");
        button.Clicked += ShowUIExplorer;
    }

    /// <inheritdoc />
    public override void DeinitializeEditor()
    {
        base.DeinitializeEditor();
        if (_menu != null)
        {
            _menu.Dispose();
            _menu = null;
        }
    }

    private void ShowUIExplorer()
    {
        var window = new UIExplorerWindow();
        window.Show();
    }
}