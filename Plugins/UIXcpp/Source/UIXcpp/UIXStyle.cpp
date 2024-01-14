#include "UIXStyle.h"
#include "UIXTooltip.h"

#include "Engine/Render2D/Font.h"
#include "Engine/Content/Content.h"

// TODO: this might need to be created if we keep this as a class and not a struct.
UIXStyle* UIXStyle::_current = nullptr;


const UIXStyle* UIXStyle::GetCurrent()
{
    if (_current != nullptr)
        return _current;


    _current = New<UIXStyle>();
    _current->Background = Color::FromRGBA(0x1C1C1CFF);
    _current->LightBackground = Color::FromRGBA(0x2D2D30FF);
    _current->Foreground = Color::FromRGBA(0xFFFFFFFF);
    _current->ForegroundGrey = Color::FromRGBA(0xA9A9B3FF);
    _current->ForegroundDisabled = Color::FromRGBA(0x787883FF);
    _current->ForegroundViewport = Color::FromRGBA(0xFFFFFFFF);
    _current->BackgroundHighlighted = Color::FromRGBA(0x54545CFF);
    _current->BorderHighlighted = Color::FromRGBA(0x6A6A75FF);
    _current->BackgroundSelected = Color::FromRGBA(0x007ACCFF);
    _current->BorderSelected = Color::FromRGBA(0x1C97EAFF);
    _current->BackgroundNormal = Color::FromRGBA(0x3F3F46FF);
    _current->BorderNormal = Color::FromRGBA(0x54545CFF);
    _current->TextBoxBackground = Color::FromRGBA(0x333337FF);
    _current->TextBoxBackgroundSelected = Color::FromRGBA(0x3F3F46FF);
    _current->CollectionBackgroundColor = Color::FromRGBA(0xCCCCCC14);
    _current->ProgressNormal = Color::FromRGBA(0x0ad328FF);


    _current->Statusbar.PlayMode = Color::FromRGBA(0x2F9135FF);
    _current->Statusbar.Failed = Color::FromRGBA(0x9C2424FF);
    _current->Statusbar.Loading = Color::FromRGBA(0x2D2D30FF);

    _current->SharedTooltip = New<UIXTooltip>(),

        _current->DragWindow = _current->BackgroundSelected * 0.7f;

    // Use optionally bundled default font (matches Editor)
    FontAsset *defaultFont = Content::LoadAsyncInternal<FontAsset>(TEXT("Editor/Fonts/Roboto-Regular"));
    if (defaultFont)
    {
        _current->SetFontTitle(defaultFont->CreateFont(18));
        _current->SetFontLarge(defaultFont->CreateFont(14));
        _current->SetFontMedium(defaultFont->CreateFont(9));
        _current->SetFontSmall(defaultFont->CreateFont(9));
    }

    return _current;
}


void UIXStyle::SetCurrent(const UIXStyle *value)
{
    if (_current == nullptr)
    {
        GetCurrent();
    }

    _current->Background = value->Background;

    _current->LightBackground = value->LightBackground;

    _current->DragWindow = value->DragWindow;

    _current->Foreground = value->Foreground;

    _current->ForegroundGrey = value->ForegroundGrey;

    _current->ForegroundDisabled = value->ForegroundDisabled;

    _current->ForegroundViewport = value->ForegroundViewport;

    _current->BackgroundHighlighted = value->BackgroundHighlighted;

    _current->BorderHighlighted = value->BorderHighlighted;

    _current->BackgroundSelected = value->BackgroundSelected;

    _current->BorderSelected = value->BorderSelected;

    _current->BackgroundNormal = value->BackgroundNormal;

    _current->BorderNormal = value->BorderNormal;

    _current->TextBoxBackground = value->TextBoxBackground;

    _current->TextBoxBackgroundSelected = value->TextBoxBackgroundSelected;

    _current->CollectionBackgroundColor = value->CollectionBackgroundColor;

    _current->ProgressNormal = value->ProgressNormal;

    _current->Statusbar = value->Statusbar;

    _current->ArrowRight = value->ArrowRight;

    _current->ArrowDown = value->ArrowDown;

    _current->Search = value->Search;

    _current->Settings = value->Settings;

    _current->Cross = value->Cross;

    _current->CheckBoxIntermediate = value->CheckBoxIntermediate;

    _current->CheckBoxTick = value->CheckBoxTick;

    _current->StatusBarSizeGrip = value->StatusBarSizeGrip;

    _current->Translate = value->Translate;

    _current->Rotate = value->Rotate;

    _current->Scale = value->Scale;

    _current->Scalar = value->Scalar;

    _current->SharedTooltip = value->SharedTooltip;
    _current->_fontTitle = value->_fontTitle;
    _current->_fontLarge = value->_fontLarge;
    _current->_fontMedium = value->_fontMedium;
    _current->_fontSmall = value->_fontSmall;
}

Font* UIXStyle::GetFontTitle() const
{
	return  _fontTitle.GetFont();
}

void UIXStyle::SetFontTitle(Font *value)
{
	_fontTitle = UIXFontReference(value);
}

Font* UIXStyle::GetFontLarge() const
{
	return  _fontLarge.GetFont();
}

void UIXStyle::SetFontLarge(Font *value)
{
	_fontLarge = UIXFontReference(value);
}

Font* UIXStyle::GetFontMedium() const
{
	return _fontMedium.GetFont();
}

void UIXStyle::SetFontMedium(Font *value)
{
	_fontMedium = UIXFontReference(value);
}


Font* UIXStyle::GetFontSmall() const
{
	return _fontSmall.GetFont();
}

void UIXStyle::SetFontSmall(Font *value)
{
	_fontSmall = UIXFontReference(value);
}


