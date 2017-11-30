//this code uses the Heart Rate Service and Characteristic but just sends random bytes from the nrf52 to the web via web-ble

//This code used the nrf52 library Adafruit Library 10a9af52aa  https://github.com/adafruit/Adafruit_nRF52_Arduino/tree/10a9af52aaa3b64e5f3832e008013d0b833f60da




#include <bluefruit.h>

#define STATUS_LED  (17)
#define BLINKY_MS   (1000)

/* HRM Service Definitions
 * Heart Rate Monitor Service:  0x180D
 */
BLEService        hrms = BLEService(UUID16_SVC_HEART_RATE);
BLECharacteristic hrmc = BLECharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance

uint32_t blinkyms;
uint8_t  bps = 200;

// Advanced function prototypes
void setupAdv(void);
void setupHRM(void);
void connect_callback(void);
void disconnect_callback(uint8_t reason);
void cccd_callback(BLECharacteristic& chr, ble_gatts_evt_write_t* request);

void setup()
{
  Serial.begin(115200);
  Serial.println("nrf52 test sending 8 bytes");
  Serial.println("-----------------------");

  // Setup LED pins and reset blinky counter
  pinMode(STATUS_LED, OUTPUT);
  blinkyms = millis();

  // Initialise the Bluefruit module
  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'Feather52 HRM'");
  Bluefruit.setName("Random Bytes");

  // Set the connect/disconnect callback handlers
  Bluefruit.setConnectCallback(connect_callback);
  Bluefruit.setDisconnectCallback(disconnect_callback);

  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("TBD");
  bledis.setModel("TBDv1");
  bledis.begin();

  // Start the BLE Battery Service and set it to 100%
  Serial.println("Configuring the Battery Service");
  blebas.begin();

  // Setup the Heart Rate Monitor service using
  // BLEService and BLECharacteristic classes
  Serial.println("Configuring the Heart Rate Monitor Service");
  setupHRM();

  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();

  // Start Advertising
  Serial.println("Ready Player One!!!");
  Serial.println("\nAdvertising");
  Bluefruit.Advertising.start();
}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include HRM Service UUID
  Bluefruit.Advertising.addService(hrms);

  // Include Name
  Bluefruit.Advertising.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void setupHRM(void)
{
  // Name                         UUID    Requirement Properties
  // ---------------------------- ------  ----------- ----------
  // Heart Rate Measurement       0x2A37  Mandatory   Notify
  hrms.begin();

  // Note: You must call .begin() on the BLEService before calling .begin() on
  // any characteristic(s) within that service definition.. Calling .begin() on
  // a BLECharacteristic will cause it to be added to the last BLEService that
  // was 'begin()'ed!

  hrmc.setProperties(CHR_PROPS_NOTIFY);
  hrmc.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  hrmc.setFixedLen(10); //was 2 changed to 4 to 10.... important to be right if in... comment out?
  hrmc.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  hrmc.begin();
  //not sure why this is needed
  uint8_t hrmdata1[] = { 1,1,1,1,1,1,1,1,1,1 }; // this used to send out data about the heart rate meter to the client
  hrmc.notify(hrmdata1, 10);    //important... tells it's sending 10 bytes              // Use .notify instead of .write!

}

void connect_callback(uint16_t conn_handle)
{
  char central_name[32] = { 0 };
  Bluefruit.Gap.getPeerName(conn_handle, central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println("Disconnected");
  Serial.println("Advertising!");
}

void cccd_callback(BLECharacteristic& chr, uint16_t cccd_value)
{
  // Display the raw request packet
  Serial.print("CCCD Updated: ");
  //Serial.printBuffer(request->data, request->len);
  Serial.print(cccd_value);
  Serial.println("");

  // Check the characteristic this CCCD update is associated with in case
  // this handler is used for multiple CCCD records.
  if (chr.uuid == hrmc.uuid) {
    if (chr.notifyEnabled()) {
        Serial.println(" 'Notify' enabled");
    } else {
        Serial.println(" 'Notify' disabled");
    }
  }
}
//------------------------------------------------------  END SETUP

void loop()
{
  // Blinky!
  if (blinkyms+BLINKY_MS < millis()) {
    blinkyms = millis();
    digitalToggle(STATUS_LED);

    if (Bluefruit.connected()) {
      uint8_t hrmdata[] = { random(0,255),random(0,255),random(0,255),random(0,255),random(0,255),random(0,255)};           // Sensor connected, increment BPS value
      Serial.println("start:");
      for(int i = 0; i < 8; i++){
        Serial.println(hrmdata[i]);
      }
      Serial.println("end");
      Serial.print("size of hrm data: ");
      Serial.println(sizeof(hrmdata));
      //err_t resp = hrmc.notify(hrmdata, sizeof(hrmdata));   // Note: We use .notify instead of .write!
      hrmc.notify(hrmdata, sizeof(hrmdata));   // Note: We use .notify instead of .write!

    }
    //change the battery data randomly
    blebas.write(random(0,100)); //sets to 0-100% See the battery data on google web-ble samples here: 
    // https://googlechrome.github.io/samples/web-bluetooth/battery-level.html
  }
}
