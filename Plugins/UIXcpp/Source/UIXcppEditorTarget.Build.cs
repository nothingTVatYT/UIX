﻿using Flax.Build;

public class UIXCppEditorTarget : GameProjectEditorTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // Reference the modules for editor
        Modules.Add("UIXCpp");
        Modules.Add("UIXCppEditor");
    }
}
