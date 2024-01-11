#pragma once

#include "Engine/Scripting/Script.h"
#include "Engine/Core/Math/Math.h"
#include "Engine/Localization/CultureInfo.h"
//#include "Engine/Localization/Localization.h"



struct UIXMargin
{
    static const int SizeInBytes;
    static const UIXMargin Zero;
    
    /// <summary>
    /// Holds the margin to the left.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(0)") float Left;

    /// <summary>
    /// Holds the margin to the right.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(1)") float Right;

    /// <summary>
    /// Holds the margin to the top.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(2)") float Top;

    /// <summary>
    /// Holds the margin to the bottom.
    /// </summary>
    API_FIELD(Attributes="EditorOrder(3)") float Bottom;

    /// <summary>
    /// Gets the margin's location (Left, Top).
    /// </summary>
    API_PROPERTY() Float2 GetLocation() const { return { Left, Top }; }

    /// <summary>
    /// Gets the margin's total size. Cumulative margin size (Left + Right, Top + Bottom).
    /// </summary>
    API_PROPERTY() Float2 GetSize() const { return { Left + Right, Top + Bottom }; }

    /// <summary>
    /// Gets the width (left + right).
    /// </summary>
    API_PROPERTY() float GetWidth() const { return Left + Right; }

    /// <summary>
    /// Gets the height (top + bottom).
    /// </summary>
    API_PROPERTY() float GetHeight() const { return Top + Bottom; }

    UIXMargin() : UIXMargin(0.0f) {}

    /// <summary>
    /// Initializes a new instance of the <see cref="UIXMargin"/> struct.
    /// </summary>
    /// <param name="value">The value.</param>
    UIXMargin(float value) : Left(value), Right(value), Top(value), Bottom(value)
    {
    }


    /// <summary>
    /// Initializes a new instance of the <see cref="UIXMargin"/> struct.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <param name="top">The top.</param>
    /// <param name="bottom">The bottom.</param>
    UIXMargin(float left, float right, float top, float bottom) : Left(left), Right(right), Top(top), Bottom(bottom)
    {
    }

    /// <summary>
    /// Gets a value indicting whether this margin is zero.
    /// </summary>
    API_FUNCTION() bool IsZero() const;

    /// <summary>
    /// Shrinks the rectangle by this margin.
    /// </summary>
    /// <param name="rect">The rectangle.</param>
    API_FUNCTION() void ShrinkRectangle(API_PARAM(Ref) Rectangle &rect) const;

    /// <summary>
    /// Expands the rectangle by this margin.
    /// </summary>
    /// <param name="rect">The rectangle.</param>
    API_FUNCTION() void ExpandRectangle(API_PARAM(Ref) Rectangle &rect) const;

    /// <summary>
    /// Adds two margins.
    /// </summary>
    /// <param name="left">The first margins to add.</param>
    /// <param name="right">The second margins to add.</param>
    /// <returns>The sum of the two margins.</returns>
    API_FUNCTION() UIXMargin operator+(const UIXMargin &right) const
    {
        return UIXMargin(Left + right.Left, Right + right.Right, Top + right.Top, Bottom + right.Bottom);
    }

    /// <summary>
    /// Subtracts two margins.
    /// </summary>
    /// <param name="left">The first margins to subtract from.</param>
    /// <param name="right">The second margins to subtract.</param>
    /// <returns>The result of subtraction of the two margins.</returns>
    API_FUNCTION() UIXMargin operator+(const UIXMargin &right) const
    {
        return UIXMargin(Left - right.Left, Right - right.Right, Top - right.Top, Bottom - right.Bottom);
    }

    /// <summary>
    /// Tests for equality between two objects.
    /// </summary>
    /// <param name="left">The first value to compare.</param>
    /// <param name="right">The second value to compare.</param>
    /// <returns><c>true</c> if <paramref name="left" /> has the same value as <paramref name="right" />; otherwise, <c>false</c>.</returns>
    /*API_FUNCTION()*/ static FORCE_INLINE bool operator==(const UIXMargin &left, const UIXMargin &right)
    {
        return left.Equals(right);
    }

    /// <summary>
    /// Tests for inequality between two objects.
    /// </summary>
    /// <param name="left">The first value to compare.</param>
    /// <param name="right">The second value to compare.</param>
    /// <returns><c>true</c> if <paramref name="left" /> has a different value than <paramref name="right" />; otherwise, <c>false</c>.</returns>
    /*API_FUNCTION()*/ static FORCE_INLINE  bool operator !=(const UIXMargin &left, const UIXMargin &right)
    {
        return !left.Equals(right);
    }

    /// <summary>
    /// Returns a <see cref="System.String" /> that represents this instance.
    /// </summary>
    /// <returns>A <see cref="System.String" /> that represents this instance.</returns>
    API_FUNCTION() String ToString() const
    {
        return String::Format(TEXT("Left:{0} Right:{1} Top:{2} Bottom:{3}"), Left, Right, Top, Bottom);
    }

    // TODO: Fix if this is needed
    ///// <summary>
    ///// Returns a <see cref="System.String" /> that represents this instance.
    ///// </summary>
    ///// <param name="format">The format.</param>
    ///// <returns>A <see cref="System.String" /> that represents this instance.</returns>
    //String ToString(String format) const
    //{
    //    return String::Format(_formatString, Left, Right, Top, Bottom );
    //}

    /*TODO: Maybe? IFormatProvider version of ToString cannot be converted to cpp because there is no equivalent*/

    /// <summary>
    /// Returns a hash code for this instance.
    /// </summary>
    /// <returns>A hash code for this instance, suitable for use in hashing algorithms and data structures like a hash table.</returns>
    API_FUNCTION() int GetHashCode() const
    {
        int hashCode = GetHash(Left);
        hashCode = (hashCode * 397) ^ GetHash(Right);
        hashCode = (hashCode * 397) ^ GetHash(Top);
        hashCode = (hashCode * 397) ^ GetHash(Bottom);
        return hashCode;
    }

    /// <summary>
            /// Determines whether the specified <see cref="UIXMargin" /> is equal to this instance.
            /// </summary>
            /// <param name="other">The <see cref="UIXMargin" /> to compare with this instance.</param>
            /// <returns><c>true</c> if the specified <see cref="UIXMargin" /> is equal to this instance; otherwise, <c>false</c>.</returns>
    API_FUNCTION() bool Equals(API_PARAM(Ref) const UIXMargin &other) const
    {
        return Math::NearEqual(other.Left, Left) &&
            Math::NearEqual(other.Right, Right) &&
            Math::NearEqual(other.Top, Top) &&
            Math::NearEqual(other.Bottom, Bottom);
    }

    /// <summary>
    /// Determines whether the specified <see cref="UIXMargin"/> are equal.
    /// </summary>
    API_FUNCTION() static FORCE_INLINE bool Equals(API_PARAM(Ref) const UIXMargin &a, API_PARAM(Ref) const UIXMargin &b)
    {
        return a.Equals(b);
    }

    /// <summary>
    /// Determines whether the specified <see cref="UIXMargin" /> is equal to this instance.
    /// </summary>
    /// <param name="other">The <see cref="UIXMargin" /> to compare with this instance.</param>
    /// <returns><c>true</c> if the specified <see cref="UIXMargin" /> is equal to this instance; otherwise, <c>false</c>.</returns>
    API_FUNCTION() bool FORCE_INLINE Equals(const UIXMargin&other) const
    {
        return Equals(other);
    }

    // TODO: Fix if this is not automatically generated and needed in C#
    ///// <summary>
    ///// Determines whether the specified <see cref="System.Object" /> is equal to this instance.
    ///// </summary>
    ///// <param name="value">The <see cref="System.Object" /> to compare with this instance.</param>
    ///// <returns><c>true</c> if the specified <see cref="System.Object" /> is equal to this instance; otherwise, <c>false</c>.</returns>
    //API_FUNCTION() bool Equals(object value) const override
    //{
    //    return value is Margin margin && Equals(ref margin);
    //}

private:
    static const String _formatString; 

};