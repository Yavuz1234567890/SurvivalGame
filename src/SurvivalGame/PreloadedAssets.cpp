#include "PreloadedAssets.h"

static struct
{
	anFont Raleway;
} sAssets;

void PreloadedAssets::Load()
{
	sAssets.Raleway.Load("assets/Raleway-Regular.ttf", GetRalewayFontSize());
}

anFont PreloadedAssets::GetRalewayFont()
{
	return sAssets.Raleway;
}

int PreloadedAssets::GetRalewayFontSize()
{
	return 24;
}
