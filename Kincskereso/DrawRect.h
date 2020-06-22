#pragma once

struct DrawRect {
	DrawRect(
		const unsigned int& x,
		const unsigned int& y,
		const unsigned int& width,
		const unsigned int& height,
		const std::wstring& data
	)
		: X(x), Y(y), Width(width), Height(height), Data(data) {}

	unsigned int X;
	unsigned int Y;
	unsigned int Width;
	unsigned int Height;
	std::wstring Data;
};
