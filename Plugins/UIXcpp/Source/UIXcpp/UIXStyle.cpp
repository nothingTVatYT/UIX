#include "UIXStyle.h"
#include "Engine/Render2D/Font.h"


void UIXStyle::SetCurrent(const UIXStyle *value)
{
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


