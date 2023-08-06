#include "AI.h"
#include "Boid.h"


void CursorFollowingAI::update(Boid& boid) {
    // Logic for birds to follow the cursor
    // Example: bird.position = getCursorPosition();
    boid.setPos({ 1, 1, 1 });
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