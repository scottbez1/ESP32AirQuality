
#include <Arduino.h>

#include "pms_task.h"
#include "semaphore_guard.h"

PmsTask::PmsTask(const uint8_t task_core) : Task{"PMS", 8192, 1, task_core}, pms_{PMS5003, Serial2} {
  semaphore_ = xSemaphoreCreateMutex();
  assert(semaphore_ != NULL);
  xSemaphoreGive(semaphore_);
}

PmsTask::~PmsTask() {
  if (semaphore_ != NULL) {
    vSemaphoreDelete(semaphore_);
  }
}

void PmsTask::run() {
    Serial2.begin(9600, SERIAL_8N1, 17, 21);
    pms_.init();

    while(1) {
        pms_.read();
        if (pms_) {
            // print formatted results
            Serial.printf("PM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]\n",
                        pms_.pm01, pms_.pm25, pms_.pm10);

            if (pms_.has_number_concentration())
            Serial.printf("N0.3 %4d, N0.5 %3d, N1.0 %2d, N2.5 %2d, N5.0 %2d, N10 %2d [#/100cc]\n",
                            pms_.n0p3, pms_.n0p5, pms_.n1p0, pms_.n2p5, pms_.n5p0, pms_.n10p0);
            {
                SemaphoreGuard lock(semaphore_);
                state_.current_pm2_5 = pms_.pm25;
            }
        } else {
            // something went wrong
            switch (pms_.status) {
                case SerialPM::OK: // should never come here
                    break;     // included to compile without warnings
                case SerialPM::ERROR_TIMEOUT:
                    Serial.println(F(PMS_ERROR_TIMEOUT));
                    break;
                case SerialPM::ERROR_MSG_UNKNOWN:
                    Serial.println(F(PMS_ERROR_MSG_UNKNOWN));
                    break;
                case SerialPM::ERROR_MSG_HEADER:
                    Serial.println(F(PMS_ERROR_MSG_HEADER));
                    break;
                case SerialPM::ERROR_MSG_BODY:
                    Serial.println(F(PMS_ERROR_MSG_BODY));
                    break;
                case SerialPM::ERROR_MSG_START:
                    Serial.println(F(PMS_ERROR_MSG_START));
                    break;
                case SerialPM::ERROR_MSG_LENGTH:
                    Serial.println(F(PMS_ERROR_MSG_LENGTH));
                    break;
                case SerialPM::ERROR_MSG_CKSUM:
                    Serial.println(F(PMS_ERROR_MSG_CKSUM));
                    break;
                case SerialPM::ERROR_PMS_TYPE:
                    Serial.println(F(PMS_ERROR_PMS_TYPE));
                    break;
            }
        }
        delay(1000);
    }
}

PmsState PmsTask::getState() {
    SemaphoreGuard lock(semaphore_);
    return state_;
}
