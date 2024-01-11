using Flax.Build;

public class UIXcppTarget : GameProjectTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // Reference the modules for game
        Modules.Add("UIXcpp");
    }
}
