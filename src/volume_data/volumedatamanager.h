#pragma once

#include "volume_data/VolumeData.h"

class VolumeDataManager
{
public:
	VolumeDataManager() = default;

	void load(const std::filesystem::path& filename)
	{
		VolumeFileLoader::load(filename, m_data);
		updateHistogram();
	}

	const VolumeData& data() const { return m_data; }

	const std::unordered_map<uint16_t, int>& histogram() const { return m_histogram; }
	uint16_t minValue() const { return m_minValue; }
	uint16_t maxValue() const { return m_maxValue; }
	int maxCount() const { return m_maxCount; }

private:
	void updateHistogram()
	{
		m_histogram.clear();
		for (const auto value : m_data.data)
		{
			if (value < m_minValue)
				m_minValue = value;

			if (value > m_maxValue)
				m_maxValue = value;

			m_histogram[value]++;

			if (m_histogram[value] > m_maxCount)
				m_maxCount = m_histogram[value];
		}
	}

	VolumeData m_data;

	std::unordered_map<uint16_t, int> m_histogram;
	uint16_t m_minValue = 0;
	uint16_t m_maxValue = 0;

	int m_maxCount = 0;
};