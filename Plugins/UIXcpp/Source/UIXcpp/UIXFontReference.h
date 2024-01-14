#pragma once

#include "Engine/Scripting/Script.h"
#include "Engine/Content/AssetReference.h"
#include "Engine/Render2D/FontAsset.h"

class Font;

API_STRUCT() struct UIXCPP_API UIXFontReference
{
    DECLARE_SCRIPTING_TYPE_MINIMAL(UIXFontReference)

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
    AssetReference<FontAsset> GetFontAsset() const { return _font; }

    /// <summary>
    /// The font asset.
    /// </summary>
    void SetFontAsset(AssetReference<FontAsset> value);
        
    /// <summary>
    /// The size of the font characters.
    /// </summary>
    float GetSize() const { return _size; }

    /// <summary>
    /// The size of the font characters.
    /// </summary>
    void SetSize(float value);

    /// <summary>
    /// Gets the font object described by the structure.
    /// </summary>
    /// <returns>The font or nullptr if descriptor is invalid.</returns>
    Font* GetFont() const;

    /// <summary>
    /// Gets the bold font object described by the structure.
    /// </summary>
    /// <returns>The bold font asset.</returns>
    UIXFontReference GetBold() const;

    /// <summary>
    /// Gets the italic font object described by the structure.
    /// </summary>
    /// <returns>The bold font asset.</returns>
    UIXFontReference GetItalic() const;

    /// <summary>
    /// Determines whether the specified <see cref="FontReference" /> is equal to this instance.
    /// </summary>
    /// <param name="other">The <see cref="FontReference" /> to compare with this instance.</param>
    /// <returns><c>true</c> if the specified <see cref="FontReference" /> is equal to this instance; otherwise, <c>false</c>.</returns>
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
    int GetHashCode();

    /// <inheritdoc />
    String ToString();

//private:

    API_FIELD(Internal, Attributes="NoSerialize")
    AssetReference<FontAsset> _font;
    API_FIELD(Internal, Attributes = "NoSerialize")
    float _size = 0.0f;
    API_FIELD(Internal, Attributes = "NoSerialize")
    mutable Font *_cachedFont = nullptr;

};

