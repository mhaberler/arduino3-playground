
#ifdef TEST_NIMBLE
#include "NimBLEDevice.h"
int scanTime = 5 * 1000; // In milliseconds, 0 = scan forever
BLEScan *pBLEScan;
bool active = false;

#include "NimBLEDevice.h"

class scanCallbacks : public BLEAdvertisedDeviceCallbacks
{

    // void onDiscovered(NimBLEAdvertisedDevice *advertisedDevice)
    // {
    //     Serial.printf("Discovered Advertised Device: %s \n",
    //                   advertisedDevice->toString().c_str());
    // }

    void onResult(NimBLEAdvertisedDevice *advertisedDevice)
    {
        Serial.printf("Advertised Device Result: %s \n",
                      advertisedDevice->toString().c_str());
    }

    void onScanEnd(NimBLEScanResults results)
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
