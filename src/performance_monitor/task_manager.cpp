#include "performance_monitor/task_manager.h"

#include <Arduino.h>
#include <esp_task_wdt.h>
#include <freertos/task.h>


//-----------------------------------------------------------------------------

TaskHandle_t loop2TaskHandle = NULL;
static bool loopTask2WDTEnabled = false;

void enableLoop2WDT()
{
  if (loop2TaskHandle != NULL) {
    if (esp_task_wdt_add(loop2TaskHandle) != ESP_OK) {
      log_e("Failed to add loop2 task to WDT");
    } else {
      loopTask2WDTEnabled = true;
    }
  }
}

//-----------------------------------------------------------------------------

void disableLoop2WDT()
{
  if (loop2TaskHandle != NULL && loopTask2WDTEnabled) {
    loopTask2WDTEnabled = false;
    if (esp_task_wdt_delete(loop2TaskHandle) != ESP_OK) {
      log_e("Failed to remove loop2 task from WDT");
    }
  }
}

//-----------------------------------------------------------------------------
