#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 30
#define LEVEL_HEIGHT 30

constexpr char SPRITESHEET_FILEPATH[] = "assets/player_spritesheet.png",
            SLASH_FILEPATH[] = "assets/slash1color3sheet.png",
           PLATFORM_FILEPATH[]    = "assets/Dungeon_Tileset.png",
           ENEMY_FILEPATH[]       = "assets/Skeleton.png";

Mix_Music *g_music;



unsigned int LEVELA_DATA[] = {
    20,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,25, // 1
    20,11,11,11,15,30,11,11,11,11,11,11,11,11,15,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11, // 2
    20,11,11,11,15,30,11,11,11,11,11,11,11,11,02,02,02,02,15,11,11,11,11,11,11,11,11,11,11,11, // 3
    20,11,11,11,15,30,11,11,11,11,11,11,11,11,11,11,11,11,15,11,11,11,11,11,11,11,11,11,11,11, // 4
    20,11,11,11,15,30,11,11,11,11,11,11,11,11,11,11,11,11,15,11,11,11,11,11,11,11,11,11,11,11, // 5
    20,11,11,11,15,30,11,11,11,11,11,11,11,11,50,55,11,11,15,11,11,11,11,11,11,11,11,11,11,11, // 6
    20,11,11,11,15,30,11,11,11,11,11,11,11,11,15,30,11,11,15,11,11,11,11,11,11,11,11,11,11,11, // 7
    20,11,11,11,02,02,11,11,11,11,11,11,11,11,15,30,11,11,15,11,11,11,11,11,11,11,11,11,11,11, // 8
    20,11,11,11,11,11,11,11,11,11,11,11,11,11,15,30,11,11,15,11,11,11,11,11,11,11,11,11,11,11, // 9
    20,41,41,41,41,41,41,41,41,41,41,41,41,41,45,30,11,11,15,11,11,11,11,11,11,11,11,11,11,11, // 10
    20,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,11,11,02,02,02,02,02,02,02,02,02,02,02,25, // 11
    20,11,11,11,11,11,11,11,11,11,15,30,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,15, // 12
    20,11,11,11,50,55,11,11,11,11,15,30,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,15, // 13
    20,11,11,11,15,30,11,11,11,11,15,30,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,15, // 14
    20,11,11,11,15,40,41,41,41,41,45,30,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,15, // 15
    20,11,11,11,02,02,02,02,02,02,15,40,55,11,50,41,41,41,55,11,50,41,41,55,11,50,41,41,41,15, // 16
    20,11,11,11,11,11,11,11,11,11,15,78,30,11,15,78,78,30,02,11,02,15,30,02,11,02,15,30,02,15, // 17
    20,11,11,11,50,55,11,11,11,11,15,78,30,11,15,78,78,30,11,11,11,15,30,11,11,11,15,30,11,15, // 18
    20,11,11,11,15,40,55,11,11,50,45,78,30,11,15,78,78,30,11,11,11,15,30,11,11,11,15,30,11,15, // 19
    20,41,41,41,45,30,02,11,11,02,15,30,02,11,02,15,78,30,11,11,11,15,30,11,11,11,15,30,11,15, // 20
    20,02,02,02,02,02,11,11,11,11,02,02,11,11,11,15,78,40,41,41,41,45,40,55,11,50,45,30,11,15, // 21
    20,11,11,11,11,11,11,11,11,11,11,11,11,11,11,15,78,30,02,02,02,15,30,02,11,02,15,30,11,15, // 22
    20,11,11,11,50,55,11,11,11,11,50,55,11,11,11,15,78,30,11,11,11,02,02,11,11,11,02,02,11,15, // 23
    20,11,11,11,15,30,11,11,11,11,15,40,41,41,41,45,78,30,11,11,11,11,11,11,11,11,11,11,11,15, // 24
    20,55,11,50,45,40,55,11,11,50,45,78,78,78,78,78,78,30,11,11,11,50,55,11,11,11,50,55,11,15, // 25
    20,02,11,02,15,30,02,11,11,02,15,78,78,78,78,78,78,40,55,11,50,45,40,55,11,50,45,30,11,15, // 26
    20,11,11,11,15,30,11,11,11,11,15,78,78,78,78,78,78,30,02,11,02,15,30,02,11,02,15,30,11,15, // 27
    20,11,11,11,15,30,11,11,11,11,15,78,78,78,78,78,78,30,11,11,11,15,30,11,11,11,15,30,11,15, // 28
    20,11,11,11,15,30,11,11,11,11,15,78,78,78,78,78,78,30,11,11,11,15,30,11,11,11,15,30,11,15, // 29
    40,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,45  // 30
//  01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
};

unsigned int LEVELA_PROPS[] = {
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 1
    00,84,84,84,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 2
    00,84,84,84,00,00,00,00,00,00,00,00,00,00,00,00,92,00,00,00,00,00,00,00,00,00,00,00,00,00, // 3
    00,93,00,93,00,00,00,00,00,00,00,00,00,00,00,77,00,77,00,00,00,00,00,00,00,00,00,00,00,00, // 4
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 5
    00,00,00,77,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 6
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 7
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 8
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 9
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 10
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,92,74,66,67,74,92,00,00,00,00,00,00,00,00,00,00, // 11
    00,00,00,00,00,00,00,00,93,98,00,00,64,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 12
    00,00,00,00,00,00,00,00,00,88,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 13
    00,00,00,00,00,00,00,00,93,98,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 14
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 15
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 16
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,92,00,92,00,00,00,00,00,00,00,00,00, // 17
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,77,00,00,98,00, // 18
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 19
    00,00,00,00,00,00,92,00,00,92,00,00,92,00,92,00,00,00,00,77,00,00,00,68,00,00,00,00,00,00, // 20
    00,92,74,92,00,00,64,00,00,00,00,76,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 21
    00,00,00,00,00,00,00,00,00,00,46,00,00,00,00,00,00,00,00,74,00,00,00,00,00,00,00,00,00,00, // 22
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,49,00,00,00,64,00,00,00,00,00,00,00,00,00,00,00, // 23
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 24
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,95,00,00,00,00, // 25
    00,00,00,00,00,00,92,00,00,92,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 26
    00,93,00,93,00,00,00,00,00,77,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00, // 27
    00,00,90,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,93,00,93,00,00,77,00,00,00,00,00,00, // 28
    00,00,00,00,00,00,68,00,00,00,00,00,00,00,00,00,00,00,00,99,00,00,00,00,00,00,00,00,77,00, // 29
    00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00  // 30
//  01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
};



LevelA::~LevelA()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    delete    m_game_state.props_map;
    Mix_FreeChunk(m_game_state.slash_sfx);
    Mix_FreeChunk(m_game_state.skel_death_sfx);
    Mix_FreeChunk(m_game_state.woosh_sfx);

    Mix_FreeChunk(m_game_state.door_open_sfx);
    Mix_FreeMusic(m_game_state.bgm);
    
}

void LevelA::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/Dungeon_Tileset.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 10, 10);
    
    GLuint props_map_texture_id = Utility::load_texture("assets/Dungeon_Tileset.png");
    m_game_state.props_map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_PROPS, props_map_texture_id, 1.0f, 10, 10);
    
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    

    int player_walking_animation[4][4] = {
        { 25, 26, 27, 7 }, // LEFT
        { 25, 26, 27, 7 }, // RIGHT
        { 31, 32, 33, 34 }, // UP
        { 19, 20, 19, 20 }  // DOWN
    };
    
    int player_slashing_animation[4][4] = {
        { 42, 43, 44, 45 }, // LEFT
        { 42, 43, 44, 45 }, // RIGHT
        { 48, 49, 50, 51 },  // UP
        { 36, 37, 38, 39 }  // DOWN
    };

    GLuint skeleton_texture_id = Utility::load_texture("assets/Skeleton.png");
    
    int skeleton_walking_animation[4][4] = {
        { 24, 26, 25, 26},   // LEFT (example)
        {24, 26, 25, 26},   // RIGHT
        {25, 27, 26, 27},  // UP
        {25, 27, 26, 27}  // DOWN
    };


    
    glm::vec3 acceleration = glm::vec3(0.0f);

    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        2.5f,                      // speed
        acceleration,              // acceleration
        5.0f,                      // jumping power
        player_walking_animation,  // animation index sets
        player_slashing_animation,
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        6,                         // animation column amount
        10,                         // animation row amount
        1.0f,                      // width
        0.8f,                       // height
        PLAYER
    );
    
    float start_x = 7.5f;
    float start_y = -28.0f;                // Just above the bottom wall
    
    m_game_state.player->set_position(glm::vec3(start_x, start_y, 0.0f));
    //m_game_state.player->set_position(glm::vec3(start_x+10, start_y+15, 0.0f));

    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);
    GLuint ghost_texture_id = Utility::load_texture("assets/Dungeon_Character_2.png");


    m_game_state.enemies = new Entity[ENEMY_COUNT];
    m_number_of_enemies = ENEMY_COUNT;


    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i] = Entity(
            skeleton_texture_id,  // Skeleton texture
            1.5f,                 // Speed
            glm::vec3(0.0f),      // Acceleration
            0.0f,                 // Jumping power (if any)
            skeleton_walking_animation, // Walking animation
            skeleton_walking_animation, 
            0.0f,
            4,  // Animation frames
            0,
            6,  // Columns
            10,  // Rows
            1.0f, 1.0f,
            ENEMY);
    }

    Entity enemy(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, WALKER, WALKING);
    std::cout << "[TEST] Type should be 2: " << enemy.get_entity_type() << std::endl;
    
    //first enemy
    m_game_state.enemies[0].set_position(glm::vec3(6.0f, -23.0f, 0.0f));
    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_ai_type(WALKER);
    m_game_state.enemies[0].set_ai_state(WALKING);
    m_game_state.enemies[0].set_patrol_bounds(6.0f, 9.0);
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[1].set_position(glm::vec3(1.0f, -13.0f, 0.0f));
    m_game_state.enemies[1].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[1].set_ai_type(WALKER);
    m_game_state.enemies[1].set_ai_state(WALKING);
    m_game_state.enemies[1].set_patrol_bounds(1.0f, 3.0f);
    m_game_state.enemies[1].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[2].set_position(glm::vec3(3.0f, -14.0f, 0.0f));
    m_game_state.enemies[2].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[2].set_ai_type(WALKER);
    m_game_state.enemies[2].set_ai_state(WALKING);
    m_game_state.enemies[2].set_patrol_bounds(1.0f, 3.0f);
    m_game_state.enemies[2].set_acceleration(glm::vec3(0.0f));

    m_game_state.enemies[3].set_position(glm::vec3(3.0f, -12.0f, 0.0f));
    m_game_state.enemies[3].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[3].set_ai_type(WALKER);
    m_game_state.enemies[3].set_ai_state(WALKING);
    m_game_state.enemies[3].set_patrol_bounds(1.0f, 3.0f);
    m_game_state.enemies[3].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[4].set_position(glm::vec3(3.0f, -11.0f, 0.0f));
    m_game_state.enemies[4].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[4].set_ai_type(GUARD);
    m_game_state.enemies[4].set_ai_state(IDLE);
    m_game_state.enemies[4].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[5].set_position(glm::vec3(1.0f, -11.0f, 0.0f));
    m_game_state.enemies[5].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[5].set_ai_type(GUARD);
    m_game_state.enemies[5].set_ai_state(IDLE);
    m_game_state.enemies[5].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[6].set_position(glm::vec3(1.0f, -18.0f, 0.0f));
    m_game_state.enemies[6].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[6].set_ai_type(GUARD);
    m_game_state.enemies[6].set_ai_state(IDLE);
    m_game_state.enemies[6].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[7].set_position(glm::vec3(3.0f, -18.0f, 0.0f));
    m_game_state.enemies[7].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[7].set_ai_type(GUARD);
    m_game_state.enemies[7].set_ai_state(IDLE);
    m_game_state.enemies[7].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[8].set_position(glm::vec3(2.0f, -26.0f, 0.0f));
    m_game_state.enemies[8].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[8].set_ai_type(GUARD);
    m_game_state.enemies[8].set_ai_state(IDLE);
    m_game_state.enemies[8].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[9].set_position(glm::vec3(18.0f, -18.0f, 0.0f));
    m_game_state.enemies[9].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[9].set_ai_type(GUARD);
    m_game_state.enemies[9].set_ai_state(IDLE);
    m_game_state.enemies[9].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[10].set_position(glm::vec3(20.0f, -18.0f, 0.0f));
    m_game_state.enemies[10].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[10].set_ai_type(GUARD);
    m_game_state.enemies[10].set_ai_state(IDLE);
    m_game_state.enemies[10].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[11].set_position(glm::vec3(23.0f, -17.0f, 0.0f));
    m_game_state.enemies[11].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[11].set_ai_type(WALKER);
    m_game_state.enemies[11].set_ai_state(WALKING);
    m_game_state.enemies[11].set_patrol_bounds(23.0f, 25.0f);
    m_game_state.enemies[11].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[12].set_position(glm::vec3(25.0f, -19.0f, 0.0f));
    m_game_state.enemies[12].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[12].set_ai_type(GUARD);
    m_game_state.enemies[12].set_ai_state(IDLE);
    m_game_state.enemies[12].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[13].set_position(glm::vec3(24.0f, -27.0f, 0.0f));
    m_game_state.enemies[13].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[13].set_ai_type(GUARD);
    m_game_state.enemies[13].set_ai_state(IDLE);
    m_game_state.enemies[13].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[14].set_position(glm::vec3(28.0f, -27.0f, 0.0f));
    m_game_state.enemies[14].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[14].set_ai_type(GUARD);
    m_game_state.enemies[14].set_ai_state(IDLE);
    m_game_state.enemies[14].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[15].set_position(glm::vec3(18.0f, -22.0f, 0.0f));
    m_game_state.enemies[15].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[15].set_ai_type(GUARD);
    m_game_state.enemies[15].set_ai_state(IDLE);
    m_game_state.enemies[15].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[16].set_position(glm::vec3(18.0f, -24.0f, 0.0f));
    m_game_state.enemies[16].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[16].set_ai_type(GUARD);
    m_game_state.enemies[16].set_ai_state(IDLE);
    m_game_state.enemies[16].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[17].set_position(glm::vec3(23.0f, -22.0f, 0.0f));
    m_game_state.enemies[17].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[17].set_ai_type(WALKER);
    m_game_state.enemies[17].set_ai_state(WALKING);
    m_game_state.enemies[17].set_patrol_bounds(23.0f, 25.0f);
    m_game_state.enemies[17].set_acceleration(glm::vec3(0.0f));
    
    //Final Room Guards
    m_game_state.enemies[18].set_position(glm::vec3(6.0f, -1.0f, 0.0f));
    m_game_state.enemies[18].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[18].set_ai_type(GUARD);
    m_game_state.enemies[18].set_ai_state(IDLE);
    m_game_state.enemies[18].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[18].set_speed(2.0f);

    m_game_state.enemies[19].set_position(glm::vec3(7.0f, -1.0f, 0.0f));
    m_game_state.enemies[19].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[19].set_ai_type(GUARD);
    m_game_state.enemies[19].set_ai_state(IDLE);
    m_game_state.enemies[19].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[19].set_speed(2.0f);

    m_game_state.enemies[20].set_position(glm::vec3(8.0f, -1.0f, 0.0f));
    m_game_state.enemies[20].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[20].set_ai_type(GUARD);
    m_game_state.enemies[20].set_ai_state(IDLE);
    m_game_state.enemies[20].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[20].set_speed(2.0f);

    
    m_game_state.enemies[21].set_position(glm::vec3(9.0f, -1.0f, 0.0f));
    m_game_state.enemies[21].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[21].set_ai_type(GUARD);
    m_game_state.enemies[21].set_ai_state(IDLE);
    m_game_state.enemies[21].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[21].set_speed(2.0f);
    
    m_game_state.enemies[22].set_position(glm::vec3(10.0f, -1.0f, 0.0f));
    m_game_state.enemies[22].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[22].set_ai_type(GUARD);
    m_game_state.enemies[22].set_ai_state(IDLE);
    m_game_state.enemies[22].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[22].set_speed(2.0f);

    m_game_state.enemies[23].set_position(glm::vec3(11.0f, -1.0f, 0.0f));
    m_game_state.enemies[23].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[23].set_ai_type(GUARD);
    m_game_state.enemies[23].set_ai_state(IDLE);
    m_game_state.enemies[23].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[23].set_speed(2.0f);
    
    m_game_state.enemies[24].set_position(glm::vec3(12.0f, -1.0f, 0.0f));
    m_game_state.enemies[24].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[24].set_ai_type(GUARD);
    m_game_state.enemies[24].set_ai_state(IDLE);
    m_game_state.enemies[24].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[24].set_speed(2.0f);
    
    m_game_state.enemies[25].set_position(glm::vec3(13.0f, -1.0f, 0.0f));
    m_game_state.enemies[25].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[25].set_ai_type(GUARD);
    m_game_state.enemies[25].set_ai_state(IDLE);
    m_game_state.enemies[25].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[25].set_speed(2.0f);

    
    m_game_state.enemies[26].set_position(glm::vec3(6.0f, -2.0f, 0.0f));
    m_game_state.enemies[26].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[26].set_ai_type(WALKER);
    m_game_state.enemies[26].set_ai_state(WALKING);
    m_game_state.enemies[26].set_patrol_bounds(6.0f, 13.0f);
    m_game_state.enemies[26].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[27].set_position(glm::vec3(6.0f, -3.0f, 0.0f));
    m_game_state.enemies[27].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[27].set_ai_type(WALKER);
    m_game_state.enemies[27].set_ai_state(WALKING);
    m_game_state.enemies[27].set_patrol_bounds(6.0f, 13.0f);
    m_game_state.enemies[27].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[28].set_position(glm::vec3(6.0f, -4.0f, 0.0f));
    m_game_state.enemies[28].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[28].set_ai_type(WALKER);
    m_game_state.enemies[28].set_ai_state(WALKING);
    m_game_state.enemies[28].set_patrol_bounds(6.0f, 13.0f);
    m_game_state.enemies[28].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[29].set_position(glm::vec3(6.0f, -5.0f, 0.0f));
    m_game_state.enemies[29].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[29].set_ai_type(WALKER);
    m_game_state.enemies[29].set_ai_state(WALKING);
    m_game_state.enemies[29].set_patrol_bounds(6.0f, 13.0f);
    m_game_state.enemies[29].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[30].set_position(glm::vec3(6.0f, -6.0f, 0.0f));
    m_game_state.enemies[30].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[30].set_ai_type(WALKER);
    m_game_state.enemies[30].set_ai_state(WALKING);
    m_game_state.enemies[30].set_patrol_bounds(6.0f, 13.0f);
    m_game_state.enemies[30].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[31].set_position(glm::vec3(6.0f, -7.0f, 0.0f));
    m_game_state.enemies[31].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[31].set_ai_type(WALKER);
    m_game_state.enemies[31].set_ai_state(WALKING);
    m_game_state.enemies[31].set_patrol_bounds(6.0f, 13.0f);
    m_game_state.enemies[31].set_acceleration(glm::vec3(0.0f));
    
    m_game_state.enemies[32].set_position(glm::vec3(6.0f, -8.0f, 0.0f));
    m_game_state.enemies[32].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[32].set_ai_type(GUARD);
    m_game_state.enemies[32].set_ai_state(IDLE);
    m_game_state.enemies[32].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[32].set_speed(2.0f);
    
    m_game_state.enemies[33].set_position(glm::vec3(7.0f, -8.0f, 0.0f));
    m_game_state.enemies[33].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[33].set_ai_type(GUARD);
    m_game_state.enemies[33].set_ai_state(IDLE);
    m_game_state.enemies[33].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[33].set_speed(2.0f);
    
    m_game_state.enemies[34].set_position(glm::vec3(8.0f, -8.0f, 0.0f));
    m_game_state.enemies[34].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[34].set_ai_type(GUARD);
    m_game_state.enemies[34].set_ai_state(IDLE);
    m_game_state.enemies[34].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[34].set_speed(2.0f);
    
    m_game_state.enemies[35].set_position(glm::vec3(9.0f, -8.0f, 0.0f));
    m_game_state.enemies[35].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[35].set_ai_type(GUARD);
    m_game_state.enemies[35].set_ai_state(IDLE);
    m_game_state.enemies[35].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[35].set_speed(2.0f);
    
    m_game_state.enemies[36].set_position(glm::vec3(10.0f, -8.0f, 0.0f));
    m_game_state.enemies[36].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[36].set_ai_type(GUARD);
    m_game_state.enemies[36].set_ai_state(IDLE);
    m_game_state.enemies[36].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[36].set_speed(2.0f);
    
    m_game_state.enemies[37].set_position(glm::vec3(11.0f, -8.0f, 0.0f));
    m_game_state.enemies[37].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[37].set_ai_type(GUARD);
    m_game_state.enemies[37].set_ai_state(IDLE);
    m_game_state.enemies[37].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[37].set_speed(2.0f);
    
    m_game_state.enemies[38].set_position(glm::vec3(12.0f, -8.0f, 0.0f));
    m_game_state.enemies[38].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[38].set_ai_type(GUARD);
    m_game_state.enemies[38].set_ai_state(IDLE);
    m_game_state.enemies[38].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[38].set_speed(2.0f);
    
    m_game_state.enemies[39].set_position(glm::vec3(13.0f, -8.0f, 0.0f));
    m_game_state.enemies[39].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[39].set_ai_type(GUARD);
    m_game_state.enemies[39].set_ai_state(IDLE);
    m_game_state.enemies[39].set_acceleration(glm::vec3(0.0f));
    m_game_state.enemies[39].set_speed(2.0f);
    
    m_game_state.enemies[40] = Entity(
        skeleton_texture_id,  // Skeleton texture
        1.5f,                 // Speed
        glm::vec3(0.0f),      // Acceleration
        0.0f,                 // Jumping power (if any)
        skeleton_walking_animation, // Walking animation
        skeleton_walking_animation,
        0.0f,
        4,  // Animation frames
        0,
        6,  // Columns
        10,  // Rows
        1.0f, 1.0f,
        ENEMY);
    m_game_state.enemies[40].set_ai_type(CHASER);
    m_game_state.enemies[40].set_ai_state(WALKING);
    m_game_state.enemies[40].set_speed(2.5f);

    
    
    /**
     
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/ominousfinalbgm.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    
    if (!m_game_state.bgm) {
        std::cerr << "Failed to load BGM: " << Mix_GetError() << std::endl;
    }

    m_game_state.slash_sfx = Mix_LoadWAV("assets/swordslash.wav");
    m_game_state.skel_death_sfx = Mix_LoadWAV("assets/skeletondeath.wav");
    m_game_state.door_open_sfx = Mix_LoadWAV("assets/dooropen.wav");
    m_game_state.potion_sfx = Mix_LoadWAV("assets/potionheal.wav");
    m_game_state.woosh_sfx = Mix_LoadWAV("assets/woosh.wav");


    
    Mix_VolumeChunk(
        m_game_state.door_open_sfx,
        MIX_MAX_VOLUME / 4
        );

}

void LevelA::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map, m_game_state.props_map);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map, m_game_state.props_map);
    }
    
    if (m_game_state.player->get_is_attacking()) {
        glm::vec3 attack_pos = m_game_state.player->get_position();

        // Offset depending on direction
        glm::vec3 direction = m_game_state.player->get_movement(); // or keep a last-facing direction
        if (direction.x < 0) attack_pos.x -= 1.0f;
        else if (direction.x > 0) attack_pos.x += 1.0f;
        else if (direction.y < 0) attack_pos.y -= 1.0f;
        else if (direction.y > 0) attack_pos.y += 1.0f;
    }
    
    // 1. Get player's current tile position
    glm::vec3 player_pos = m_game_state.player->get_position();
    int player_tile_x = floor((player_pos.x + 0.5f)); // tile size is 1.0
    int player_tile_y = floor(-player_pos.y + 0.5f);  // y-axis inverted (positive up)


    
    if (player_tile_x >= 0 && player_tile_x < LEVEL_WIDTH && player_tile_y >= 0 && player_tile_y < LEVEL_HEIGHT)
    {
        int props_tile = LEVELA_PROPS[player_tile_y * LEVEL_WIDTH + player_tile_x];

        if (props_tile == 98)
        {
            m_game_state.player->change_health(1);
            LEVELA_PROPS[player_tile_y * LEVEL_WIDTH + player_tile_x] = 0;
            delete m_game_state.props_map;
            GLuint props_map_texture_id = Utility::load_texture("assets/Dungeon_Tileset.png");
            m_game_state.props_map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_PROPS, props_map_texture_id, 1.0f, 10, 10);
            Mix_PlayChannel(-1,  get_state().potion_sfx, 0);


        }
        else if (props_tile == 88)
        {
            // Find and remove all 46 props on the map
            for (int i = 0; i < LEVEL_WIDTH * LEVEL_HEIGHT; i++)
            {
                if (LEVELA_PROPS[i] == 46)
                {
                    LEVELA_PROPS[i] = 0;
                    LEVELA_PROPS[i-30] = 66;
                }
                Mix_PlayChannel(-1,  get_state().door_open_sfx, 0);

            }

            LEVELA_PROPS[player_tile_y * LEVEL_WIDTH + player_tile_x] = 0;
            delete m_game_state.props_map;
            GLuint props_map_texture_id = Utility::load_texture("assets/Dungeon_Tileset.png");
            m_game_state.props_map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_PROPS, props_map_texture_id, 1.0f, 10, 10);
        }
        else if (props_tile == 99)
        {

            for (int i = 0; i < LEVEL_WIDTH * LEVEL_HEIGHT; i++)
            {
                if (LEVELA_PROPS[i] == 66)
                {
                    LEVELA_PROPS[i] = 57;
                    LEVELA_PROPS[i+1] = 58;
                }
            }

            LEVELA_PROPS[player_tile_y * LEVEL_WIDTH + player_tile_x] = 0;
            delete m_game_state.props_map;
            GLuint props_map_texture_id = Utility::load_texture("assets/Dungeon_Tileset.png");
            m_game_state.props_map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_PROPS, props_map_texture_id, 1.0f, 10, 10);
            Mix_PlayChannel(-1,  get_state().door_open_sfx, 0);

        }
        
        else if (props_tile == 90)
        {

            for (int i = 0; i < LEVEL_WIDTH * LEVEL_HEIGHT; i++)
            {
                if (LEVELA_PROPS[i] == 92)
                {
                    LEVELA_PROPS[i] = 90;
                }
            }

            LEVELA_PROPS[player_tile_y * LEVEL_WIDTH + player_tile_x] = 0;
            delete m_game_state.props_map;
            GLuint props_map_texture_id = Utility::load_texture("assets/Dungeon_Tileset.png");
            m_game_state.props_map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_PROPS, props_map_texture_id, 1.0f, 10, 10);
            light_on();
            Mix_PlayChannel(-1,  get_state().woosh_sfx, 0);

        }
        
        else if (props_tile == 84)
        {
            m_game_state.player->win_game();
            
        }
    }


}


void LevelA::render(ShaderProgram *g_shader_program)
{
    //g_shader_program->set_view_matrix(glm::mat4(1.0f));
    g_shader_program ->set_light_position_matrix(m_game_state.player->get_position());

    m_game_state.map->render(g_shader_program);
    m_game_state.props_map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (m_game_state.enemies[i].get_is_active()) {
            m_game_state.enemies[i].render(g_shader_program);
        }
    }
    
    
    
    GLuint font_texture_id = Utility::load_texture("assets/font1.png");
    std::string hp_text = "HP: " + std::to_string(m_game_state.player->get_health());
    g_shader_program->set_view_matrix(glm::mat4(1.0f)); // disable camera for HUD
    Utility::draw_text(g_shader_program, font_texture_id, hp_text, 0.5f, 0.05f, glm::vec3(-4.5f, 3.5f, 0.0f));


    

}
