#ifndef LOSE_H
#define LOSE_H

#include "Scene.h"

class Lose : public Scene {
public:
    ~Lose();
    void initialise();
    void update(float delta_time);
    void render(ShaderProgram *program);
    void set_player(Entity *player); // Even if unused
};

#endif
