//
// Created by user on 5/9/2024.
//

namespace globals {
    bool sticky_aim = false;

    bool esp = false;
    bool box = false;
    bool filledbox = false;
    bool outline = false;
    bool distance_esp = false;
    bool local_box = false;
    bool rainbowbox = false;
    bool name_esp = false;
    bool tracers = false;
    bool healthinfo = false;
    bool healthbar = false;
    bool fov_filled = false;
    bool deadzone_fov_filled = false;

    float box_color[3] = { 255, 255, 255 };
    float enemy_team_color[3] = { 255, 0, 0 };
    float deadzone_color[3] = { 255, 255, 255 };
    float fill_color[3] = { 255, 255, 255 };
    float fov_fill_color[3] = { 255, 255, 255 };
    float fov_color[3] = { 255, 255, 255 };
    float name_color[3] = { 255, 255, 255 };
    float tracers_color[3] = { 255, 255, 255 };

    bool prediction = false;
    bool stabilizer = false;
    bool vsync = true;
    bool deadzone = false;
    bool show_deadzone = false;
    bool streamproof = false;
    bool aimbot = false;
    bool autologin = false;
    bool shake = false;
    bool triggerbot = false;
    bool resolver = false;
    bool autopred = false;

    int aimpart = 0;
    int boxtype = 0;

    int x_offset = 0;
    int y_offset = 0;

    int max_dist = 10000;

    float smoothness_x = 3;
    float smoothness_y = 1.5;

    float stabilizer_x = 0;
    float stabilizer_y = 0;

    float prediction_x = 5.5;
    float prediction_y = 6.5;

    float shake_value = 5;

    float range = 0;
    float range_mult = 0;

    float sensitivity = 1.0f;

    int fov = 100;
    int deadzone_value = 50;

    bool fov_on = false;
    bool disable_outside_fov = false;
    bool show_auth = false;

    bool healthcheck = false;
    bool knock_check = false;
    bool team_check = false;
    bool team_check_esp = false;
}