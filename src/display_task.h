#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

#include "pms_task.h"
#include "task.h"

class DisplayTask : public Task<DisplayTask> {
    friend class Task<DisplayTask>; // Allow base Task to invoke protected run()

    public:
        DisplayTask(PmsTask& pms_task, const uint8_t task_core);

    protected:
        void run();

    private:
        PmsTask& pms_task_;
        TFT_eSPI tft_ = TFT_eSPI();

        /** Full-size sprite used as a framebuffer */
        TFT_eSprite spr_ = TFT_eSprite(&tft_);
};
