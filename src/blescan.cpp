
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

                case 0x0499:
                case 0x0059:
                case 0x0100:
                case 172:
                    // log_d("Advertised Device Result: %s",
                    //       advertisedDevice->toString().c_str());
                    // TPMS mfids
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

void startBLEscan(void) {

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
    pBLEScan->start(scanTime);
}

#else
void startBLEscan(void) {}
#endif
