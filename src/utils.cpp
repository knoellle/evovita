#include "utils.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

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

float angle_difference(float a, float b) {
  // const auto c = M_PI;
  // return c - fabs(fmod(fabs(a - b), 2 * c) - c);
  return fmod(a - b + M_PI, M_PI * 2.0) - M_PI;
}

float clamp(float input, float min, float max) {
  return input <= max ? (input >= min ? input : min) : max;
}
