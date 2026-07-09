#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL
{
    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

        //Comment-in this part when you implement the constructor

        for (int i = 0; i < num_nodes; i++)
        {
			double x = start.x + (end.x - start.x) * (double)i / (double)(num_nodes - 1);
			double y = start.y + (end.y - start.y) * (double)i / (double)(num_nodes - 1);
			Vector2D position = Vector2D(x, y);
			masses.push_back(new Mass(position, node_mass, false));
        }

        for (int i = 0; i < num_nodes - 1; i++)
        {
            springs.push_back(new Spring(masses[i], masses[i + 1], k));
        }

        for (auto& i : pinned_nodes)
        {
            masses[i]->pinned = true;
        }
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            //TODO (Part 2): Use Hooke's law to calculate the force on a node
            Vector2D dir = s->m2->position - s->m1->position;
            Vector2D f = s->k * dir / dir.norm() * (dir.norm() - s->rest_length);
            
			s->m1->forces = s->m1->forces + f;
            s->m2->forces = s->m2->forces - f;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
				Vector2D a = m->forces / m->mass + gravity;
				m->velocity += a * delta_t;
				m->position += m->velocity * delta_t;

                // TODO (Part 2): Add global damping
                m->velocity += -0.0005 * m->velocity;
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            Vector2D dir = s->m2->position - s->m1->position;
            Vector2D f = s->k * dir / dir.norm() * (dir.norm() - s->rest_length);

            s->m1->forces = s->m1->forces + f;
            s->m2->forces = s->m2->forces - f;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                Vector2D temp_position = m->position;
                // TODO (Part 3.1): Set the new position of the rope mass
                // TODO (Part 4): Add global Verlet damping

                Vector2D a = m->forces / m->mass + gravity;
                m->position = temp_position + (1 - 0.0005) * (temp_position - m->last_position) + a * delta_t * delta_t;
                m->last_position = temp_position;
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }
}
