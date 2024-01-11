using FlaxEngine.GUI;

namespace UICpp
{
    partial class UIXContainerControl
    {
        /// <summary>
        /// Creates a new control and adds it to the container.
        /// </summary>
        /// <returns>The added control.</returns>
        public T AddChild<T>() where T : UIXControl
        {
            return (T)Internal_AddChild(typeof(T), this);
        }

        /// <summary>
        /// Adds the control to the container.
        /// </summary>
        /// <param name="child">The control to add.</param>
        public T AddChild<T>(T child) where T : Control
        {
            return (T)Internal_AddChild(child, typeof(T), this);
        }

        public T GetChild<T>() where T : Control
        {
            return (T)Internal_GetChild(typeof(T), this);
        }
    }
}