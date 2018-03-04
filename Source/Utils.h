#pragma once

template<typename T>
T Lerp(const T &a, const T &b, float scalar)
{
	return (a + (b - a) * scalar);
}

template<typename T>
T Min(const T &a, const T &b)
{
	if (a < b) {
		return a;
	}
	return b;
}

template<typename T>
T Max(const T &a, const T &b)
{
	if (a > b) {
		return a;
	}
	return b;
}

template<typename T>
T Clamp(const T &min, const T &max, const T &value)
{
	if (value < min) {
		return min;
	} else if (value > max) {
		return max;
	}
	return value;
}
