#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"

void Entity::ai_activate(Entity *player)
{
    switch (m_ai_type)
    {
        case WALKER:
            ai_walk();
            break;
            
        case GUARD:
            ai_guard(player);
            break;
            
        case CHASER:
            ai_chase(player);
            break;

            
        default:
            break;
    }
}

void Entity::ai_walk()
{
    if (m_position.x <= m_left_patrol_bound) {
        m_movement.x = 1.5f;          // Walk right
        m_animation_direction = RIGHT;
        set_flip_x(false);            // Face right
        face_right();                 // (refresh animation indices)
    }
    else if (m_position.x >= m_right_patrol_bound) {
        m_movement.x = -1.5f;         // Walk left
        m_animation_direction = LEFT;
        set_flip_x(true);             // Face left
        face_left();                  // (refresh animation indices)
    }
}




void Entity::ai_guard(Entity *player)
{
    switch (m_ai_state) {
        case IDLE:
            if (glm::distance(m_position, player->get_position()) < 3.5f) m_ai_state = WALKING;
            break;
            
        case WALKING:
        {
            glm::vec3 direction = player->get_position() - m_position;
            
            if (glm::length(direction) > 0.0f) {
                m_movement = glm::normalize(direction);
            } else {
                m_movement = glm::vec3(0.0f);
            }
        }
        break;

            
        case ATTACKING:
            break;
            
        default:
            break;
    }
}

void Entity::ai_chase(Entity* player)
{
    if (!player) return;

    glm::vec3 direction = player->get_position() - m_position;
    m_movement = glm::normalize(direction);
}


// Default constructor
Entity::Entity()
    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
    m_speed(0.0f), m_animation_cols(0), m_animation_frames(0), m_animation_index(0),
    m_animation_rows(0), m_animation_indices(nullptr), m_animation_time(0.0f),
    m_texture_id(0), m_velocity(0.0f), m_acceleration(0.0f), m_width(0.0f), m_height(0.0f)
{
    // Initialize m_walking with zeros or any default value
    for (int i = 0; i < SECONDS_PER_FRAME; ++i)
        for (int j = 0; j < SECONDS_PER_FRAME; ++j) m_walking[i][j] = 0;
}

// Parameterized constructor
Entity::Entity(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power, int walking[4][4], int slashing[4][4], float animation_time,
    int animation_frames, int animation_index, int animation_cols,
    int animation_rows, float width, float height, EntityType EntityType)
    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
    m_speed(speed),m_acceleration(acceleration), m_jumping_power(jump_power), m_animation_cols(animation_cols),
    m_animation_frames(animation_frames), m_animation_index(animation_index),
    m_animation_rows(animation_rows), m_animation_indices(nullptr),
    m_animation_time(animation_time), m_texture_id(texture_id), m_velocity(0.0f),
    m_width(width), m_height(height), m_entity_type(EntityType)
{
    face_right();
    set_walking(walking);
    set_slashing(slashing);
}

Entity::Entity(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power,
               int walking[4][4], int slashing[4][4],
               float animation_time, int animation_frames, int animation_index,
               int animation_cols, int animation_rows,
               float width, float height,
               EntityType entity_type, AIType ai_type, AIState ai_state)
    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
      m_speed(speed), m_acceleration(acceleration), m_jumping_power(jump_power),
      m_animation_cols(animation_cols), m_animation_frames(animation_frames), m_animation_index(animation_index),
      m_animation_rows(animation_rows), m_animation_indices(nullptr),
      m_animation_time(animation_time), m_texture_id(texture_id), m_velocity(0.0f),
      m_width(width), m_height(height), m_entity_type(entity_type),
      m_ai_type(ai_type), m_ai_state(ai_state)
{
    face_right();
    set_walking(walking);
    set_slashing(slashing);
}




// Simpler constructor for partial initialization
Entity::Entity(GLuint texture_id, float speed,  float width, float height, EntityType EntityType)
    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
    m_speed(speed), m_animation_cols(0), m_animation_frames(0), m_animation_index(0),
    m_animation_rows(0), m_animation_indices(nullptr), m_animation_time(0.0f),
    m_texture_id(texture_id), m_velocity(0.0f), m_acceleration(0.0f), m_width(width), m_height(height),m_entity_type(EntityType)
{
    // Initialize m_walking with zeros or any default value
    for (int i = 0; i < SECONDS_PER_FRAME; ++i)
        for (int j = 0; j < SECONDS_PER_FRAME; ++j) m_walking[i][j] = 0;
}
Entity::Entity(GLuint texture_id, float speed, float width, float height, EntityType EntityType, AIType AIType, AIState AIState): m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
m_speed(speed), m_animation_cols(0), m_animation_frames(0), m_animation_index(0),
m_animation_rows(0), m_animation_indices(nullptr), m_animation_time(0.0f),
m_texture_id(texture_id), m_velocity(0.0f), m_acceleration(0.0f), m_width(width), m_height(height),m_entity_type(EntityType), m_ai_type(AIType), m_ai_state(AIState)
{
// Initialize m_walking with zeros or any default value
for (int i = 0; i < SECONDS_PER_FRAME; ++i)
    for (int j = 0; j < SECONDS_PER_FRAME; ++j) m_walking[i][j] = 0;
}

Entity::~Entity() { }

void Entity::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index)
{
    // Step 1: Calculate the UV location of the indexed frame
    float u_coord = (float)(index % m_animation_cols) / (float)m_animation_cols;
    float v_coord = (float)(index / m_animation_cols) / (float)m_animation_rows;

    // Step 2: Calculate its UV size
    float width = 1.0f / (float)m_animation_cols;
    float height = 1.0f / (float)m_animation_rows;

    // Step 3: Just as we have done before, match the texture coordinates to the vertices
    float tex_coords[] =
    {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };

    float vertices[] =
    {
        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
    };

    // Step 4: And render
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}
/*
bool const Entity::check_collision(Entity* other) const
{
    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}
*/

bool const Entity::check_collision(Entity* other, float COLLISION_SHRINK) const
{
    //const float COLLISION_SHRINK = 0.5f; // Adjust this (0.2 = 20% shrink)
    
    float my_width = m_width - COLLISION_SHRINK;
    float my_height = m_height - COLLISION_SHRINK;
    float other_width = other->m_width - COLLISION_SHRINK;
    float other_height = other->m_height - COLLISION_SHRINK;

    float x_distance = fabs(m_position.x - other->m_position.x) - ((my_width + other_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((my_height + other_height) / 2.0f);

    return (x_distance < 0.0f && y_distance < 0.0f);
}

void const Entity::check_collision_y(Entity *collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity *collidable_entity = &collidable_entities[i];
        
        if (check_collision(collidable_entity, 0.5))
        {
            float y_distance = fabs(m_position.y - collidable_entity->m_position.y);
            float y_overlap = fabs(y_distance - (m_height / 2.0f) - (collidable_entity->m_height / 2.0f));
            if (m_velocity.y > 0)
            {
                m_position.y   -= y_overlap;
                m_velocity.y    = 0;

                // Collision!
                m_collided_top  = true;
            } else if (m_velocity.y < 0)
            {
                m_position.y      += y_overlap;
                m_velocity.y       = 0;

                // Collision!
                m_collided_bottom  = true;
            }
        }
    }
}

constexpr float COLLISION_MARGIN = 0.2f; // tweak this if you want looser/tighter collision

void const Entity::check_collision_x(Map *map)
{
    // Shrink probe points slightly toward the center
    glm::vec3 left  = glm::vec3(m_position.x - (m_width / 2.0f) + COLLISION_MARGIN, m_position.y, m_position.z);
    glm::vec3 right = glm::vec3(m_position.x + (m_width / 2.0f) - COLLISION_MARGIN, m_position.y, m_position.z);

    float penetration_x = 0.0f;
    float penetration_y = 0.0f;

    // Check left wall
    if (map->is_solid(left, &penetration_x, &penetration_y) && m_velocity.x < 0)
    {
        m_position.x += penetration_x;
        m_velocity.x = 0;
        m_collided_left = true;
    }

    // Check right wall
    if (map->is_solid(right, &penetration_x, &penetration_y) && m_velocity.x > 0)
    {
        m_position.x -= penetration_x;
        m_velocity.x = 0;
        m_collided_right = true;
    }
}

void const Entity::check_collision_y(Map *map)
{
    // Shrink probe points slightly toward the center
    glm::vec3 top    = glm::vec3(m_position.x, m_position.y + (m_height / 2.0f) - COLLISION_MARGIN, m_position.z);
    glm::vec3 bottom = glm::vec3(m_position.x, m_position.y - (m_height / 2.0f) + COLLISION_MARGIN, m_position.z);

    float penetration_x = 0.0f;
    float penetration_y = 0.0f;

    // Check ceiling
    if (map->is_solid(top, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }

    // Check ground
    if (map->is_solid(bottom, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
    }
}


void Entity::update(float delta_time, Entity *player, Entity *collidable_entities, int collidable_entity_count, Map *map, Map *props_map)
{
    if (m_ai_type == CHASER && m_is_respawning) {
        m_respawn_timer -= delta_time;
        if (m_respawn_timer <= 0.0f) {
            m_is_respawning = false;
            m_is_active = true;
            m_position = glm::vec3(2.0f, -1.0f, 0.0f);
        }
        return;
    }

    if (!m_is_active) return;

    // Reset collision flags
    m_collided_top    = false;
    m_collided_bottom = false;
    m_collided_left   = false;
    m_collided_right  = false;

    if (m_entity_type == ENEMY) ai_activate(player);
    
    // Add knockback to regular velocity
    m_velocity += m_knockback_velocity;

    // Dampen knockback over time
    m_knockback_velocity *= 0.9f;


    // Handle invincibility
    if (m_is_invincible) {
        m_invincibility_time -= delta_time;
        if (m_invincibility_time <= 0.0f) {
            m_is_invincible = false;
            m_invincibility_time = 0.0f;
        }
    }


    // Handle slashing animation
    if (m_is_attacking) {
        m_attack_timer -= delta_time;

        // Use slashing animation set for the current direction
        m_animation_indices = m_slashing[m_animation_direction];

        m_animation_time += delta_time;
        float frames_per_second = 1.0f / SECONDS_PER_FRAME;

        if (m_animation_time >= frames_per_second) {
            m_animation_time = 0.0f;
            m_animation_index = (m_animation_index + 1) % m_animation_frames;
        }

        if (m_attack_timer <= 0.0f) {
            m_is_attacking = false;
            m_animation_index = 0;

            // Reset to walking animation based on last direction
            switch (m_animation_direction) {
                case LEFT:  m_animation_indices = m_walking[LEFT];  break;
                case RIGHT: m_animation_indices = m_walking[RIGHT]; break;
                case UP:    m_animation_indices = m_walking[UP];    break;
                case DOWN:  m_animation_indices = m_walking[DOWN];  break;
                    
            }
        }
        
        // Only the player does slash-based damage
        if (m_entity_type == PLAYER) {
            check_slash_collision(collidable_entities, collidable_entity_count);
        }
    }
    // If moving, use walking animation
    else if (glm::length(m_movement) != 0) {
        m_animation_indices = m_walking[m_animation_direction];

        m_animation_time += delta_time;
        
        float frames_per_second = 1.0f / SECONDS_PER_FRAME;

        
        if (m_animation_time >= frames_per_second) {
            m_animation_time = 0.0f;
            m_animation_index = (m_animation_index + 1) % m_animation_frames;
        }
    }
    if (m_entity_type == PLAYER && collidable_entities != nullptr) {
        for (int i = 0; i < collidable_entity_count; i++) {
            Entity* enemy = &collidable_entities[i];
            if (enemy->get_is_active() && enemy->m_entity_type == ENEMY) {
                if (check_collision(enemy, 0.5)) {
                    take_damage(enemy->get_position()); // pass in enemy's position for knockback
                    break;
                }
            }
        }
    }


    // Movement and physics
    /*
    m_velocity.x = m_movement.x * m_speed;
    m_velocity.y = m_movement.y * m_speed;

    m_velocity += m_acceleration * delta_time;
    m_position += m_velocity * delta_time;
    */
    m_velocity.x = m_movement.x * m_speed + m_knockback_velocity.x;
    m_velocity.y = m_movement.y * m_speed + m_knockback_velocity.y;

    m_velocity += m_acceleration * delta_time;
    m_position += m_velocity * delta_time;

    
    if (m_ai_type != CHASER) {
        check_collision_x(map);
        check_collision_y(map);
    }


    
    check_collision_x(props_map);
    check_collision_y(props_map);


    // Update model matrix
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
}


void Entity::take_damage(glm::vec3 damage_source_position)
{
    if (m_is_invincible) return; // Already invincible, ignore

    m_health -= 1;
    std::cout << "Player took damage! Health now: " << m_health << std::endl;

    // Become invincible temporarily
    m_is_invincible = true;
    m_invincibility_time = m_invincibility_duration;

    // Apply knockback away from the source
    glm::vec3 knockback_direction = glm::normalize(m_position - damage_source_position);
    m_knockback_velocity = knockback_direction * 5.0f; // adjust 5.0f knockback strength

    if (m_health <= 0) {
        //m_is_active = false; // Dead
        std::cout << "Player died!" << std::endl;
    }
}




void Entity::render(ShaderProgram* program)
{
    glm::mat4 model = glm::mat4(1.0f);

    // First move to position
    model = glm::translate(model, m_position);

    // Then apply flip about the center of the quad
    if (m_flip_x) {
        // Flip around center of sprite
        model = glm::scale(model, glm::vec3(-1.0f, 1.0f, 1.0f));
    }


    program->set_model_matrix(model);

    if (m_animation_indices != NULL)
    {
        draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void Entity::set_patrol_bounds(float left, float right)
{
    m_left_patrol_bound = left;
    m_right_patrol_bound = right;
}
