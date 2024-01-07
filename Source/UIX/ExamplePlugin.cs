using System;
using FlaxEngine;

namespace UIX
{
    /// <summary>
    /// The sample game plugin.
    /// </summary>
    /// <seealso cref="FlaxEngine.GamePlugin" />
    public class UIX : GamePlugin
    {
        /// <inheritdoc />
        public UIX()
        {
            _description = new PluginDescription
            {
                Name = "UIX",
                Category = "UI",
                Author = "nothingTV",
                AuthorUrl = null,
                HomepageUrl = null,
                RepositoryUrl = "https://github.com/FlaxEngine/UIX",
                Description = "Flax UI extension plugin.",
                Version = new Version(0, 1),
                IsAlpha = true,
                IsBeta = false,
            };
        }

        /// <inheritdoc />
        public override void Initialize()
        {
            base.Initialize();

            Debug.Log("UIX initialized.");
        }

        /// <inheritdoc />
        public override void Deinitialize()
        {
            // Use it to cleanup data

            base.Deinitialize();
        }
    }
}
