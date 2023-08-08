#include "AI.h"
#include "Boid.h"
#include <iostream> 


void CursorFollowingAI::update(Boid& boid, double xpos, double ypos) {
    // Logic for birds to follow the cursor
    // Example: bird.position = getCursorPosition();
    //vector_2 newPos = boid.getPos();
    //newPos.x += 0.00001f;
    //boid.setPos({ newPos });

    vector_2 vec = { xpos - boid.getPos().x, ypos - boid.getPos().y, 0 };
    double mag = sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
    vector_2 vec_norm = { vec.x / mag, vec.y / mag, vec.z / mag };
    //double debug_mag = sqrt(abs(vec_norm.x) + abs(vec_norm.y) + abs(vec_norm.z));
    //std::cout << debug_mag << std::endl;
    vector_2 newPos = { boid.getPos().x + vec_norm.x * boid.SPEED, boid.getPos().y + vec_norm.y * boid.SPEED, 0 };
    boid.setPos(newPos);
}


void BoidFollowingAI::update(Boid& boid, double xpos, double ypos) {
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