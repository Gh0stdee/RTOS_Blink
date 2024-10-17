#include <Arduino.h>

int quickDelay = 200 ;
int slowDelay = 1000 ;

TaskHandle_t slow;
TaskHandle_t quick;

void q_Blink(void* parameter)
{
  for(int count1 = 0;;)
  {
    digitalWrite(LED_BUILTIN, 1);
    vTaskDelay(quickDelay / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN,0);
    vTaskDelay(quickDelay / portTICK_PERIOD_MS);  
    count1++;
    if (count1 %2) 
    {
      vTaskResume(slow);
      vTaskSuspend(quick);
    }
  }
  
}

void s_Blink(void* parameter)
{
  for (int count2 = 0; ;)
  {
    digitalWrite(LED_BUILTIN, 1);
    vTaskDelay(slowDelay / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN,0);
    vTaskDelay(slowDelay / portTICK_PERIOD_MS); 
    count2++;
    if (count2 %2) 
    {
      vTaskResume(quick);
      vTaskSuspend(slow);
    }
  }
   
}


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  xTaskCreatePinnedToCore(q_Blink, "Quick_Blink", 1024, NULL, 1, &quick, 1);
  xTaskCreatePinnedToCore(s_Blink, "Slow_Blink", 1024, NULL, 1, &slow, 1);
}


void loop()
{

}