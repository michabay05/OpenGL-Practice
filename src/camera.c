#include <GLFW/glfw3.h>

#define RAYMATH_IMPLEMENTATION
#include "camera.h"

static const float DEFAULT_YAW = -90.0f;
static const float DEFAULT_PITCH = 0.0f;
static const float DEFAULT_SPEED = 2.5f;
static const float DEFAULT_SENSITIVITY = 0.1f;
static const float DEFAULT_ZOOM = 45.0f; // In degrees

static void update_camera_vectors(Camera* camera);

Camera camera_default(Vector3 pos)
{
    Vector3 up = { 0.0f, 1.0f, 0.0f };
    Camera camera = {
        // Camera attributes
        .position = pos,
        .front = (Vector3) { 0.0f, 0.0f, -1.0f },
        .up = up,
        .right = Vector3Zero(),
        .world_up = up,
        // Euler angles
        .yaw = DEFAULT_YAW,
        .pitch = DEFAULT_PITCH,
        // Camera options
        .speed = DEFAULT_SPEED,
        .sensitivity = DEFAULT_SENSITIVITY,
        .zoom = DEFAULT_ZOOM,
    };
    update_camera_vectors(&camera);
    return camera;
}

Camera camera_new(Vector3 pos, Vector3 up, float yaw, float pitch)
{
    Camera camera = camera_default(pos);
    camera.position = pos;
    camera.up = up;
    camera.yaw = yaw;
    camera.pitch = pitch;

    update_camera_vectors(&camera);
    return camera;
}

Matrix camera_view_matrix(Camera camera)
{
    return MatrixLookAt(camera.position, Vector3Add(camera.position, camera.front), camera.up);
}

void camera_process_keyboard(Camera* camera, CameraMovement direction, float delta_time)
{
    float velocity = camera->speed * delta_time; // adjust accordingly
    if (direction == CM_FORWARD) {
        // pos += front * velocity
        camera->position = Vector3Add(camera->position, Vector3Scale(camera->front, velocity));
    }
    if (direction == CM_BACKWARD) {
        // pos -= front * velocity
        camera->position = Vector3Subtract(camera->position, Vector3Scale(camera->front, velocity));
    }
    if (direction == CM_LEFT) {
        // pos -= right * velocity
        camera->position = Vector3Subtract(camera->position, Vector3Scale(camera->right, velocity));
    }
    if (direction == CM_RIGHT) {
        // pos += right * velocity
        camera->position = Vector3Add(camera->position, Vector3Scale(camera->right, velocity));
    }
}

void camera_process_mouse(Camera* camera, float x_offset, float y_offset, bool constrain_pitch)
{
    x_offset *= camera->sensitivity;
    y_offset *= camera->sensitivity;

    camera->yaw += x_offset;
    camera->pitch += y_offset;

    if (constrain_pitch) {
        // camera->pitch = fminf(fmaxf(camera->pitch, -89.0f), 89.0f);
        if (camera->pitch >  89.0f) camera->pitch = 89.0f;
        if (camera->pitch < -89.0f) camera->pitch = -89.0f;
    }
    update_camera_vectors(camera);
}

void camera_process_scroll(Camera* camera, float y_offset)
{
    camera->zoom -= (float) y_offset;
    if (camera->zoom <  1.0f) camera->zoom =  1.0f;
    if (camera->zoom > 45.0f) camera->zoom = 45.0f;
}

static void update_camera_vectors(Camera* camera)
{
    float yaw_rad = camera->yaw * DEG2RAD;
    float pitch_rad = camera->pitch * DEG2RAD;
    Vector3 direction = {
        .x = cosf(yaw_rad) * cos(pitch_rad),
        .y = sin(pitch_rad),
        .z = sin(yaw_rad) * cos(pitch_rad)
    };
    camera->front = Vector3Normalize(direction);

    // Recalculate the Right and Up vectors
    // Normalize the vectors, because their length gets closer to 0 the more you look up
    // or down which results in slower movement.
    camera->right = Vector3Normalize(Vector3CrossProduct(camera->front, camera->world_up));
    camera->up = Vector3Normalize(Vector3CrossProduct(camera->right, camera->front));
}
