#pragma once

#include <QVector2D>

#include <vector>

class CrowdSim
{
public:
	struct SimulationParams
	{
		float agentSpeed = 100.0f;
		float collisionDistance = 10.0f;
	};

	struct Agent
	{
		enum class State
		{
			Healthy,
			Infected,
			Recovered,
			Dead
		};

		QVector2D position;
		QVector2D velocity;

		State state = State::Healthy;
	};

	CrowdSim() = default;
	~CrowdSim() = default;

	void setBounds(int width, int height);

	void spawnAgents(int count);

	void update(float timeDelta);

	const std::vector<Agent>& agents() const { return m_agents; }
	auto agentCount() const { return m_agents.size(); }

private:
	void checkBounds(Agent& agent);
	void checkCollisions(Agent& agent);

	SimulationParams m_params;

	int m_width = 0;
	int m_height = 0;
	
	std::vector<Agent> m_agents;
};