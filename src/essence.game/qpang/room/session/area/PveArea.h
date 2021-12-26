#pragma once

#include <cstdint>

class PveArea
{
public:
	struct Bound
	{
		float x;
		float z;
	};

	PveArea(const Bound& minBound, const Bound& maxBound);

#pragma region Setters
	void setId(uint32_t id);

	void setIsInitialized(bool isInitialized);
#pragma endregion

#pragma region Getters
	[[nodiscard]]
	uint32_t getId() const;

	[[nodiscard]]
	Bound getMinBound() const;

	[[nodiscard]]
	Bound getMaxBound() const;

	[[nodiscard]]
	bool isInitialized() const;
#pragma endregion

private:
	uint32_t m_id;

	Bound m_minBound;
	Bound m_maxBound;

	bool m_isInitialized;
};