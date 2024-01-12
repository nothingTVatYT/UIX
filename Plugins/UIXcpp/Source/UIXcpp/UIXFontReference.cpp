#include "UIXFontReference.h"
#include "Engine/Render2D/Font.h"
#include "Engine/Core/Types/Variant.h"

UIXFontReference::UIXFontReference()
{
    _font = nullptr;
    _size = 30;
}

/// <summary>
/// Initializes a new instance of the <see cref="FontReference"/> struct.
/// </summary>
/// <param name="font">The font.</param>
/// <param name="size">The font size.</param>
UIXFontReference::UIXFontReference(AssetReference<FontAsset> font, float size)
{
    _font = font;
    _size = size;
}

/// <summary>
/// Initializes a new instance of the <see cref="FontReference"/> struct.
/// </summary>
/// <param name="other">The other font reference.</param>
UIXFontReference::UIXFontReference(const UIXFontReference &other)
{
    _font = other._font;
    _size = other._size;
    _cachedFont = other._cachedFont;
}

/// <summary>
/// Initializes a new instance of the <see cref="FontReference"/> struct.
/// </summary>
/// <param name="font">The font.</param>
UIXFontReference::UIXFontReference(Font *font)
{
    if (font != nullptr)
    {
        _font = font->GetAsset();
        _size = font->GetSize();
    }
    else
    {
        _font = nullptr;
        _size = 30;
    }
    _cachedFont = font;
}

void UIXFontReference::SetFontAsset(AssetReference<FontAsset> value)
{
    if (_font != value)
    {
        _font = value;
        _cachedFont = ShareAlive<Font>();
    }
}
  
void UIXFontReference::SetSize(float value)
{
    if (_size != value)
    {
        _size = value;
        _cachedFont = nullptr;
    }
}
        
Font* UIXFontReference::GetFont() const
{
    if (_cachedFont != nullptr)
        return _cachedFont;
    if (_font)
        _cachedFont = _font->CreateFont(_size);
    return _cachedFont;
}


UIXFontReference UIXFontReference::GetBold() const
{
    return UIXFontReference(_font != nullptr ? _font->GetBold() : nullptr, _size);
}

UIXFontReference UIXFontReference::GetItalic() const
{
    return UIXFontReference(_font != nullptr ? _font->GetItalic() : nullptr, _size);
}

bool UIXFontReference::Equals(const UIXFontReference &other) const
{
    return _font == other._font && _size == other._size;
}

bool UIXFontReference::operator==(const UIXFontReference &lhs, const UIXFontReference &rhs)
{
    return lhs.Equals(rhs);
}

/// <summary>
/// Compares two font references.
/// </summary>
/// <param name="lhs">The left.</param>
/// <param name="rhs">The right.</param>
/// <returns>True if font references are not equal, otherwise false.</returns>
API_FUNCTION()
bool UIXFontReference::operator!=(const UIXFontReference &lhs, const UIXFontReference &rhs)
{
    return !lhs.Equals(rhs);
}

int UIXFontReference::GetHashCode()
{
    int hashCode = _font != nullptr ? GetHash(_font) : 0;
    hashCode = (hashCode * 397) ^ GetHash(_size);
    return hashCode;
}
