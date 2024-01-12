#pragma once

#include "UIXRootControl.h"


class UIXCanvas;

/// <summary>
/// Root control implementation used by the <see cref="UIXCanvas"/> actor.
/// </summary>
/// <seealso cref="FlaxEngine.GUI.RootControl" />
API_CLASS(Attributes="HideInEditor, Sealed") class UIXCPP_API UIXCanvasRootControl : public UIXRootControl
{
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
    FORCE_INLINE bool Is2D() { _canvas->RenderMode == CanvasRenderMode::ScreenSpace; }

    /// <summary>
    /// Gets a value indicating whether canvas is 3D (world-space or camera-space).
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE bool Is3D() { _canvas->RenderMode != CanvasRenderMode::ScreenSpace; }

    /// <summary>
    /// Checks if the 3D canvas intersects with a given 3D mouse ray.
    /// </summary>
    /// <param name="ray">The input ray to test (in world-space).</param>
    /// <param name="canvasLocation">Output canvas-space local position.</param>
    /// <returns>True if canvas intersects with that point, otherwise false.</returns>
    API_FUNCTION()
    bool Intersects3D(API_PARAM(Ref) Ray &ray, API_PARAM(Out) Float2 &canvasLocation) const;

    /// <inheritdoc />
    API_PROPERTY()
    FORCE_INLINE CursorType GetCursor() const override { return CursorType(); }

    /// <inheritdoc />
    API_PROPERTY()
    void SetCursor(CursorType value) override {}

    /// <inheritdoc />
    API_PROPERTY()
    UIXControl* GetFocusedControl() const override;

    /// <inheritdoc />
    API_PROPERTY()
    void SetFocusedControl(UIXControl *value) override;

    /// <inheritdoc />
    API_PROPERTY()
    FORCE_INLINE Float2 GetTrackingMouseOffset() const override { return Float2::Zero; }

    /// <inheritdoc />
    API_PROPERTY()
    FORCE_INLINE Float2 GetMousePosition() const override { return  _mousePosition; }

    /// <inheritdoc />
    API_PROPERTY()
    void SetMousePosition(Float2 value) override { ; }

    /// <inheritdoc />
    API_FUNCTION() void StartTrackingMouse(UIXControl *control, bool useMouseScreenOffset) override;

    /// <inheritdoc />
    API_FUNCTION() void EndTrackingMouse() override
    {
        var parent = Parent ? .Root;
        parent ? .EndTrackingMouse();
    }

    /// <inheritdoc />
    API_FUNCTION() bool GetKey(KeyboardKeys key) override
    {
        return Input.GetKey(key);
    }

    /// <inheritdoc />
    API_FUNCTION() bool GetKeyDown(KeyboardKeys key) override
    {
        return Input.GetKeyDown(key);
    }

    /// <inheritdoc />
    API_FUNCTION() bool GetKeyUp(KeyboardKeys key) override
    {
        return Input.GetKeyUp(key);
    }

    /// <inheritdoc />
    API_FUNCTION() bool GetMouseButton(MouseButton button) override
    {
        return Input.GetMouseButton(button);
    }

    /// <inheritdoc />
    API_FUNCTION() bool GetMouseButtonDown(MouseButton button) override
    {
        return Input.GetMouseButtonDown(button);
    }

    /// <inheritdoc />
    API_FUNCTION() bool GetMouseButtonUp(MouseButton button) override
    {
        return Input.GetMouseButtonUp(button);
    }
private:
    /// <summary>
    /// Initializes a new instance of the <see cref="CanvasRootControl"/> class.
    /// </summary>
    /// <param name="canvas">The canvas.</param>
    /*internal*/ UIXCanvasRootControl(UIXCanvas *canvas);

    bool SkipEvents() { return !_canvas->ReceivesEvents() || !_canvas->IsVisible(); }

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

};

/*



        

        

       

        /// <inheritdoc />
        public override Float2 PointToParent(ref Float2 location)
        {
            if (Is2D)
                return base.PointToParent(ref location);

            var camera = Camera.MainCamera;
            if (!camera)
                return location;

            // Transform canvas local-space point to the game root location
            _canvas.GetWorldMatrix(out Matrix world);
            Vector3 locationCanvasSpace = new Vector3(location, 0.0f);
            Vector3.Transform(ref locationCanvasSpace, ref world, out Vector3 locationWorldSpace);
            camera.ProjectPoint(locationWorldSpace, out location);
            return location;
        }

        /// <inheritdoc />
        public override Float2 PointFromParent(ref Float2 locationParent)
        {
            if (Is2D)
                return base.PointFromParent(ref locationParent);

            var camera = Camera.MainCamera;
            if (!camera)
                return locationParent;

            // Use world-space ray to convert it to the local-space of the canvas
            UIXCanvas.CalculateRay(ref locationParent, out Ray ray);
            Intersects3D(ref ray, out var location);
            return location;
        }

        /// <inheritdoc />
        public override bool ContainsPoint(ref Float2 location)
        {
            return base.ContainsPoint(ref location)
                   && (_canvas.TestCanvasIntersection == null || _canvas.TestCanvasIntersection(ref location));
        }

        /// <inheritdoc />
        public override void Update(float deltaTime)
        {
            // Update navigation
            if (SkipEvents)
            {
                _navigationHeldTimeUp = _navigationHeldTimeDown = _navigationHeldTimeLeft = _navigationHeldTimeRight = 0;
                _navigationRateTimeUp = _navigationRateTimeDown = _navigationRateTimeLeft = _navigationRateTimeRight = 0;
            }
            {
                UpdateNavigation(deltaTime, _canvas.NavigateUp.Name, NavDirection.Up, ref _navigationHeldTimeUp, ref _navigationRateTimeUp);
                UpdateNavigation(deltaTime, _canvas.NavigateDown.Name, NavDirection.Down, ref _navigationHeldTimeDown, ref _navigationRateTimeDown);
                UpdateNavigation(deltaTime, _canvas.NavigateLeft.Name, NavDirection.Left, ref _navigationHeldTimeLeft, ref _navigationRateTimeLeft);
                UpdateNavigation(deltaTime, _canvas.NavigateRight.Name, NavDirection.Right, ref _navigationHeldTimeRight, ref _navigationRateTimeRight);
                UpdateNavigation(deltaTime, _canvas.NavigateSubmit.Name, ref _navigationHeldTimeSubmit, ref _navigationRateTimeSubmit, SubmitFocused);
            }

            base.Update(deltaTime);
        }

        private void UpdateNavigation(float deltaTime, string actionName, NavDirection direction, ref float heldTime, ref float rateTime)
        {
            if (Input.GetAction(actionName))
            {
                if (heldTime <= Mathf.Epsilon)
                {
                    Navigate(direction);
                }
                if (heldTime > _canvas.NavigationInputRepeatDelay)
                {
                    rateTime += deltaTime;
                }
                if (rateTime > _canvas.NavigationInputRepeatRate)
                {
                    Navigate(direction);
                    rateTime = 0;
                }
                heldTime += deltaTime;
            }
            else
            {
                heldTime = rateTime = 0;
            }
        }

        private void UpdateNavigation(float deltaTime, string actionName, ref float heldTime, ref float rateTime, Action action)
        {
            if (Input.GetAction(actionName))
            {
                if (heldTime <= Mathf.Epsilon)
                {
                    action();
                }
                if (heldTime > _canvas.NavigationInputRepeatDelay)
                {
                    rateTime += deltaTime;
                }
                if (rateTime > _canvas.NavigationInputRepeatRate)
                {
                    action();
                    rateTime = 0;
                }
                heldTime += deltaTime;
            }
            else
            {
                heldTime = rateTime = 0;
            }
        }

        /// <inheritdoc />
        public override bool OnCharInput(char c)
        {
            if (SkipEvents)
                return false;

            return base.OnCharInput(c);
        }

        /// <inheritdoc />
        public override DragDropEffect OnDragDrop(ref Float2 location, DragData data)
        {
            if (SkipEvents)
                return DragDropEffect.None;

            return base.OnDragDrop(ref location, data);
        }

        /// <inheritdoc />
        public override DragDropEffect OnDragEnter(ref Float2 location, DragData data)
        {
            if (SkipEvents)
                return DragDropEffect.None;

            return base.OnDragEnter(ref location, data);
        }

        /// <inheritdoc />
        public override void OnDragLeave()
        {
            if (SkipEvents)
                return;

            base.OnDragLeave();
        }

        /// <inheritdoc />
        public override DragDropEffect OnDragMove(ref Float2 location, DragData data)
        {
            if (SkipEvents)
                return DragDropEffect.None;

            return base.OnDragMove(ref location, data);
        }

        /// <inheritdoc />
        public override bool OnKeyDown(KeyboardKeys key)
        {
            if (SkipEvents)
                return false;

            return base.OnKeyDown(key);
        }

        /// <inheritdoc />
        public override void OnKeyUp(KeyboardKeys key)
        {
            if (SkipEvents)
                return;

            base.OnKeyUp(key);
        }

        /// <inheritdoc />
        public override bool OnMouseDoubleClick(Float2 location, MouseButton button)
        {
            if (SkipEvents)
                return false;

            return base.OnMouseDoubleClick(location, button);
        }

        /// <inheritdoc />
        public override bool OnMouseDown(Float2 location, MouseButton button)
        {
            if (SkipEvents)
                return false;

            return base.OnMouseDown(location, button);
        }

        /// <inheritdoc />
        public override void OnMouseEnter(Float2 location)
        {
            if (SkipEvents)
                return;

            _mousePosition = location;
            base.OnMouseEnter(location);
        }

        /// <inheritdoc />
        public override void OnMouseLeave()
        {
            _mousePosition = Float2.Zero;
            if (SkipEvents)
                return;

            base.OnMouseLeave();
        }

        /// <inheritdoc />
        public override void OnMouseMove(Float2 location)
        {
            if (SkipEvents)
                return;

            _mousePosition = location;
            base.OnMouseMove(location);
        }

        /// <inheritdoc />
        public override bool OnMouseUp(Float2 location, MouseButton button)
        {
            if (SkipEvents)
                return false;

            return base.OnMouseUp(location, button);
        }

        /// <inheritdoc />
        public override bool OnMouseWheel(Float2 location, float delta)
        {
            if (SkipEvents)
                return false;

            return base.OnMouseWheel(location, delta);
        }

        /// <inheritdoc />
        public override void OnTouchEnter(Float2 location, int pointerId)
        {
            if (SkipEvents)
                return;

            base.OnTouchEnter(location, pointerId);
        }

        /// <inheritdoc />
        public override bool OnTouchDown(Float2 location, int pointerId)
        {
            if (SkipEvents)
                return false;

            return base.OnTouchDown(location, pointerId);
        }

        /// <inheritdoc />
        public override void OnTouchMove(Float2 location, int pointerId)
        {
            if (SkipEvents)
                return;

            base.OnTouchMove(location, pointerId);
        }

        /// <inheritdoc />
        public override bool OnTouchUp(Float2 location, int pointerId)
        {
            if (SkipEvents)
                return false;

            return base.OnTouchUp(location, pointerId);
        }

        /// <inheritdoc />
        public override void OnTouchLeave(int pointerId)
        {
            if (SkipEvents)
                return;

            base.OnTouchLeave(pointerId);
        }

        /// <inheritdoc />
        public override void Focus()
        {
        }

        /// <inheritdoc />
        public override void DoDragDrop(DragData data)
        {
        }
    } */