# ble_testing

Program your Feather nrF52 and then open a chrome web browser in MacOS, chromeOS, Android, or in ios using the safari wrapper webble. The files must be hosted as https:// such as https://mywebsite.com</p>

Here's a preview: https://cdn.rawgit.com/hydronics2/nrF52_web-ble_example/8ede9c4b/index.html


<p>Click on the bluetooth icon for your browser to connect to the nrF52.

I finally got all the code up and running on Webble. It’s a $2 app on itunes that’s a wrapper for Safari that exposes the web-ble functions we’re using to connect over Bluetooth to the eggs. It hadn’t worked previously because of a bug but by updating ios to 11.0 and downloading the newest WEBBLE app 1.1.5 it all works seamlessly.

Webble App on itunes: https://itunes.apple.com/us/app/webble/id1193531073?mt=8
web-ble: https://developers.google.com/web/updates/2015/07/interact-with-ble-devices-on-the-web


The Arduino nrF52 sketch:
//This code used the nrf52 library Adafruit Library  #include <bluefruit.h>
 10a9af52aa  https://github.com/adafruit/Adafruit_nRF52_Arduino/tree/10a9af52aaa3b64e5f3832e008013d0b833f60da
