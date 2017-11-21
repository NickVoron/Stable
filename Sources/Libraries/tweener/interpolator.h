#pragma once

#include "math/library.include.h"
#include "newmath/library.include.h"
#include "memory/library.include.h"

namespace tween
{
	template<class Value> void makeDefault(Value& value) { mem::memzero(value);	}

	inline void makeDefault(Quaternion& value) { value.Identity(); }
	inline void makeDefault(State& value) { value.SetIdentity(); }

	Quaternion lerp(const Quaternion& v0, const Quaternion& v1, float t);
	State lerp(const State& v0, const State& v1, float t);

	template<class ValueType>	struct Interpolator				 { static inline ValueType evaluate(const ValueType& v0, const ValueType& v1, float t) { return nm::lerp(v0, v1, t); } };
	template<>					struct Interpolator<Quaternion>	 { static inline Quaternion evaluate(const Quaternion& v0, const Quaternion& v1, float t) { return lerp(v0, v1, t); } };
	template<>					struct Interpolator<State>		 { static inline State evaluate(const State& v0, const State& v1, float t) { return lerp(v0, v1, t); } };
}