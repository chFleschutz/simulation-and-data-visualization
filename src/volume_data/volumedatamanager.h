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

	const VolumeData& data() const
	{
		return m_data;
	}

	const std::unordered_map<uint16_t, int>& histogram() const
	{
		return m_histogram;
	}

private:
	void updateHistogram()
	{
		m_histogram.clear();
		for (const auto value : m_data.data)
		{
			m_histogram[value]++;
		}
	}

	VolumeData m_data;
	std::unordered_map<uint16_t, int> m_histogram;
};