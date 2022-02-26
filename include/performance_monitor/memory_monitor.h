/*
 * memory_monitor.h
 */
#ifndef PERFORMANCE_MONITOR_MEMORY_MONITOR_H_
#define PERFORMANCE_MONITOR_MEMORY_MONITOR_H_
#include <Arduino.h>

#include <esp_task_wdt.h>
#include "performance_monitor/task_manager.h"
class MemoryMonitor
{
public:
  MemoryMonitor();
  void setup();
  void loop();
  void getMemoryUsage();
  uint32_t memory_water_mark_;
  uint32_t free_heap_memory_;
};

#endif  // PERFORMANCE_MONITOR_MEMORY_MONITOR_H_
