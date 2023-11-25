
#ifdef TEST_NIMBLE
#include "NimBLEDevice.h"
#include "messages.hpp"
#include "lv_setup.hpp"
#include "events.hpp"

int scanTime = 60 * 1000; // In milliseconds, 0 = scan forever
BLEScan *pBLEScan;
bool active = false;

static ruuviAd_t ruuvi_report;
static double oat_tmp, oat_hum, env_tmp, env_hum;

#include "NimBLEDevice.h"

int getShort(const byte *data, int index)
{
    return (short)((data[index] << 8) + (data[index + 1]));
}

int getShortone(const byte *data, int index) { return (short)((data[index])); }

unsigned int getUShort(const byte *data, int index)
{
    return (unsigned short)((data[index] << 8) + (data[index + 1]));
}

unsigned int getUShortone(const byte *data, int index)
{
    return (unsigned short)((data[index]));
}

void DecodeV5(const byte *data, ruuviAd_t *ra)
{
    ra->ruuvi_format = 5;
    ra->temperature = (double)getShort(data, 3) * 0.005;
    ra->humidity = (double)getUShort(data, 5) * 0.0025;
    ra->pressure = (double)getUShort(data, 7) / 100 + 500;
    ra->accelX = getShort(data, 9);
    ra->accelY = getShort(data, 11);
    ra->accelZ = getShort(data, 13);
    ra->voltage = (data[15] << 3 | data[16] >> 5) + 1600;
    ra->power = (data[16] & 0x1F) * 2 - 40;
    ra->moveCount = getUShortone(data, 17);
    ra->sequence = getUShort(data, 18);
}

void DecodeV3(const byte *data, ruuviAd_t *ra)
{
    ra->ruuvi_format = 3;
    double t = (double)(getUShortone(data, 4) & 0b01111111) +
               (double)getUShortone(data, 5) / 100;
    ra->temperature = (getUShortone(data, 4) & 0b10000000) == 128 ? t * -1 : t;
    ra->humidity = (double)getUShortone(data, 3) * 0.5;
    ra->pressure = (double)getUShort(data, 6) / 100 + 500;
    ra->accelX = getShort(data, 8);
    ra->accelY = getShort(data, 10);
    ra->accelZ = getShort(data, 12);
    ra->voltage = (short)getUShort(data, 14);
}

class scanCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(NimBLEAdvertisedDevice *advertisedDevice)
    {
        // Serial.printf("Advertised Device Result: %s \n",
        //               advertisedDevice->toString().c_str());
        if (advertisedDevice->haveManufacturerData())
        {
            const uint8_t *data =
                (const uint8_t *)advertisedDevice->getManufacturerData().data();
            size_t len = advertisedDevice->getManufacturerData().length();

            uint16_t mfid = data[1] << 8 | data[0];

            // HexDump(Serial, (void *)data, len);

            switch (mfid)
            {

            case 0x0499:
            {
                ruuviAd_t *ruuvi_ad = &ruuvi_report;
                // https://mybeacons.info/packetFormats.html
                if (data[2] == 0x3 && len > 15)
                {
                    DecodeV3(data, ruuvi_ad);
                    goto RUUVI_DECODED;
                }

                if (data[2] == 0x5 && len > 19)
                {
                    DecodeV5(data, ruuvi_ad);
                    goto RUUVI_DECODED;
                }
                Serial.printf("%s: failed to decode ruuvi msg\n",
                              advertisedDevice->getAddress().toString().c_str());
                return;
            RUUVI_DECODED:
                ruuvi_ad->rssi = advertisedDevice->getRSSI();

                memcpy(&ruuvi_ad->address, advertisedDevice->getAddress().toString().c_str(), sizeof(ruuvi_ad->address));
                if (advertisedDevice->haveName())
                {
                    strncpy(ruuvi_ad->name, advertisedDevice->getName().c_str(),
                            sizeof(ruuvi_ad->name));
                }

                Serial.printf("ruuvi addr=%s temp=%3.2f hum=%.2f rssi=%d "
                              "volt=%1.3f sequence=%d \n",
                              ruuvi_ad->address,
                              ruuvi_ad->temperature, ruuvi_ad->humidity, ruuvi_ad->rssi,
                              ruuvi_ad->voltage / 1000.0, ruuvi_ad->sequence);

                // from a non-lvgl thread, use lvgl_msg_send_prot()
                // dd:79:c6:8f:bd:a2 oat hum
                // e6:91:df:7b:e5:4d env
                if (strcasecmp(ruuvi_ad->address, RUUVI_ENV) == 0)
                {
                    env_tmp = ruuvi_ad->temperature;
                    env_hum = ruuvi_ad->humidity;
                    lvgl_msg_send_prot(MSG_ENV_TEMP_UPDATE, &env_tmp);
                    lvgl_msg_send_prot(MSG_ENV_HUM_UPDATE, &env_hum);
                }
                if (strcasecmp(ruuvi_ad->address, RUUVI_OAT) == 0)
                {
                    oat_tmp = ruuvi_ad->temperature;
                    oat_hum = ruuvi_ad->humidity;
                    lvgl_msg_send_prot(MSG_OAT_TEMP_UPDATE, &oat_tmp);
                    lvgl_msg_send_prot(MSG_OAT_HUM_UPDATE, &oat_hum);
                }
            }
            break;

                // case 0x0059: // nRF/Mopeka Pro

                // done with have manufacturer ID
            }
        }
    }

    void
    onScanEnd(NimBLEScanResults results)
    {
        Serial.println("Scan Ended");
        // active = !active;
        pBLEScan->setActiveScan(active);
        Serial.printf("scan start, active = %u\n", active);
        pBLEScan->start(scanTime);
    }
};

void startBLEscan(void)
{

    Serial.println("start BLE scan...");

    NimBLEDevice::init("");
    pBLEScan = NimBLEDevice::getScan();
#ifdef NIMBLE_OLDAPI
    pBLEScan->setAdvertisedDeviceCallbacks(new scanCallbacks());
#else
    pBLEScan->setScanCallbacks(new scanCallbacks());
#endif
    pBLEScan->setActiveScan(active);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    pBLEScan->start(scanTime);
}

#else
void startBLEscan(void) {}
#endif
