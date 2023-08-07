#include "AI.h"
#include "Boid.h"


void CursorFollowingAI::update(Boid& boid) {
    // Logic for birds to follow the cursor
    // Example: bird.position = getCursorPosition();
    vector_2 newPos = boid.getPos();
    newPos.x += 0.00001f;
    newPos.y += 0.00001f;
    boid.setPos({ newPos });
}


void BoidFollowingAI::update(Boid& boid) {
    // Logic for birds to follow boids behavior
    // Example: bird.position = calculateNewBoidPosition(bird);

    // Access other boids in the behavior
    for (Boid* otherBoid : allBoids) {
        if (otherBoid != &boid) {
            // Apply boids algorithm to update the bird's position
            // Example: bird.position = calculateBoidsBehavior(bird, *otherBird);
        }
    }
}