#pragma once

#include "UIXRootControl.h"
#include "UIXEnums.h"

class UIXCanvas;

/// <summary>
/// Root control implementation used by the <see cref="UIXCanvas"/> actor.
/// </summary>
/// <seealso cref="FlaxEngine.GUI.RootControl" />
API_CLASS(NoSpawn, Sealed, Attributes="HideInEditor") class UIXCPP_API UIXCanvasRootControl : public UIXRootControl
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIXCanvasRootControl)
    
//API_AUTO_SERIALIZATION();
//DECLARE_SCRIPTING_TYPE(UIXCanvasRootControl);
public:
    /// <summary>
    /// Gets the owning canvas.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE UIXCanvas *GetCanvas() const { return _canvas; }

    /// <summary>
    /// Gets a value indicating whether canvas is 2D (screen-space).
    /// </summary>
    API_PROPERTY()
    bool Is2D() const;

    /// <summary>
    /// Gets a value indicating whether canvas is 3D (world-space or camera-space).
    /// </summary>
    API_PROPERTY()
    bool Is3D() const;

    /// <summary>
    /// Checks if the 3D canvas intersects with a given 3D mouse ray.
    /// </summary>
    /// <param name="ray">The input ray to test (in world-space).</param>
    /// <param name="canvasLocation">Output canvas-space local position.</param>
    /// <returns>True if canvas intersects with that point, otherwise false.</returns>
    API_FUNCTION()
    bool Intersects3D(API_PARAM(Ref) Ray &ray, API_PARAM(Out) Float2 &canvasLocation) const;

    /// <inheritdoc />
    FORCE_INLINE CursorType GetCursor() const override { return CursorType(); }

    /// <inheritdoc />
    void SetCursor(CursorType value) override {}

    /// <inheritdoc />
    UIXControl* GetFocusedControl() const override;

    /// <inheritdoc />
    void SetFocusedControl(UIXControl *value) override;

    /// <inheritdoc />

    FORCE_INLINE Float2 GetTrackingMouseOffset() const override { return Float2::Zero; }

    /// <inheritdoc />
    FORCE_INLINE Float2 GetMousePosition() const override { return  _mousePosition; }

    /// <inheritdoc />
    void SetMousePosition(Float2 value) override { ; }

    /// <inheritdoc />
    void StartTrackingMouse(UIXControl *control, bool useMouseScreenOffset) override;

    /// <inheritdoc />
    void EndTrackingMouse() override;

    /// <inheritdoc />
    bool GetKey(KeyboardKeys key) override;

    /// <inheritdoc />
    bool GetKeyDown(KeyboardKeys key) override;

    /// <inheritdoc />
    bool GetKeyUp(KeyboardKeys key) override;

    /// <inheritdoc />
    bool GetMouseButton(MouseButton button) override;

    /// <inheritdoc />
    bool GetMouseButtonDown(MouseButton button) override;

    /// <inheritdoc />
    bool GetMouseButtonUp(MouseButton button) override;

    /// <inheritdoc />
    Float2 PointToParent(Float2 location) const override;

    /// <inheritdoc />
    Float2 PointFromParent(Float2 locationParent) const override;

    /// <inheritdoc />
    bool ContainsPoint(Float2 location) const override;

    /// <inheritdoc />
    void Update(float deltaTime) override;

    /// <inheritdoc />
    bool OnCharInput(char c) override;
    
    /// <inheritdoc />
    //API_FUNCTION() DragDropEffect OnDragDrop(Float2 location, const DragData &data) override;

    /// <inheritdoc />
    //API_FUNCTION() DragDropEffect OnDragEnter(Float2 location, const DragData &data) override;

    /// <inheritdoc />
    //API_FUNCTION() void OnDragLeave() override;

    /// <inheritdoc />
    //API_FUNCTION() DragDropEffect OnDragMove(Float2 location, const DragData &data) override;

    /// <inheritdoc />
    bool OnKeyDown(KeyboardKeys key) override;

    /// <inheritdoc />
    void OnKeyUp(KeyboardKeys key) override;

    /// <inheritdoc />
    bool OnMouseDoubleClick(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    bool OnMouseDown(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    void OnMouseEnter(Float2 location) override;

    /// <inheritdoc />
    void OnMouseLeave() override;

    /// <inheritdoc />
    void OnMouseMove(Float2 location) override;

    /// <inheritdoc />
    bool OnMouseUp(Float2 location, MouseButton button) override;

    /// <inheritdoc />
    bool OnMouseWheel(Float2 location, float delta) override;

    /// <inheritdoc />
    void OnTouchEnter(Float2 location, int pointerId) override;

    /// <inheritdoc />
    bool OnTouchDown(Float2 location, int pointerId) override;

    /// <inheritdoc />
    void OnTouchMove(Float2 location, int pointerId) override;

    /// <inheritdoc />
    bool OnTouchUp(Float2 location, int pointerId) override;

    /// <inheritdoc />
    void OnTouchLeave(int pointerId) override;

    /// <inheritdoc />
    void Focus() override;

    /// <inheritdoc />
    //API_FUNCTION() void DoDragDrop(const DragData &data) override;

    /// <summary>
    /// Initializes a new instance of the <see cref="CanvasRootControl"/> class.
    /// </summary>
    /// <param name="canvas">The canvas.</param>
    /*internal*/ UIXCanvasRootControl(UIXCanvas* canvas);
private:
    ///// <summary>
    ///// Initializes a new instance of the <see cref="CanvasRootControl"/> class.
    ///// </summary>
    ///// <param name="canvas">The canvas.</param>
    ///*internal*/ UIXCanvasRootControl(const SpawnParams &params, UIXCanvas *canvas);


    bool SkipEvents() const;

    void UpdateNavigation(float deltaTime, String actionName, UIXNavDirection direction, API_PARAM(Ref) float &heldTime, API_PARAM(Ref) float &rateTime);
    // Changed from C# that used a delegate as last parameter to call a function only found in this class. It now directly calls the function.
    void UpdateNavigation(float deltaTime, String actionName, API_PARAM(Ref) float &heldTime, API_PARAM(Ref) float &rateTime);

    UIXCanvas* _canvas;
    Float2 _mousePosition;
    float _navigationHeldTimeUp;
    float _navigationHeldTimeDown;
    float _navigationHeldTimeLeft;
    float _navigationHeldTimeRight;
    float _navigationHeldTimeSubmit;

    float _navigationRateTimeUp;
    float _navigationRateTimeDown;
    float _navigationRateTimeLeft;
    float _navigationRateTimeRight;
    float _navigationRateTimeSubmit;

    friend class UIXCanvas;
};

/*
        

       

        
    } */