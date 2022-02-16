#pragma once

#include <cmath>

#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/sysmodule.h>

#define M_PI 3.14159265358979323846

#define LOG(msg, ...)                                                          \
  do {                                                                         \
    log(msg, ##__VA_ARGS__);                                                   \
  } while (0)
#define LOGF(msg, ...)                                                         \
  do {                                                                         \
    log(fmt::format(msg, ##__VA_ARGS__).c_str());                              \
  } while (0)

static int g_log_socket;

void log(const char *msg, ...);
void start_debug_log(void);

float randf(float min, float max);

float angle_difference(float a, float b);

float clamp(float input, float min, float max);
