#include "PveArea.h"

PveArea::PveArea(const Bound& minBound, const Bound& maxBound) :
	m_minBound(minBound),
	m_maxBound(maxBound),
	m_isInitialized(false)
{
}

void PveArea::setId(const uint32_t id)
{
	m_id = id;
}

void PveArea::setIsInitialized(const bool isInitialized)
{
	m_isInitialized = isInitialized;
}

uint32_t PveArea::getId() const
{
	return m_id;
}

PveArea::Bound PveArea::getMinBound() const
{
	return m_minBound;
}

PveArea::Bound PveArea::getMaxBound() const
{
	return m_maxBound;
}

bool PveArea::isInitialized() const
{
	return m_isInitialized;
}
