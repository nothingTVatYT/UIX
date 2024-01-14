#pragma once

#include "Engine/Scripting/Script.h"
#include "UIXFontReference.h"
#include "Engine/Core/Math/Color.h"
#include "Engine/Render2D/SpriteAtlas.h"
#include "Engine/Core/Config.h"

class Font;
class UIXTooltip;

API_CLASS(NoSpawn) class UIXCPP_API UIXStyle
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIXStyle);
public:
    /// <summary>
    /// Style for the Statusbar
    /// </summary>
    API_STRUCT(Attributes = "System.Serializable, ShowInEditor") struct StatusbarStyle
    {
        DECLARE_SCRIPTING_TYPE_MINIMAL(StatusbarStyle)

        /// <summary>
        /// Color of the Statusbar when in Play Mode
        /// </summary>
        API_FIELD()
        Color PlayMode;

        /// <summary>
        /// Color of the Statusbar when in loading state (e.g. when importing assets)
        /// </summary>
        API_FIELD()
        Color Loading;

        /// <summary>
        /// Color of the Statusbar in its failed state (e.g. with compilation errors)
        /// </summary>
        API_FIELD()
        Color Failed;
    };

    /// <summary>
    /// Global GUI style used by all the controls.
    /// </summary>
    API_PROPERTY()
    static const UIXStyle* GetCurrent();

    /// <summary>
    /// Global GUI style used by all the controls.
    /// </summary>
    API_PROPERTY()
    static void SetCurrent(const UIXStyle *value);

    /// <summary>
    /// Gets the title font.
    /// </summary>
    API_PROPERTY(Attributes="NoSerialize, EditorOrder(10)")
    Font* GetFontTitle() const;
    /// <summary>
    /// Sets the title font.
    /// </summary>
    API_PROPERTY()
    void SetFontTitle(Font *value);

    /// <summary>
    /// Gets the large font.
    /// </summary>
    API_PROPERTY(Attributes = "NoSerialize, EditorOrder(20)")
    Font* GetFontLarge() const;
    /// <summary>
    /// Sets the large font.
    /// </summary>
    API_PROPERTY()
    void SetFontLarge(Font *value);

    /// <summary>
    /// Gets the medium font.
    /// </summary>
    API_PROPERTY(Attributes = "NoSerialize, EditorOrder(30)")
    Font* GetFontMedium() const;
    /// <summary>
    /// Sets the medium font.
    /// </summary>
    API_PROPERTY()
    void SetFontMedium(Font *value);

    /// <summary>
    /// Gets the small font.
    /// </summary>
    API_PROPERTY(Attributes = "NoSerialize, EditorOrder(40)")
    Font* GetFontSmall() const;
    /// <summary>
    /// Sets the small font.
    /// </summary>
    API_PROPERTY()
    void SetFontSmall(Font *value);

    /// <summary>
    /// The background color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(60)")
    Color Background;

    /// <summary>
    /// The light background color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(70)")
    Color LightBackground;

    /// <summary>
    /// The drag window color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(80)")
    Color DragWindow;

    /// <summary>
    /// The foreground color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(90)")
    Color Foreground;

    /// <summary>
    /// The foreground grey.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(100)")
    Color ForegroundGrey;

    /// <summary>
    /// The foreground disabled.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(110)")
    Color ForegroundDisabled;

    /// <summary>
    /// The foreground color in viewports (usually have a dark background)
    /// </summary>
    API_FIELD(Attributes="EditorOrder(115)")
    Color ForegroundViewport;

    /// <summary>
    /// The background highlighted color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(120)")
    Color BackgroundHighlighted;

    /// <summary>
    /// The border highlighted color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(130)")
    Color BorderHighlighted;

    /// <summary>
    /// The background selected color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(140)")
    Color BackgroundSelected;

    /// <summary>
    /// The border selected color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(150)")
    Color BorderSelected;

    /// <summary>
    /// The background normal color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(160)")
    Color BackgroundNormal;

    /// <summary>
    /// The border normal color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(170)")
    Color BorderNormal;

    /// <summary>
    /// The text box background color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(180)")
    Color TextBoxBackground;

    /// <summary>
    /// The text box background selected color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(190)")
    Color TextBoxBackgroundSelected;

    /// <summary>
    /// The collection background color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(195)")
    Color CollectionBackgroundColor;

    /// <summary>
    /// The progress normal color.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(200)")
    Color ProgressNormal;


    /// <summary>
    /// The status bar style
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(210)")
    StatusbarStyle Statusbar;

    /// <summary>
    /// The arrow right icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(220)")
    SpriteHandle ArrowRight;

    /// <summary>
    /// The arrow down icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(230)")
    SpriteHandle ArrowDown;

    /// <summary>
    /// The search icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(240)")
    SpriteHandle Search;

    /// <summary>
    /// The settings icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(250)")
    SpriteHandle Settings;

    /// <summary>
    /// The cross icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(260)")
    SpriteHandle Cross;

    /// <summary>
    /// The CheckBox intermediate icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(270)")
    SpriteHandle CheckBoxIntermediate;

    /// <summary>
    /// The CheckBox tick icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(280)")
    SpriteHandle CheckBoxTick;

    /// <summary>
    /// The status bar size grip icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(290)")
    SpriteHandle StatusBarSizeGrip;

    /// <summary>
    /// The translate icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(300)")
    SpriteHandle Translate;

    /// <summary>
    /// The rotate icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(310)")
    SpriteHandle Rotate;

    /// <summary>
    /// The scale icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(320)")
    SpriteHandle Scale;

    /// <summary>
    /// The scalar icon.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(330)")
    SpriteHandle Scalar;

    /// <summary>
    /// The shared tooltip control used by the controls if no custom tooltip is provided.
    /// </summary>
    API_FIELD(Attributes = "EditorOrder(340)")
    UIXTooltip* SharedTooltip;

private:
    static UIXStyle *_current;

    API_FIELD(Attributes="Serialize")
    UIXFontReference _fontTitle;

    API_FIELD(Attributes = "Serialize")
    UIXFontReference _fontLarge;

    API_FIELD(Attributes = "Serialize")
    UIXFontReference _fontMedium;

    API_FIELD(Attributes = "Serialize")
    UIXFontReference _fontSmall;
};


