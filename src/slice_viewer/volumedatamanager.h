#pragma once

#include "slice_viewer/volumedata.h"

struct SlowHistogram
{
	std::unordered_map<uint16_t, int> data;
	uint16_t minValue = std::numeric_limits<uint16_t>::max();
	uint16_t maxValue = std::numeric_limits<uint16_t>::min();
	int maxCount = 0;
};

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
	const SlowHistogram& histogram() const { return m_histogram; }

private:
	void updateHistogram()
	{
		// Reset the histogram
		m_histogram = SlowHistogram{}; 

		for (const auto value : m_data.data)
		{
			if (value < m_histogram.minValue)
				m_histogram.minValue = value;

			if (value > m_histogram.maxValue)
				m_histogram.maxValue = value;

			m_histogram.data[value]++;

			if (m_histogram.data[value] > m_histogram.maxCount)
				m_histogram.maxCount = m_histogram.data[value];
		}
	}

	VolumeData m_data;
	SlowHistogram m_histogram;
};