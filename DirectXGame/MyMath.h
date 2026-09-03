#pragma once
#include "KamataEngine.h"

KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation);

float EaseInOut(float x1, float x2, float t);



struct AABB_G
{
	KamataEngine::Vector3 min;
	KamataEngine::Vector3 max;
};
bool IsCollition_G(const AABB_G& aabbG1, const AABB_G& aabbG2);



struct AABB_B
{
	KamataEngine::Vector3 min;
	KamataEngine::Vector3 max;
};
bool IsCollition_B(const AABB_B& aabb3, const AABB_B& aabb4);