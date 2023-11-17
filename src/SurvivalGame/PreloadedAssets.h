#ifndef PRELOADED_ASSETS_H_
#define PRELOADED_ASSETS_H_

#include "Core/anFont.h"
#include "Device/anTexture.h"

class PreloadedAssets
{
public:
	static void Load();
	static anFont GetRalewayFont();
	static int GetRalewayFontSize();
};

#endif
