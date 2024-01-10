using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX;

public class UIXPasswordBox : TextBox
{
    private TextLayoutOptions _layout;
    private const string Asterisks = "**************************************************";

    /// <inheritdoc />
    public UIXPasswordBox() : this(false, 0, 0)
    {
        MaxLength = Asterisks.Length;
    }

    /// <inheritdoc />
    public UIXPasswordBox(bool isMultiline, float x, float y, float width = 120) : base(false, x, y, width)
    {
        MaxLength = Asterisks.Length;
        _layout = TextLayoutOptions.Default;
        _layout.VerticalAlignment = TextAlignment.Center;
        _layout.TextWrapping = TextWrapping.NoWrap;
        _layout.Bounds = new Rectangle(DefaultMargin, 1, Width - 2 * DefaultMargin, Height - 2);
        TextColor = UIXStyle.Style.Foreground;
    }

    /// <inheritdoc />
    public override void DrawSelf()
    {
        // Cache data
        var rect = new Rectangle(Float2.Zero, Size);
        bool enabled = EnabledInHierarchy;
        var font = Font.GetFont();
        if (!font)
            return;

        // Background
        Color backColor = BackgroundColor;
        if (IsMouseOver || IsNavFocused)
            backColor = BackgroundSelectedColor;
        Render2D.FillRectangle(rect, backColor);
        if (HasBorder)
            Render2D.DrawRectangle(rect, IsFocused ? BorderSelectedColor : BorderColor, BorderThickness);

        // Apply view offset and clip mask
        if (ClipText)
            Render2D.PushClip(TextClipRectangle);
        bool useViewOffset = !_viewOffset.IsZero;
        if (useViewOffset)
            Render2D.PushTransform(Matrix3x3.Translation2D(-_viewOffset));

        // Check if sth is selected to draw selection
        if (HasSelection)
        {
            var leftEdge = font.GetCharPosition(_text, SelectionLeft, ref _layout);
            var rightEdge = font.GetCharPosition(_text, SelectionRight, ref _layout);
            float fontHeight = font.Height / DpiScale;

            // Draw selection background
            float alpha = Mathf.Min(1.0f, Mathf.Cos(_animateTime * BackgroundSelectedFlashSpeed) * 0.5f + 1.3f);
            alpha *= alpha;
            Color selectionColor = SelectionColor * alpha;
            //
            int selectedLinesCount = 1 + Mathf.FloorToInt((rightEdge.Y - leftEdge.Y) / fontHeight);
            if (selectedLinesCount == 1)
            {
                // Selected is part of single line
                Rectangle r1 = new Rectangle(leftEdge.X, leftEdge.Y, rightEdge.X - leftEdge.X, fontHeight);
                Render2D.FillRectangle(r1, selectionColor);
            }
            else
            {
                float leftMargin = _layout.Bounds.Location.X;

                // Selected is more than one line
                Rectangle r1 = new Rectangle(leftEdge.X, leftEdge.Y, 1000000000, fontHeight);
                Render2D.FillRectangle(r1, selectionColor);
                //
                for (int i = 3; i <= selectedLinesCount; i++)
                {
                    leftEdge.Y += fontHeight;
                    Rectangle r = new Rectangle(leftMargin, leftEdge.Y, 1000000000, fontHeight);
                    Render2D.FillRectangle(r, selectionColor);
                }
                //
                Rectangle r2 = new Rectangle(leftMargin, rightEdge.Y, rightEdge.X - leftMargin, fontHeight);
                Render2D.FillRectangle(r2, selectionColor);
            }
        }

        // Text or watermark
        if (_text.Length > 0)
        {
            var color = TextColor;
            if (!enabled)
                color *= 0.6f;
            var replacementForText = Asterisks[.._text.Length];
            Render2D.DrawText(font, replacementForText, color, ref _layout, TextMaterial);
        }
        else if (!string.IsNullOrEmpty(_watermarkText) && !IsFocused)
        {
            Render2D.DrawText(font, _watermarkText, WatermarkTextColor, ref _layout, TextMaterial);
        }

        // Caret
        if (IsFocused && CaretPosition > -1)
        {
            float alpha = Mathf.Saturate(Mathf.Cos(_animateTime * CaretFlashSpeed) * 0.5f + 0.7f);
            alpha = alpha * alpha * alpha * alpha * alpha * alpha;
            Render2D.FillRectangle(CaretBounds, CaretColor * alpha);
        }

        // Restore rendering state
        if (useViewOffset)
            Render2D.PopTransform();
        if (ClipText)
            Render2D.PopClip();
    }

    /// <inheritdoc />
    public override Float2 GetTextSize()
    {
        var font = Font.GetFont();
        if (font == null)
        {
            return Float2.Zero;
        }

        return font.MeasureText(Asterisks[.._text.Length], ref _layout);
    }

    /// <inheritdoc />
    public override Float2 GetCharPosition(int index, out float height)
    {
        var font = Font.GetFont();
        if (font == null)
        {
            height = Height;
            return Float2.Zero;
        }

        height = font.Height / DpiScale;
        return font.GetCharPosition(Asterisks[.._text.Length], index, ref _layout);
    }

    /// <inheritdoc />
    public override int HitTestText(Float2 location)
    {
        var font = Font.GetFont();
        if (font == null)
        {
            return 0;
        }

        return font.HitTestText(Asterisks[.._text.Length], location, ref _layout);
    }

}