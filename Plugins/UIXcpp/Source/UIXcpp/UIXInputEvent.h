#pragma once

#include "Engine/Scripting/ScriptingObject.h"
#include "Engine/Input/Enums.h"


/// <summary>
/// Virtual input action binding. Helps with listening for a selected input event.
/// </summary>
API_CLASS(NoSpawn)
class UIXCPP_API UIXInputEvent : public ScriptingObject
{
public:

    /// <summary>
    /// Initializes a new instance of the <see cref="InputEvent"/> class.
    /// </summary>
    UIXInputEvent();

    /// <summary>
    /// Initializes a new instance of the <see cref="InputEvent"/> class.
    /// </summary>
    /// <param name="name">The action name.</param>
    UIXInputEvent(String name);

    /// <summary>
    /// Finalizes an instance of the <see cref="InputEvent"/> class.
    /// </summary>
    ~UIXInputEvent();

    /// <summary>
    /// The name of the action to use. See <see cref="Input.ActionMappings"/>.
    /// </summary>
    API_FIELD(Attributes="Tooltip(\"The name of the action to use.\")")
    String Name;

    /// <summary>
    /// Returns true if the event has been triggered during the current frame (e.g. user pressed a key). Use <see cref="Pressed"/> to catch events without active waiting.
    /// </summary>
    API_PROPERTY()
    bool Active();

    /// <summary>
    /// Returns the event state. Use Use <see cref="Pressed"/>, <see cref="Pressing"/>, <see cref="Released"/> to catch events without active waiting.
    /// </summary>
    API_PROPERTY()
    InputActionState GetState() const;

    /// <summary>
    /// Occurs when event is pressed (e.g. user pressed a key). Called before scripts update.
    /// </summary>
    API_EVENT() Action Pressed;

    /// <summary>
    /// Occurs when event is being pressing (e.g. user pressing a key). Called before scripts update.
    /// </summary>
    API_EVENT() Action Pressing;

    /// <summary>
    /// Occurs when event is released (e.g. user releases a key). Called before scripts update.
    /// </summary>
    API_EVENT() Action Released;

    /// <summary>
    /// Releases this object.
    /// </summary>
    void Dispose();

private:
    void Handler(StringView name, InputActionState state);
    bool bound;
};

/*
 
/// <summary>
/// Virtual input action binding. Helps with listening for a selected input event.
/// </summary>
public class InputEvent
{


    ///// <summary>
    ///// Occurs when event is triggered (e.g. user pressed a key). Called before scripts update.
    ///// </summary>
    //[System.Obsolete("Depreciated in 1.7, use Pressed Action.")]
    //    public event Action Triggered;

}

*/