const find = require('local-devices');

const macPrefix = '08:AA:3E:8B:7C'; // Sphynx default MAC Address Prefix

const findSphynx = (devices, macPrefix) => {
  return devices.filter(device => {
    return device.mac === macPrefix;
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