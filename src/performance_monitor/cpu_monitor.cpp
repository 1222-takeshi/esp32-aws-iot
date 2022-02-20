/*
 * cpu_monitor.cpp
 */
#include "performance_monitor/cpu_monitor.h"

int _total_idle_count;
int _core0_idle_count;
int _core1_idle_count;
int _total_tick_count;
int _core0_tick_count;
int _core1_tick_count;
int perf_count;
typedef struct
{
  int _total_idle_count;
  int _core0_idle_count;
  int _core1_idle_count;
  int _total_tick_count;
  int _core0_tick_count;
  int _core1_tick_count;
} COUNTER_t;

float _CPU_idleRate;
float _CORE0_idleRate;
float _CORE1_idleRate;

QueueHandle_t _xQueue;

CpuMonitor::CpuMonitor()
{
  _total_idle_count = 0;
  _core0_idle_count = 0;
  _core1_idle_count = 0;
  _total_tick_count = 0;
  _core0_tick_count = 0;
  _core1_tick_count = 0;
  _CPU_idleRate = 0;
  _CORE0_idleRate = 0;
  _CORE1_idleRate = 0;
  perf_count = 0;
}
bool CpuMonitor::ApplicationIdleHook(void)
{
  _total_idle_count++;
  return true;
}
bool CpuMonitor::Core0IdleHook(void)
{
  _core0_idle_count++;
  return true;
}

bool CpuMonitor::Core1IdleHook(void)
{
  _core1_idle_count++;
  return true;
}
void CpuMonitor::ApplicationTickHook(void)
{
  _total_tick_count++;
}

void CpuMonitor::Core0TickHook(void)
{
  _core0_tick_count++;
}

void CpuMonitor::Core1TickHook(void)
{
  _core1_tick_count++;
}

void CpuMonitor::clearHook(void)
{
  _total_idle_count = 0;
  _core0_idle_count = 0;
  _core1_idle_count = 0;
  _total_tick_count = 0;
  _core0_tick_count = 0;
  _core1_tick_count = 0;
}

void CpuMonitor::monitorTask(void * pvParameters)
{
  COUNTER_t mess;
  while (1) {
    xQueueReceive(_xQueue, &mess, portMAX_DELAY);

    std::cout << "monitor" << std::endl;
    _CPU_idleRate = static_cast<float>(mess._total_idle_count) /
      static_cast<float>(mess._total_tick_count) * 100.0;
    if (_CPU_idleRate > 100.0) {_CPU_idleRate = 100.0;}
    std::cout << "CPU Idle:" << std::fixed << std::setprecision(2) << _CPU_idleRate << std::endl;

    _CORE0_idleRate = static_cast<float>(mess._core0_idle_count) /
      static_cast<float>(mess._core0_tick_count) * 100.0;
    if (_CORE0_idleRate > 100.0) {_CORE0_idleRate = 100.0;}
    std::cout << "CORE0 Idle:" << std::fixed << std::setprecision(2) << _CORE0_idleRate <<
      std::endl;

    _CORE1_idleRate = static_cast<float>(mess._core1_idle_count) /
      static_cast<float>(mess._core1_tick_count) * 100.0;
    if (_CORE1_idleRate > 100.0) {_CORE1_idleRate = 100.0;}
    std::cout << "CORE1 Idle:" << std::fixed << std::setprecision(2) << _CORE1_idleRate <<
      std::endl;
    delay(5000);
  }   // end while

  vTaskDelete(NULL);
}

void CpuMonitor::perfTask(void * pvParameters)
{
  COUNTER_t mess;

  while (1) {
    std::cout << "perf" << std::endl;
    CpuMonitor::clearHook();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    mess._total_idle_count = _total_idle_count;
    mess._core0_idle_count = _core0_idle_count;
    mess._core1_idle_count = _core1_idle_count;
    mess._total_tick_count = _total_tick_count;
    mess._core0_tick_count = _core0_tick_count;
    mess._core1_tick_count = _core1_tick_count;
    xQueueOverwrite(_xQueue, &mess);
  }
}

void CpuMonitor::cpu_monitor_loop()
{
  delay(1);

  /* Create queue */
  _xQueue = xQueueCreate(1, sizeof(COUNTER_t) );
  configASSERT(_xQueue);

  esp_register_freertos_idle_hook(&ApplicationIdleHook);
  esp_register_freertos_tick_hook(&ApplicationTickHook);
  esp_register_freertos_idle_hook_for_cpu(&Core0IdleHook, 0);
  esp_register_freertos_idle_hook_for_cpu(&Core1IdleHook, 1);
  esp_register_freertos_tick_hook_for_cpu(&Core0TickHook, 0);
  esp_register_freertos_tick_hook_for_cpu(&Core1TickHook, 1);
  xTaskCreateUniversal(
    CpuMonitor::perfTask,
    "perfTask",
    4096,
    NULL,
    1,
    NULL,
    APP_CPU_NUM
  );
  perf_count++;
  if (perf_count > 100) {
    vTaskDelete(NULL);
    perf_count = 0;
  }
}
