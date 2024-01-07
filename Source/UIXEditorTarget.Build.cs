using Flax.Build;

public class UIXEditorTarget : GameProjectEditorTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // Reference the modules for editor
        Modules.Add("UIX");
        Modules.Add("UIXEditor");
    }
}
