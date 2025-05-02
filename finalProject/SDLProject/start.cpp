#include "Start.h"
#include "Utility.h"
#include "Map.h"
#include <SDL_mixer.h>

#define LEVEL_WIDTH 10
#define LEVEL_HEIGHT 10

unsigned int Start_DATA[] = {
    0,0,0,0,0,0,0,0,0,0, // 1
    0,0,0,0,0,0,0,0,0,0, // 2
    0,0,0,0,0,0,0,0,0,0, // 3
    0,0,0,0,0,0,0,0,0,0, // 4
    0,0,0,0,0,0,0,0,0,0, // 5
    0,0,0,0,0,0,0,0,0,0, // 6
    0,0,0,0,0,0,0,0,0,0, // 7
    0,0,0,0,0,0,0,0,0,0, // 8
    0,0,0,0,0,0,0,0,0,0, // 9
    0,0,0,0,0,0,0,0,0,0  // 10
};

Start::~Start() {
    delete m_game_state.map;
    Mix_FreeMusic(m_game_state.bgm);

}

void Start::initialise() {
    GLuint map_texture_id = Utility::load_texture("assets/Dungeon_tileset.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, Start_DATA, map_texture_id, 1.0f, 20, 12);
    
    m_game_state.player = new Entity();


    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/ominousfinalbgm.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
}

void Start::update(float delta_time) {
    // No update logic needed for static start screen
}


void Start::render(ShaderProgram *program) {
    m_game_state.map->render(program);

    GLuint font_texture_id = Utility::load_texture("assets/font1.png");


    Utility::draw_text(program, font_texture_id, "Dungeon Adventure", 0.5f, 0.05f, glm::vec3(-4.5f, 1.0f, 0.0f));
    Utility::draw_text(program, font_texture_id, "Press ENTER", 0.5f, 0.05f, glm::vec3(-2.75f, -1.0f, 0.0f));
    
}

void Start::set_player(Entity *player) {
    // not used, but required by Scene
    m_game_state.player = player;
}
