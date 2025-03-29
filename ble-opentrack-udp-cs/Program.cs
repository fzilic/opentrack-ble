namespace ble_opentrack_udp;

using System.Net.Sockets;
using InTheHand.Bluetooth;

class Program
{

  async static Task Main(string[] args)
  {
    bool isRunning = false;

    Console.WriteLine("BLE OpenTrack UDP");
    var devices = await DeviceDiscovery();

    if (devices.Count == 0)
    {
      Console.WriteLine("No compatible devices found");
      return;
    }

    var device = devices.First();
    device.GattServerDisconnected += (sender, args) =>
    {
      Console.WriteLine("Device disconnected");
      isRunning = false;
    };

    await device.Gatt.ConnectAsync();

    if (!device.Gatt.IsConnected)
    {
      Console.WriteLine("Failed to connect");
      return;
    }

    var service = await device.Gatt.GetPrimaryServiceAsync(BluetoothUuid.FromGuid(new Guid("FC7C1FD2-0C82-11F0-BBE4-00155DD2D8C6")));
    if (service == null)
    {
      Console.WriteLine("Failed to connect to service");
      return;
    }
    var characteristic = await service.GetCharacteristicAsync(BluetoothUuid.FromGuid(new Guid("7E13A434-0C83-11F0-9DE9-00155DD2D8C6")));
    if (characteristic == null)
    {
      Console.WriteLine("Failed to connect to service");
      return;
    }

    var udpClient = new UdpClient();
    try
    {
      udpClient.Connect("localhost", 4242);
    }
    catch (Exception e)
    {
      Console.WriteLine($"Failed ot open UDP socket {e}");
    }


    characteristic.CharacteristicValueChanged += (sender, args) =>
    {
      var data = args.Value;
      if (data != null)
      {
        if (data.Length == 12)
        {
          var roll = BitConverter.ToSingle(data, 0);
          var pitch = BitConverter.ToSingle(data, 4);
          var yaw = BitConverter.ToSingle(data, 8);
          // Console.WriteLine($"PARS: R:{roll},P:{pitch},Y:{yaw}");

          byte[] bytes = new byte[48];
          BitConverter.GetBytes(0.0d).CopyTo(bytes, 0);
          BitConverter.GetBytes(0.0d).CopyTo(bytes, 8);
          BitConverter.GetBytes(0.0d).CopyTo(bytes, 16);
          BitConverter.GetBytes(Convert.ToDouble(yaw)).CopyTo(bytes, 24);
          BitConverter.GetBytes(Convert.ToDouble(pitch)).CopyTo(bytes, 32);
          BitConverter.GetBytes(Convert.ToDouble(roll)).CopyTo(bytes, 40);

          udpClient.SendAsync(bytes, 48);
        }


      }
    };

    await characteristic.StartNotificationsAsync();
    isRunning = true;

    while (isRunning)
    {
      // wait
    }
  }

  private static async Task<IReadOnlyCollection<BluetoothDevice>> DeviceDiscovery()
  {
    Console.WriteLine("Scanning for devices (takes up to 30 seconds)");
    var devices = await Bluetooth.ScanForDevicesAsync(new RequestDeviceOptions()
    {
      Filters = { new BluetoothLEScanFilter { Name = "HT N33SBLER2" } }
    });

    foreach (var device in devices)
    {
      Console.WriteLine($"Found {device.Name}");
    }
    return devices;
  }

}
