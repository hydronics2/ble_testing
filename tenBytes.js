

let chosenBleService = null;

function myFunction() {
  navigator.bluetooth.requestDevice({
      filters: [
        { services: [0x180D] },
        {name: 'Bluefruit52'} //including the name is required on web ble  but not chrome
      ]
    })
  .then(device => device.gatt.connect())
  .then(server => server.getPrimaryService(0x180D))
  .then(service => {
    console.log("connected, got primary service");
    chosenBleService = service;
    return Promise.all([
      service.getCharacteristic(0x2A37)
        .then(handleBleCharacteristic),
    ]);
  });
}

function handleBleCharacteristic(characteristic) {
  return characteristic.startNotifications()
  .then(char => {
    characteristic.addEventListener('characteristicvaluechanged',
                                    onDataChanged);
  });
}

function onDataChanged(event) {
  let characteristic = event.target;
  console.log(parseData(characteristic.value));
}



function parseData(data) {
  let result = {};
  console.log("receiving 10 bytes");

  console.log(data.getUint8(0));
  console.log(data.getUint8(1));
  console.log(data.getUint8(2));
  console.log(data.getUint8(3));
  console.log(data.getUint8(4));
  console.log(data.getUint8(5));
  console.log(data.getUint8(6));
  console.log(data.getUint8(7));
  console.log(data.getUint8(8));
  console.log(data.getUint8(9));

  document.getElementById("mytext").value = data.getUint8(1);
  result.bitOne = data.getUint8(1);
  return result;
}
