#pragma once

#include <vector>

class Histogram
{
public:
	Histogram() = default;

	uint32_t minCount() const { return m_minCount; }
	uint32_t maxCount() const { return m_maxCount; }
	const std::vector<uint32_t>& data() const { return m_data; }

	template <typename T>
	void update(const std::vector<T>& data, size_t maxValueCount)
	{
		m_minCount = std::numeric_limits<T>::max();
		m_maxCount = std::numeric_limits<T>::min();

		m_data.clear();
		m_data.resize(maxValueCount, 0);
		for (const auto value : data)
		{
			auto count = ++m_data[value];

			if (count < m_minCount)
				m_minCount = count;

			if (count > m_maxCount)
				m_maxCount = count;
		}
	}

private:
	std::vector<uint32_t> m_data;
	uint32_t m_minCount = std::numeric_limits<uint32_t>::max();
	uint32_t m_maxCount = std::numeric_limits<uint32_t>::min();
};