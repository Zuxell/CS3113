#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define LOG(argument) std::cout << argument << '\n'

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "cmath"
#include <ctime>

enum AppStatus { RUNNING, TERMINATED };

constexpr float WINDOW_SIZE_MULT = 1.5f;

constexpr int WINDOW_WIDTH  = 640 * WINDOW_SIZE_MULT,
              WINDOW_HEIGHT = 480 * WINDOW_SIZE_MULT;

constexpr float BG_RED     = 0.9765625f,
                BG_GREEN   = 0.97265625f,
                BG_BLUE    = 0.9609375f,
                BG_OPACITY = 1.0f;

constexpr int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

constexpr GLint NUMBER_OF_TEXTURES = 1;
constexpr GLint LEVEL_OF_DETAIL    = 0;
constexpr GLint TEXTURE_BORDER     = 0;

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

constexpr char BLUE_SPRITE_FILEPATH[] = "rectangle_blue_real.png",
               RED_SPRITE_FILEPATH[]  = "rectangle_red_real.png",
                BALL_SPRITE_FILEPATH[] = "new_ball.png";

constexpr float MINIMUM_COLLISION_DISTANCE = 1.0f;
constexpr glm::vec3 INIT_SCALE_RED  = glm::vec3(0.5f, 2.0f, 0.0f),
                    INIT_POS_RED    = glm::vec3(-4.75f, 0.0f, 0.0f),
                    INIT_SCALE_BLUE = glm::vec3(0.5f, 2.0f, 0.0f),
                    INIT_POS_BLUE   = glm::vec3(4.75f, 0.0f, 0.0f),
                    INIT_SCALE_BALL = glm::vec3(0.5f, 0.5f, 0.0f),
                    INIT_POS_BALL = glm::vec3(0.0f, 0.0f, 0.0f);

SDL_Window* g_display_window;

AppStatus g_app_status = RUNNING;
ShaderProgram g_shader_program = ShaderProgram();
glm::mat4 g_view_matrix, g_BLUE_matrix, g_projection_matrix, g_RED_matrix, g_BALL_matrix;

float g_previous_ticks = 0.0f;

GLuint g_BLUE_texture_id;
GLuint g_RED_texture_id;
GLuint g_BALL_texture_id;


constexpr float PADDLE_SPEED = 3.0f;
constexpr float BALL_SPEED = 3.0f;

glm::vec3 g_BLUE_position   = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_BLUE_movement = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 g_RED_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_RED_movement = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 g_BALL_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_BALL_movement = glm::vec3(-0.5f, 0.5f, 0.0f);

bool red_at_ceiling = false;
bool red_at_floor = false;
bool blue_at_ceiling = false;
bool blue_at_floor = false;
bool ball_at_ceiling = false;
bool ball_at_floor = false;

bool single_player = false;



bool game_running = true;
bool red_win = false;
bool blue_win = false;

void initialise();
void process_input();
void update();
void render();
void shutdown();

GLuint load_texture(const char* filepath)
{
    // STEP 1: Loading the image file
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);

    if (image == NULL)
    {
        LOG("Unable to load image. Make sure the path is correct.");
        assert(false);
    }

    // STEP 2: Generating and binding a texture ID to our image
    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // STEP 3: Setting our texture filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // STEP 4: Releasing our file from memory and returning our texture id
    stbi_image_free(image);

    return textureID;
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("Pong Clone",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);


    if (g_display_window == nullptr) shutdown();

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_BLUE_matrix = glm::mat4(1.0f);
    g_RED_matrix = glm::mat4(1.0f);
    g_BALL_matrix = glm::mat4(1.0f);

    g_RED_matrix = glm::translate(g_RED_matrix, glm::vec3(1.0f, 1.0f, 0.0f));
    g_RED_position += g_RED_movement;

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    g_BLUE_texture_id = load_texture(BLUE_SPRITE_FILEPATH);
    g_RED_texture_id = load_texture(RED_SPRITE_FILEPATH);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



void process_input()
{
    

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {

            // End game
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_app_status = TERMINATED;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_q: g_app_status = TERMINATED; break;
                    default: break;
                }
                
                

            default:
                break;
                
        }
    }
    
     g_RED_movement.y = 0.0f;
    
     const Uint8 *key_state = SDL_GetKeyboardState(NULL);
     
     if (key_state[SDL_SCANCODE_W] and red_at_ceiling == false){
         g_RED_movement.y = 1.0f;
     }
     else if (key_state[SDL_SCANCODE_S] and red_at_floor == false){
         g_RED_movement.y = -1.0f;
     }
    
     if (g_RED_position.y > 2.75f){
         red_at_ceiling = true;
     }
     else{
         red_at_ceiling = false;
     }
    
     if (g_RED_position.y < -2.75f){
         red_at_floor = true;
     }
     else{
         red_at_floor = false;
     }
    
    if (key_state[SDL_SCANCODE_T]){
        single_player = true;
    }
    if (key_state[SDL_SCANCODE_Y]){
        single_player = false;
    }
    
    if (single_player == true){

        if (g_BLUE_position.y <= -2.60f){
            g_BLUE_movement.y = 1.0f;
        }
        else if (g_BLUE_position.y >= 2.60f){
            g_BLUE_movement.y = -1.0f;

        }
        else if (g_BLUE_movement.y == 0.0f){
            g_BLUE_movement.y = 1.0f;
        }
    }
     
    
    if (single_player == false)
    {
        g_BLUE_movement.y = 0.0f;
        if (key_state[SDL_SCANCODE_UP] and blue_at_ceiling == false){
            g_BLUE_movement.y = 1.0f;
        }
        else if (key_state[SDL_SCANCODE_DOWN] and blue_at_floor == false){
            g_BLUE_movement.y = -1.0f;
        }
        
        if (g_BLUE_position.y > 2.75f){
            blue_at_ceiling = true;
        }
        else{
            blue_at_ceiling = false;
        }
        
        if (g_BLUE_position.y < -2.75f){
            blue_at_floor = true;
        }
        else{
            blue_at_floor = false;
        }
    }
    
    //Up and Down bouncing
    if (game_running == true){
        if (g_BALL_position.y > 3.5f){
            ball_at_ceiling = true;
            g_BALL_movement.y = -g_BALL_movement.y;
        }
        else{
            ball_at_ceiling = false;
        }
        
        if (g_BALL_position.y < -3.5f){
            ball_at_floor = true;
            g_BALL_movement.y = -g_BALL_movement.y;

        }
        else{
            ball_at_floor = false;
        }

        float x_distance_red = fabs(g_BALL_position.x - INIT_POS_RED.x) - ((INIT_SCALE_BALL.x + INIT_SCALE_RED.x) / 2.0f);
        float y_distance_red = fabs(g_BALL_position.y - g_RED_position.y) - ((INIT_SCALE_BALL.y + INIT_SCALE_RED.y) / 2.0f);

        if (x_distance_red < 0 && y_distance_red <= 0){
            
            if (fabs(x_distance_red) < fabs(y_distance_red)) {
                // Collision on the side reflect X movement
                g_BALL_movement.x = -g_BALL_movement.x;
                // Reposition outside of the paddle
                g_BALL_position.x = INIT_POS_RED.x + ((INIT_SCALE_BALL.x + INIT_SCALE_RED.x) / 2.0f) * (g_BALL_position.x > INIT_POS_RED.x ? 1.0f : -1.0f);
            }
            else {
                // Collision on top/bottom reflect X and Y movement
                g_BALL_movement.y = -g_BALL_movement.y;
                g_BALL_movement.x = -g_BALL_movement.x;

                // Reposition outside of the paddle
                g_BALL_position.y = g_RED_position.y + ((INIT_SCALE_BALL.y + INIT_SCALE_RED.y) / 2.0f) * (g_BALL_position.y > g_RED_position.y ? 1.0f : -1.0f);
            }


        }
        
        float x_distance_blue = fabs(g_BALL_position.x - INIT_POS_BLUE.x) - ((INIT_SCALE_BALL.x + INIT_SCALE_BLUE.x) / 2.0f);
        float y_distance_blue = fabs(g_BALL_position.y - g_BLUE_position.y) - ((INIT_SCALE_BALL.y + INIT_SCALE_BLUE.y) / 2.0f);

        if (x_distance_blue < 0 and y_distance_blue <= 0){
            
            if (fabs(x_distance_blue) < fabs(y_distance_blue)) {
                // Collision on the side -> reflect X movement
                g_BALL_movement.x = -g_BALL_movement.x;
                // Reposition outside of the paddle
                g_BALL_position.x = INIT_POS_BLUE.x + ((INIT_SCALE_BALL.x + INIT_SCALE_BLUE.x) / 2.0f) * (g_BALL_position.x > INIT_POS_BLUE.x ? 1.0f : -1.0f);
            }
            else {
                // Collision on top/bottom -> reflect Y movement
                g_BALL_movement.y = -g_BALL_movement.y;
                g_BALL_movement.x = -g_BALL_movement.x;

                // Reposition outside of the paddle
                g_BALL_position.y = g_BLUE_position.y + ((INIT_SCALE_BALL.y + INIT_SCALE_BLUE.y) / 2.0f) * (g_BALL_position.y > g_BLUE_position.y ? 1.0f : -1.0f);
            }

        }
  
        //end game
        
        if (g_BALL_position.x > 4.75){
            game_running = false;
            red_win = true;
        }
        
        if (g_BALL_position.x < -4.75){
            game_running = false;
            blue_win = true;
        }
        
        
    }
}

void update()
{
    
    // --- DELTA TIME CALCULATIONS --- //
    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;
    


    // --- ACCUMULATOR LOGIC --- //
    g_RED_position += g_RED_movement * PADDLE_SPEED * delta_time;
    g_BLUE_position += g_BLUE_movement * PADDLE_SPEED * delta_time;
    g_BALL_position += g_BALL_movement * BALL_SPEED * delta_time;
    
    

    // --- TRANSLATION --- //
    g_BLUE_matrix = glm::mat4(1.0f);
    g_BLUE_matrix = glm::translate(g_BLUE_matrix, INIT_POS_BLUE);
    g_BLUE_matrix = glm::translate(g_BLUE_matrix, g_BLUE_position);
    g_BLUE_matrix = glm::scale(g_BLUE_matrix, INIT_SCALE_BLUE);


    g_RED_matrix = glm::mat4(1.0f);
    g_RED_matrix = glm::translate(g_RED_matrix, INIT_POS_RED);
    g_RED_matrix = glm::translate(g_RED_matrix, g_RED_position);
    g_RED_matrix = glm::scale(g_RED_matrix, INIT_SCALE_RED);
    
    g_BALL_matrix = glm::mat4(1.0f);
    g_BALL_matrix = glm::translate(g_BALL_matrix, INIT_POS_BALL);
    g_BALL_matrix = glm::translate(g_BALL_matrix, g_BALL_position);
    g_BALL_matrix = glm::scale(g_BALL_matrix, INIT_SCALE_BALL);




}

void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id)
{
    g_shader_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6); // we are now drawing 2 triangles, so we use 6 instead of 3
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
    };

    // Textures
    float texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
    };

    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());

    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    // Bind texture
    draw_object(g_RED_matrix, g_RED_texture_id);
    draw_object(g_BLUE_matrix, g_BLUE_texture_id);
    draw_object(g_BALL_matrix, g_BALL_texture_id);

    // We disable two attribute arrays now
    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() { SDL_Quit(); }


int main(int argc, char* argv[])
{
    initialise();

    while (g_app_status == RUNNING)
    {
            process_input();
        if(game_running == true){
            update();
        }
            render();
        
        
        
    }

    shutdown();
    return 0;
}

