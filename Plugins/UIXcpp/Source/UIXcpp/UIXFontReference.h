#pragma once

#include "Engine/Scripting/Script.h"
#include "Engine/Content/AssetReference.h"
#include "Engine/Render2D/FontAsset.h"
#include "keep_alive.h"

class Font;

API_CLASS(NoSpawn) class UIXCPP_API UIXFontReference : public ScriptingObject
{
public:
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIXFontReference)

    /// <summary>
    /// Initializes a new instance of the <see cref="FontReference"/> struct.
    /// </summary>
    UIXFontReference();

    /// <summary>
    /// Initializes a new instance of the <see cref="FontReference"/> struct.
    /// </summary>
    /// <param name="font">The font.</param>
    /// <param name="size">The font size.</param>
    UIXFontReference(AssetReference<FontAsset> font, float size);

    /// <summary>
    /// Initializes a new instance of the <see cref="FontReference"/> struct.
    /// </summary>
    /// <param name="other">The other font reference.</param>
    UIXFontReference(const UIXFontReference &other);

    /// <summary>
    /// Initializes a new instance of the <see cref="FontReference"/> struct.
    /// </summary>
    /// <param name="font">The font.</param>
    UIXFontReference(Font *font);

    /// <summary>
    /// The font asset.
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(0), Tooltip(\"The font asset to use as characters source.\")")
    FORCE_INLINE AssetReference<FontAsset> GetFontAsset() const { return _font; }

    /// <summary>
    /// The font asset.
    /// </summary>
    API_PROPERTY()
    void SetFontAsset(AssetReference<FontAsset> value);
        
    /// <summary>
    /// The size of the font characters.
    /// </summary>
    API_PROPERTY(Attributes="EditorOrder(10), Limit(1, 500, 0.1f), Tooltip(\"The size of the font characters.\")")
    FORCE_INLINE float GetSize() const { return _size; }

    /// <summary>
    /// The size of the font characters.
    /// </summary>
    API_PROPERTY()
    void SetSize(float value);

    /// <summary>
    /// Gets the font object described by the structure.
    /// </summary>
    /// <returns>The font or nullptr if descriptor is invalid.</returns>
    API_FUNCTION()
    Font* GetFont() const;

    /// <summary>
    /// Gets the bold font object described by the structure.
    /// </summary>
    /// <returns>The bold font asset.</returns>
    API_FUNCTION()
    UIXFontReference GetBold() const;

    /// <summary>
    /// Gets the italic font object described by the structure.
    /// </summary>
    /// <returns>The bold font asset.</returns>
    API_FUNCTION()
    UIXFontReference GetItalic() const;

    /// <summary>
    /// Determines whether the specified <see cref="FontReference" /> is equal to this instance.
    /// </summary>
    /// <param name="other">The <see cref="FontReference" /> to compare with this instance.</param>
    /// <returns><c>true</c> if the specified <see cref="FontReference" /> is equal to this instance; otherwise, <c>false</c>.</returns>
    API_FUNCTION(Attributes="MethodImpl(MethodImplOptions.AggressiveInlining)")
    bool Equals(const UIXFontReference &other) const;

    /// <summary>
    /// Compares two font references.
    /// </summary>
    /// <param name="lhs">The left.</param>
    /// <param name="rhs">The right.</param>
    /// <returns>True if font references are equal, otherwise false.</returns>
    bool operator ==(const UIXFontReference &rhs);

    /// <summary>
    /// Compares two font references.
    /// </summary>
    /// <param name="lhs">The left.</param>
    /// <param name="rhs">The right.</param>
    /// <returns>True if font references are not equal, otherwise false.</returns>
    bool operator !=(const UIXFontReference &rhs);

    // TODO: create a c# object that implements this if needed
    ///// <inheritdoc />
    //bool Equals(object other) override
    //{
    //    if (!(other is FontReference))
    //        return false;
    //    var fontReference = (FontReference)other;
    //    return Equals(fontReference);
    //}

    /// <inheritdoc />
    API_FUNCTION()
    int GetHashCode();

    /// <inheritdoc />
    API_FUNCTION()
    String ToString();

private:

    API_FIELD(Attributes="NoSerialize")
    AssetReference<FontAsset> _font;

    API_FIELD(Attributes="NoSerialize")
    float _size;

    API_FIELD(Attributes="NoSerialize")
    mutable ShareAlive<Font> _cachedFont;
};

