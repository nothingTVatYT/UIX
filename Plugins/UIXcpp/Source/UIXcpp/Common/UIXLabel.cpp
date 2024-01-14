#include "UIXLabel.h"
#include "../UIXStyle.h"
#include "Engine/Render2D/Render2D.h"
#include "Engine/Render2D/Font.h"


UIXLabel::UIXLabel()
    : UIXContainerControl(0, 0, 100, 20)
{
    SetAutoFocus(false);
    auto style = UIXStyle::GetCurrent();
    SetFont(UIXFontReference(style->GetFontMedium()));
    SetTextColor(style->Foreground);
    SetTextColorHighlighted(style->Foreground);
}

/// <inheritdoc />
UIXLabel::UIXLabel(float x, float y, float width, float height)
    : UIXContainerControl(x, y, width, height)
{
    SetAutoFocus(false);
    auto style = UIXStyle::GetCurrent();
    SetFont(UIXFontReference(style->GetFontMedium()));
    SetTextColor(style->Foreground);
    SetTextColorHighlighted(style->Foreground);
}


void UIXLabel::SetText(const LocalizedString &value)
{
    if (_text != value)
    {
        _text = value;
        _textSize = Float2::Zero;
        PerformLayout();
    }
}
    
void UIXLabel::SetFont(UIXFontReference value)
{
    if (_font != value)
    {
        _font = value;

        if (_autoWidth || _autoHeight || _autoFitText)
        {
            _textSize = Float2::Zero;
            PerformLayout();
        }
    }
}

void UIXLabel::SetAutoWidth(bool value)
{
    if (_autoWidth != value)
    {
        _autoWidth = value;
        PerformLayout();
    }
}

void UIXLabel::SetAutoHeight(bool value)
{
    if (_autoHeight != value)
    {
        _autoHeight = value;
        PerformLayout();
    }
}

void UIXLabel::SetAutoFitText(bool value)
{
    if (_autoFitText != value)
    {
        _autoFitText = value;
        PerformLayout();
    }
}

void UIXLabel::DrawSelf()
{
    UIXContainerControl::DrawSelf();

    Rectangle rect = Rectangle(Float2(GetMargin().Left, GetMargin().Top), GetSize() - GetMargin().GetSize());

    if (GetClipText())
        Render2D::PushClip(Rectangle(Float2::Zero, GetSize()));

    Color color = GetIsMouseOver() || GetIsNavFocused() ? GetTextColorHighlighted() : GetTextColor();

    if (!GetEnabledInHierarchy())
        color *= 0.6f;

    float scale = 1.0f;
    TextAlignment hAlignment = GetHorizontalAlignment();
    TextAlignment vAlignment = GetVerticalAlignment();
    if (_autoFitText)
    {
        if (!_textSize.IsZero())
        {
            scale = (rect.Size / _textSize).MinValue();
            scale = Math::Clamp(scale, _autoFitTextRange.X, _autoFitTextRange.Y);
        }
    }

    TextLayoutOptions layout;
    layout.HorizontalAlignment = hAlignment;
    layout.VerticalAlignment = vAlignment;
    layout.TextWrapping = GetWrapping();
    layout.BaseLinesGapScale = GetBaseLinesGapScale();
    layout.Scale = scale;
    layout.Bounds = rect;
    Render2D::DrawText(_font.GetFont(), _text.ToString(), color, layout, GetMaterial());

    if (GetClipText())
        Render2D::PopClip();
}

void UIXLabel::PerformLayoutBeforeChildren()
{
    if (_autoWidth || _autoHeight || _autoFitText)
    {
        Font *font = _font.GetFont();
        if (font != nullptr)
        {
            // Calculate text size
            TextLayoutOptions layout;
            layout.TextWrapping = GetWrapping();
            if (_autoHeight && !_autoWidth)
                layout.Bounds.Size.X = GetWidth() - GetMargin().GetWidth();
            else if (_autoWidth && !_autoHeight)
                layout.Bounds.Size.Y = GetHeight() - GetMargin().GetHeight();
            _textSize = font->MeasureText(_text.ToString(), layout);
            _textSize.Y *= GetBaseLinesGapScale();

            // Check if size is controlled via text
            if (_autoWidth || _autoHeight)
            {
                Float2 size = GetSize();
                if (_autoWidth)
                    size.X = _textSize.X + GetMargin().GetWidth();
                if (_autoHeight)
                    size.Y = _textSize.Y + GetMargin().GetHeight();
                SetSize(size);
            }
        }
    }

    UIXContainerControl::PerformLayoutBeforeChildren();
}
