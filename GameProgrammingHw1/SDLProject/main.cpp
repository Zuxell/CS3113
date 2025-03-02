/**
 *
 * Author: Brian Yin
 * Assignment: Simple 2D Scene
 * Date due: 2025-02-15, 11:59pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 *
 * @copyright Copyright NYU Tandon School of Engineering (c) 2024
 *
 */

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

// Our window dimensions
constexpr int WINDOW_WIDTH  = 640 * WINDOW_SIZE_MULT,
              WINDOW_HEIGHT = 480 * WINDOW_SIZE_MULT;

// Background color components
constexpr float BG_RED     = 0.9765625f,
                BG_GREEN   = 0.97265625f,
                BG_BLUE    = 0.9609375f,
                BG_OPACITY = 1.0f;

// Our viewport—or our "camera"'s—position and dimensions
constexpr int VIEWPORT_X      = 0,
              VIEWPORT_Y      = 0,
              VIEWPORT_WIDTH  = WINDOW_WIDTH,
              VIEWPORT_HEIGHT = WINDOW_HEIGHT;

// Our shader filepaths; these are necessary for a number of things
// Not least, to actually draw our shapes
// We'll have a whole lecture on these later
constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

constexpr char KNIGHT_SPRITE_FILEPATH[] = "HK1.png",
               HORNET_SPRITE_FILEPATH[]  = "rectangle_blue_real.png";

SDL_Window* g_display_window;

AppStatus g_app_status = RUNNING;

ShaderProgram g_shader_program = ShaderProgram();

glm::mat4 g_view_matrix, g_knight_matrix, g_hornet_matrix, g_projection_matrix;

GLuint g_knight_texture_id;
GLuint g_hornet_texture_id;

constexpr GLint NUMBER_OF_TEXTURES = 1;  // to be generated, that is
constexpr GLint LEVEL_OF_DETAIL    = 0;  // base image level; Level n is the nth mipmap reduction image
constexpr GLint TEXTURE_BORDER     = 0;  // this value MUST be zero

float g_knight_x      = 0.0f;
float g_knight_rotate = 0.0f;
float g_hornet_x      = -3.0f;
float g_hornet_y      = -3.0f;

constexpr float DEGREES_PER_SECOND     = 90.0f;

// ——————————— GLOBAL VARS AND CONSTS FOR TRANSFORMATIONS ——————————— //
constexpr float BASE_SCALE = 1.0f,      // The unscaled size of your object
               MAX_AMPLITUDE = 0.1f,  // The most our triangle will be scaled up/down
               PULSE_SPEED = 10.0f;    // How fast you want your triangle to "beat"

constexpr float RADIUS = 2.0f;      // radius of your circle
constexpr float ORBIT_SPEED = 1.0f;  // rotational speed
float       g_angle = 0.0f;     // current angle
float       g_x_offset = 0.0f, // current x and y coordinates
           g_y_offset = 0.0f;
float g_previous_ticks = 0;
float g_pulse_time = 0.0f;

// —————————————————————————————————————————————————————————————————— //


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
    g_display_window = SDL_CreateWindow("Hello, Homework 1!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    if (g_display_window == nullptr)
    {
        std::cerr << "ERROR: SDL Window could not be created.\n";
        g_app_status = TERMINATED;
        
        SDL_Quit();
        exit(1);
    }
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    // Initialise our camera
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    // Load up our shaders
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    // Initialise our view, model, and projection matrices
    g_view_matrix       = glm::mat4(1.0f);  // Defines the position (location and orientation) of the camera
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    
    g_knight_matrix      = glm::mat4(1.0f);  // Defines every translation, rotations, or scaling applied to an object
    g_hornet_matrix      = glm::mat4(1.0f);  // Defines every translation, rotations, or scaling applied to an object
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    // Notice we haven't set our model matrix yet!
    
    // Each object has its own unique ID
    glUseProgram(g_shader_program.get_program_id());
    
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    
    g_hornet_texture_id = load_texture(HORNET_SPRITE_FILEPATH);
    g_knight_texture_id = load_texture(KNIGHT_SPRITE_FILEPATH);
}

void process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            g_app_status = TERMINATED;
        }
    }
}

void update()
{
    /* Delta Time Calculations */
    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND; // current # of ticks
    float delta_time = ticks - g_previous_ticks; // tick difference from the last frame
    g_previous_ticks = ticks;
    
    // ——————————— PULSE TRANSFORMATIONS ——————————— //
    // Instead of using g_frame_counter, use delta_time to accumulate time for pulsing.
    g_pulse_time += delta_time * PULSE_SPEED; // Scale the pulse speed by delta_time
    float scale_factor = BASE_SCALE + MAX_AMPLITUDE * glm::sin(g_pulse_time); // Use sine for a smoother pulse
    
    // ——————————— ORBIT TRANSFORMATIONS ——————————— //
    // Use delta_time to make the orbiting independent of framerate
    g_angle += ORBIT_SPEED * delta_time;  // increment g_angle by ORBIT_SPEED scaled by delta_time

    // Calculate new x, y using trigonometry for orbit
    g_x_offset = RADIUS * glm::cos(g_angle);
    g_y_offset = RADIUS * glm::sin(g_angle);

    // Reset the model matrix and apply transformations
    g_knight_matrix = glm::mat4(1.0f);
    g_knight_matrix = glm::translate(g_hornet_matrix, glm::vec3(g_x_offset, g_y_offset, 0.0f));
    g_knight_matrix = glm::scale(g_knight_matrix, glm::vec3(scale_factor, scale_factor, 1.0f));
    
    g_hornet_matrix = glm::mat4(1.0f);
    g_hornet_x += 1.0f * delta_time;
    g_hornet_y += 1.0f * delta_time;
    g_hornet_matrix = glm::translate(g_hornet_matrix, glm::vec3(g_hornet_x, g_hornet_y, 0.0f));



    g_knight_rotate += DEGREES_PER_SECOND * delta_time; // 90-degrees per second

    g_knight_matrix = glm::rotate(g_knight_matrix, glm::radians(g_knight_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
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
    draw_object(g_hornet_matrix, g_hornet_texture_id);
    draw_object(g_knight_matrix, g_knight_texture_id);

    // We disable two attribute arrays now
    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() { SDL_Quit(); }


int main(int argc, char* argv[])
{
    // Initialise our program—whatever that means
    initialise();
    
    while (g_app_status == RUNNING)
    {
        process_input();  // If the player did anything—press a button, move the joystick—process it
        update();         // Using the game's previous state, and whatever new input we have, update the game's state
        render();         // Once updated, render those changes onto the screen
    }
    
    shutdown();
    return 0;
}
