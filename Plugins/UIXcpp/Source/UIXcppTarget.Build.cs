using Flax.Build;

public class UIXCppTarget : GameProjectTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // Reference the modules for game
        Modules.Add("UIXCpp");
    }
}
