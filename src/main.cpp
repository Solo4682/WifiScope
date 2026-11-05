#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("=== WiFiScope Memory Test ===");

    Serial.printf("Flash: %lu MB\n",
        ESP.getFlashChipSize() / (1024UL * 1024UL));

    Serial.printf("PSRAM: %lu MB\n",
        ESP.getPsramSize() / (1024UL * 1024UL));

    Serial.printf("psramFound(): %s\n",
        psramFound() ? "YES" : "NO");

#ifdef BOARD_HAS_PSRAM
    Serial.println("BOARD_HAS_PSRAM: DEFINED");
#else
    Serial.println("BOARD_HAS_PSRAM: NOT DEFINED");
#endif
}

void loop()
{
}