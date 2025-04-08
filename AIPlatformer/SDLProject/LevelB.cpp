#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

constexpr char SPRITESHEET_FILEPATH[] = "assets/DinoSprites.png",
           ENEMY_FILEPATH[]       = "assets/aiplatformerenemy.png";

unsigned int LEVELB_DATA[] =
{
    160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 160,
    160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 160,
    160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 160,
    160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 160,
    160, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 160,
    160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    160, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    160, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162
};

LevelB::~LevelB()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeChunk(m_game_state.hit_sfx);
    Mix_FreeChunk(m_game_state.win_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelB::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/tilemap_packed.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 20, 12);
    /*
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);

    
    int player_walking_animation[4][4] = {
        { 5, 6, 7, 8 }, // LEFT
        { 5, 6, 7, 8 }, // RIGHT
        { 0, 1, 2, 3 }, // UP
        { 0, 1, 2, 3 }  // DOWN
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -4.81f, 0.0f);

    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        2.5f,                      // speed
        acceleration,              // acceleration
        5.0f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        24,                         // animation column amount
        1,                         // animation row amount
        1.0f,                      // width
        0.8f,                       // height
        PLAYER
    );
   
    m_game_state.player->set_position(glm::vec3(1.0f, 0.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(4.0f);
     */
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, GUARD, IDLE);
    }


    m_game_state.enemies[0].set_position(glm::vec3(12.0f, -5.0f, 0.0f));
    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_game_state.bgm = Mix_LoadMUS("assets/aiplatbgm.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(0.0f);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/aijump.wav");
    m_game_state.win_sfx = Mix_LoadWAV("assets/winlevel.wav");
    m_game_state.hit_sfx = Mix_LoadWAV("assets/aihit.wav");
    
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

}

void LevelB::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (m_game_state.player->check_collision(&m_game_state.enemies[i]))
        {
            m_game_state.player->lose_life();
            std::cout << "Player hit! Lives left: " << m_game_state.player->get_lives() << std::endl;
            Mix_PlayChannel(-1, m_game_state.hit_sfx, 0);

            m_game_state.player->set_position(glm::vec3(2.0f, 4.0f, 0.0f));
        }
    }
}


void LevelB::render(ShaderProgram *g_shader_program)
{
    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    for (int i = 0; i < m_number_of_enemies; i++)
            m_game_state.enemies[i].render(g_shader_program);
}

void LevelB::set_player(Entity* player) {
    m_game_state.player = player;
    m_game_state.player->set_position(glm::vec3(1.0f, 0.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(4.0f);
}
