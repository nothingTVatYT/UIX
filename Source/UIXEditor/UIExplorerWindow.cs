using System;
using System.Collections.Generic;
using FlaxEditor;
using FlaxEditor.CustomEditors;
using FlaxEditor.GUI.Tree;
using FlaxEditor.Scripting;
using FlaxEngine;
using FlaxEngine.GUI;

namespace UIXEditor;

class ControlTreeNode : TreeNode
{
    private Control _linkedControl;
    public Control LinkedControl
    {
        set
        {
            Text = value.ToString(); _linkedControl = value;
        }
        get => _linkedControl;
    }
}

internal class TrackingControl : Control
{
    public Action<Control> SelectedControl;

    private bool _isDragging;
    public override void PerformLayout(bool force = false)
    {
        Width = 16;
        Height = 16;
        base.PerformLayout(force);
    }

    public override bool OnMouseDown(Float2 location, MouseButton button)
    {
        if (button == MouseButton.Left)
        {
            _isDragging = true;
            StartMouseCapture();
            return true;
        }

        return false;
    }

    public override bool OnMouseUp(Float2 location, MouseButton button)
    {
        if (_isDragging)
        {
            EndMouseCapture();
            _isDragging = false;
            var screenLocation = PointToScreen(location);
            var rootControl = Editor.Instance.Windows.GameWin.Root;
            var gameWindowLocation = rootControl.PointFromScreen(screenLocation);
            var uiCanvas = Level.Scenes[0].FindActor<UICanvas>();
            var canvasLocation = uiCanvas.GUI.PointFromScreen(screenLocation);
            var control = uiCanvas.GUI.GetChildAt(canvasLocation);
            //var control = Editor.Instance.Windows.GameWin.Root.GetChildAt(gameWindowLocation);
            Debug.Log("Found control: " + control);
            SelectedControl?.Invoke(control);
            return true;
        }

        return false;
    }

    public override void OnMouseMove(Float2 location)
    {
        base.OnMouseMove(location);
    }

    public override void Draw()
    {
        Render2D.FillRectangle(new Rectangle(Float2.Zero, Size), Style.Current.Background);
        Render2D.DrawText(Style.Current.FontMedium, "?", Style.Current.Foreground, Float2.Zero);
    }
}

/// <summary>
/// UIExplorerWindow Script.
/// </summary>
public class UIExplorerWindow : CustomEditorWindow
{
    private Control _control;
    private Control _selectedControl;
    private TrackingControl _trackingControl;

    public override void Initialize(LayoutElementsContainer layout)
    {
        var trackingElement = layout.Custom<TrackingControl>();
        _trackingControl = trackingElement.CustomControl;
        _trackingControl.SelectedControl += OnSelectedControl;
        var uiDetails = layout.Group("UI Details");
        if (_control != null)
        {
            // show details
            var panel = uiDetails.VerticalPanel();
            var tree = new Tree
            {
                Parent = panel.Panel
            };
            tree.AddChild(BuildTree(_control));
            tree.SelectedChanged += OnTreeSelected;

            if (_selectedControl != null)
            {
                var scriptType = new ScriptType(_selectedControl.GetType());
                var cvc = new CustomValueContainer(scriptType, _selectedControl,
                    (_, _) => _selectedControl);
                panel.Object(cvc);
            }
        }

        var pickButton = layout.Button("Show selected UI container");
        pickButton.Button.Clicked += OnPickContainer;
    }

    private void OnTreeSelected(List<TreeNode> before, List<TreeNode> after)
    {
        _selectedControl = after.Count > 0 ? (after[0] as ControlTreeNode)?.LinkedControl : null;
        RebuildLayout();
    }

    private ControlTreeNode BuildTree(Control _control)
    {
        var node = new ControlTreeNode();
        node.LinkedControl = _control;
        if (_control is ContainerControl containerControl)
        {
            foreach (var child in containerControl.Children)
            {
                BuildTree(child, node);
            }
        }
        return node;
    }

    private void BuildTree(Control control, ControlTreeNode parentNode)
    {
        var node = new ControlTreeNode
        {
            LinkedControl = control,
            Parent = parentNode
        };
        if (control is ContainerControl containerControl)
        {
            foreach (var child in containerControl.Children)
            {
                BuildTree(child, node);
            }
        }
    }

    public void OnSelectedControl(Control control)
    {
        _control = control;
        RebuildLayout();
    }

    public void OnPickContainer()
    {
        var selected = Editor.Instance.Windows.SceneWin.Editor.SceneEditing.Selection;
        if (selected.Count > 0)
        {
            var selectedNode = selected[0];
            if (selectedNode.EditableObject is UIControl uiControl)
            {
                _control = uiControl.Control;
                RebuildLayout();
            }
            else
            {
                Debug.Log("Selected: " + selectedNode.EditableObject);
            }
        }
        else
        {
            Debug.Log("nothing selected");
        }
    }
}
