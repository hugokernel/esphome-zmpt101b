import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_NAME, CONF_UNIT_OF_MEASUREMENT

from esphome.components import sensor
adc_ns = cg.esphome_ns.namespace("adc")
ADCSensor = adc_ns.class_("ADCSensor", sensor.Sensor)

CODEOWNERS = ["@toniotruel"]

CONF_ADC_ID = "adc_id"
CONF_SENSITIVITY = "sensitivity"
CONF_FREQUENCY = "frequency"
CONF_LOOP_COUNT = "loop_count"

zmpt101b_ns = cg.esphome_ns.namespace("zmpt101b")
ZMPT101BSensor = zmpt101b_ns.class_("ZMPT101BSensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement="V",
    accuracy_decimals=1,
    state_class="measurement",
).extend({
    cv.GenerateID(): cv.declare_id(ZMPT101BSensor),
    cv.Required(CONF_ADC_ID): cv.use_id(ADCSensor),
    cv.Required(CONF_SENSITIVITY): cv.float_,
    cv.Optional(CONF_FREQUENCY, default=50): cv.positive_int,
    cv.Optional(CONF_LOOP_COUNT, default=50): cv.positive_int,
}).extend(cv.polling_component_schema('30s'))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await sensor.register_sensor(var, config)
    await cg.register_component(var, config)

    adc = await cg.get_variable(config[CONF_ADC_ID])
    cg.add(var.set_adc_sensor(adc))
    cg.add(var.set_sensitivity(config[CONF_SENSITIVITY]))
    cg.add(var.set_frequency(config[CONF_FREQUENCY]))
    cg.add(var.set_loop_count(config[CONF_LOOP_COUNT]))
