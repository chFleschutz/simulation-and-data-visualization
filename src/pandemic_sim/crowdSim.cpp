#include "crowdSim.h"

#include <QDebug>

#include <random>
#include <numbers>
#include <cmath>
#include <algorithm>

CrowdSim::CrowdSim()
	: m_gen(std::random_device()())
{
	m_agentStateCount.resize(static_cast<int>(Agent::State::MaxEnum), 0);
}

void CrowdSim::setBounds(int width, int height)
{
	m_width = width;
	m_height = height;
}

void CrowdSim::initialize()
{
	// Spawn agents
	std::uniform_real_distribution<float> disX(0, m_width);
	std::uniform_real_distribution<float> disY(0, m_height);
	std::uniform_real_distribution<float> moveAngle(0.0f, 2.0f * std::numbers::pi);

	m_agents.clear();
	m_agents.resize(m_params.agentCount);
	for (auto& agent : m_agents)
	{
		agent.position = QVector2D(disX(m_gen), disY(m_gen));

		auto angle = moveAngle(m_gen);
		agent.velocity = QVector2D(cos(angle), sin(angle)) * m_params.agentSpeed;
	}

	// Infect initial agents
	auto infected = std::min(m_params.initialInfected, static_cast<int>(m_agents.size()));
	for (int i = 0; i < infected; ++i)
	{
		m_agents[i].state = Agent::State::Infected;
	}

	// Reset agent state count
	std::fill(m_agentStateCount.begin(), m_agentStateCount.end(), 0);
	m_agentStateCount[static_cast<int>(Agent::State::Healthy)] = m_params.agentCount - infected;
	m_agentStateCount[static_cast<int>(Agent::State::Infected)] = infected;

	m_timer.start();
}

void CrowdSim::update(float timeDelta)
{
	// Reset agent state count
	std::fill(m_agentStateCount.begin(), m_agentStateCount.end(), 0);

	bool checkDeath = m_timer.elapsed() > 1000;
	if (checkDeath)
		m_timer.restart();

	// Update agents
	for (auto& agent : m_agents)
	{
		// Update agent state count
		++m_agentStateCount[static_cast<int>(agent.state)];

		// Skip dead agents
		if (agent.state == Agent::State::Dead)
			continue;

		// Check bounds and bounce back
		checkBounds(agent);
		checkCollisions(agent);
		checkState(agent, timeDelta, checkDeath);

		// Move agent
		agent.position += agent.velocity * timeDelta;
	}
}

void CrowdSim::clearAgents()
{
	m_agents.clear();
}

void CrowdSim::checkBounds(Agent& agent)
{
	if (agent.position.x() < 0 || agent.position.x() > m_width)
	{
		// Keep agent inside bounds
		if (agent.position.x() < -0.1f || agent.position.x() > m_width + 0.1f)
			agent.position.setX(std::clamp(agent.position.x(), 0.0f, static_cast<float>(m_width)));

		agent.velocity.setX(-agent.velocity.x());
	}

	if (agent.position.y() < 0 || agent.position.y() > m_height)
	{
		// Keep agent inside bounds
		if (agent.position.y() < -0.1f || agent.position.y() > m_height + 0.1f)
			agent.position.setY(std::clamp(agent.position.y(), 0.0f, static_cast<float>(m_height)));

		agent.velocity.setY(-agent.velocity.y());
	}
}

void CrowdSim::checkCollisions(Agent& agent)
{
	for (auto& other : m_agents)
	{
		if (&agent == &other)
			continue;

		auto distance = QVector2D(agent.position - other.position).length();
		if (distance < m_params.collisionDistance)
		{
			// Collision detected
			//agent.velocity = -agent.velocity;
			auto direction = QVector2D(agent.position - other.position).normalized();
			agent.velocity = direction * m_params.agentSpeed;

			// Check for infection
			switch (agent.state)
			{
			case Agent::State::Healthy:
				if (other.state == Agent::State::Infected)
					agent.state = Agent::State::Infected;
				break;
			case Agent::State::Infected:
				if (other.state == Agent::State::Healthy)
					other.state = Agent::State::Infected;
				break;
			default:
				break;
			}
		}
	}
}

void CrowdSim::checkState(Agent& agent, float timeDelta, bool checkDeath)
{
	if (agent.state != Agent::State::Infected)
		return;

	// Check for recovery
	agent.timeInfected += timeDelta;
	if (agent.timeInfected > m_params.recoveryTime)
	{
		agent.state = Agent::State::Recovered;
		return;
	}

	if (!checkDeath)
		return;

	// Check for death
	std::uniform_real_distribution<float> deathDis(0.0f, 100.0f);
	if (deathDis(m_gen) < m_params.deathProbability)
		agent.state = Agent::State::Dead;
}
