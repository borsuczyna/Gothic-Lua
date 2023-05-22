namespace GOTHIC_ENGINE {
	float zUtils::ClampFloat(float n, float lower, float upper) {
		if (n < lower) return lower;
		if (n > upper) return upper;
		return n;
	}
}