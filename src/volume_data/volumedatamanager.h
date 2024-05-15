#pragma once

#include "volume_data/VolumeData.h"

template <typename T>
struct Histogram
{
	std::unordered_map<T, int> data;
	T minValue = std::numeric_limits<T>::max();
	T maxValue = std::numeric_limits<T>::min();
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
	const Histogram<uint16_t>& histogram() const { return m_histogram; }

private:
	void updateHistogram()
	{
		m_histogram.data.clear();
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
	Histogram<uint16_t> m_histogram;
};