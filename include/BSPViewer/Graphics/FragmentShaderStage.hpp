#pragma once

#include "BSPViewer/Graphics/ShaderStage.hpp"
#include "BSPViewer/Graphics/ShaderStageType.hpp"

class FragmentShaderStage : public ShaderStage {
public:
	FragmentShaderStage(const std::string& source) : ShaderStage(source, ShaderStageType::Fragment) {}

	FragmentShaderStage(const FragmentShaderStage& other) = delete;

    FragmentShaderStage& operator =(const FragmentShaderStage& other) = delete;
};
