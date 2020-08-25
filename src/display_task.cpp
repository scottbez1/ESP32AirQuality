#include "display_task.h"

DisplayTask::DisplayTask(PmsTask& pms_task, const uint8_t task_core) : Task{"Display", 8192, 1, task_core}, pms_task_{pms_task} {}

void DisplayTask::run() {
    tft_.begin();
    tft_.invertDisplay(1);
    tft_.setRotation(0);

    spr_.setColorDepth(16);
    spr_.createSprite(TFT_WIDTH, TFT_HEIGHT);
    spr_.setFreeFont(&Roboto_Thin_24);
    spr_.setTextColor(0xFFFF, TFT_BLACK);
    
    while(1) {
        PmsState state = pms_task_.getState();

        spr_.fillSprite(TFT_BLACK);
        spr_.setCursor(0, 40);
        spr_.printf("PM2.5:\n%u ug/m3", state.current_pm2_5);

        spr_.pushSprite(0, 0);
        delay(10);
    }
}
