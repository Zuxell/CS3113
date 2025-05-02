#ifndef START_H
#define START_H

#include "Scene.h"

class Start : public Scene {
public:
    ~Start();
    void initialise();
    void update(float delta_time);
    void render(ShaderProgram *program);
    void set_player(Entity *player); // Even if unused
};

#endif
