#include "zmpt101b.h"
#include <cmath>
#include "esphome/core/log.h"

namespace esphome {
namespace zmpt101b {

static const char *const TAG = "zmpt101b";

int ZMPT101BSensor::getZeroPoint_() {
	uint32_t Vsum = 0;
	uint32_t measurements_count = 0;
	uint32_t t_start = micros();

	while (micros() - t_start < this->period_) {
		Vsum += adc_sensor_->sample() * ADC_SCALE;
		measurements_count++;
	}

	return measurements_count ? (Vsum / measurements_count) : 0;
}

void ZMPT101BSensor::update() {
	double readingVoltage = 0.0;

	for (uint8_t i = 0; i < this->loop_count_; i++) {
		int zeroPoint = this->getZeroPoint_();

		int32_t Vnow = 0;
		uint32_t Vsum = 0;
		uint32_t measurements_count = 0;

		uint32_t t_start = micros();
		while (micros() - t_start < this->period_) {
			Vnow = (adc_sensor_->sample() * ADC_SCALE) - zeroPoint;
			Vsum += Vnow * Vnow;
			measurements_count++;
		}

		double v_out_rms = sqrt((double)Vsum / (double)measurements_count) / ADC_SCALE * VREF;
		double mains_v_rms = v_out_rms * (1000.0 / this->sensitivity_);
		readingVoltage += mains_v_rms;
	}

	this->publish_state(readingVoltage / this->loop_count_);
}

}  // namespace zmpt101b
}  // namespace esphome
