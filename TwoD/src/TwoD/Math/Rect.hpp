#pragma once
#include <glm/glm.hpp>
#include "TwoD/Serialization/Serialization.hpp"


namespace TwoD {
	template<typename T  > struct Rect {
	public: glm::vec<2, T, glm::defaultp> min; glm::vec<2, T, glm::defaultp> max; public: Rect() {
	}; Rect(glm::vec<2, T, glm::defaultp> min, glm::vec<2, T, glm::defaultp> max) : min(min), max(max) {
	} ~Rect() = default; Rect(const Rect& other) noexcept = default; Rect(Rect&& other) noexcept = default; Rect& operator=(const Rect& other) noexcept = default; Rect& operator=(Rect&& other) noexcept = default; template<typename A> Rect(const Rect<A>& other) : min(other.min), max(other.max) {
	} glm::vec<2, T, glm::defaultp> GetSize() const {
		return max - min;
	} T GetWidth() const {
		return max.x - min.x;
	} T GetHeight() const {
		return max.y - min.y;
	} bool IsInside(glm::vec<2, T, glm::defaultp> pos) const {
		return pos.x >= min.x && pos.x <= max.x && pos.y >= min.y && pos.y <= max.y;
	} bool operator==(const Rect<T>& other) const noexcept {
		return this == &other || (min == other.min && max == other.max);
	}
	};
} namespace TwoD {
	template<typename T > struct Deserializable<::TwoD::Rect <T  >> {
		static bool Deserialize(const Deserializer& deserializer, ::TwoD::Rect <T  >& value) {
			bool success = true; success &= deserializer["min"].As<glm::vec<2, T, glm::defaultp>>(value.min); success &= deserializer["max"].As<glm::vec<2, T, glm::defaultp>>(value.max); return success;
		}
	};
} namespace TwoD {
	template<typename T > struct Debuggable<::TwoD::Rect <T  >> {
		static bool Draw(::TwoD::Rect <T  >& value, const char* name) {
			bool changed = false; changed |= Debuggable<glm::vec<2, T, glm::defaultp>>::Draw(value.min, "min"); changed |= Debuggable<glm::vec<2, T, glm::defaultp>>::Draw(value.max, "max"); return changed;
		}
	};
}

