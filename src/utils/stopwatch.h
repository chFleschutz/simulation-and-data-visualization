#pragma once

#include <chrono>

// Simple stopwatch class to measure elapsed time in seconds
class StopWatch
{
public:
	using Clock = std::chrono::high_resolution_clock;
	using Duration = std::chrono::duration<double>;

	StopWatch() = default;
	~StopWatch() = default;

	// Start the stopwatch
	void start()
	{
		m_startTime = Clock::now();
		m_isRunning = true;
	}

	// Stop the stopwatch
	void pause()
	{
		m_elapsedTime = elapsedTime();
		m_isRunning = false;
	}

	// Reset the stopwatch
	void reset()
	{
		m_elapsedTime = 0.0;
		m_isRunning = false;
	}

	// Returns the elapsed time in seconds
	double elapsedTime() const
	{
		if (!m_isRunning)
			return m_elapsedTime;

		auto elapsedTime = std::chrono::duration_cast<Duration>(Clock::now() - m_startTime);
		return m_elapsedTime + elapsedTime.count();
	}

	// Returns true if the stopwatch is running
	bool isRunning() const { return m_isRunning; }

private:
	Clock::time_point m_startTime;
	double m_elapsedTime = 0.0;
	bool m_isRunning = false;
};
