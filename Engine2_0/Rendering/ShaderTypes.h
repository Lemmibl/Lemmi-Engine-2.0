#pragma once

namespace ShaderEnums
{
	typedef int Name;
	
	enum
	{
		GeneralShader = 0,
		ShadowMap,
		Compose,
		PostProcessing,
		ShadowPrePass,
		GaussianBlurPass,

		//ALWAYS LEAVE THIS ONE LAST
		SHADER_COUNT
	};
};