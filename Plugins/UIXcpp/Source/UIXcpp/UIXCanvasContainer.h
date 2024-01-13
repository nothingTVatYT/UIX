#pragma once

#include "UIXContainerControl.h"


class UIXCanvasRootControl;

/// <summary>
/// The root container control used to sort and manage child UICanvas controls. Helps with sending input events.
/// </summary>
/// <seealso cref="FlaxEngine.GUI.ContainerControl" />
API_CLASS(Sealed, Attributes="HideInEditor")
class UIXCPP_API UIXCanvasContainer : public UIXContainerControl
{
//API_AUTO_SERIALIZATION();
//DECLARE_SCRIPTING_TYPE(UIXCanvasContainer);


// This was an internal class in C#, and is not listed in the documentation. Every member, including
// originally /*public*/ ones should be private or /*protected*/.
//TODO: serialization of private fields and properties
private:
    friend class UIXRootControl;

    /// <summary>
    /// Sorts the canvases by order.
    /// </summary>

    /*public*/ void SortCanvases();

    /*private*/ int SortCanvas(UIXControl *a, UIXControl *b);

    /*internal*/ UIXCanvasContainer(const SpawnParams& params);

    /// <inheritdoc />
    /*public*/  void OnChildrenChanged() override;

    /// <inheritdoc />
    /*protected*/ void DrawChildren() override;

    /// <inheritdoc />
    /*public*/  bool IntersectsChildContent(UIXControl *child, Float2 location, API_PARAM(Out) Float2 &childSpaceLocation) const;

    /// <inheritdoc />
    /*public*/  void OnMouseEnter(Float2 location) override;

    /// <inheritdoc />
    /*public*/  void OnMouseMove(Float2 location) override;

    /// <inheritdoc />
    /*public*/  bool OnMouseWheel(Float2 location, float delta) override;

    /// <inheritdoc />
    /*public*/  bool OnMouseDown(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    /*public*/  bool OnMouseUp(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    /*public*/  bool OnMouseDoubleClick(Float2 location, MouseButton button) override;
};

