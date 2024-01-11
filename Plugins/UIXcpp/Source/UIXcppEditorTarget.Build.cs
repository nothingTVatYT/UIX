using Flax.Build;

public class UIXcppEditorTarget : GameProjectEditorTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // Reference the modules for editor
        Modules.Add("UIXcpp");
        Modules.Add("UIXcppEditor");
    }
}
