#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <math.h>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL_ttf.h>
#include <psp2/appmgr.h>
#include <psp2/kernel/clib.h>
#include <psp2/kernel/processmgr.h>

#include "board.hpp"
#include "individual.hpp"
#include "vector.hpp"

// Screen dimension constants
enum { SCREEN_WIDTH = 960, SCREEN_HEIGHT = 544 };

class App {
private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_GameController *controller;
  TTF_Font *font;

  Vector2 shift;
  Vector2 momentum;
  float zoom;
  float zoom_momentum;

  Board board;

  int initialize();

  void render_line(const Vector2 p1, const Vector2 p2);
  void render_individual(const Individual &individual);
  void render_food(const Vector2 &food);
  void render_text(const char *string, Vector2 position, float scale = 1.0);
  void render();
  void tick(float time_delta);
  void process_events(float time_delta);

public:
  App();
  int run();
};

App::App()
    : shift{480.0f, 277.0f}, momentum{0.0, 0.0}, zoom{20.0f}, zoom_momentum{
                                                                  0.0} {};

int App::initialize() {
  start_debug_log();
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    return -1;

  if ((window = SDL_CreateWindow("Evovita", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                 SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) == NULL)
    return -1;

  if ((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL)
    return -1;

  controller = SDL_GameControllerOpen(0);
  if (!controller) {
    LOG("Error opening controller: %s", SDL_GetError());
    return -1;
  }

  TTF_Init();
  font = TTF_OpenFont("resources/PublicPixel.ttf", 25);

  for (int i = 0; i < 15; i++) {
    board.individuals.emplace_back(
        1.0, Vector2(randf(-10.0, 10.0), randf(-10.0, 10.0)),
        randf(0.0, M_PI * 2.0));
    board.individuals[board.individuals.size() - 1].clock_offset =
        randf(0.0, M_PI * 2.0);
  }
  for (int i = 0; i < 150; i++) {
    board.food.emplace_back(randf(-30.0, 30.0), randf(-30.0, 30.0));
  }
  LOG("init done");
  return 0;
}

void App::render_line(const Vector2 p1, const Vector2 p2) {
  Vector2 p1t = p1 * zoom + shift;
  Vector2 p2t = p2 * zoom + shift;
  SDL_RenderDrawLineF(renderer, p1t.x, p1t.y, p2t.x, p2t.y);
}

void App::render_text(const char *string, Vector2 position, float scale) {
  auto surface = TTF_RenderText_Solid(font, string, {255, 255, 255, 255});
  auto texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FRect rect{position.x, position.y, static_cast<float>(surface->w) * scale,
                 static_cast<float>(surface->h) * scale};
  SDL_RenderCopyF(renderer, texture, NULL, &rect);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}

void App::render_individual(const Individual &individual) {
  float size = 1.0;
  Vector2 forward = individual.position +
                    Vector2(std::cos(individual.orientation) * size * 0.6,
                            std::sin(individual.orientation) * size * 0.6);
  Vector2 left =
      individual.position +
      Vector2(std::cos(individual.orientation + M_PI * 0.8) * size * 0.4,
              std::sin(individual.orientation + M_PI * 0.8) * size * 0.4);
  Vector2 right =
      individual.position +
      Vector2(std::cos(individual.orientation - M_PI * 0.8) * size * 0.4,
              std::sin(individual.orientation - M_PI * 0.8) * size * 0.4);

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  render_line(forward, left);
  render_line(forward, right);
  render_line(left, right);

  for (int i = 0; i < 10; i++) {
    uint8_t intensity = std::round(individual.inputs[i] * 255.0);
    SDL_SetRenderDrawColor(renderer, intensity, intensity, intensity, 255);

    float a = (static_cast<float>(i) - 4.5) / 5.0 * M_PI * 0.25;
    render_line(
        individual.position,
        individual.position +
            Vector2(cos(individual.orientation + a) * board.view_range,
                    sin(individual.orientation + a) * board.view_range));
  }

  char buff[64];
  sprintf(buff, "%f", individual.life);
  render_text(buff, individual.position * zoom + shift, 0.5);
}

void App::render_food(const Vector2 &food) {
  Vector2 mapped = food * zoom + shift;
  float size = 2;
  SDL_FRect rect{mapped.x - size / 0.5, mapped.y - size / 0.5, size, size};
  SDL_RenderDrawRectF(renderer, &rect);
};

void App::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  for (auto &individual : board.individuals) {
    render_individual(individual);
  }
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  for (auto &food : board.food) {
    render_food(food);
  }

  Vector2 vpoints[] = {{-25.0, -25.0},
                       {-25.0, 25.0},
                       {25.0, 25.0},
                       {25.0, -25.0},
                       {-25.0, -25.0}};
  SDL_FPoint points[5];
  for (int i = 0; i < 5; i++) {
    points[i] = (vpoints[i] * zoom + shift).as_point();
  }
  SDL_RenderDrawLinesF(renderer, points, 5);

  SDL_RenderPresent(renderer);
}

void App::tick(float time_delta) { board.tick(time_delta); }

void App::process_events(float time_delta) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    LOG("Event: %d", event.type);
  }
  Vector2 left_stick{
      SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) /
          32767.0,
      SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) /
          32767.0};
  Vector2 right_stick{
      SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) /
          32767.0,
      SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) /
          32767.0};
  momentum = momentum / (1.0 + time_delta * 10.0);
  momentum = momentum + left_stick * time_delta;
  shift = shift - momentum * time_delta * 5000.0;
  zoom_momentum = zoom_momentum / (1.0 + time_delta * 10.0);
  zoom_momentum = zoom_momentum - right_stick.y * time_delta * 0.5;
  zoom = zoom * (1.0 + zoom_momentum);
  LOG("Left: %f %f", left_stick.x, left_stick.y);
  LOG("Momentum: %f %f", momentum.x, momentum.y);
  LOG("Shift: %f %f", shift.x, shift.y);
  LOG("Zoom Mom: %f", zoom_momentum);
  LOG("Zoom: %f", zoom);
  // board.individuals[0].position += left_stick * time_delta;
  // board.individuals[0].orientation += right_stick.x * time_delta;
}

int App::run() {
  int result = initialize();
  if (result) {
    return result;
  }
  for (auto &individual : board.individuals) {
    LOG("%f", individual.clock_offset);
  }
  LOG("Number of Joysticks: %d", SDL_NumJoysticks());

  uint64_t last_update = SDL_GetPerformanceCounter();

  while (true) {
    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t milliseconds_passed = now - last_update;
    last_update = now;

    float time_delta =
        static_cast<float>(milliseconds_passed) / SDL_GetPerformanceFrequency();
    LOG("FPS: %f", 1.0 / time_delta);

    process_events(time_delta);
    tick(time_delta);
    render();
    // SDL_Delay(40);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();

  SDL_Quit();
  sceKernelExitProcess(0);
  return 0;
};

int main(int argc, char *argv[]) {
  App app;
  return app.run();
}
