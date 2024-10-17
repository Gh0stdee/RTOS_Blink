#include <Arduino.h>

//set up the blinking speed (in ms)
int quickDelay = 200 ;
int intermediateDelay = 500;
int slowDelay = 1000 ;

//variable to control switching of tasks
int count = 0;

//create corresponding task handles for suspend and resume
TaskHandle_t slow;
TaskHandle_t intermediate;
TaskHandle_t quick;

// Normal blinking sequence: quick->intermediate->slow->quick...
// Abnomral blinking sequence(count is multiple of 3): slow->intermediate->quick->slow...
void q_Blink(void* parameter)
{
  for(;;)
  {
    digitalWrite(LED_BUILTIN, 1);
    vTaskDelay(quickDelay / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN,0);
    vTaskDelay(quickDelay / portTICK_PERIOD_MS);  
    count++;
    if (count %3) //abnormal
    {
      vTaskResume(slow);
      vTaskSuspend(intermediate);
    }
    else //normal
    {
      vTaskResume(intermediate);
      vTaskSuspend(slow);
    }
    vTaskSuspend(NULL); //suspend current task
  }
  
}

void i_Blink(void* parameter)
{
  for(;;)
  {
    digitalWrite(LED_BUILTIN, 1);
    vTaskDelay(intermediateDelay / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN,0);
    vTaskDelay(intermediateDelay / portTICK_PERIOD_MS);  
    count++;
    if (count %3) //abnomral
    {
      vTaskResume(slow);
      vTaskSuspend(quick);
    }
    else //normal
    {
      vTaskResume(quick);
      vTaskSuspend(slow);
    }
    vTaskSuspend(NULL); //suspend current task
  }
  
}

void s_Blink(void* parameter)
{
  for (;;)
  {
    digitalWrite(LED_BUILTIN, 1);
    vTaskDelay(slowDelay / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN,0);
    vTaskDelay(slowDelay / portTICK_PERIOD_MS); 
    count++;
    if (count % 3) //abnoraml
    {
      vTaskResume(intermediate);
      vTaskSuspend(quick);
    }
    else{ //normal
      vTaskResume(quick);
      vTaskSuspend(intermediate);
    }
      
    vTaskSuspend(NULL); //suspend current task
  }
   
}


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  xTaskCreatePinnedToCore(q_Blink, "Quick_Blink", 1024, NULL, 1, &quick, 1);
  xTaskCreatePinnedToCore(i_Blink, "Intermediate_Blink", 1024, NULL, 1, &intermediate, 1);
  xTaskCreatePinnedToCore(s_Blink, "Slow_Blink", 1024, NULL, 1, &slow, 1);
}


void loop()
{

}