using FlaxEngine.GUI;
using System;

namespace UIXCpp
{
    partial class UIXContainerControl
    {
        /// <summary>
        /// Creates a new control and adds it to the container.
        /// </summary>
        /// <returns>The added control.</returns>
        public T AddChild<T>() where T : UIXControl
        {

            UIXControl child = Activator.CreateInstance<T>();
            AddChildInternal(child);
            return (T)child;
        }

        /// <summary>
        /// Adds the control to the container.
        /// </summary>
        /// <param name="child">The control to add.</param>
        public T AddChild<T>(T child) where T : UIXControl
        {
            AddChildInternal((UIXControl)child);
            return child;
        }

        /// <summary>
        /// Find the first child with a given type.
        /// </summary>
        /// <returns>The control matching the type.</returns>
        public T GetChild<T>() where T : UIXControl
        {
            for (int ix = 0, siz = ChildrenCount; ix < siz; ++ix)
            {
                var child = GetChild(ix);
                if (child is T)
                    return (T)child;
            }
            return null;
        }
    }
}