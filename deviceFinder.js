import find from 'local-devices'


const scanNetwork = async () => {
  try {
    const devices = await find();

    return devices
  
  } catch (error) {
    console.error('Error:', error);
  }
};

export default scanNetwork;