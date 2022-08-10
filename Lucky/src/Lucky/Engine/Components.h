#pragma once

#include "../../LuckyMacros.h"

#include "../Core/UUID.h"
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"

#include <string>

NAMESPACE_LUCKY_START

struct IDComponent
{
	UUID ID;

	IDComponent() = default;
	IDComponent(const IDComponent&) = default;
};

struct TagComponent
{
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag) : Tag(tag) {}
};

struct TransformComponent
{
	Lucky_Math::Vector3 Position;
	Lucky_Math::Quaternion Rotation;
	Lucky_Math::Vector3 Scale;

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const Lucky_Math::Vector3 translation) { Position = translation; }


};

NAMESPACE_LUCKY_END