var group__usb__device__controller__driver =
[
    [ "usb_device_callback_message_struct_t", "group__usb__device__controller__driver.html#structusb__device__callback__message__struct__t", [
      [ "buffer", "group__usb__device__controller__driver.html#a179ef4c43731a77064db06db6a843dca", null ],
      [ "length", "group__usb__device__controller__driver.html#aac1c4c228922ec362c8cbc387e59a8d9", null ],
      [ "code", "group__usb__device__controller__driver.html#a54c9f7b0dbaa283ea351aac1e854b495", null ],
      [ "isSetup", "group__usb__device__controller__driver.html#abac68a9fb39de27549f126874db0106a", null ]
    ] ],
    [ "usb_device_controller_interface_struct_t", "group__usb__device__controller__driver.html#structusb__device__controller__interface__struct__t", [
      [ "deviceInit", "group__usb__device__controller__driver.html#aac2bf2cf30d26d9a0bf5f270af261f07", null ],
      [ "deviceDeinit", "group__usb__device__controller__driver.html#a8109ab99d80c27adaa86dee9545857c3", null ],
      [ "deviceSend", "group__usb__device__controller__driver.html#a1c04868489a1819008238162c6608edf", null ],
      [ "deviceRecv", "group__usb__device__controller__driver.html#a2870d74680ad2860eec0caf7b7fe99c6", null ],
      [ "deviceCancel", "group__usb__device__controller__driver.html#a532cc07370a9fc7870417ddcadf0e5aa", null ],
      [ "deviceControl", "group__usb__device__controller__driver.html#ab3156591037c063d394620e2b81d8745", null ]
    ] ],
    [ "usb_device_struct_t", "group__usb__device__controller__driver.html#structusb__device__struct__t", [
      [ "controllerHandle", "group__usb__device__controller__driver.html#a70a60277def9a089add1a656a5569010", null ],
      [ "controllerInterface", "group__usb__device__controller__driver.html#a775b73d9f1957db2b7c729b09807b62b", null ],
      [ "deviceCallback", "group__usb__device__controller__driver.html#ac6481bca6317672c1d44580351e544b0", null ],
      [ "endpointCallback", "group__usb__device__controller__driver.html#a13dbeca8f2542135003bb4a4e43475cd", null ],
      [ "deviceAddress", "group__usb__device__controller__driver.html#aa9ece9f3701b8f8722c06a6b6e782252", null ],
      [ "controllerId", "group__usb__device__controller__driver.html#a4994e9512c6f87e053175a440f3acdc5", null ],
      [ "state", "group__usb__device__controller__driver.html#a966a0d0e4d603cb5d9b0c9b2eac854e3", null ]
    ] ],
    [ "usb_device_controller_handle", "group__usb__device__controller__driver.html#gaec03ce2e4732aa876cd2a91cf8e93b5b", null ],
    [ "usb_device_controller_init_t", "group__usb__device__controller__driver.html#ga7f8af48e2f4f886f521f8128b6a05e9b", null ],
    [ "usb_device_controller_deinit_t", "group__usb__device__controller__driver.html#ga283139f5df38421250c30a094f34088d", null ],
    [ "usb_device_controller_send_t", "group__usb__device__controller__driver.html#ga5879c987e628c5c5a27b5fcc685e90c8", null ],
    [ "usb_device_controller_recv_t", "group__usb__device__controller__driver.html#gac14df7129a0dc5e7c0b7148b05992dc8", null ],
    [ "usb_device_controller_cancel_t", "group__usb__device__controller__driver.html#ga476c2b07d6a5e467e104568aad59ef44", null ],
    [ "usb_device_controller_control_t", "group__usb__device__controller__driver.html#gae484b958b5a84af6ce5921f8538ff1c4", null ],
    [ "usb_device_notification_t", "group__usb__device__controller__driver.html#gad551f12945d7c222417bdfe0d0ab1fa1", [
      [ "kUSB_DeviceNotifyBusReset", "group__usb__device__controller__driver.html#ggad551f12945d7c222417bdfe0d0ab1fa1a6b3844cfeb2f1acd91ec86e553cb4938", null ],
      [ "kUSB_DeviceNotifySuspend", "group__usb__device__controller__driver.html#ggad551f12945d7c222417bdfe0d0ab1fa1aa9a1526acc96165a26fe187a771bd7ca", null ],
      [ "kUSB_DeviceNotifyResume", "group__usb__device__controller__driver.html#ggad551f12945d7c222417bdfe0d0ab1fa1af82ebeb85b3be0b518d0608052acfb46", null ],
      [ "kUSB_DeviceNotifyError", "group__usb__device__controller__driver.html#ggad551f12945d7c222417bdfe0d0ab1fa1a703fdebcf80a09a04f9149cc9171576f", null ],
      [ "kUSB_DeviceNotifyDetach", "group__usb__device__controller__driver.html#ggad551f12945d7c222417bdfe0d0ab1fa1a26b9d31e47d99f112ebc74b17e1879cd", null ],
      [ "kUSB_DeviceNotifyAttach", "group__usb__device__controller__driver.html#ggad551f12945d7c222417bdfe0d0ab1fa1a9b4873734c36adcc45b205b5f55bad31", null ]
    ] ],
    [ "usb_device_control_type_t", "group__usb__device__controller__driver.html#ga032c851b1984fcd2659ff1a58976ab36", [
      [ "kUSB_DeviceControlRun", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36a0d4334c836c8ddba19dd1b3c343bf718", null ],
      [ "kUSB_DeviceControlStop", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36a3e65ecf4751583140d63e6ad7129bb9a", null ],
      [ "kUSB_DeviceControlEndpointInit", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36a5f2f7690579a2582b74013d569f7a2b5", null ],
      [ "kUSB_DeviceControlEndpointDeinit", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36a758593417eb515bd9a64b5c9d9faf6c3", null ],
      [ "kUSB_DeviceControlEndpointStall", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36a667e55c04a0399ec26fd94a9aa6918e4", null ],
      [ "kUSB_DeviceControlEndpointUnstall", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36ae3d343db3b169349791d2c1b14b8140b", null ],
      [ "kUSB_DeviceControlGetDeviceStatus", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36aa04b915379ec7616d8e7318618652a1a", null ],
      [ "kUSB_DeviceControlGetEndpointStatus", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36a904c707abbfeac703932a25b731f668e", null ],
      [ "kUSB_DeviceControlSetDeviceAddress", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36a2e2b710fe6f299d1cd6655672b5d5c52", null ],
      [ "kUSB_DeviceControlGetSynchFrame", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36ab290661a0aa5f30d61a22f9117a58899", null ],
      [ "kUSB_DeviceControlResume", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36a8bd921505bfadde8059c154c2025ae4e", null ],
      [ "kUSB_DeviceControlSetDefaultStatus", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36aca7240a60464f7f19347ef3d518fd97f", null ],
      [ "kUSB_DeviceControlGetSpeed", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36aa5de8d34f68ecf4e2948033b27f67567", null ],
      [ "kUSB_DeviceControlGetOtgStatus", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36a47e409659c5b7c11a8aa26ad4ec65279", null ],
      [ "kUSB_DeviceControlSetOtgStatus", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36a9b5a35a9f0054382e96410fffb7f2e8b", null ],
      [ "kUSB_DeviceControlSetTestMode", "group__usb__device__controller__driver.html#gga032c851b1984fcd2659ff1a58976ab36ad3c6f680a4c3f112607d3a582b8b2cd8", null ]
    ] ]
];