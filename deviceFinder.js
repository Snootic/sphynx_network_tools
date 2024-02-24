const find = require('local-devices');

const macPrefix = '08:AA:3E:8B:7C' // Sphynx default MAC Address Prefix

const findSphynx = (devices, macPrefix) => {
  return devices.filter(device => {
    return device.mac.startsWith(macPrefix);
  });
};


const scanNetwork = async () => {
  try {
    const startTime = Date.now(); // Start time of the scanning

    const devices = await find();

    // Find ESP32 devices by MAC address prefix
    const sphynxDevices = findSphynx(devices, macPrefix);

    const endTime = Date.now(); // End time of the scanning
    const scanTime = endTime - startTime;

    console.log("Find " + sphynxDevices.length + " Sphynx devices in your network")
    console.log(sphynxDevices);
    console.log("Scan completed in " + scanTime + " milliseconds");
  } catch (error) {
    console.error('Error:', error);
  }
};

scanNetwork();