import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';

import '../utils/snackbar.dart';
import '../utils/extra.dart';

class DeviceScreen extends StatefulWidget {
  final BluetoothDevice device;

  const DeviceScreen({super.key, required this.device});

  @override
  State<DeviceScreen> createState() => _DeviceScreenState();
}

class _DeviceScreenState extends State<DeviceScreen> {
  BluetoothConnectionState _connectionState = BluetoothConnectionState.disconnected;
  List<BluetoothService> _services = [];
  bool _isConnecting = false;
  bool _isDisconnecting = false;

  BluetoothCharacteristic? _rxCharacteristic;

  late StreamSubscription<BluetoothConnectionState> _connectionStateSubscription;
  late StreamSubscription<bool> _isConnectingSubscription;
  late StreamSubscription<bool> _isDisconnectingSubscription;

  final String characteristicUuidRx = '04d3552e-b9b3-4be6-a8b4-aa43c4507c4d';

  @override
  void initState() {
    super.initState();

    _connectionStateSubscription = widget.device.connectionState.listen((state) async {
      _connectionState = state;
      if (state == BluetoothConnectionState.connected) {
        await _discoverServices();
      }
      if (mounted) {
        setState(() {});
      }
    });

    _isConnectingSubscription = widget.device.isConnecting.listen((value) {
      _isConnecting = value;
      if (mounted) {
        setState(() {});
      }
    });

    _isDisconnectingSubscription = widget.device.isDisconnecting.listen((value) {
      _isDisconnecting = value;
      if (mounted) {
        setState(() {});
      }
    });
  }

  @override
  void dispose() {
    _connectionStateSubscription.cancel();
    _isConnectingSubscription.cancel();
    _isDisconnectingSubscription.cancel();
    super.dispose();
  }

  bool get isConnected {
    return _connectionState == BluetoothConnectionState.connected;
  }

  Future _discoverServices() async {
    try {
      _services = await widget.device.discoverServices();
      for (var service in _services) {
        for (var characteristic in service.characteristics) {
          if (characteristic.uuid.toString() == characteristicUuidRx) {
            _rxCharacteristic = characteristic;
            if (mounted) setState(() {});
            break;
          }
        }
      }
    } catch (e) {
      Snackbar.show(ABC.c, "Error discovering services: $e", success: false);
    }
  }

  Future onConnectPressed() async {
    try {
      await widget.device.connectAndUpdateStream();
      Snackbar.show(ABC.c, "Connect: Success", success: true);
    } catch (e) {
      if (e is FlutterBluePlusException && e.code == FbpErrorCode.connectionCanceled.index) {
        // ignore
      } else {
        Snackbar.show(ABC.c, prettyException("Connect Error:", e), success: false);
      }
    }
  }

  Future onDisconnectPressed() async {
    try {
      await widget.device.disconnectAndUpdateStream();
      Snackbar.show(ABC.c, "Disconnect: Success", success: true);
    } catch (e) {
      Snackbar.show(ABC.c, prettyException("Disconnect Error:", e), success: false);
    }
  }

  Future sendCommand(int command, String label) async {
    if (_rxCharacteristic == null) {
      return;
    }

    try {
      await _rxCharacteristic!.write([command]);
      // Snackbar removido - no muestra notificación
    } catch (e) {
      // Solo muestra error si falla el envío
      Snackbar.show(ABC.c, "Error: $e", success: false);
    }
  }

  Widget buildDPadButton(IconData icon, int command, String label) {
    return GestureDetector(
      onTap: () => sendCommand(command, label),
      child: Container(
        width: 70,
        height: 70,
        decoration: BoxDecoration(
          color: Colors.grey[800],
          shape: BoxShape.circle,
          boxShadow: [
            BoxShadow(
              color: Colors.black.withOpacity(0.5),
              blurRadius: 8,
              offset: const Offset(0, 4),
            ),
          ],
        ),
        child: Icon(icon, size: 35, color: Colors.white),
      ),
    );
  }

  Widget buildDPad() {
    return Column(
      mainAxisSize: MainAxisSize.min,
      children: [
        // Arriba
        buildDPadButton(Icons.arrow_drop_up, 1, "Arriba"),
        const SizedBox(height: 5),
        // Izquierda - Centro - Derecha
        Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            buildDPadButton(Icons.arrow_left, 3, "Izquierda"),
            const SizedBox(width: 5),
            Container(
              width: 70,
              height: 70,
              decoration: BoxDecoration(
                color: Colors.grey[900],
                shape: BoxShape.circle,
              ),
            ),
            const SizedBox(width: 5),
            buildDPadButton(Icons.arrow_right, 4, "Derecha"),
          ],
        ),
        const SizedBox(height: 5),
        // Abajo
        buildDPadButton(Icons.arrow_drop_down, 2, "Abajo"),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return ScaffoldMessenger(
      key: Snackbar.snackBarKeyC,
      child: Scaffold(
        backgroundColor: Colors.grey[900],
        body: Center(
          child: isConnected && _rxCharacteristic != null
              ? buildDPad()
              : const Text(
                  'Conecta el dispositivo primero',
                  style: TextStyle(fontSize: 18, color: Colors.white70),
                ),
        ),
      ),
    );
  }
}
