
#ifdef TEST_NIMBLE
#include "NimBLEDevice.h"
#include "blescan.hpp"
#include <algorithm>

static int scanTime = 60 * 1000;
static BLEScan *pBLEScan;
static uint32_t queue_full;

QueueHandle_t bleadv_queue;

class scanCallbacks : public BLEAdvertisedDeviceCallbacks {

    void onResult(NimBLEAdvertisedDevice *advertisedDevice) {
        if (advertisedDevice->haveManufacturerData()) {
            const uint8_t *data =
                (const uint8_t *)advertisedDevice->getManufacturerData().data();
            size_t len = advertisedDevice->getManufacturerData().length();
            bleAdvMsg_t ble_adv;

            uint16_t mfid = data[1] << 8 | data[0];
            switch (mfid) {

                case 0x0499: // Ruuvi manufacturer ID
                case 0x0059: // Mopeka manufacturer ID
                case 0x0100: // TPMS manufacturer ID variant 1
                case 0x00AC: // TPMS manufacturer ID variant 2 - TPMS3_, hijacked TomTom
                    // log_d("Advertised Device Result: %s",
                    //       advertisedDevice->toString().c_str());
                    ble_adv.mfid = mfid;
                    ble_adv.msg_size = len;
                    ble_adv.mac64 = (uint64_t) advertisedDevice->getAddress();
                    ble_adv.rssi =  advertisedDevice->getRSSI();
                    memcpy(ble_adv.message, data, std::min(sizeof(ble_adv.message), len));
                    if (xQueueSend(bleadv_queue, (void *)&ble_adv, sizeof(ble_adv)) != pdTRUE) {
                        queue_full++;
                    }
                    break;
                default:
                    ;
            }
        }
    }

    void onScanEnd(NimBLEScanResults results) {
        pBLEScan->setActiveScan(false);
        pBLEScan->start(scanTime);
    }
};

void ble_setup(void) {

    log_d("start BLE scan.");
    bleadv_queue = xQueueCreate(BLE_ADV_QUEUELEN, sizeof(bleAdvMsg_t));

    NimBLEDevice::init("");
    pBLEScan = NimBLEDevice::getScan();
#ifdef NIMBLE_OLDAPI
    pBLEScan->setAdvertisedDeviceCallbacks(new scanCallbacks());
#else
    pBLEScan->setScanCallbacks(new scanCallbacks());
#endif
    pBLEScan->setActiveScan(false);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
#ifdef NIMBLE_OLDAPI
    pBLEScan->start(scanTime/1000, nullptr, false);
#else
    pBLEScan->start(scanTime);
#endif
}

void ble_loop(void) {
    if(pBLEScan->isScanning() == false) {
        // Start scan with: duration = 0 seconds(forever), no scan end callback, not a continuation of a previous scan.
#ifdef NIMBLE_OLDAPI
        pBLEScan->start(scanTime/1000, nullptr, false);
#else
        pBLEScan->start(scanTime);
#endif
    }

}

#else
void ble_setup(void) {}
void ble_loop(void) {}

#endif
