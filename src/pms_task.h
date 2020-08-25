
#pragma once

#include <Arduino.h>

#include "PMserial.h"
#include "task.h"


struct PmsState {
    // TODO: include buffer of historical data
    uint16_t current_pm2_5;
};

class PmsTask : public Task<PmsTask> {
    friend class Task<PmsTask>; // Allow base Task to invoke protected run()

    public:
        PmsTask(const uint8_t taskCore);
        ~PmsTask();
        
        PmsState getState();

    protected:
        void run();

    private:
        SerialPM pms_;
        xSemaphoreHandle semaphore_;
        PmsState state_; // Guarded by semaphore_
};
