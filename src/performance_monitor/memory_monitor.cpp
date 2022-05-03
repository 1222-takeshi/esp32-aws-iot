/*
 * memory_monitor.cpp
 */
#include "performance_monitor/memory_monitor.h"

MemoryMonitor::MemoryMonitor()
{
  memory_water_mark_ = 0;
  free_heap_memory_ = 0;
}

void MemoryMonitor::getMemoryUsage()
{
  static uint32_t m1 = UINT32_MAX, mh = UINT32_MAX;
  extern TaskHandle_t loopTaskHandle;
//   extern TaskHandle_t loop2TaskHandle;

  uint32_t m;
  if (loopTaskHandle != NULL) {
    m = uxTaskGetStackHighWaterMark(loopTaskHandle);
    if (m < m1) {
      m1 = m;
      memory_water_mark_ = m;
      Serial.printf(
        "%s water mark = %d\n", pcTaskGetTaskName(loopTaskHandle),
        m);
    }
  }
//   if (loop2TaskHandle != NULL) {
//     m = uxTaskGetStackHighWaterMark(loop2TaskHandle);
//     if (m < m2) {
//       m2 = m;
//       Serial.printf(
//         "%s water mark = %d\n", pcTaskGetTaskName(loop2TaskHandle),
//         m);
//     }
//   }
  m = esp_get_minimum_free_heap_size();
  if (m < mh) {
    mh = m;
    free_heap_memory_ = m;
    Serial.printf("free heap memory = %d\n", m);
  }
}
void MemoryMonitor::setup()
{
  esp_task_wdt_init(60, true);  // time in seconds
  enableLoopWDT();

  delay(100);
  MemoryMonitor::getMemoryUsage();
}

void MemoryMonitor::loop()
{
  // put your main code here, to run repeatedly:
  MemoryMonitor::getMemoryUsage();
  delay(1000);
}
