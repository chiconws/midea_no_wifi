#include "esphome.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"

using namespace esphome;
using namespace esphome::climate;

class MyCustomClimate : public esphome::climate::Climate, public esphome::Component {
  public:
    void setup() override {
      // This will be called by App.setup()
    }
    
    esphome::climate::ClimateTraits traits() override {
      auto traits = esphome::climate::ClimateTraits();
      traits.set_supported_modes({esphome::climate::CLIMATE_MODE_COOL, esphome::climate::CLIMATE_MODE_OFF});
      traits.set_supported_fan_modes({esphome::climate::CLIMATE_FAN_LOW, esphome::climate::CLIMATE_FAN_MEDIUM, esphome::climate::CLIMATE_FAN_HIGH, esphome::climate::CLIMATE_FAN_AUTO});
      traits.set_visual_temperature_step(1.0f);
      traits.set_visual_min_temperature(17.0f);
      traits.set_visual_max_temperature(30.0f);
      return traits;
    }
    
    void control(const esphome::climate::ClimateCall &call) override {
      if (call.get_mode().has_value()) {
        esphome::climate::ClimateMode mode = *call.get_mode();
        if (mode == esphome::climate::CLIMATE_MODE_OFF) {
          send_hex("AA:91:0D:B2:4D:7B:84:E0:1F:B2:4D:7B:84:E0:1F:00:BE");
        } else if (mode == esphome::climate::CLIMATE_MODE_COOL) {
          // Handle temperature and fan mode changes below
        }
        this->mode = mode;
        this->publish_state();
      }
      
      if (call.get_target_temperature().has_value()) {
        float temp = *call.get_target_temperature();
        if (call.get_fan_mode().has_value()) {
          esphome::climate::ClimateFanMode fan_mode = *call.get_fan_mode();
          send_temperature_command(temp, fan_mode);
        }
        this->target_temperature = temp;
        this->publish_state();
      }

      if (call.get_fan_mode().has_value()) {
        esphome::climate::ClimateFanMode fan_mode = *call.get_fan_mode();
        if (call.get_target_temperature().has_value()) {
          float temp = *call.get_target_temperature();
          send_temperature_command(temp, fan_mode);
        }
        this->fan_mode = fan_mode;
        this->publish_state();
      }
    }

    void send_temperature_command(float temp, esphome::climate::ClimateFanMode fan_mode) {
      std::string hex;
      if (fan_mode == esphome::climate::CLIMATE_FAN_LOW) {
        hex = get_hex_for_temp_and_fan(temp, "low");
      } else if (fan_mode == esphome::climate::CLIMATE_FAN_MEDIUM) {
        hex = get_hex_for_temp_and_fan(temp, "med");
      } else if (fan_mode == esphome::climate::CLIMATE_FAN_HIGH) {
        hex = get_hex_for_temp_and_fan(temp, "high");
      } else if (fan_mode == esphome::climate::CLIMATE_FAN_AUTO) {
        hex = get_hex_for_temp_and_fan(temp, "auto");
      }
      send_hex(hex.c_str());
    }

    std::string get_hex_for_temp_and_fan(float temp, const std::string &fan) {
      if (fan == "low") {
        if (temp == 30) return "AA:91:0D:B2:4D:9F:60:B0:4F:B2:4D:9F:60:B0:4F:00:BE";
        if (temp == 29) return "AA:91:0D:B2:4D:9F:60:A0:5F:B2:4D:9F:60:A0:5F:00:BE";
        if (temp == 28) return "AA:91:0D:B2:4D:9F:60:80:7F:B2:4D:9F:60:80:7F:00:BE";
        if (temp == 27) return "AA:91:0D:B2:4D:9F:60:90:6F:B2:4D:9F:60:90:6F:00:BE";
        if (temp == 26) return "AA:91:0D:B2:4D:9F:60:D0:2F:B2:4D:9F:60:D0:2F:00:BE";
        if (temp == 25) return "AA:91:0D:B2:4D:9F:60:C0:3F:B2:4D:9F:60:C0:3F:00:BE";
        if (temp == 24) return "AA:91:0D:B2:4D:9F:60:40:BF:B2:4D:9F:60:40:BF:00:BE";
        if (temp == 23) return "AA:91:0D:B2:4D:9F:60:50:AF:B2:4D:9F:60:50:AF:00:BE";
        if (temp == 22) return "AA:91:0D:B2:4D:9F:60:70:8F:B2:4D:9F:60:70:8F:00:BE";
        if (temp == 21) return "AA:91:0D:B2:4D:9F:60:60:9F:B2:4D:9F:60:60:9F:00:BE";
        if (temp == 20) return "AA:91:0D:B2:4D:9F:60:20:DF:B2:4D:9F:60:20:DF:00:BE";
        if (temp == 19) return "AA:91:0D:B2:4D:9F:60:30:CF:B2:4D:9F:60:30:CF:00:BE";
        if (temp == 18) return "AA:91:0D:B2:4D:9F:60:10:EF:B2:4D:9F:60:10:EF:00:BE";
        if (temp == 17) return "AA:91:0D:B2:4D:9F:60:00:FF:B2:4D:9F:60:00:FF:00:BE";
      } else if (fan == "med") {
        if (temp == 30) return "AA:91:0D:B2:4D:5F:A0:B0:4F:B2:4D:5F:A0:B0:4F:00:BE";
        if (temp == 29) return "AA:91:0D:B2:4D:5F:A0:A0:5F:B2:4D:5F:A0:A0:5F:00:BE";
        if (temp == 28) return "AA:91:0D:B2:4D:5F:A0:80:7F:B2:4D:5F:A0:80:7F:00:BE";
        if (temp == 27) return "AA:91:0D:B2:4D:5F:A0:90:6F:B2:4D:5F:A0:90:6F:00:BE";
        if (temp == 26) return "AA:91:0D:B2:4D:5F:A0:D0:2F:B2:4D:5F:A0:D0:2F:00:BE";
        if (temp == 25) return "AA:91:0D:B2:4D:5F:A0:C0:3F:B2:4D:5F:A0:C0:3F:00:BE";
        if (temp == 24) return "AA:91:0D:B2:4D:5F:A0:40:BF:B2:4D:5F:A0:40:BF:00:BE";
        if (temp == 23) return "AA:91:0D:B2:4D:5F:A0:50:AF:B2:4D:5F:A0:50:AF:00:BE";
        if (temp == 22) return "AA:91:0D:B2:4D:5F:A0:70:8F:B2:4D:5F:A0:70:8F:00:BE";
        if (temp == 21) return "AA:91:0D:B2:4D:5F:A0:60:9F:B2:4D:5F:A0:60:9F:00:BE";
        if (temp == 20) return "AA:91:0D:B2:4D:5F:A0:20:DF:B2:4D:5F:A0:20:DF:00:BE";
        if (temp == 19) return "AA:91:0D:B2:4D:5F:A0:30:CF:B2:4D:5F:A0:30:CF:00:BE";
        if (temp == 18) return "AA:91:0D:B2:4D:5F:A0:10:EF:B2:4D:5F:A0:10:EF:00:BE";
        if (temp == 17) return "AA:91:0D:B2:4D:5F:A0:00:FF:B2:4D:5F:A0:00:FF:00:BE";
      } else if (fan == "high") {
        if (temp == 30) return "AA:91:0D:B2:4D:3F:C0:B0:4F:B2:4D:3F:C0:B0:4F:00:BE";
        if (temp == 29) return "AA:91:0D:B2:4D:3F:C0:A0:5F:B2:4D:3F:C0:A0:5F:00:BE";
        if (temp == 28) return "AA:91:0D:B2:4D:3F:C0:80:7F:B2:4D:3F:C0:80:7F:00:BE";
        if (temp == 27) return "AA:91:0D:B2:4D:3F:C0:90:6F:B2:4D:3F:C0:90:6F:00:BE";
        if (temp == 26) return "AA:91:0D:B2:4D:3F:C0:D0:2F:B2:4D:3F:C0:D0:2F:00:BE";
        if (temp == 25) return "AA:91:0D:B2:4D:3F:C0:C0:3F:B2:4D:3F:C0:C0:3F:00:BE";
        if (temp == 24) return "AA:91:0D:B2:4D:3F:C0:40:BF:B2:4D:3F:C0:40:BF:00:BE";
        if (temp == 23) return "AA:91:0D:B2:4D:3F:C0:50:AF:B2:4D:3F:C0:50:AF:00:BE";
        if (temp == 22) return "AA:91:0D:B2:4D:3F:C0:70:8F:B2:4D:3F:C0:70:8F:00:BE";
        if (temp == 21) return "AA:91:0D:B2:4D:3F:C0:60:9F:B2:4D:3F:C0:60:9F:00:BE";
        if (temp == 20) return "AA:91:0D:B2:4D:3F:C0:20:DF:B2:4D:3F:C0:20:DF:00:BE";
        if (temp == 19) return "AA:91:0D:B2:4D:3F:C0:30:CF:B2:4D:3F:C0:30:CF:00:BE";
        if (temp == 18) return "AA:91:0D:B2:4D:3F:C0:10:EF:B2:4D:3F:C0:10:EF:00:BE";
        if (temp == 17) return "AA:91:0D:B2:4D:3F:C0:00:FF:B2:4D:3F:C0:00:FF:00:BE";
      } else if (fan == "auto") {
        if (temp == 30) return "AA:91:0D:B2:4D:BF:40:B0:4F:B2:4D:BF:40:B0:4F:00:BE";
        if (temp == 29) return "AA:91:0D:B2:4D:BF:40:A0:5F:B2:4D:BF:40:A0:5F:00:BE";
        if (temp == 28) return "AA:91:0D:B2:4D:BF:40:80:7F:B2:4D:BF:40:80:7F:00:BE";
        if (temp == 27) return "AA:91:0D:B2:4D:BF:40:90:6F:B2:4D:BF:40:90:6F:00:BE";
        if (temp == 26) return "AA:91:0D:B2:4D:BF:40:D0:2F:B2:4D:BF:40:D0:2F:00:BE";
        if (temp == 25) return "AA:91:0D:B2:4D:BF:40:C0:3F:B2:4D:BF:40:C0:3F:00:BE";
        if (temp == 24) return "AA:91:0D:B2:4D:BF:40:40:BF:B2:4D:BF:40:40:BF:00:BE";
        if (temp == 23) return "AA:91:0D:B2:4D:BF:40:50:AF:B2:4D:BF:40:50:AF:00:BE";
        if (temp == 22) return "AA:91:0D:B2:4D:BF:40:70:8F:B2:4D:BF:40:70:8F:00:BE";
        if (temp == 21) return "AA:91:0D:B2:4D:BF:40:60:9F:B2:4D:BF:40:60:9F:00:BE";
        if (temp == 20) return "AA:91:0D:B2:4D:BF:40:20:DF:B2:4D:BF:40:20:DF:00:BE";
        if (temp == 19) return "AA:91:0D:B2:4D:BF:40:30:CF:B2:4D:BF:40:30:CF:00:BE";
        if (temp == 18) return "AA:91:0D:B2:4D:BF:40:10:EF:B2:4D:BF:40:10:EF:00:BE";
        if (temp == 17) return "AA:91:0D:B2:4D:BF:40:00:FF:B2:4D:BF:40:00:FF:00:BE";
      }
      return "";
    }

    void send_hex(const char *hex) {
      std::vector<uint8_t> data;
      while (*hex) {
        uint8_t byte = strtol(hex, NULL, 16);
        data.push_back(byte);
        hex += 3; // Skip 2 hex digits and the colon
      }
      // Assuming IR transmitter is on GPIO pin 14
      auto ir_tx = esphome::remote_base::transmitter::remote_transmitter->transmit_raw(data, 38000);
      if (ir_tx != nullptr) {
        ir_tx->send();
      }
    }
};
