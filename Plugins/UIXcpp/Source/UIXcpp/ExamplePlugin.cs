using System;
using FlaxEngine;

namespace UIXCpp
{
    /// <summary>
    /// The sample game plugin.
    /// </summary>
    /// <seealso cref="FlaxEngine.GamePlugin" />
    public class UIXCpp : GamePlugin
    {
        /// <inheritdoc />
        public UIXCpp()
        {
            _description = new PluginDescription
            {
                Name = "UIXCpp",
                Category = "UI",
                Author = "",
                AuthorUrl = null,
                HomepageUrl = null,
                RepositoryUrl = "https://github.com/FlaxEngine/UIXCpp",
                Description = "C++ conversion of the Flax UI",
                Version = new Version(0, 0, 1),
                IsAlpha = true,
                IsBeta = false,
            };
        }

        /// <inheritdoc />
        public override void Initialize()
        {
            base.Initialize();

            Debug.Log("Hello from plugin code!");
        }

        /// <inheritdoc />
        public override void Deinitialize()
        {
            // Use it to cleanup data

            base.Deinitialize();
        }
    }
}
