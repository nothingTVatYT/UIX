﻿using Flax.Build;

public class UIXTarget : GameProjectTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // Reference the modules for game
        Modules.Add("UIX");
    }
}
