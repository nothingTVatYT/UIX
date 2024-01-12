#include "UIXStyle.h"
#include "Engine/Render2D/Font.h"


void UIXStyle::SetCurrent(const UIXStyle &value)
{

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


