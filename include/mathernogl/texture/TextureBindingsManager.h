#pragma once

namespace mathernogl {


namespace{
	class TextureBindingsManager {

	};

	TextureBindingsManager manager;
}

TextureBindingsManager* getTextureBindingsManager(){
	return &manager;
}

}
