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

BoidFollowingAI::BoidFollowingAI(std::vector<Boid*> allBoids) : allBoids(allBoids) {
    std::mt19937 gen(rd()); // Standard Mersenne Twister engine
    std::uniform_real_distribution<float> dist(-10.0, 10.0);

}


void BoidFollowingAI::update(Boid& boid, double xpos, double ypos) {
    // Logic for birds to follow boids behavior
    // Example: bird.position = calculateNewBoidPosition(bird);
    double xpos_avg = 0, ypos_avg = 0, xvel_avg = 0, yvel_avg = 0, neighboring_boids = 0, close_dx = 0, close_dy = 0;
    // Access other boids in the behavior
    for (Boid* otherBoid : allBoids) {
        
        if (otherBoid != &boid) {
            // Apply boids algorithm to update the bird's position
            // Example: bird.position = calculateBoidsBehavior(bird, *otherBird);

            // Zero all accumulator variables(can't do this in one line in C)
            // For every other boid in the flock . . .

                // Compute differences in x and y coordinates
            double dx = boid.getPos().x - otherBoid->getPos().x;
            double dy = boid.getPos().y - otherBoid->getPos().y;

            // Are both those differences less than the visual range ?
            if (abs(dx) < visual_range and abs(dy) < visual_range) {

                // If so, calculate the squared distance
                double squared_distance = dx * dx + dy * dy;

                // Is squared distance less than the protected range ?
                if (squared_distance < (protectedRange * protectedRange)) {

                    // If so, calculate difference in x / y - coordinates to nearfield boid
                    close_dx += boid.getPos().x - otherBoid->getPos().x;
                    close_dy += boid.getPos().y - otherBoid->getPos().y;
                    if (close_dx == 0 and close_dy == 0) {
                        close_dx = dist(gen);
                        close_dy = dist(gen);
                    }
                }
                // If not in protected range, is the boid in the visual range ?
                else if (squared_distance < (visual_range * visual_range)) {

                    // Add other boid's x/y-coord and x/y vel to accumulator variables
                    xpos_avg += otherBoid->getPos().x;
                    ypos_avg += otherBoid->getPos().y;
                    xvel_avg += otherBoid->vx;
                    yvel_avg += otherBoid->vy;

                    // Increment number of boids within visual range
                    neighboring_boids += 1;
                }
            }
        }
    }
            // If there were any boids in the visual range . . .
        if (neighboring_boids > 0) {

            // Divide accumulator variables by number of boids in visual range
            xpos_avg = xpos_avg / neighboring_boids;
            ypos_avg = ypos_avg / neighboring_boids;
            xvel_avg = xvel_avg / neighboring_boids;
            yvel_avg = yvel_avg / neighboring_boids;

            // Add the centering / matching contributions to velocity
            boid.vx = (boid.vx + (xpos_avg - boid.getPos().x) * centeringfactor + (xvel_avg - boid.vx) * matchingfactor);
            boid.vy = (boid.vy + (ypos_avg - boid.getPos().y) * centeringfactor + (yvel_avg - boid.vy) * matchingfactor);
        }
        // Add the avoidance contribution to velocity
        boid.vx = boid.vx + (close_dx * avoidfactor);
        boid.vy = boid.vy + (close_dy * avoidfactor);


        // If the boid is near an edge, make it turn by turnfactor
        // (this describes a box, will vary based on boundary conditions)
        if (boid.getPos().y > 1080) {
            boid.vy = boid.vy - turnfactor;
        }
        if (boid.getPos().x > 1920) {
            boid.vx = boid.vx - turnfactor;
        }
        if (boid.getPos().x < 0) {
            boid.vx = boid.vx + turnfactor;
        }
        if (boid.getPos().y < 0) {
            boid.vy = boid.vy + turnfactor;
        }
        

        // Calculate the boid's speed
        // Slow step!Lookup the "alpha max plus beta min" algorithm
        boid.SPEED = sqrt(boid.vx * boid.vx + boid.vy * boid.vy);

        // Enforce min and max speeds
        if (boid.SPEED < minspeed and boid.SPEED != 0) {
            boid.vx = (boid.vx / boid.SPEED) * minspeed;
            boid.vy = (boid.vy / boid.SPEED) * minspeed;
            boid.SPEED = minspeed;
        }
        if (boid.SPEED > maxspeed) {
            boid.vx = (boid.vx / boid.SPEED) * maxspeed;
            boid.vy = (boid.vy / boid.SPEED) * maxspeed;
            boid.SPEED = maxspeed;
        }
        if (boid.SPEED == 0) {
            boid.SPEED = minspeed;
        }

        // Update boid's position
        boid.setPos({ boid.getPos().x + boid.vx, boid.getPos().y + boid.vy });
    
}