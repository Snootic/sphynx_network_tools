const find = require('local-devices');

const macPrefix = '10:aa:bb:cc:33'; // Sphynx default MAC Address Prefix
const namePrefix = 'ESP32-SPHYNX'; // Sphynx default MAC Address Prefix

const findSphynx = (devices, macPrefix) => {
  return devices.filter(device => {
    let listaConjunto = (device.mac.split(":"));

    listaConjunto.splice(5, 1)
    let final = listaConjunto.join(":");

    return final === macPrefix;
  });
};

const findSphynxByName = (devices, namePrefix) => {
  return devices.filter(device => {
    let nomeDispositivo = device.name;

    return nomeDispositivo === namePrefix;
  });
};

const scanNetwork = async () => {
  try {
    console.log("Starting scan of local devices...")

    const startTime = Date.now(); // Start time of the scanning

    const devices = await find();

    // Find Sphynx devices by MAC address
    const sphynxDevices = findSphynx(devices, macPrefix);

    const endTime = Date.now(); // End time of the scanning
    const scanTime = endTime - startTime;

    console.log("Find " + sphynxDevices.length + " Sphynx devices in your network");
    console.log(sphynxDevices);
    console.log("Scan completed in " + scanTime + " milliseconds");
  } catch (error) {
    console.error('Error:', error);
  }
};

scanNetwork();