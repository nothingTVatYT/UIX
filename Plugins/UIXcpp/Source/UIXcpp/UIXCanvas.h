﻿#pragma once

#include "Engine/Level/Actor.h"
#include "Engine/Level/Actors/Camera.h"
#include "Engine/Render2D/Render2D.h"
#include "Engine/Graphics/PostProcessEffect.h"
#include "Engine/Graphics/Textures/GPUTexture.h"
#include "Engine/Graphics/RenderTask.h"
#include "Engine/Graphics/GPUContext.h"
#include "Engine/Graphics/RenderBuffers.h"
#include "Engine/Core/Math/OrientedBoundingBox.h"
#include "Engine/Scripting/Scripting.h"
#include "Engine/Profiler/ProfilerGPU.h"

#include "UIXCanvasRootControl.h"

/// <summary>
/// The canvas rendering modes.
/// </summary>
API_ENUM()
enum class UIXCanvasRenderMode
{
    /// <summary>
    /// The screen space rendering mode that places UI elements on the screen rendered on top of the scene. If the screen is resized or changes resolution, the Canvas will automatically change size to match this.
    /// </summary>
    ScreenSpace = 0,

    /// <summary>
    /// The camera space rendering mode that places Canvas in a given distance in front of a specified Camera. The UI elements are rendered by this camera, which means that the Camera settings affect the appearance of the UI. If the Camera is set to Perspective, the UI elements will be rendered with perspective, and the amount of perspective distortion can be controlled by the Camera Field of View. If the screen is resized, changes resolution, or the camera frustum changes, the Canvas will automatically change size to match as well.
    /// </summary>
    CameraSpace = 1,

    /// <summary>
    /// The world space rendering mode that places Canvas as any other object in the scene. The size of the Canvas can be set manually using its Transform, and UI elements will render in front of or behind other objects in the scene based on 3D placement. This is useful for UIs that are meant to be a part of the world. This is also known as a 'diegetic interface'.
    /// </summary>
    WorldSpace = 2,

    /// <summary>
    /// The world space rendering mode that places Canvas as any other object in the scene and orients it to face the camera. The size of the Canvas can be set manually using its Transform, and UI elements will render in front of or behind other objects in the scene based on 3D placement. This is useful for UIs that are meant to be a part of the world. This is also known as a 'diegetic interface'.
    /// </summary>
    WorldSpaceFaceCamera = 3,
};

/// <summary>
/// PostFx used to render the <see cref="UICanvas"/>. Used when render mode is <see cref="CanvasRenderMode.CameraSpace"/> or <see cref="CanvasRenderMode.WorldSpace"/>.
/// </summary>
/// <seealso cref="FlaxEngine.PostProcessEffect" />
API_CLASS(Sealed, Attributes = "HideInEditor")
class UIXCanvasRenderer : public PostProcessEffect
{
    
public:
    /// <summary>
    /// The canvas to render.
    /// </summary>
    API_FIELD() UIXCanvas* Canvas;

    /// <inheritdoc />
    UIXCanvasRenderer()
    {
        UseSingleTarget = true;
    }

    /// <inheritdoc />
    API_FUNCTION() FORCE_INLINE bool CanRender() const override
    {
        // Sync with canvas options
        Location = Canvas->GetRenderLocation();
        Order = Canvas->GetOrder();

        return PostProcessEffect::CanRender();
    }

    /// <inheritdoc />
    void Render(GPUContext* context, API_PARAM(Ref) RenderContext& renderContext, GPUTexture* input, GPUTexture* output) override
    {
        if (!Canvas->IsVisible(renderContext.View.RenderLayersMask))
            return;
        auto bounds = Canvas->GetBounds();
        bounds.Transformation.Translation -= renderContext.View.Origin;
        if (renderContext.View.Frustum.Contains(bounds.GetBoundingBox()) == ContainmentType::Disjoint)
            return;

        int profilerEvent = ProfilerGPU::BeginEvent(TEXT("UI Canvas"));

        // Calculate rendering matrix (world*view*projection)
        Matrix worldMatrix;
        Canvas->GetWorldMatrix(renderContext.View.Origin, worldMatrix);

        Matrix viewMatrix;
        Matrix::Multiply(worldMatrix, renderContext.View.View, viewMatrix);
        Matrix viewProjectionMatrix;
        Matrix::Multiply(viewMatrix, renderContext.View.Projection, viewProjectionMatrix);

        // Pick a depth buffer
        GPUTexture* depthBuffer = Canvas->IgnoreDepth ? null : renderContext.Buffers->DepthBuffer;

        // Render GUI in 3D
        auto features = Render2D::Features;
        if (Canvas->RenderMode == UIXCanvasRenderMode::WorldSpace || Canvas->RenderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera)
            Render2D::Features &= ~Render2D::RenderingFeatures.VertexSnapping;
        Render2D::CallDrawing(Canvas->GUI, context, input, depthBuffer, viewProjectionMatrix);
        Render2D::Features = features;

        ProfilerGPU::EndEvent(profilerEvent);
    }
};

API_CLASS(Sealed, NoConstructor, Attributes = "ActorContextMenu(\"New/UI/UI Canvas\"), ActorToolbox(\"GUI\")")
class UIXCPP_API UIXCanvas : public Actor
{
    DECLARE_SCENE_OBJECT(UIXCanvas);

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="UIXCanvas"/> class.
    /// </summary>
    UIXCanvas(const SpawnParams& params);

    /// <summary>
    /// Finalizes an instance of the <see cref="UICanvas"/> class.
    /// </summary>
    ~UIXCanvas();

    /// <summary>
    /// Delegate schema for the callback used to perform custom canvas intersection test. Can be used to implement a canvas that has a holes or non-rectangular shape.
    /// </summary>
    /// <param name="location">The location of the point to test in coordinates of the canvas root control (see <see cref="GUI"/>).</param>
    /// <returns>True if canvas was hit, otherwise false.</returns>
    typedef Function<bool(API_PARAM(Ref) Float2& location)> TestCanvasIntersectionDelegate;

    /// <summary>
    /// Delegate schema for callback used to evaluate the world-space ray from the screen-space position (eg. project mouse position).
    /// </summary>
    /// <param name="location">The location in screen-space.</param>
    /// <param name="ray">The output ray in world-space.</param>
    typedef Function<void(API_PARAM(Ref) Float2& location, API_PARAM(Out) Ray& ray)> CalculateRayDelegate;

    /// <summary>
    /// The callback used to perform custom canvas intersection test. Can be used to implement a canvas that has a holes or non-rectangular shape.
    /// </summary>
    API_FIELD(Attributes = "HideInEditor")
    TestCanvasIntersectionDelegate TestCanvasIntersection;

    /// <summary>
    /// The current implementation of the <see cref="CalculateRayDelegate"/> used to calculate the mouse ray in 3D from the 2D location. Cannot be null.
    /// </summary>
    static CalculateRayDelegate CalculateRay;
    // TODO: Assign a default value to CalculateRay in the constructor of UIXCanvas.

    /// <summary>
    /// The default implementation of the <see cref="CalculateRayDelegate"/> that uses the <see cref="Camera.MainCamera"/> to evaluate the 3D ray.
    /// </summary>
    /// <param name="location">The location in screen-space.</param>
    /// <param name="ray">The output ray in world-space.</param>
    static void DefaultCalculateRay(Float2 location, API_PARAM(Out) Ray& ray);

    /// <summary>
    /// Gets the canvas rendering mode.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(10), EditorDisplay(\"Canvas\"), Tooltip(\"Canvas rendering mode.\")")
    FORCE_INLINE UIXCanvasRenderMode GetRenderMode() const
    {
        return _renderMode;
    }

    /// <summary>
    /// Sets the canvas rendering mode.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(10), EditorDisplay(\"Canvas\"), Tooltip(\"Canvas rendering mode.\")")
    void SetRenderMode(UIXCanvasRenderMode value);

    /// <summary>
    /// Gets or sets the canvas rendering location within rendering pipeline. Used only in <see cref="CanvasRenderMode.CameraSpace"/> or <see cref="CanvasRenderMode.WorldSpace"/> or <see cref="CanvasRenderMode.WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(13), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_Is3D\"), Tooltip(\"Canvas rendering location within the rendering pipeline. Change this if you want GUI to affect the lighting or post processing effects like bloom.\")")
    FORCE_INLINE PostProcessEffectLocation GetRenderLocation()
    {
        return _renderLocation;
    }

    /// <summary>
    /// Gets or sets the canvas rendering location within rendering pipeline. Used only in <see cref="CanvasRenderMode.CameraSpace"/> or <see cref="CanvasRenderMode.WorldSpace"/> or <see cref="CanvasRenderMode.WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(13), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_Is3D\"), Tooltip(\"Canvas rendering location within the rendering pipeline. Change this if you want GUI to affect the lighting or post processing effects like bloom.\")")
    FORCE_INLINE void SetRenderLocation(PostProcessEffectLocation value)
    {
        _renderLocation = value;
    }

    /// <summary>
    /// Gets or sets the canvas rendering and input events gather order. Created GUI canvas objects are sorted before rendering (from the lowest order to the highest order). Canvas with the highest order can handle input event first.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(14), EditorDisplay(\"Canvas\"), Tooltip(\"The canvas rendering and input events gather order. Created GUI canvas objects are sorted before rendering (from the lowest order to the highest order). Canvas with the highest order can handle input event first.\")")
    FORCE_INLINE int GetOrder()
    {
        return _order;
    }

    /// <summary>
    /// Gets or sets the canvas rendering and input events gather order. Created GUI canvas objects are sorted before rendering (from the lowest order to the highest order). Canvas with the highest order can handle input event first.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(14), EditorDisplay(\"Canvas\"), Tooltip(\"The canvas rendering and input events gather order. Created GUI canvas objects are sorted before rendering (from the lowest order to the highest order). Canvas with the highest order can handle input event first.\")")
    void SetOrder(int value);

    /// <summary>
    /// Gets or sets a value indicating whether canvas can receive the input events.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(15), EditorDisplay(\"Canvas\"), Tooltip(\"If checked, canvas can receive the input events.\")")
    FORCE_INLINE bool GetReceivesEvents()
    {
        return _receivesEvents;
    }

    /// <summary>
    /// Gets or sets a value indicating whether canvas can receive the input events.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(15), EditorDisplay(\"Canvas\"), Tooltip(\"If checked, canvas can receive the input events.\")")
    FORCE_INLINE void SetReceivesEvents(bool value)
    {
        _receivesEvents = value;
    }

    /// <summary>
    /// Gets or sets the size of the canvas. Used only in <see cref="CanvasRenderMode.WorldSpace"/> or <see cref="CanvasRenderMode.WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(20), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_IsWorldSpace\"), Tooltip(\"Canvas size.\")")
    FORCE_INLINE Float2 GetSize()
    {
        return _guiRoot->GetSize();
    }

    /// <summary>
    /// Gets or sets the size of the canvas. Used only in <see cref="CanvasRenderMode.WorldSpace"/> or <see cref="CanvasRenderMode.WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(20), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_IsWorldSpace\"), Tooltip(\"Canvas size.\")")
    void SetSize(Float2 value);

    /// <summary>
    /// Gets or sets a value indicating whether ignore scene depth when rendering the GUI (scene objects won't cover the interface).
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(30), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_Is3D\"), Tooltip(\"If checked, scene depth will be ignored when rendering the GUI (scene objects won't cover the interface).\")")
    FORCE_INLINE bool GetIgnoreDepth()
    {
        return _ignoreDepth;
    }

    /// <summary>
    /// Gets or sets a value indicating whether ignore scene depth when rendering the GUI (scene objects won't cover the interface).
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(30), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_Is3D\"), Tooltip(\"If checked, scene depth will be ignored when rendering the GUI (scene objects won't cover the interface).\")")
    FORCE_INLINE void SetIgnoreDepth(bool value)
    {
        _ignoreDepth = value;
    }

    /// <summary>
    /// Gets or sets the camera used to place the GUI when render mode is set to <see cref="CanvasRenderMode.CameraSpace"/> or <see cref="CanvasRenderMode.WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(50), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_UseRenderCamera\"), Tooltip(\"Camera used to place the GUI when RenderMode is set to CameraSpace or WorldSpaceFaceCamera.\")")
    FORCE_INLINE Camera* GetRenderCamera()
    {
        return _renderCamera;
    }

    /// <summary>
    /// Gets or sets the camera used to place the GUI when render mode is set to <see cref="CanvasRenderMode.CameraSpace"/> or <see cref="CanvasRenderMode.WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(50), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_UseRenderCamera\"), Tooltip(\"Camera used to place the GUI when RenderMode is set to CameraSpace or WorldSpaceFaceCamera.\")")
    FORCE_INLINE void SetRenderCamera(Camera* value)
    {
        _renderCamera = value;
    }

    /// <summary>
    /// Gets or sets the distance from the <see cref="RenderCamera"/> to place the plane with GUI. If the screen is resized, changes resolution, or the camera frustum changes, the Canvas will automatically change size to match as well.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(60), Limit(0.01f), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_IsCameraSpace\"), Tooltip(\"Distance from the RenderCamera to place the plane with GUI. If the screen is resized, changes resolution, or the camera frustum changes, the Canvas will automatically change size to match as well.\")")
    FORCE_INLINE float GetDistance()
    {
        return _distance;
    }

    /// <summary>
    /// Gets or sets the distance from the <see cref="RenderCamera"/> to place the plane with GUI. If the screen is resized, changes resolution, or the camera frustum changes, the Canvas will automatically change size to match as well.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(60), Limit(0.01f), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_IsCameraSpace\"), Tooltip(\"Distance from the RenderCamera to place the plane with GUI. If the screen is resized, changes resolution, or the camera frustum changes, the Canvas will automatically change size to match as well.\")")
    FORCE_INLINE void SetDistance(float value)
    {
        _distance = value;
    }

    /// <summary>
    /// Gets the canvas GUI root control.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE UIXCanvasRootControl* GetGUI()
    {
        return _guiRoot;
    }

    // TODO: Navigation.
    /*
    /// <summary>
    /// The delay (in seconds) before a navigation input event starts repeating if input control is held down (Input Action mode is set to Pressing).
    /// </summary>
    [EditorOrder(505), EditorDisplay("Navigation", "Input Repeat Delay")]
    [Tooltip("TheThe delay (in seconds) before a navigation input event starts repeating if input control is held down (Input Action mode is set to Pressing).")]
    public float NavigationInputRepeatDelay{ get; set; } = 0.5f;

    /// <summary>
    /// The delay (in seconds) between successive repeated navigation input events after the first one.
    /// </summary>
    [EditorOrder(506), EditorDisplay("Navigation", "Input Repeat Rate")]
    [Tooltip("The delay (in seconds) between successive repeated navigation input events after the first one.")]
    public float NavigationInputRepeatRate{ get; set; } = 0.05f;

    /// <summary>
    /// The input action for performing UI navigation Up (from Input Settings).
    /// </summary>
    [EditorOrder(510), EditorDisplay("Navigation", "Navigate Up")]
    [Tooltip("The input action for performing UI navigation Up (from Input Settings).")]
    public InputEvent NavigateUp { get; set; } = new InputEvent("NavigateUp");

    /// <summary>
    /// The input action for performing UI navigation Down (from Input Settings).
    /// </summary>
    [EditorOrder(520), EditorDisplay("Navigation", "Navigate Down")]
    [Tooltip("The input action for performing UI navigation Down (from Input Settings).")]
    public InputEvent NavigateDown { get; set; } = new InputEvent("NavigateDown");

    /// <summary>
    /// The input action for performing UI navigation Left (from Input Settings).
    /// </summary>
    [EditorOrder(530), EditorDisplay("Navigation", "Navigate Left")]
    [Tooltip("The input action for performing UI navigation Left (from Input Settings).")]
    public InputEvent NavigateLeft { get; set; } = new InputEvent("NavigateLeft");

    /// <summary>
    /// The input action for performing UI navigation Right (from Input Settings).
    /// </summary>
    [EditorOrder(540), EditorDisplay("Navigation", "Navigate Right")]
    [Tooltip("The input action for performing UI navigation Right (from Input Settings).")]
    public InputEvent NavigateRight { get; set; } = new InputEvent("NavigateRight");

    /// <summary>
    /// The input action for performing UI navigation Submit (from Input Settings).
    /// </summary>
    [EditorOrder(550), EditorDisplay("Navigation", "Navigate Submit")]
    [Tooltip("The input action for performing UI navigation Submit (from Input Settings).")]
    public InputEvent NavigateSubmit { get; set; } = new InputEvent("NavigateSubmit");
    */

    /// <summary>
    /// Gets the world-space oriented bounding box that contains a 3D canvas.
    /// </summary>
    API_PROPERTY()
    OrientedBoundingBox GetBounds()
    {
        OrientedBoundingBox bounds = OrientedBoundingBox();
        bounds.Extents = Float3(_guiRoot->GetSize() * 0.5f, 0.1e-7);

        Matrix world;
        GetWorldMatrix(world);

        Matrix offset;
        Matrix::Translation((float)bounds.Extents.X, (float)bounds.Extents.Y, 0, offset);

        Matrix boxWorld;
        Matrix::Multiply(offset, world, boxWorld);
        boxWorld.Decompose(bounds.Transformation);
        return bounds;
    }

    /// <summary>
    /// Gets the world matrix used to transform the GUI from the local space to the world space. Handles canvas rendering mode
    /// </summary>
    /// <param name="world">The world.</param>
    API_FUNCTION()
    void GetWorldMatrix(API_PARAM(Out) Matrix& world)
    {
        GetWorldMatrix(Vector3::Zero, world);
    }

    /// <summary>
    /// Gets the world matrix used to transform the GUI from the local space to the world space. Handles canvas rendering mode
    /// </summary>
    /// <param name="viewOrigin">The view origin (when using relative-to-camera rendering).</param>
    /// <param name="world">The world.</param>
    API_FUNCTION()
    void GetWorldMatrix(Vector3 viewOrigin, API_PARAM(Out) Matrix& world)
    {
        auto transform = Transform();
        Float3 translation = transform.Translation - viewOrigin;
        // TODO: Remove inline out declarations, and replace them with locals. Uncomment the FLAX_EDITOR if case.
//#if FLAX_EDITOR
        // Override projection for editor preview
        if (_editorTask)
        {
            if (_renderMode == UIXCanvasRenderMode::WorldSpace)
            {
                Matrix::Transformation(transform.Scale, transform.Orientation, translation, world);
            }
            else if (_renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera)
            {
                auto view = _editorTask.View;
                Matrix::Translation(_guiRoot->GetWidth() * -0.5f, _guiRoot->GetHeight() * -0.5f, 0, out var m1);
                Matrix::Scaling(transform.Scale, out var m2);
                Matrix::Multiply(m1, m2, out var m3);
                Quaternion::Euler(180, 180, 0, out var quat);
                Matrix::RotationQuaternion(ref quat, m2);
                Matrix::Multiply(m3, m2, m1);
                m2 = Matrix::Transformation(Float3::One, Quaternion::FromDirection(-view.Direction), translation);
                Matrix::Multiply(m1, m2, world);
            }
            else if (_renderMode == UIXCanvasRenderMode::CameraSpace)
            {
                auto view = _editorTask.View;
                auto frustum = view.Frustum;
                if (!frustum.IsOrthographic)
                    _guiRoot->SetSize(Float2(frustum.GetWidthAtDepth(GetDistance()), frustum.GetHeightAtDepth(GetDistance())));
                else
                    _guiRoot->SetSize(_editorTask.Viewport.Size);
                Matrix::Translation(_guiRoot->GetWidth() / -2.0f, _guiRoot->GetHeight() / -2.0f, 0, world);
                Matrix::RotationYawPitchRoll(Math::Pi, Math::Pi, 0, out var tmp2);
                Matrix::Multiply(world, tmp2, out var tmp1);
                Float3 viewPos = view.Position - viewOrigin;
                auto viewRot = view.Direction != Float3::Up ? Quaternion::LookRotation(view.Direction, Float3::Up) : Quaternion::LookRotation(view.Direction, Float3::Right);
                auto viewUp = Float3::Up * viewRot;
                auto viewForward = view.Direction;
                auto pos = view.Position + view.Direction * GetDistance();
                Matrix::Billboard(pos, viewPos, viewUp, viewForward, tmp2);
                Matrix::Multiply(tmp1, tmp2, world);
                return;
            }
            else
            {
                world = Matrix::Identity;
            }
            return;
        }
//#endif

        // Use default camera is not specified
        auto camera = GetRenderCamera() == nullptr ? Camera::GetMainCamera() : GetRenderCamera();

        if (_renderMode == UIXCanvasRenderMode::WorldSpace || (_renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera && !camera))
        {
            // In 3D world
            Matrix::Transformation(transform.Scale, transform.Orientation, translation, out world);
        }
        else if (_renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera)
        {
            // In 3D world face camera
            Matrix::Translation(_guiRoot->GetWidth() * -0.5f, _guiRoot->GetHeight() * -0.5f, 0, out var m1);
            Matrix::Scaling(transform.Scale, out var m2);
            Matrix::Multiply(m1, m2, out var m3);
            Quaternion::Euler(180, 180, 0, out var quat);
            Matrix::RotationQuaternion(quat, out m2);
            Matrix::Multiply(m3, m2, m1);
            Matrix::Transformation(Vector3::One, Quaternion::FromDirection(-camera->GetDirection()), translation, m2);
            Matrix::Multiply(m1, m2, world);
        }
        else if (_renderMode == UIXCanvasRenderMode::CameraSpace && camera)
        {
            Matrix tmp1, tmp2;

            // Adjust GUI size to the viewport size at the given distance form the camera
            auto viewport = camera->GetViewport();
            if (camera->GetUsePerspective())
            {
                camera->GetMatrices(tmp1, out var tmp3, viewport);
                Matrix::Multiply(tmp1, tmp3, tmp2);
                auto frustum = BoundingFrustum(tmp2);
                _guiRoot->SetSize(Float2(frustum.GetWidthAtDepth(GetDistance()), frustum.GetHeightAtDepth(GetDistance())));
            }
            else
            {
                _guiRoot->SetSize(viewport.Size * camera->GetOrthographicScale());
            }

            // Center viewport (and flip)
            Matrix::Translation(_guiRoot->GetWidth() / -2.0f, _guiRoot->GetHeight() / -2.0f, 0, world);
            Matrix::RotationYawPitchRoll(Math::Pi, Math::Pi, 0, tmp2);
            Matrix::Multiply(world, tmp2, tmp1);

            // In front of the camera
            Float3 viewPos = camera->GetPosition() - viewOrigin;
            auto viewRot = camera->GetOrientation();
            auto viewUp = viewRot * Float3::Up;
            auto viewForward = viewRot * Float3::Forward;
            auto pos = viewPos + viewForward * GetDistance();
            Matrix::Billboard(pos, viewPos, viewUp, viewForward, tmp2);

            Matrix::Multiply(tmp1, tmp2, world);
        }
        else
        {
            // Direct projection
            world = Matrix::Identity;
        }
    }
    
    /// <summary>
    /// Gets a value indicating whether canvas is 2D (screen-space).
    /// </summary>
    API_PROPERTY()
    bool GetIs2D()
    {
        return _renderMode == UIXCanvasRenderMode::ScreenSpace;
    }
    
    /// <summary>
    /// Gets a value indicating whether canvas is 3D (world-space or camera-space).
    /// </summary>
    API_PROPERTY()
    bool GetIs3D()
    {
        return _renderMode != UIXCanvasRenderMode::ScreenSpace;
    }

private:
    int _order;
    UIXCanvasRenderMode _renderMode;
    /* readonly */ UIXCanvasRootControl* _guiRoot;
    UIXCanvasRenderer* _renderer;
    bool _isLoading, _isRegisteredForTick;
    PostProcessEffectLocation _renderLocation = PostProcessEffectLocation::Default;
    bool _receivesEvents;
    bool _ignoreDepth;
    Camera* _renderCamera;
    float _distance;

    
    #if FLAX_EDITOR
    API_PROPERTY()
    bool GetEditor_IsWorldSpace()
    {
        return _renderMode == CanvasRenderMode.WorldSpace || _renderMode == CanvasRenderMode.WorldSpaceFaceCamera;
    }

    API_PROPERTY()
    bool GetEditor_IsCameraSpace()
    {
        return _renderMode == CanvasRenderMode.CameraSpace;
    }

    API_PROPERTY()
    bool GetEditor_Is3D()
    {
        return _renderMode != CanvasRenderMode.ScreenSpace;
    }

    API_PROPERTY()
    bool GetEditor_UseRenderCamera()
    {
        return _renderMode == CanvasRenderMode.CameraSpace || _renderMode == CanvasRenderMode.WorldSpaceFaceCamera;
    }
    #endif
    /*
        /// <summary>
        /// Gets the world matrix used to transform the GUI from the local space to the world space. Handles canvas rendering mode
        /// </summary>
        /// <param name="world">The world.</param>
        public void GetWorldMatrix(out Matrix world)
        {
            GetWorldMatrix(Vector3.Zero, out world);
        }

        /// <summary>
        /// Gets the world matrix used to transform the GUI from the local space to the world space. Handles canvas rendering mode
        /// </summary>
        /// <param name="viewOrigin">The view origin (when using relative-to-camera rendering).</param>
        /// <param name="world">The world.</param>
        public void GetWorldMatrix(Vector3 viewOrigin, out Matrix world)
        {
            var transform = Transform;
            Float3 translation = transform.Translation - viewOrigin;

#if FLAX_EDITOR
            // Override projection for editor preview
            if (_editorTask)
            {
                if (_renderMode == CanvasRenderMode.WorldSpace)
                {
                    Matrix.Transformation(ref transform.Scale, ref transform.Orientation, ref translation, out world);
                }
                else if (_renderMode == CanvasRenderMode.WorldSpaceFaceCamera)
                {
                    var view = _editorTask.View;
                    Matrix.Translation(_guiRoot.Width * -0.5f, _guiRoot.Height * -0.5f, 0, out var m1);
                    Matrix.Scaling(ref transform.Scale, out var m2);
                    Matrix.Multiply(ref m1, ref m2, out var m3);
                    Quaternion.Euler(180, 180, 0, out var quat);
                    Matrix.RotationQuaternion(ref quat, out m2);
                    Matrix.Multiply(ref m3, ref m2, out m1);
                    m2 = Matrix.Transformation(Float3.One, Quaternion.FromDirection(-view.Direction), translation);
                    Matrix.Multiply(ref m1, ref m2, out world);
                }
                else if (_renderMode == CanvasRenderMode.CameraSpace)
                {
                    var view = _editorTask.View;
                    var frustum = view.Frustum;
                    if (!frustum.IsOrthographic)
                        _guiRoot.Size = new Float2(frustum.GetWidthAtDepth(Distance), frustum.GetHeightAtDepth(Distance));
                    else
                        _guiRoot.Size = _editorTask.Viewport.Size;
                    Matrix.Translation(_guiRoot.Width / -2.0f, _guiRoot.Height / -2.0f, 0, out world);
                    Matrix.RotationYawPitchRoll(Mathf.Pi, Mathf.Pi, 0, out var tmp2);
                    Matrix.Multiply(ref world, ref tmp2, out var tmp1);
                    Float3 viewPos = view.Position - viewOrigin;
                    var viewRot = view.Direction != Float3.Up ? Quaternion.LookRotation(view.Direction, Float3.Up) : Quaternion.LookRotation(view.Direction, Float3.Right);
                    var viewUp = Float3.Up * viewRot;
                    var viewForward = view.Direction;
                    var pos = view.Position + view.Direction * Distance;
                    Matrix.Billboard(ref pos, ref viewPos, ref viewUp, ref viewForward, out tmp2);
                    Matrix.Multiply(ref tmp1, ref tmp2, out world);
                    return;
                }
                else
                {
                    world = Matrix.Identity;
                }
                return;
            }
#endif

            // Use default camera is not specified
            var camera = RenderCamera ?? Camera.MainCamera;

            if (_renderMode == CanvasRenderMode.WorldSpace || (_renderMode == CanvasRenderMode.WorldSpaceFaceCamera && !camera))
            {
                // In 3D world
                Matrix.Transformation(ref transform.Scale, ref transform.Orientation, ref translation, out world);
            }
            else if (_renderMode == CanvasRenderMode.WorldSpaceFaceCamera)
            {
                // In 3D world face camera
                Matrix.Translation(_guiRoot.Width * -0.5f, _guiRoot.Height * -0.5f, 0, out var m1);
                Matrix.Scaling(ref transform.Scale, out var m2);
                Matrix.Multiply(ref m1, ref m2, out var m3);
                Quaternion.Euler(180, 180, 0, out var quat);
                Matrix.RotationQuaternion(ref quat, out m2);
                Matrix.Multiply(ref m3, ref m2, out m1);
                m2 = Matrix.Transformation(Vector3.One, Quaternion.FromDirection(-camera.Direction), translation);
                Matrix.Multiply(ref m1, ref m2, out world);
            }
            else if (_renderMode == CanvasRenderMode.CameraSpace && camera)
            {
                Matrix tmp1, tmp2;

                // Adjust GUI size to the viewport size at the given distance form the camera
                var viewport = camera.Viewport;
                if (camera.UsePerspective)
                {
                    camera.GetMatrices(out tmp1, out var tmp3, ref viewport);
                    Matrix.Multiply(ref tmp1, ref tmp3, out tmp2);
                    var frustum = new BoundingFrustum(tmp2);
                    _guiRoot.Size = new Float2(frustum.GetWidthAtDepth(Distance), frustum.GetHeightAtDepth(Distance));
                }
                else
                {
                    _guiRoot.Size = viewport.Size * camera.OrthographicScale;
                }

                // Center viewport (and flip)
                Matrix.Translation(_guiRoot.Width / -2.0f, _guiRoot.Height / -2.0f, 0, out world);
                Matrix.RotationYawPitchRoll(Mathf.Pi, Mathf.Pi, 0, out tmp2);
                Matrix.Multiply(ref world, ref tmp2, out tmp1);

                // In front of the camera
                Float3 viewPos = camera.Position - viewOrigin;
                var viewRot = camera.Orientation;
                var viewUp = Float3.Up * viewRot;
                var viewForward = Float3.Forward * viewRot;
                var pos = viewPos + viewForward * Distance;
                Matrix.Billboard(ref pos, ref viewPos, ref viewUp, ref viewForward, out tmp2);

                Matrix.Multiply(ref tmp1, ref tmp2, out world);
            }
            else
            {
                // Direct projection
                world = Matrix.Identity;
            }
        }

        private void Setup()
        {
            if (_isLoading)
                return;

            switch (_renderMode)
            {
            case CanvasRenderMode.ScreenSpace:
            {
                // Fill the screen area
                _guiRoot.AnchorPreset = AnchorPresets.StretchAll;
                _guiRoot.Offsets = Margin.Zero;
                if (_renderer)
                {
#if FLAX_EDITOR
                    if (_editorTask != null)
                        _editorTask.RemoveCustomPostFx(_renderer);
#endif
                    SceneRenderTask.RemoveGlobalCustomPostFx(_renderer);
                    _renderer.Canvas = null;
                    Destroy(_renderer);
                    _renderer = null;
                }
#if FLAX_EDITOR
                if (_editorRoot != null && IsActiveInHierarchy)
                {
                    _guiRoot.Parent = _editorRoot;
                    _guiRoot.IndexInParent = 0;
                }
#endif
                if (_isRegisteredForTick)
                {
                    _isRegisteredForTick = false;
                    Scripting.Update -= OnUpdate;
                }
                break;
            }
            case CanvasRenderMode.CameraSpace:
            case CanvasRenderMode.WorldSpace:
            case CanvasRenderMode.WorldSpaceFaceCamera:
            {
                // Render canvas manually
                _guiRoot.AnchorPreset = AnchorPresets.TopLeft;
#if FLAX_EDITOR
                if (_editorRoot != null && _guiRoot != null)
                    _guiRoot.Parent = null;
#endif
                if (_renderer == null)
                {
                    _renderer = New<CanvasRenderer>();
                    _renderer.Canvas = this;
                    if (IsActiveInHierarchy && Scene)
                    {
#if FLAX_EDITOR
                        if (_editorTask != null)
                        {
                            _editorTask.AddCustomPostFx(_renderer);
                            break;
                        }
#endif
                        SceneRenderTask.AddGlobalCustomPostFx(_renderer);
                    }
#if FLAX_EDITOR
                    else if (_editorTask != null && IsActiveInHierarchy)
                    {
                        _editorTask.AddCustomPostFx(_renderer);
                    }
#endif
                }
                if (!_isRegisteredForTick)
                {
                    _isRegisteredForTick = true;
                    Scripting.Update += OnUpdate;
                }
                break;
            }
            }
        }

        private void OnUpdate()
        {
            if (this && IsActiveInHierarchy && _renderMode != CanvasRenderMode.ScreenSpace)
            {
                try
                {
                    Profiler.BeginEvent(Name);
                    _guiRoot.Update(Time.UnscaledDeltaTime);
                }
                finally
                {
                    Profiler.EndEvent();
                }
            }
        }

        internal string Serialize()
        {
            StringBuilder sb = new StringBuilder(256);
            StringWriter sw = new StringWriter(sb, CultureInfo.InvariantCulture);
            using (JsonTextWriter jsonWriter = new JsonTextWriter(sw))
            {
                jsonWriter.IndentChar = '\t';
                jsonWriter.Indentation = 1;
                jsonWriter.Formatting = Formatting.Indented;

                jsonWriter.WriteStartObject();

                jsonWriter.WritePropertyName("RenderMode");
                jsonWriter.WriteValue(_renderMode);

                jsonWriter.WritePropertyName("RenderLocation");
                jsonWriter.WriteValue(RenderLocation);

                jsonWriter.WritePropertyName("Order");
                jsonWriter.WriteValue(Order);

                jsonWriter.WritePropertyName("ReceivesEvents");
                jsonWriter.WriteValue(ReceivesEvents);

                jsonWriter.WritePropertyName("IgnoreDepth");
                jsonWriter.WriteValue(IgnoreDepth);

                jsonWriter.WritePropertyName("RenderCamera");
                jsonWriter.WriteValue(Json.JsonSerializer.GetStringID(RenderCamera));

                jsonWriter.WritePropertyName("Distance");
                jsonWriter.WriteValue(Distance);

                if (RenderMode == CanvasRenderMode.WorldSpace || RenderMode == CanvasRenderMode.WorldSpaceFaceCamera)
                {
                    jsonWriter.WritePropertyName("Size");
                    jsonWriter.WriteStartObject();
                    jsonWriter.WritePropertyName("X");
                    jsonWriter.WriteValue(Size.X);
                    jsonWriter.WritePropertyName("Y");
                    jsonWriter.WriteValue(Size.Y);
                    jsonWriter.WriteEndObject();
                }

                jsonWriter.WritePropertyName("NavigationInputRepeatDelay");
                jsonWriter.WriteValue(NavigationInputRepeatDelay);
                jsonWriter.WritePropertyName("NavigationInputRepeatRate");
                jsonWriter.WriteValue(NavigationInputRepeatRate);

                jsonWriter.WritePropertyName("NavigateUp");
                jsonWriter.WriteStartObject();
                jsonWriter.WritePropertyName("Name");
                jsonWriter.WriteValue(NavigateUp.Name);
                jsonWriter.WriteEndObject();

                jsonWriter.WritePropertyName("NavigateDown");
                jsonWriter.WriteStartObject();
                jsonWriter.WritePropertyName("Name");
                jsonWriter.WriteValue(NavigateDown.Name);
                jsonWriter.WriteEndObject();

                jsonWriter.WritePropertyName("NavigateLeft");
                jsonWriter.WriteStartObject();
                jsonWriter.WritePropertyName("Name");
                jsonWriter.WriteValue(NavigateLeft.Name);
                jsonWriter.WriteEndObject();

                jsonWriter.WritePropertyName("NavigateRight");
                jsonWriter.WriteStartObject();
                jsonWriter.WritePropertyName("Name");
                jsonWriter.WriteValue(NavigateRight.Name);
                jsonWriter.WriteEndObject();

                jsonWriter.WritePropertyName("NavigateSubmit");
                jsonWriter.WriteStartObject();
                jsonWriter.WritePropertyName("Name");
                jsonWriter.WriteValue(NavigateSubmit.Name);
                jsonWriter.WriteEndObject();

                jsonWriter.WriteEndObject();
            }

            return sw.ToString();
        }

        internal string SerializeDiff(UICanvas other)
        {
            StringBuilder sb = new StringBuilder(256);
            StringWriter sw = new StringWriter(sb, CultureInfo.InvariantCulture);
            using (JsonTextWriter jsonWriter = new JsonTextWriter(sw))
            {
                jsonWriter.IndentChar = '\t';
                jsonWriter.Indentation = 1;
                jsonWriter.Formatting = Formatting.Indented;

                jsonWriter.WriteStartObject();

                if (_renderMode != other._renderMode)
                {
                    jsonWriter.WritePropertyName("RenderMode");
                    jsonWriter.WriteValue(_renderMode);
                }

                if (RenderLocation != other.RenderLocation)
                {
                    jsonWriter.WritePropertyName("RenderLocation");
                    jsonWriter.WriteValue(RenderLocation);
                }

                if (Order != other.Order)
                {
                    jsonWriter.WritePropertyName("Order");
                    jsonWriter.WriteValue(Order);
                }

                if (ReceivesEvents != other.ReceivesEvents)
                {
                    jsonWriter.WritePropertyName("ReceivesEvents");
                    jsonWriter.WriteValue(ReceivesEvents);
                }

                if (IgnoreDepth != other.IgnoreDepth)
                {
                    jsonWriter.WritePropertyName("IgnoreDepth");
                    jsonWriter.WriteValue(IgnoreDepth);
                }

                if (RenderCamera != other.RenderCamera)
                {
                    jsonWriter.WritePropertyName("RenderCamera");
                    jsonWriter.WriteValue(Json.JsonSerializer.GetStringID(RenderCamera));
                }

                if (Mathf.Abs(Distance - other.Distance) > Mathf.Epsilon)
                {
                    jsonWriter.WritePropertyName("Distance");
                    jsonWriter.WriteValue(Distance);
                }

                if ((RenderMode == CanvasRenderMode.WorldSpace ||
                     RenderMode == CanvasRenderMode.WorldSpaceFaceCamera ||
                     other.RenderMode == CanvasRenderMode.WorldSpace ||
                     other.RenderMode == CanvasRenderMode.WorldSpaceFaceCamera) && Size != other.Size)
                {
                    jsonWriter.WritePropertyName("Size");
                    jsonWriter.WriteStartObject();
                    jsonWriter.WritePropertyName("X");
                    jsonWriter.WriteValue(Size.X);
                    jsonWriter.WritePropertyName("Y");
                    jsonWriter.WriteValue(Size.Y);
                    jsonWriter.WriteEndObject();
                }

                if (!Mathf.NearEqual(NavigationInputRepeatDelay, other.NavigationInputRepeatDelay))
                {
                    jsonWriter.WritePropertyName("NavigationInputRepeatDelay");
                    jsonWriter.WriteValue(NavigationInputRepeatDelay);
                }
                if (!Mathf.NearEqual(NavigationInputRepeatRate, other.NavigationInputRepeatRate))
                {
                    jsonWriter.WritePropertyName("NavigationInputRepeatRate");
                    jsonWriter.WriteValue(NavigationInputRepeatRate);
                }
                if (NavigateUp.Name != other.NavigateUp.Name)
                {
                    jsonWriter.WritePropertyName("NavigateUp");
                    jsonWriter.WriteStartObject();
                    jsonWriter.WritePropertyName("Name");
                    jsonWriter.WriteValue(NavigateUp.Name);
                    jsonWriter.WriteEndObject();
                }
                if (NavigateDown.Name != other.NavigateDown.Name)
                {
                    jsonWriter.WritePropertyName("NavigateDown");
                    jsonWriter.WriteStartObject();
                    jsonWriter.WritePropertyName("Name");
                    jsonWriter.WriteValue(NavigateDown.Name);
                    jsonWriter.WriteEndObject();
                }
                if (NavigateLeft.Name != other.NavigateLeft.Name)
                {
                    jsonWriter.WritePropertyName("NavigateLeft");
                    jsonWriter.WriteStartObject();
                    jsonWriter.WritePropertyName("Name");
                    jsonWriter.WriteValue(NavigateLeft.Name);
                    jsonWriter.WriteEndObject();
                }
                if (NavigateRight.Name != other.NavigateRight.Name)
                {
                    jsonWriter.WritePropertyName("NavigateRight");
                    jsonWriter.WriteStartObject();
                    jsonWriter.WritePropertyName("Name");
                    jsonWriter.WriteValue(NavigateRight.Name);
                    jsonWriter.WriteEndObject();
                }
                if (NavigateSubmit.Name != other.NavigateSubmit.Name)
                {
                    jsonWriter.WritePropertyName("NavigateSubmit");
                    jsonWriter.WriteStartObject();
                    jsonWriter.WritePropertyName("Name");
                    jsonWriter.WriteValue(NavigateSubmit.Name);
                    jsonWriter.WriteEndObject();
                }

                jsonWriter.WriteEndObject();
            }

            return sw.ToString();
        }

        internal void Deserialize(string json)
        {
            _isLoading = true;
            Json.JsonSerializer.Deserialize(this, json);
            _isLoading = false;
        }

        internal void PostDeserialize()
        {
            Setup();
        }

        internal void ParentChanged()
        {
#if FLAX_EDITOR
            if (RenderMode == CanvasRenderMode.ScreenSpace && _editorRoot != null && _guiRoot != null)
            {
                _guiRoot.Parent = IsActiveInHierarchy ? _editorRoot : null;
                _guiRoot.IndexInParent = 0;
            }
#endif
        }

        internal void Enable()
        {
#if FLAX_EDITOR
            if (_editorRoot != null)
            {
                _guiRoot.Parent = _editorRoot;
                _guiRoot.IndexInParent = 0;
            }
            else
            {
                _guiRoot.Parent = RootControl.CanvasRoot;
            }
#else
            _guiRoot.Parent = RootControl.CanvasRoot;
#endif

            if (_renderer)
            {
#if FLAX_EDITOR
                if (_editorTask != null)
                {
                    _editorTask.AddCustomPostFx(_renderer);
                    return;
                }
#endif
                SceneRenderTask.AddGlobalCustomPostFx(_renderer);
            }
        }

        internal void Disable()
        {
            _guiRoot.Parent = null;

            if (_renderer)
            {
                SceneRenderTask.RemoveGlobalCustomPostFx(_renderer);
            }
        }

#if FLAX_EDITOR
        internal void OnActiveInTreeChanged()
        {
            if (RenderMode == CanvasRenderMode.ScreenSpace && _editorRoot != null && _guiRoot != null)
            {
                _guiRoot.Parent = IsActiveInHierarchy ? _editorRoot : null;
                _guiRoot.IndexInParent = 0;
            }
        }
#endif

        internal void EndPlay()
        {
            if (_isRegisteredForTick)
            {
                _isRegisteredForTick = false;
                Scripting.Update -= OnUpdate;
            }

            if (_renderer)
            {
                SceneRenderTask.RemoveGlobalCustomPostFx(_renderer);
                _renderer.Canvas = null;
                Destroy(_renderer);
                _renderer = null;
            }
        }

        internal bool IsVisible()
        {
            return IsVisible(MainRenderTask.Instance?.ViewLayersMask ?? LayersMask.Default);
        }

        internal bool IsVisible(LayersMask layersMask)
        {
#if FLAX_EDITOR
            if (_editorTask != null || _editorRoot != null)
                return true;
#endif
            return layersMask.HasLayer(Layer);
        }

#if FLAX_EDITOR
        private SceneRenderTask _editorTask;
        private ContainerControl _editorRoot;

        internal void EditorOverride(SceneRenderTask task, ContainerControl root)
        {
            if (_editorTask == task && _editorRoot == root)
                return;
            if (_editorTask != null && _renderer != null)
                _editorTask.RemoveCustomPostFx(_renderer);
            if (_editorRoot != null && _guiRoot != null)
                _guiRoot.Parent = null;

            _editorTask = task;
            _editorRoot = root;
            Setup();

            if (RenderMode == CanvasRenderMode.ScreenSpace && _editorRoot != null && _guiRoot != null && IsActiveInHierarchy)
            {
                _guiRoot.Parent = _editorRoot;
                _guiRoot.IndexInParent = 0;
            }
        }
#endif
    }
}

    */
};
