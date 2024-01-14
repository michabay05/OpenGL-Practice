#pragma once

#include <stdbool.h>
#include "raymath.h"

typedef enum {
    CM_FORWARD,
    CM_BACKWARD,
    CM_LEFT,
    CM_RIGHT,
} CameraMovement;

typedef struct {
    // Camera attributes
    Vector3 position;
    Vector3 front;
    Vector3 up;
    Vector3 right;
    Vector3 world_up;
    // Euler angles
    float yaw;
    float pitch;
    // Camera options
    float speed;
    float sensitivity;
    float zoom;
} Camera;

Camera camera_default(Vector3 pos);
Camera camera_new(Vector3 pos, Vector3 up, float yaw, float pitch);
Matrix camera_view_matrix(Camera camera);
void camera_process_keyboard(Camera* camera, CameraMovement direction, float delta_time);
void camera_process_mouse(Camera* camera, float x_offset, float y_offset, bool constrain_pitch);
void camera_process_scroll(Camera* camera, float y_offset);
