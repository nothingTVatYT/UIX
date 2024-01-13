#include "UIXCanvas.h"
#include "UIXCanvasRootControl.h"

#include "Engine/Level/Scene/Scene.h"
#include "Engine/Profiler/Profiler.h"
#include "Engine/Render2D/Render2D.h"
#include "Engine/Graphics/GPUContext.h"
#include "Engine/Graphics/RenderTask.h"
#include "Engine/Graphics/RenderBuffers.h"
#include "Engine/Level/Actors/Camera.h"
#include "Engine/Core/Log.h"
#include "Engine/Profiler/ProfilerGPU.h"
#include "Engine/Profiler/ProfilerCPU.h"





bool UIXCanvasRenderer::CanRender() const
{
    // Sync with canvas options
    SetLocation(Canvas->GetRenderLocation());
    SetOrder(Canvas->GetOrder());

    return PostProcessEffect::CanRender();
}

void UIXCanvasRenderer::Render(GPUContext* context, API_PARAM(Ref) RenderContext& renderContext, GPUTexture* input, GPUTexture* output)
{
    if (!Canvas->GetIsVisible(renderContext.View.RenderLayersMask))
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
    GPUTexture* depthBuffer = Canvas->GetIgnoreDepth() ? nullptr : renderContext.Buffers->DepthBuffer;

    // Render GUI in 3D
    auto features = Render2D::Features;

    if (Canvas->GetRenderMode() == UIXCanvasRenderMode::WorldSpace || Canvas->GetRenderMode() == UIXCanvasRenderMode::WorldSpaceFaceCamera)
        Render2D::Features = (Render2D::RenderingFeatures)((int)Render2D::Features & ~(int)Render2D::RenderingFeatures::VertexSnapping);

    //Render2D::CallDrawing(Canvas->GUI, context, input, depthBuffer, viewProjectionMatrix);
    // Replacement of the CallDrawing call which is missing from C++, but only takes up a few lines:

    if (context != nullptr && input != nullptr && Canvas->GetGUI() != nullptr)
    {
        Render2D::Begin(context, input);
        // TODO: check if we can get around the try/catch, since it's bad for peformance.
        try
        {
            Canvas->GetGUI()->Draw();
        }
        catch (...)
        {
            LOG(Error, "Drawing of the UI failed with an exception.");
        }
        Render2D::End();
    }

    Render2D::Features = features;

    ProfilerGPU::EndEvent(profilerEvent);
}





UIXCanvas::UIXCanvas(const SpawnParams& params) : Actor(params), _guiRoot(New<UIXCanvasRootControl>(this))
{
    _guiRoot->SetIsLayoutLocked(false);
}

UIXCanvas::~UIXCanvas()
{
    if (_isRegisteredForTick)
    {
        // TODO: Figure out how to register to Scripting Events.

        _isRegisteredForTick = false;
        //Scripting::Update -= OnUpdate;

        // C# and C++ ticking at different places. This should work (until proven wrong)
        GetScene()->Ticking.Update.RemoveTick(this);
    }
}

void UIXCanvas::SetRenderMode(UIXCanvasRenderMode value)
{
    if (_renderMode != value)
    {
        auto previous = _renderMode;

        _renderMode = value;

        Setup();

        // Reset size
        if (previous == UIXCanvasRenderMode::ScreenSpace || (_renderMode == UIXCanvasRenderMode::WorldSpace || _renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera))
            SetSize(Float2(500, 500));
    }
}

void UIXCanvas::SetOrder(int value)
{
    if (_order != value)
    {
        _order = value;
        RootControl.CanvasRoot.SortCanvases();
    }
}

Float2 UIXCanvas::GetSize() const
{
    return _guiRoot->GetSize();
}

void UIXCanvas::SetSize(Float2 value)
{
    if (_renderMode == UIXCanvasRenderMode::WorldSpace || _renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera || _isLoading)
    {
        _guiRoot->SetSize(value);
    }
}

OrientedBoundingBox UIXCanvas::GetBounds()
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

void UIXCanvas::GetWorldMatrix(Vector3 viewOrigin, API_PARAM(Out) Matrix& world) const
{
    auto transform = Transform();
    Float3 translation = transform.Translation - viewOrigin;
    // TODO: Remove inline out declarations, and replace them with locals. Uncomment the FLAX_EDITOR if case.
//#if FLAX_EDITOR
#if 0
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
#endif

    // Use default camera is not specified
    auto camera = GetRenderCamera() == nullptr ? Camera::GetMainCamera() : GetRenderCamera();

    if (_renderMode == UIXCanvasRenderMode::WorldSpace || (_renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera && !camera))
    {
        // In 3D world
        Matrix::Transformation(transform.Scale, transform.Orientation, translation, world);
    }
    else if (_renderMode == UIXCanvasRenderMode::WorldSpaceFaceCamera)
    {
        // In 3D world face camera
        Matrix m1;
        Matrix::Translation(_guiRoot->GetWidth() * -0.5f, _guiRoot->GetHeight() * -0.5f, 0, m1);
        Matrix m2;
        Matrix::Scaling(transform.Scale, m2);
        Matrix m3;
        Matrix::Multiply(m1, m2, m3);
        Quaternion quat = Quaternion::Euler(180, 180, 0);
        Matrix::RotationQuaternion(quat, m2);
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
            Matrix tmp3;
            camera->GetMatrices(tmp1, tmp3, viewport);
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
        Matrix::RotationYawPitchRoll(PI, PI, 0, tmp2);
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


void UIXCanvas::DefaultCalculateRay(Float2 location, API_PARAM(Out) Ray& ray)
{
    auto camera = Camera::GetMainCamera();
    if (camera)
    {
        ray = camera->ConvertMouseToRay(location * Platform::GetDpiScale());
    }
    else
    {
        ray = Ray(Vector3::Zero, Vector3::Forward);
    }
}

void UIXCanvas::Setup()
{
    if (_isLoading)
        return;

    switch (_renderMode)
    {
        case UIXCanvasRenderMode::ScreenSpace:
        {
            // Fill the screen area
            _guiRoot->SetAnchorPreset(UIXAnchorPresets::StretchAll);
            _guiRoot->SetOffsets(UIXMargin::Zero);
            if (_renderer)
            {
                //#if FLAX_EDITOR
#if 0
                if (_editorTask != nullptr)
                    _editorTask.RemoveCustomPostFx(_renderer);
#endif
                SceneRenderTask::RemoveGlobalCustomPostFx(_renderer);
                _renderer->Canvas = nullptr;

                // TODO: make sure this doesn't crash. Replacement for: Destroy(_renderer);
                _renderer->DeleteObjectNow();

                _renderer = nullptr;
            }
            //#if FLAX_EDITOR
#if 0
            if (_editorRoot != nullptr && IsActiveInHierarchy)
            {
                _guiRoot->Parent = _editorRoot;
                _guiRoot->IndexInParent = 0;
            }
#endif
            if (_isRegisteredForTick)
            {
                _isRegisteredForTick = false;

                //Scripting.Update -= OnUpdate;
                GetScene()->Ticking.Update.RemoveTick(this);
            }
            break;
        }
        case UIXCanvasRenderMode::CameraSpace:
        case UIXCanvasRenderMode::WorldSpace:
        case UIXCanvasRenderMode::WorldSpaceFaceCamera:
        {
            // Render canvas manually
            _guiRoot->SetAnchorPreset(UIXAnchorPresets::TopLeft);
            //#if FLAX_EDITOR
#if 0
            if (_editorRoot != nullptr && _guiRoot != nullptr)
                _guiRoot->Parent = nullptr;
#endif
            if (_renderer == nullptr)
            {
                _renderer = New<UIXCanvasRenderer>();
                _renderer->Canvas = this;
                if (IsActiveInHierarchy && GetScene() != nullptr)
                {
                    //#if FLAX_EDITOR
#if 0

                    if (_editorTask != nullptr)
                    {
                        _editorTask.AddCustomPostFx(_renderer);
                        break;
                    }
#endif
                    SceneRenderTask::AddGlobalCustomPostFx(_renderer);
                }
                //#if FLAX_EDITOR
#if 0
                else if (_editorTask != nullptr && IsActiveInHierarchy)
                {
                    _editorTask.AddCustomPostFx(_renderer);
                }
#endif
            }
            if (!_isRegisteredForTick)
            {
                _isRegisteredForTick = true;

                GetScene()->Ticking.Update.Addcript<UIXCanvas, &UIXCanvas::OnUpdate>(this);
                //Scripting.Update += OnUpdate;
            }
            break;
        }
    }
}

void UIXCanvas::OnUpdate()
{
    if (this && IsActiveInHierarchy && _renderMode != UIXCanvasRenderMode::ScreenSpace)
    {
        try
        {
            ProfilerCPU::BeginEvent(Name);
            _guiRoot->Update(Time.UnscaledDeltaTime);
        }
        catch (...)
        {
            ProfilerCPU::EndEvent();
            LOG(Error, "Exception thrown during gui root update.");
            return;
        }

        ProfilerCPU::EndEvent();
    }
}


void UIXCanvas::ParentChanged()
{
//#if FLAX_EDITOR
#if 0
    if (RenderMode == UIXCanvasRenderMode::ScreenSpace && _editorRoot != nullptr && _guiRoot != nullptr)
    {
        _guiRoot->Parent = IsActiveInHierarchy ? _editorRoot : nullptr;
        _guiRoot->IndexInParent = 0;
    }
#endif
}

void UIXCanvas::Enable()
{
//#if FLAX_EDITOR
#if 0
    if (_editorRoot != nullptr)
    {
        _guiRoot->Parent = _editorRoot;
        _guiRoot->IndexInParent = 0;
    }
    else
    {
        _guiRoot->Parent = RootControl.CanvasRoot;
    }
#else
    _guiRoot->SetParent(RootControl.CanvasRoot);
#endif

    if (_renderer)
    {
//#if FLAX_EDITOR
#if 0
        if (_editorTask != nullptr)
        {
            _editorTask.AddCustomPostFx(_renderer);
            return;
        }
#endif
        SceneRenderTask::AddGlobalCustomPostFx(_renderer);
    }
}

void UIXCanvas::Disable()
{
    _guiRoot->SetParent(nullptr);

    if (_renderer)
    {
        SceneRenderTask::RemoveGlobalCustomPostFx(_renderer);
    }
}

void UIXCanvas::EndPlay()
{
    if (_isRegisteredForTick)
    {
        _isRegisteredForTick = false;
        //Scripting.Update -= OnUpdate;
        GetScene()->Ticking.Update.RemoveTick(this);
    }

    if (_renderer)
    {
        SceneRenderTask::RemoveGlobalCustomPostFx(_renderer);
        _renderer->Canvas = nullptr;

        // TODO: wait for the crash.
        _renderer->DeleteObject();
        //Destroy(_renderer);

        _renderer = nullptr;
    }
}

bool UIXCanvas::IsVisible() const
{
    LayersMask lm = MainRenderTask::Instance != nullptr ? MainRenderTask::Instance->View.RenderLayersMask : LayersMask();
    return IsVisible(lm);
}

bool UIXCanvas::IsVisible(LayersMask layersMask) const
{
//#if FLAX_EDITOR
#if 0
    if (_editorTask != nullptr || _editorRoot != nullptr)
        return true;
#endif
    return layersMask.HasLayer(GetLayer());
}

