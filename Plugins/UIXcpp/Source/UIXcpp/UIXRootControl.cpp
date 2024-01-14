#include "UIXRootControl.h"
#include "UIXCanvasContainer.h"

UIXContainerControl *UIXRootControl::_gameRoot = nullptr;
UIXCanvasContainer *UIXRootControl::_canvasContainer = New<UIXCanvasContainer>();

UIXRootControl::UIXRootControl() : UIXContainerControl(0, 0, 100, 60)//, UpdateCallbacks(1024)
{
    SetAutoFocus(false);
}

void UIXRootControl::Navigate(UIXNavDirection direction)
{
    if (direction == UIXNavDirection::None)
        return;

    if (CustomNavigation.IsBinded())
    {
        // Custom
        CustomNavigation(direction);
        return;
    }

    UIXControl *focused = GetFocusedControl();
    if (focused == nullptr)
    {
        // Nothing is focused so go to the first control
        Array<UIXControl*> tmp;
        focused = OnNavigate(direction, Float2::Zero, this, tmp);
        if (focused != nullptr)
            focused->NavigationFocus();
        return;
    }

    UIXControl *target = focused->GetNavTarget(direction);
    if (target != nullptr)
    {
        // Explicitly specified focus target
        target->NavigationFocus();
        return;
    }

    // Automatic navigation routine
    Array<UIXControl*> tmp;
    target = focused->OnNavigate(direction, focused->GetNavOrigin(direction), this, tmp);
    if (target != nullptr)
        target->NavigationFocus();
}

void UIXRootControl::SubmitFocused()
{
    if (GetFocusedControl() != nullptr)
        GetFocusedControl()->OnSubmit();
}


void UIXRootControl::Update(float deltaTime)
{
    UIXContainerControl::Update(deltaTime);

    // Flush requests
    //ProfilerCPU::BeginEvent("RootControl.SyncCallbacks");

    Array<Function<void(float)>> rdelegates;
    rdelegates.Resize(UpdateCallbacksToRemove.Count());
    UpdateCallbacksToRemove.GetBindings(rdelegates.Get(), rdelegates.Count());


    Array<Function<void(float)>> adelegates;
    adelegates.Resize(UpdateCallbacksToAdd.Count());
    UpdateCallbacksToAdd.GetBindings(adelegates.Get(), adelegates.Count());

    for (int i = 0; i < adelegates.Count(); i++)
    {
        UpdateCallbacks.Bind(adelegates[i]);
    }
    UpdateCallbacksToAdd.UnbindAll();
    for (int i = 0; i < rdelegates.Count(); i++)
    {
        UpdateCallbacks.Unbind(rdelegates[i]);
    }
    UpdateCallbacksToRemove.UnbindAll();

    //ProfilerCPU.EndEvent();

    // Perform the UI update
    try
    {
        ////ProfilerCPU.BeginEvent("RootControl.Update");
        //for (int i = 0; i < UpdateCallbacks.Count(); i++)
        //{
        //    UpdateCallbacks[i](deltaTime);
        //}
        UpdateCallbacks(deltaTime);
    }
    catch (...)
    {
        //ProfilerCPU.EndEvent();
        return;
    }
    //ProfilerCPU.EndEvent();
}


/*static*/ void UIXRootControl::SetGameRoot(UIXContainerControl *value)
{
    _gameRoot = value;
    _canvasContainer->SetParent(_gameRoot);
}

/*static*/ UIXCanvasContainer* UIXRootControl::GetCanvasRoot()
{
    return _canvasContainer;
}