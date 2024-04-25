#pragma once

#include <QVector2D>
#include <QElapsedTimer>

#include <vector>
#include <random>

class CrowdSim
{
public:
	struct SimulationParams
	{
		int agentCount = 100;
		int initialInfected = 3;

		float agentSpeed = 100.0f;
		float collisionDistance = 10.0f;
		float recoveryTime = 5.0f;
		float deathProbability = 1.0f;
	};

	struct Agent
	{
		enum class State
		{
			Healthy,
			Infected,
			Recovered,
			Dead,
			MaxEnum
		};

		QVector2D position;
		QVector2D velocity;

		State state = State::Healthy;
		float timeInfected = 0.0f;
	};

	CrowdSim();
	~CrowdSim() = default;

	void setBounds(int width, int height);

	void initialize();
	void clearAgents();

	/// @brief Updates the simulation.
	void update(float timeDelta);

	void setAgentCount(int count) { m_params.agentCount = count; }
	void setCollisionDistance(float distance) { m_params.collisionDistance = distance; }
	void setAgentSpeed(float speed) { m_params.agentSpeed = speed; }
	void setRecoveryTime(float time) { m_params.recoveryTime = time; }
	void setInitialInfected(int count) { m_params.initialInfected = count; }
	void setDeathRate(float rate) { m_params.deathProbability = rate; }

	const std::vector<Agent>& agents() const { return m_agents; }
	auto agentCount() const { return m_agents.size(); }

	/// @brief Returns the number of agents in the specified state.
	int agentCount(Agent::State state) const { return m_agentStateCount[static_cast<int>(state)]; }

private:
	void checkBounds(Agent& agent);
	void checkCollisions(Agent& agent);
	void checkState(Agent& agent, float timeDelta, bool checkDeath);

	SimulationParams m_params;

	int m_width = 0;
	int m_height = 0;
	
	std::vector<Agent> m_agents;
	std::vector<int> m_agentStateCount;

	QElapsedTimer m_timer;

	// Random number generator
	std::mt19937 m_gen;
};