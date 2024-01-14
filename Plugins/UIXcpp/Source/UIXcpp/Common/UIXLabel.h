#pragma once

#include "../UIXContainerControl.h"
#include "../UIXFontReference.h"
#include "Engine/Localization//LocalizedString.h"
#include "Engine/Render2D/TextLayoutOptions.h"
#include "Engine/Content/Assets/MaterialBase.h"

/// <summary>
/// The basic GUI label control.
/// </summary>
/// <seealso cref="FlaxEngine.GUI.ContainerControl" />
API_CLASS(NoSpawn) class UIXCPP_API UIXLabel : public UIXContainerControl
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIXLabel)
public:
    /// <summary>
    /// Initializes a new instance of the <see cref=\"Label\"/> class.
    /// </summary>
    UIXLabel();

    /// <inheritdoc />
    UIXLabel(float x, float y, float width, float height);

    /// <summary>
    /// Gets the text.
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(10), MultilineText, Tooltip(\"The label text.\")")
    FORCE_INLINE LocalizedString GetText() const { return _text;}

    /// <summary>
    /// Sets the text.
    /// </summary>
    API_PROPERTY()
    void SetText(const LocalizedString &value);

    /// <summary>
    /// Gets the color of the text.
    /// </summary>
    API_PROPERTY(Attributes="EditorDisplay(\"Text Style\"), EditorOrder(2010), Tooltip(\"The color of the text.\"), ExpandGroups")
    FORCE_INLINE Color GetTextColor() const { return _textColor; }

    /// <summary>
    /// Sets the color of the text.
    /// </summary>
    API_PROPERTY()
    void SetTextColor(Color value) { _textColor = value; }

    /// <summary>
    /// Gets or sets the color of the text when it is highlighted (mouse is over).
    /// </summary>
    API_PROPERTY(Attributes="EditorDisplay(\"Text Style\"), EditorOrder(2011), Tooltip(\"The color of the text when it is highlighted(mouse is over).\")")
    FORCE_INLINE Color GetTextColorHighlighted() const { return _textColorHighlighted; }

    /// <summary>
    /// Gets or sets the color of the text when it is highlighted (mouse is over).
    /// </summary>
    API_PROPERTY()
    void SetTextColorHighlighted(Color value) { _textColorHighlighted = value; }

    /// <summary>
    /// Gets or sets the horizontal text alignment within the control bounds.
    /// </summary>
    API_PROPERTY(Attributes = "EditorDisplay(\"Text Style\"), EditorOrder(2020), Tooltip(\"The horizontal text alignment within the control bounds.\")")
    FORCE_INLINE TextAlignment GetHorizontalAlignment() const { return _horizontalAlignment; }

    /// <summary>
    /// Gets or sets the horizontal text alignment within the control bounds.
    /// </summary>
    API_PROPERTY()
    void SetHorizontalAlignment(TextAlignment value) { _horizontalAlignment = value; }

    /// <summary>
    /// Gets or sets the vertical text alignment within the control bounds.
    /// </summary>
    API_PROPERTY(Attributes = "EditorDisplay(\"Text Style\"), EditorOrder(2021), Tooltip(\"The vertical text alignment within the control bounds.\")")
    FORCE_INLINE TextAlignment GetVerticalAlignment() const { return _verticalAlignment; }

    /// <summary>
    /// Gets or sets the vertical text alignment within the control bounds.
    /// </summary>
    API_PROPERTY()
    void SetVerticalAlignment(TextAlignment value) { _verticalAlignment = value; }

    /// <summary>
    /// Gets or sets the text wrapping within the control bounds.
    /// </summary>
    API_PROPERTY(Attributes = "EditorDisplay(\"Text Style\"), EditorOrder(2022), Tooltip(\"The text wrapping within the control bounds.\")")
    FORCE_INLINE TextWrapping GetWrapping() const { return _wrapping; }

    /// <summary>
    /// Gets or sets the text wrapping within the control bounds.
    /// </summary>
    API_PROPERTY()
    void SetWrapping(TextWrapping value) { _wrapping = value; }

    /// <summary>
    /// Gets or sets the text wrapping within the control bounds.
    /// </summary>
    API_PROPERTY(Attributes = "EditorDisplay(\"Text Style\"), EditorOrder(2023), Tooltip(\"The gap between lines when wrapping and more than a single line is displayed.\"), Limit(0f)")
    FORCE_INLINE float GetBaseLinesGapScale() const { return _baseLinesGapScale; }

    /// <summary>
    /// Gets or sets the text wrapping within the control bounds.
    /// </summary>
    API_PROPERTY()
    void SetBaseLinesGapScale(float value) { _baseLinesGapScale = value; }

    /// <summary>
    /// Gets or sets the font.
    /// </summary>
    API_PROPERTY(Attributes = "EditorDisplay(\"Text Style\"), EditorOrder(2024)")
    FORCE_INLINE UIXFontReference GetFont() const { return _font; }

    /// <summary>
    /// Gets or sets the font.
    /// </summary>
    API_PROPERTY()
    void SetFont(UIXFontReference value);

    /// <summary>
    /// Gets or sets the custom material used to render the text. It must has domain set to GUI and have a public texture parameter named Font used to sample font atlas texture with font characters data.
    /// </summary>
    API_PROPERTY(Attributes = "EditorDisplay(\"Text Style\"), EditorOrder(2025)")
    FORCE_INLINE AssetReference<MaterialBase> GetMaterial() const { return _material; }

    /// <summary>
    /// Gets or sets the custom material used to render the text. It must has domain set to GUI and have a public texture parameter named Font used to sample font atlas texture with font characters data.
    /// </summary>
    API_PROPERTY()
    void SetMaterial(AssetReference<MaterialBase> value) { _material = value; }

    /// <summary>
    /// Gets or sets the margin for the text within the control bounds.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(70), Tooltip(\"The margin for the text within the control bounds.\")")
    FORCE_INLINE UIXMargin GetMargin() const { return _margin; }

    /// <summary>
    /// Gets or sets the margin for the text within the control bounds.
    /// </summary>
    API_PROPERTY()
    void SetMargin(UIXMargin value) { _margin = value; }

    /// <summary>
    /// Gets or sets a value indicating whether clip text during rendering.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(80), DefaultValue(false), Tooltip(\"If checked, text will be clipped during rendering.\")")
    FORCE_INLINE bool GetClipText() const { return _clipText; }

    /// <summary>
    /// Gets or sets a value indicating whether clip text during rendering.
    /// </summary>
    API_PROPERTY()
    void SetClipText(bool value) { _clipText = value; }

    /// <summary>
    /// Gets or sets a value indicating whether set automatic width based on text contents.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(85), DefaultValue(false), Tooltip(\"If checked, the control width will be based on text contents.\")")
    FORCE_INLINE bool GetAutoWidth() const { return _autoWidth; }

    /// <summary>
    /// Gets or sets a value indicating whether set automatic width based on text contents.
    /// </summary>
    API_PROPERTY()
    void SetAutoWidth(bool value);

    /// <summary>
    /// Gets or sets a value indicating whether set automatic height based on text contents.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(90), DefaultValue(false), Tooltip(\"If checked, the control height will be based on text contents.\")")
    FORCE_INLINE bool GetAutoHeight() const { return _autoHeight; }

    /// <summary>
    /// Gets or sets a value indicating whether set automatic height based on text contents.
    /// </summary>
    API_PROPERTY()
    void SetAutoHeight(bool value);

    /// <summary>
    /// Gets or sets a value indicating whether scale text to fit the label control bounds. Disables using text alignment, automatic width and height.
    /// </summary>
    API_PROPERTY(Attributes = "EditorOrder(100), DefaultValue(false), Tooltip(\"If checked, enables scaling text to fit the label control bounds. Disables using text alignment, automatic width and height.\")")
    FORCE_INLINE bool GetAutoFitText() const
    {
        return _autoFitText;
    }

    /// <summary>
    /// Gets or sets a value indicating whether scale text to fit the label control bounds. Disables using text alignment, automatic width and height.
    /// </summary>
    API_PROPERTY()
    void SetAutoFitText(bool value);

    /// <summary>
    /// Gets or sets the text scale range (min and max) for automatic fit text option. Can be used to constraint the text scale adjustment.
    /// </summary>
    API_PROPERTY(Attributes = "VisibleIf(nameof(AutoFitText)), EditorOrder(110), DefaultValue(typeof(Float2), \"0.1, 100\"), Tooltip(\"The text scale range (min and max) for automatic fit text option. Can be used to constraint the text scale adjustment.\")")
    FORCE_INLINE Float2 GetAutoFitTextRange() const { return _autoFitTextRange; }

    /// <summary>
    /// Gets or sets the text scale range (min and max) for automatic fit text option. Can be used to constraint the text scale adjustment.
    /// </summary>
    API_PROPERTY()
    void SetAutoFitTextRange(Float2 value) { _autoFitTextRange = value; }

    /// <inheritdoc />
    void DrawSelf() override;

protected:
    /// <inheritdoc />
    void PerformLayoutBeforeChildren() override;

    /// <summary>
    /// The text.
    /// </summary>
    LocalizedString _text;

    /// <summary>
    /// The font.
    /// </summary>
    UIXFontReference _font;
private:
    bool _autoWidth = false;
    bool _autoHeight = false;
    bool _autoFitText = false;
    Float2 _textSize;
    Float2 _autoFitTextRange = Float2(0.1f, 100.0f);

    Color _textColor;
    Color _textColorHighlighted;
    TextAlignment _horizontalAlignment;
    TextAlignment _verticalAlignment;
    TextWrapping _wrapping;
    float _baseLinesGapScale = 0.0f;
    AssetReference<MaterialBase> _material;
    UIXMargin _margin;
    bool _clipText = false;
};

