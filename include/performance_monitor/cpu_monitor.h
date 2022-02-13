/*
 * cpu_monitor.h
 */
#ifndef PERFORMANCE_MONITOR_CPU_MONITOR_H_
#define PERFORMANCE_MONITOR_CPU_MONITOR_H_
#include <iostream>
#include <iomanip>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"
#include "WiFi.h"
#include "esp_freertos_hooks.h"
#include "esp_err.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
class CpuMonitor
{
public:
  CpuMonitor();


  static bool ApplicationIdleHook(void);
  static bool Core0IdleHook(void);
  static bool Core1IdleHook(void);
  static void ApplicationTickHook(void);
  static void Core0TickHook(void);
  static void Core1TickHook(void);

  static void clearHook(void);

  static void monitorTask(void * pvParameters);

  static void perfTask(void * pvParameters);

  void cpu_monitor_setup();
  void cpu_monitor_publisher();

private:
  SemaphoreHandle_t _xSemaphore0;
  SemaphoreHandle_t _xSemaphore1;
  SemaphoreHandle_t _xSemaphore2;

};

#endif  // PERFORMANCE_MONITOR_CPU_MONITOR_H_
