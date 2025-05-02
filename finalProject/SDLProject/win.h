#ifndef WIN_H
#define WIN_H

#include "Scene.h"

class Win : public Scene {
public:
    ~Win();
    void initialise();
    void update(float delta_time);
    void render(ShaderProgram *program);
    void set_player(Entity *player); // Even if unused
};

#endif
