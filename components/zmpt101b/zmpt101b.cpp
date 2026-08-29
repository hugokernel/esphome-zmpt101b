#include "zmpt101b.h"
#include <cmath>
#include "esphome/core/log.h"

namespace esphome {
namespace zmpt101b {

static const char *const TAG = "zmpt101b";

void ZMPT101BSensor::update() {
	// Snap the sampling window to a whole number of mains cycles to reduce RMS error.
	uint32_t window = this->measurement_duration_;
	if (this->period_ > 0) {
		uint32_t cycles = this->measurement_duration_ / this->period_;
		if (cycles == 0) cycles = 1;
		window = cycles * this->period_;
	}

	double sum = 0.0;
	double sum_sq = 0.0;
	uint32_t measurements_count = 0;

	// Single pass: accumulate sum and sum-of-squares to derive both the DC
	// offset (mean) and the AC RMS (sqrt of variance) without a separate pass.
	uint32_t t_start = micros();
	while (micros() - t_start < window) {
		double v = adc_sensor_->sample() * ADC_SCALE;
		sum += v;
		sum_sq += v * v;
		measurements_count++;
	}

	if (measurements_count == 0) {
		ESP_LOGW(TAG, "No ADC samples collected");
		return;
	}

	double mean = sum / measurements_count;
	double variance = (sum_sq / measurements_count) - (mean * mean);
	if (variance < 0.0) variance = 0.0;  // guard against floating point rounding

	double v_out_rms = sqrt(variance) / ADC_SCALE * VREF;
	double mains_v_rms = v_out_rms * (1000.0 / this->sensitivity_);

	this->publish_state(mains_v_rms);
}

}  // namespace zmpt101b
}  // namespace esphome
