#pragma once


namespace TwoD
{
	class Component;
	class Entity;
	class EntityHandle;
	class CompenentStorage;
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	class ComponentStorageImpl;
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	class ComponentHandle;
}