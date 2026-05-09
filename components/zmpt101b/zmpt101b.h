#pragma once

#include "esphome/core/component.h"
#include "esphome/components/adc/adc_sensor.h"

namespace esphome {
namespace zmpt101b {

#define ADC_SCALE 4095
#define VREF 3.3

class ZMPT101BSensor : public sensor::Sensor, public PollingComponent {
 public:
  void set_adc_sensor(adc::ADCSensor *sensor) { adc_sensor_ = sensor; }
  void set_frequency(uint16_t freq) { period_ = 1000000UL / freq; }
  void set_sensitivity(float sens) { sensitivity_ = sens; }
  void set_loop_count(float sens) { loop_count_ = sens; }

  void setup() override {}
  void update() override;

 protected:
  adc::ADCSensor *adc_sensor_;
  uint32_t period_;
  float sensitivity_;
  int loop_count_;

  int getZeroPoint_();
};

}  // namespace zmpt101b
}  // namespace esphome
