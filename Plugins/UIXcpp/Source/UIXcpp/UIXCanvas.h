#pragma once

#include "Engine/Graphics/PostProcessEffect.h"
#include "Engine/Core/Math/OrientedBoundingBox.h"
#include "Engine/Level/Actor.h"
#include "Engine/Core/Types/LayersMask.h"
#include "Engine/Core/Types/BaseTypes.h"
#include "Engine/Core/Math/Vector2.h"
#include "Engine/Core/Delegate.h"
#include "UIXInputEvent.h"

//#include "Engine/Graphics/Textures/GPUTexture.h"
//#include "Engine/Scripting/Scripting.h"


class UIXCanvasRootControl;
class UIXCanvas;

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
/// PostFx used to render the <see cref="UICanvas"/>. Used when render mode is <see cref="UIXCanvasRenderMode::CameraSpace"/> or <see cref="UIXCanvasRenderMode::WorldSpace"/>.
/// </summary>
/// <seealso cref="FlaxEngine.PostProcessEffect" />
API_CLASS(Sealed, NoSpawn, Attributes = "HideInEditor")
class UIXCanvasRenderer : public PostProcessEffect
{
    DECLARE_SCENE_OBJECT_NO_SPAWN(UIXCanvasRenderer);

public:
    /// <inheritdoc />
    UIXCanvasRenderer() : PostProcessEffect(SpawnParams(Guid::New(), TypeInitializer)), Canvas(nullptr)
    {
        UseSingleTarget = true;
    }

    /// <inheritdoc />
    FORCE_INLINE bool CanRender() const override;

    /// <inheritdoc />
    void Render(GPUContext* context, API_PARAM(Ref) RenderContext& renderContext, GPUTexture* input, GPUTexture* output) override;

    /// <summary>
    /// The canvas to render.
    /// </summary>
    API_FIELD() UIXCanvas *Canvas;
};

API_CLASS(Sealed, NoConstructor, Attributes = "ActorContextMenu(\"New/UI/UI Canvas\"), ActorToolbox(\"GUI\")")
class UIXCPP_API UIXCanvas : public Actor
{
    DECLARE_SCENE_OBJECT(UIXCanvas);

public:
    // This is done with the declare scene object.
    ///// <summary>
    ///// Initializes a new instance of the <see cref="UIXCanvas"/> class.
    ///// </summary>
    //UIXCanvas(const SpawnParams& params);

    /// <summary>
    /// Finalizes an instance of the <see cref="UICanvas"/> class.
    /// </summary>
    ~UIXCanvas();

    //// TODO: work around the shortcoming of unsupported delegate marshalling with return value or refs
    ///// <summary>
    ///// Delegate schema for the callback used to perform custom canvas intersection test. Can be used to implement a canvas that has a holes or non-rectangular shape.
    ///// </summary>
    ///// <param name="location">The location of the point to test in coordinates of the canvas root control (see <see cref="GUI"/>).</param>
    ///// <returns>True if canvas was hit, otherwise false.</returns>
    //typedef Delegate<bool(Float2 &)> TestCanvasIntersectionDelegate;

    ///// <summary>
    ///// The callback used to perform custom canvas intersection test. Can be used to implement a canvas that has a holes or non-rectangular shape.
    ///// </summary>
    //API_EVENT(Attributes="HideInEditor")
    //TestCanvasIntersectionDelegate TestCanvasIntersection;

    /// <summary>
    /// Delegate schema for callback used to evaluate the world-space ray from the screen-space position (eg. project mouse position).
    /// </summary>
    /// <param name="location">The location in screen-space.</param>
    /// <param name="ray">The output ray in world-space.</param>
     
    typedef Delegate<Float2&, Ray&> CalculateRayDelegate;

    /// <summary>
    /// The current implementation of the <see cref="CalculateRayDelegate"/> used to calculate the mouse ray in 3D from the 2D location. Cannot be nullptr.
    /// </summary>
    API_EVENT()
    static Delegate<Float2&, Ray&> CalculateRay;
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
    API_PROPERTY()
    void SetRenderMode(UIXCanvasRenderMode value);

    /// <summary>
    /// Gets or sets the canvas rendering location within rendering pipeline. Used only in <see cref="UIXCanvasRenderMode::CameraSpace"/> or <see cref="UIXCanvasRenderMode::WorldSpace"/> or <see cref="UIXCanvasRenderMode::WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(13), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_Is3D\"), Tooltip(\"Canvas rendering location within the rendering pipeline. Change this if you want GUI to affect the lighting or post processing effects like bloom.\")")
    FORCE_INLINE PostProcessEffectLocation GetRenderLocation() const
    {
        return _renderLocation;
    }

    /// <summary>
    /// Gets or sets the canvas rendering location within rendering pipeline. Used only in <see cref="UIXCanvasRenderMode::CameraSpace"/> or <see cref="UIXCanvasRenderMode::WorldSpace"/> or <see cref="UIXCanvasRenderMode::WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY()
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
    API_PROPERTY()
    void SetOrder(int value);

    /// <summary>
    /// Gets or sets a value indicating whether canvas can receive the input events.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(15), EditorDisplay(\"Canvas\"), Tooltip(\"If checked, canvas can receive the input events.\")")
    FORCE_INLINE bool GetReceivesEvents() const
    {
        return _receivesEvents;
    }

    /// <summary>
    /// Gets or sets a value indicating whether canvas can receive the input events.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE void SetReceivesEvents(bool value)
    {
        _receivesEvents = value;
    }

    /// <summary>
    /// Gets or sets the size of the canvas. Used only in <see cref="UIXCanvasRenderMode::WorldSpace"/> or <see cref="UIXCanvasRenderMode::WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(20), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_IsWorldSpace\"), Tooltip(\"Canvas size.\")")
    Float2 GetSize() const;

    /// <summary>
    /// Gets or sets the size of the canvas. Used only in <see cref="UIXCanvasRenderMode::WorldSpace"/> or <see cref="UIXCanvasRenderMode::WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY()
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
    API_PROPERTY()
    FORCE_INLINE void SetIgnoreDepth(bool value)
    {
        _ignoreDepth = value;
    }

    /// <summary>
    /// Gets or sets the camera used to place the GUI when render mode is set to <see cref="UIXCanvasRenderMode::CameraSpace"/> or <see cref="UIXCanvasRenderMode::WorldSpaceFaceCamera"/>.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(50), EditorDisplay(\"Canvas\"), VisibleIf(\"Editor_UseRenderCamera\"), Tooltip(\"Camera used to place the GUI when RenderMode is set to CameraSpace or WorldSpaceFaceCamera.\")")
    FORCE_INLINE Camera* GetRenderCamera() const
    {
        return _renderCamera;
    }

    /// <summary>
    /// Gets or sets the camera used to place the GUI when render mode is set to <see cref="UIXCanvasRenderMode::CameraSpace"/> or <see cref="UIXCanvasRenderMode::WorldSpaceFaceCamera"/>.
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
    FORCE_INLINE float GetDistance() const
    {
        return _distance;
    }

    /// <summary>
    /// Gets or sets the distance from the <see cref="RenderCamera"/> to place the plane with GUI. If the screen is resized, changes resolution, or the camera frustum changes, the Canvas will automatically change size to match as well.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE void SetDistance(float value)
    {
        _distance = value;
    }

    /// <summary>
    /// Gets the canvas GUI root control.
    /// </summary>
    API_PROPERTY()
    FORCE_INLINE UIXCanvasRootControl* GetGUI() const
    {
        return _guiRoot;
    }

    /// <summary>
    /// The delay (in seconds) before a navigation input event starts repeating if input control is held down (Input Action mode is set to Pressing).
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(505), EditorDisplay(\"Navigation\", \"Input Repeat Delay\"),Tooltip(\"TheThe delay (in seconds) before a navigation input event starts repeating if input control is held down (Input Action mode is set to Pressing).\")")
    FORCE_INLINE float GetNavigationInputRepeatDelay() const { return _navigationInputRepeatDelay; }
    /// <summary>
    /// The delay (in seconds) before a navigation input event starts repeating if input control is held down (Input Action mode is set to Pressing).
    /// </summary>
    API_PROPERTY()
    void SetNavigationInputRepeatDelay(float value) { _navigationInputRepeatDelay = value; }

    /// <summary>
    /// The delay (in seconds) between successive repeated navigation input events after the first one.
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(506), EditorDisplay(\"Navigation\", \"Input Repeat Rate\"), Tooltip(\"The delay (in seconds) between successive repeated navigation input events after the first one.\")")
    FORCE_INLINE float GetNavigationInputRepeatRate() const { return _NavigationInputRepeatRate; }
    /// <summary>
    /// The delay (in seconds) between successive repeated navigation input events after the first one.
    /// </summary>
    API_PROPERTY()
    void SetNavigationInputRepeatRate(float value) { _NavigationInputRepeatRate = value; }



    API_FIELD(Attributes = "EditorOrder(510), EditorDisplay(\"Navigation\", \"Navigate Up\"), Tooltip(\"The input action for performing UI navigation Up (from Input Settings).\")")
    UIXInputEvent *NavigateUp;
    API_FIELD(Attributes = "EditorOrder(520), EditorDisplay(\"Navigation\", \"Navigate Down\"), Tooltip(\"The input action for performing UI navigation Down (from Input Settings).\")")
    UIXInputEvent *NavigateDown;
    API_FIELD(Attributes = "EditorOrder(530), EditorDisplay(\"Navigation\", \"Navigate Left\"), Tooltip(\"The input action for performing UI navigation Left (from Input Settings).\")")
    UIXInputEvent *NavigateLeft;
    API_FIELD(Attributes = "EditorOrder(540), EditorDisplay(\"Navigation\", \"Navigate Right\"), Tooltip(\"The input action for performing UI navigation Right (from Input Settings).\")")
    UIXInputEvent *NavigateRight;
    API_FIELD(Attributes = "EditorOrder(550), EditorDisplay(\"Navigation\", \"Navigate Submit\"), Tooltip(\"The input action for performing UI navigation Submit (from Input Settings).\")")
    UIXInputEvent *NavigateSubmit;

    // TODO: Navigation.
    /*

    /// <summary>
    /// The input action for performing UI navigation Up (from Input Settings).
    /// </summary>
    []
    []
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
    OrientedBoundingBox GetBounds() const;

    /// <summary>
    /// Gets the world matrix used to transform the GUI from the local space to the world space. Handles canvas rendering mode
    /// </summary>
    /// <param name="world">The world.</param>
    API_FUNCTION()
    void GetWorldMatrix(API_PARAM(Out) Matrix& world) const
    {
        GetWorldMatrix(Vector3::Zero, world);
    }

    /// <summary>
    /// Gets the world matrix used to transform the GUI from the local space to the world space. Handles canvas rendering mode
    /// </summary>
    /// <param name="viewOrigin">The view origin (when using relative-to-camera rendering).</param>
    /// <param name="world">The world.</param>
    API_FUNCTION()
    void GetWorldMatrix(Vector3 viewOrigin, API_PARAM(Out) Matrix& world) const;
    
    /// <summary>
    /// Gets a value indicating whether canvas is 2D (screen-space).
    /// </summary>
    API_PROPERTY()
    bool GetIs2D() const
    {
        return _renderMode == UIXCanvasRenderMode::ScreenSpace;
    }
    
    /// <summary>
    /// Gets a value indicating whether canvas is 3D (world-space or camera-space).
    /// </summary>
    API_PROPERTY()
    bool GetIs3D() const
    {
        return _renderMode != UIXCanvasRenderMode::ScreenSpace;
    }


private:
    void Setup();
    void OnUpdate();

    /*internal*/ void ParentChanged();
    /*internal*/ void Enable();
    /*internal*/ void Disable();
    /*internal*/ void EndPlay();

    /*internal*/ bool IsVisible() const;

    /*internal*/ bool IsVisible(LayersMask layersMask) const;

//#if FLAX_EDITOR
#if 0
    private SceneRenderTask _editorTask;
    private ContainerControl _editorRoot;

    internal void EditorOverride(SceneRenderTask task, ContainerControl root)
    {
        if (_editorTask == task && _editorRoot == root)
            return;
        if (_editorTask != nullptr && _renderer != nullptr)
            _editorTask.RemoveCustomPostFx(_renderer);
        if (_editorRoot != nullptr && _guiRoot != nullptr)
            _guiRoot->Parent = nullptr;

        _editorTask = task;
        _editorRoot = root;
        Setup();

        if (RenderMode == UIXCanvasRenderMode::ScreenSpace && _editorRoot != nullptr && _guiRoot != nullptr && IsActiveInHierarchy)
        {
            _guiRoot->Parent = _editorRoot;
            _guiRoot->IndexInParent = 0;
        }
    }
#endif


//#if FLAX_EDITOR
#if 0
    API_PROPERTY()
    bool GetEditor_IsWorldSpace()
    {
        return _renderMode == UIXCanvasRenderMode::WorldSpace || _renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera;
    }

    API_PROPERTY()
    bool GetEditor_IsCameraSpace()
    {
        return _renderMode == UIXCanvasRenderMode::CameraSpace;
    }

    API_PROPERTY()
    bool GetEditor_Is3D()
    {
        return _renderMode != UIXCanvasRenderMode::ScreenSpace;
    }

    API_PROPERTY()
    bool GetEditor_UseRenderCamera()
    {
        return _renderMode == UIXCanvasRenderMode::CameraSpace || _renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera;
    }

    /*internal*/ void OnActiveInTreeChanged()
    {
        if (RenderMode == UIXCanvasRenderMode::ScreenSpace && _editorRoot != nullptr && _guiRoot != nullptr)
        {
            _guiRoot->Parent = IsActiveInHierarchy ? _editorRoot : nullptr;
            _guiRoot->IndexInParent = 0;
        }
    }

#endif

    int _order;
    UIXCanvasRenderMode _renderMode;
    /* readonly */ UIXCanvasRootControl* const _guiRoot;
    UIXCanvasRenderer* _renderer;
    bool _isLoading, _isRegisteredForTick;
    PostProcessEffectLocation _renderLocation = PostProcessEffectLocation::Default;
    bool _receivesEvents;
    bool _ignoreDepth;
    Camera* _renderCamera;
    float _distance;

    float _navigationInputRepeatDelay = 0.5f;
    float _NavigationInputRepeatRate = 0.5f;



    friend class UIXCanvasRootControl;
    friend class UIXCanvasRenderer;

    /*

       

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

                if (RenderMode == UIXCanvasRenderMode::WorldSpace || RenderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera)
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

                if ((RenderMode == UIXCanvasRenderMode::WorldSpace ||
                     RenderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera ||
                     other.RenderMode == UIXCanvasRenderMode::WorldSpace ||
                     other.RenderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera) && Size != other.Size)
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

        




    }
}

    */
};
