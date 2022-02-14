#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <cmath>
#include <cstdint>
#include <math.h>
#include <vector>

#include <SDL2/SDL.h>
#include <psp2/appmgr.h>
#include <psp2/kernel/clib.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/sysmodule.h>

#include "individual.hpp"
#include "vector.hpp"

#define M_PI 3.14159265358979323846 /* pi */
#define LOG(msg, ...)                                                          \
  do {                                                                         \
    log(msg, ##__VA_ARGS__);                                                   \
  } while (0)
#define LOGF(msg, ...)                                                         \
  do {                                                                         \
    log(fmt::format(msg, ##__VA_ARGS__).c_str());                              \
  } while (0)
static int g_log_socket;
void log(const char *msg, ...) {
  char buffer[512];

  va_list args;
  va_start(args, msg);
  int len = vsnprintf(buffer, sizeof(buffer) - 1, msg, args);
  va_end(args);
  buffer[len] = '\n';

  sceNetSend(g_log_socket, buffer, len + 1, 0);
}
void start_debug_log(void) {

  sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
  static uint8_t netmem[1024 * 1024];
  SceNetInitParam net = {
      .memory = netmem,
      .size = sizeof(netmem),
      .flags = 0,
  };

  sceNetInit(&net);
  g_log_socket = sceNetSocket("log_socket", SCE_NET_AF_INET, SCE_NET_SOCK_DGRAM,
                              SCE_NET_IPPROTO_UDP);

  SceNetSockaddrIn addr{};
  addr.sin_family = SCE_NET_AF_INET;
  addr.sin_port = sceNetHtons(30000);
  sceNetInetPton(SCE_NET_AF_INET, "239.255.0.100", &addr.sin_addr);

  sceNetConnect(g_log_socket, (SceNetSockaddr *)&addr, sizeof(addr));
  LOG("debug logging socket initialized");
}

float randf(float min, float max) {
  float range = max - min;
  return min +
         (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u)) *
             range;
}

// Screen dimension constants
enum { SCREEN_WIDTH = 960, SCREEN_HEIGHT = 544 };

class App {
private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  Vector2 shift;
  float zoom;

  std::vector<Individual> individuals;

  int initialize();

  void render_line(const Vector2 p1, const Vector2 p2);
  void render_individual(const Individual &individual);
  void render();
  void tick(float time_delta);

public:
  App();
  int run();
};

App::App() : shift{480.0f, 277.0f}, zoom{20.0f} {};

int App::initialize() {
  start_debug_log();
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return -1;

  if ((window = SDL_CreateWindow("Evovita", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                 SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) == NULL)
    return -1;

  if ((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL)
    return -1;

  for (int i = 0; i < 15; i++) {
    individuals.emplace_back(1.0,
                             Vector2(randf(-10.0, 10.0), randf(-10.0, 10.0)),
                             randf(0.0, M_PI * 2.0));
    individuals[individuals.size() - 1].clock_offset = randf(0.0, M_PI * 2.0);
  }
  LOG("init done");
  return 0;
}

void App::render_line(const Vector2 p1, const Vector2 p2) {
  Vector2 p1t = p1 * zoom + shift;
  Vector2 p2t = p2 * zoom + shift;
  SDL_RenderDrawLineF(renderer, p1t.x, p1t.y, p2t.x, p2t.y);
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
  render_line(forward, left);
  render_line(forward, right);
  render_line(left, right);
}

void App::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  for (auto &individual : individuals) {
    render_individual(individual);
  }
  SDL_RenderPresent(renderer);
}

void App::tick(float time_delta) {
  for (auto &individual : individuals) {
    float speed =
        (1.0 + std::sin(individual.clock_offset +
                        static_cast<float>(SDL_GetPerformanceCounter()) /
                            SDL_GetPerformanceFrequency()));
    individual.orientation += time_delta * 3.14 / 2.0;
    individual.position =
        individual.position +
        Vector2::from_polar(individual.orientation, speed * time_delta);
  }
}

int App::run() {
  int result = initialize();
  if (result) {
    return result;
  }
  for (auto &individual : individuals) {
    LOG("%f", individual.clock_offset);
  }

  uint64_t last_update = SDL_GetPerformanceCounter();

  while (true) {
    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t milliseconds_passed = now - last_update;
    last_update = now;

    float time_delta =
        static_cast<float>(milliseconds_passed) / SDL_GetPerformanceFrequency();
    LOG("FPS: %f", 1.0 / time_delta);

    tick(time_delta);
    render();
    // SDL_Delay(40);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
  sceKernelExitProcess(0);
  return 0;
};

int main(int argc, char *argv[]) {
  App app;
  return app.run();
}
