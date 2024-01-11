using System;
using FlaxEngine;

namespace UIXcpp
{
    /// <summary>
    /// The sample game plugin.
    /// </summary>
    /// <seealso cref="FlaxEngine.GamePlugin" />
    public class UIXcpp : GamePlugin
    {
        /// <inheritdoc />
        public UIXcpp()
        {
            _description = new PluginDescription
            {
                Name = "UIXcpp",
                Category = "Other",
                Author = "",
                AuthorUrl = null,
                HomepageUrl = null,
                RepositoryUrl = "https://github.com/FlaxEngine/UIXcpp",
                Description = "This is an example plugin project.",
                Version = new Version(0, 0, 1),
                IsAlpha = false,
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
