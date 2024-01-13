#pragma once

#include "UIXContainerControl.h"

API_CLASS(Attributes="HideInEditor") class UIXScrollableControl : public UIXContainerControl
{
public:
    UIXScrollableControl(const SpawnParams &params);


    /// <summary>
    /// Gets current view offset for all the controls (used by the scroll bars).
    /// </summary>
    API_PROPERTY(Attributes="HideInEditor, NoSerialize")
    FORCE_INLINE Float2 GetViewOffset() const { return _viewOffset; }

    /// <summary>
    /// Gets current view offset for all the controls (used by the scroll bars).
    /// </summary>
    API_PROPERTY()
    virtual void SetViewOffset(Float2 value);

    /// <inheritdoc />
    API_FUNCTION() bool IntersectsChildContent(UIXControl *child, Float2 location, API_PARAM(Out) Float2 &childSpaceLocation) const override;

    /// <inheritdoc />
    API_FUNCTION() bool IntersectsContent(Float2 locationParent, API_PARAM(Out) Float2 &location) const override;

    /// <inheritdoc />
    API_FUNCTION() Float2 PointToParent(Float2 location) const override;

    /// <inheritdoc />
    API_FUNCTION() Float2 PointFromParent(Float2 location) const override;
protected:
    /// <summary>
    /// Called when view offset gets changed.
    /// </summary>
    virtual void OnViewOffsetChanged() {}

    /// <inheritdoc />
    void DrawChildren() override;

    /// <summary>
    /// The view offset. Useful to offset contents of the container (used by the scrollbars and drop panels).
    /// </summary>
    Float2 _viewOffset;
private:

};

