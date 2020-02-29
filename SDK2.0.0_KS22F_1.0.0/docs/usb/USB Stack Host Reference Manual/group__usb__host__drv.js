var group__usb__host__drv =
[
    [ "USB Host Controller driver", "group__usb__host__controller__driver.html", "group__usb__host__controller__driver" ],
    [ "usb_host_ep_t", "group__usb__host__drv.html#structusb__host__ep__t", [
      [ "epDesc", "group__usb__host__drv.html#acbe4454109c7aa41f9cea3619b74d3c1", null ],
      [ "epExtension", "group__usb__host__drv.html#a19d259fffab9d82960366e8ffb76aa0a", null ],
      [ "epExtensionLength", "group__usb__host__drv.html#a00ba83eb8333fccf84410a7569a64110", null ]
    ] ],
    [ "usb_host_interface_t", "group__usb__host__drv.html#structusb__host__interface__t", [
      [ "epList", "group__usb__host__drv.html#af6185a10b259aa930e512a5dbabe2a53", null ],
      [ "interfaceDesc", "group__usb__host__drv.html#a6e75737845e06f78ccf5b8130754d6e6", null ],
      [ "interfaceExtension", "group__usb__host__drv.html#acf405b77e6f0e35362fa053504d5eee0", null ],
      [ "interfaceExtensionLength", "group__usb__host__drv.html#af9e2d21e3a4ba1bc6ff533add53838c0", null ],
      [ "interfaceIndex", "group__usb__host__drv.html#a6a7a772cd59cb957d5e4744a49a67144", null ],
      [ "alternateSettingNumber", "group__usb__host__drv.html#a0ce65e5dedca44a6a2aab13cbbf84d63", null ],
      [ "epCount", "group__usb__host__drv.html#a1f1eb2c9defaf4074e72a7a6ba8b6a76", null ]
    ] ],
    [ "usb_host_configuration_t", "group__usb__host__drv.html#structusb__host__configuration__t", [
      [ "interfaceList", "group__usb__host__drv.html#ac5e5c6f6cb4ec4c7f948a76b31234db7", null ],
      [ "configurationDesc", "group__usb__host__drv.html#a5b9ef50bbc0969515f0fc7d1e6d0fa44", null ],
      [ "configurationExtension", "group__usb__host__drv.html#ae7f40d410ce609b2967f2695b2944af8", null ],
      [ "configurationExtensionLength", "group__usb__host__drv.html#a07ee07181426c6e2ac87ea8ba47bfaf1", null ],
      [ "interfaceCount", "group__usb__host__drv.html#a15ad7387fecb7c3c9008a30f52a14bfb", null ]
    ] ],
    [ "usb_host_pipe_t", "group__usb__host__drv.html#structusb__host__pipe__t", [
      [ "next", "group__usb__host__drv.html#a7ed0cea812030728feb60109c2e4558a", null ],
      [ "deviceHandle", "group__usb__host__drv.html#a5ff867ed4ccd74b339d2c382dd7b0a0c", null ],
      [ "currentCount", "group__usb__host__drv.html#ae1cff02b0d9f069f97af0f7916eb5898", null ],
      [ "nakCount", "group__usb__host__drv.html#a48468f88d3ed337f69abc23017f0c6be", null ],
      [ "maxPacketSize", "group__usb__host__drv.html#aeee94749aebfd8d51b8f72c93c3dde39", null ],
      [ "interval", "group__usb__host__drv.html#a8c13a0b598fd37ed14fbf0157158df79", null ],
      [ "open", "group__usb__host__drv.html#a2d3ae04a4e83afa65352ecbefc68833a", null ],
      [ "nextdata01", "group__usb__host__drv.html#af8f50a7e02198d82f7cc611e92209fdf", null ],
      [ "endpointAddress", "group__usb__host__drv.html#aef103621d34333219dc71bf38fa69ce4", null ],
      [ "direction", "group__usb__host__drv.html#a0e9e65d98143faf23d86f579bbc53a11", null ],
      [ "pipeType", "group__usb__host__drv.html#ad2fc05c8d1911606e0c7c74550ae25b3", null ],
      [ "numberPerUframe", "group__usb__host__drv.html#a87db1861369a7440b4a9e8f6d9f43fc5", null ]
    ] ],
    [ "usb_host_transfer_t", "group__usb__host__drv.html#structusb__host__transfer__t", [
      [ "next", "group__usb__host__drv.html#a29d7f0a07a7bc0962f0e4a9e225ec41b", null ],
      [ "transferBuffer", "group__usb__host__drv.html#ac58b2798ef66e8c4114381e43987a045", null ],
      [ "transferLength", "group__usb__host__drv.html#a7364f33f2672de83504d177677d62e85", null ],
      [ "transferSofar", "group__usb__host__drv.html#a3d7367b25cabfdf70d43956e617f5311", null ],
      [ "callbackFn", "group__usb__host__drv.html#ae68ecae1b8c08ed2b997c3193a5d51da", null ],
      [ "callbackParam", "group__usb__host__drv.html#a9a282fb2b9a9ebb24f3431860afa6b3e", null ],
      [ "transferPipe", "group__usb__host__drv.html#a10ad210b4dec2e0247f84642dcd1bb82", null ],
      [ "setupPacket", "group__usb__host__drv.html#a368974b5941f063a3c2e75678c0c8c8a", null ],
      [ "direction", "group__usb__host__drv.html#a83362da67c711d0f1a9f449b30895c17", null ],
      [ "setupStatus", "group__usb__host__drv.html#a599d23f14d69e08b46b54e2ab475b156", null ]
    ] ],
    [ "usb_host_pipe_init_t", "group__usb__host__drv.html#structusb__host__pipe__init__t", [
      [ "devInstance", "group__usb__host__drv.html#aee29644c09c5eca74156400b713e977e", null ],
      [ "nakCount", "group__usb__host__drv.html#ab61b504eb84a79caee7115565e931589", null ],
      [ "maxPacketSize", "group__usb__host__drv.html#a4d6855dcb91202d0c92809bd41188de6", null ],
      [ "interval", "group__usb__host__drv.html#a986337fe5180134902a2bdb2363ceaa1", null ],
      [ "endpointAddress", "group__usb__host__drv.html#aac461f3f418f699d05143f580114e446", null ],
      [ "direction", "group__usb__host__drv.html#ab0112b125344eb8958caa9435909917f", null ],
      [ "pipeType", "group__usb__host__drv.html#a1ad0eea116f08e8a91901fbfadbe810b", null ],
      [ "numberPerUframe", "group__usb__host__drv.html#aab59cd9a4a17a90defc96f85be0ffba0", null ]
    ] ],
    [ "usb_host_cancel_param_t", "group__usb__host__drv.html#structusb__host__cancel__param__t", [
      [ "pipeHandle", "group__usb__host__drv.html#ad1185fdda85b0d1e97c27a0d0164a1b5", null ],
      [ "transfer", "group__usb__host__drv.html#af0ef069861c8a4558328594b5a928e85", null ]
    ] ],
    [ "usb_host_device_instance_t", "group__usb__host__drv.html#structusb__host__device__instance__t", [
      [ "next", "group__usb__host__drv.html#af38a68a64e5bdea535a49af9c5dc1bf1", null ],
      [ "hostHandle", "group__usb__host__drv.html#ae4abd55d3a1e7a34792bafdb832433a6", null ],
      [ "configuration", "group__usb__host__drv.html#aa856a7cb88a8d81ff08b69e0899ae261", null ],
      [ "deviceDescriptor", "group__usb__host__drv.html#ab21e9d8c2ec822be67f2c6df4970e23a", null ],
      [ "controlPipe", "group__usb__host__drv.html#a394885710b4cb7f6069e9b542d5e7028", null ],
      [ "configurationDesc", "group__usb__host__drv.html#a65b8fc8e155adaa3e63d3ca1c1436513", null ],
      [ "configurationLen", "group__usb__host__drv.html#a08bb215e60aa114f6f451d54da6269b0", null ],
      [ "configurationValue", "group__usb__host__drv.html#ad1f3d55d1ef15c64d51b710862d8d891", null ],
      [ "interfaceStatus", "group__usb__host__drv.html#a3728a4f6e93efdb516d5857a44950384", null ],
      [ "enumBuffer", "group__usb__host__drv.html#a8bb5c612a6d905810062ffe622ebf154", null ],
      [ "state", "group__usb__host__drv.html#a483fe9ce82afb55deb6e305a7e76a450", null ],
      [ "enumRetries", "group__usb__host__drv.html#a830324186e3597206e80180956af92b4", null ],
      [ "stallRetries", "group__usb__host__drv.html#ad42a7e5cb4664384c3a46973f412b0c7", null ],
      [ "speed", "group__usb__host__drv.html#a01fd0fcad6f690cde3c1f0ff91363f3a", null ],
      [ "allocatedAddress", "group__usb__host__drv.html#a02af6b8c396072b0c94adb0b4a58ca88", null ],
      [ "setAddress", "group__usb__host__drv.html#a92688fffff5e5d9e823585b049591b62", null ],
      [ "deviceAttachState", "group__usb__host__drv.html#a0361bd402180d8fe2964ab29edbed642", null ]
    ] ],
    [ "usb_host_instance_t", "group__usb__host__drv.html#structusb__host__instance__t", [
      [ "controllerHandle", "group__usb__host__drv.html#ad1aaf9dea2046f4b89906009e471239c", null ],
      [ "deviceCallback", "group__usb__host__drv.html#a1e648b0356c00da1ec0f7a20dbd44abe", null ],
      [ "hostMutex", "group__usb__host__drv.html#a636c3d718a05296e00869bd1d890aaa8", null ],
      [ "transferList", "group__usb__host__drv.html#a58e4d1985f1e7c829abaced1654538aa", null ],
      [ "transferHead", "group__usb__host__drv.html#a22957cbb31375a9fb6c0c960de28e978", null ],
      [ "controllerTable", "group__usb__host__drv.html#a1f4b388ec02b5f3e20b4e3aa8c7fc684", null ],
      [ "deviceList", "group__usb__host__drv.html#a73eac1528f62e8479209d277d4971708", null ],
      [ "addressBitMap", "group__usb__host__drv.html#a3579ccd83d30ae92571ed3a2c320c108", null ],
      [ "occupied", "group__usb__host__drv.html#a0cd44b66da64f049d0bde476c27be187", null ],
      [ "controllerId", "group__usb__host__drv.html#ab1f2aa132bc287ef8ba5601f2ca852cb", null ]
    ] ],
    [ "usb_host_class_handle", "group__usb__host__drv.html#gae07b4c197a7e5455641dcc25aa757c94", null ],
    [ "usb_host_controller_handle", "group__usb__host__drv.html#ga5ceca27e9215169f4c8cc002638d1eae", null ],
    [ "usb_host_configuration_handle", "group__usb__host__drv.html#ga029fdbfec88ffc949fa2211b34f48397", null ],
    [ "usb_host_interface_handle", "group__usb__host__drv.html#gae3de44b4050945e6e6871cae0266a175", null ],
    [ "usb_host_pipe_handle", "group__usb__host__drv.html#ga76dffa04033e4280c80c75a9240a57d6", null ],
    [ "host_callback_t", "group__usb__host__drv.html#gafb7d4b1e4f7c348f270b1dc2eb1a80cf", null ],
    [ "transfer_callback_t", "group__usb__host__drv.html#gabafbefb64e0cdd5aa93946a4b6e5ce17", null ],
    [ "host_inner_transfer_callback_t", "group__usb__host__drv.html#ga687323a868507e6aebc2c9cabce7e29c", null ],
    [ "usb_host_event_t", "group__usb__host__drv.html#gaa22921f48e11276dd06f95734709f585", [
      [ "kUSB_HostEventAttach", "group__usb__host__drv.html#ggaa22921f48e11276dd06f95734709f585a9fcc1ca6ea723d53f50a3ce702ca55f4", null ],
      [ "kUSB_HostEventDetach", "group__usb__host__drv.html#ggaa22921f48e11276dd06f95734709f585ac4a83f143684bba5025e0ef724e0fb05", null ],
      [ "kUSB_HostEventEnumerationDone", "group__usb__host__drv.html#ggaa22921f48e11276dd06f95734709f585accaa9d8fce35163fd49b13501ab45585", null ],
      [ "kUSB_HostEventNotSupported", "group__usb__host__drv.html#ggaa22921f48e11276dd06f95734709f585af2648ec07157229b1a1fbb0066cd129b", null ]
    ] ],
    [ "usb_host_dev_info_t", "group__usb__host__drv.html#gaa5e143f1705e6e0ed33487c660da3a2f", [
      [ "kUSB_HostGetDeviceAddress", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fa13d69e77dae6103c7471807a3ab9467b", null ],
      [ "kUSB_HostGetDeviceHubNumber", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fa0535c9f2d35275034d35546b3e345634", null ],
      [ "kUSB_HostGetDevicePortNumber", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fa85035e5ab7f72fdb80f00dc2c2ba5558", null ],
      [ "kUSB_HostGetDeviceSpeed", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fa6cd447f07af70595862d4ebdfc17b6a4", null ],
      [ "kUSB_HostGetDeviceHSHubNumber", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fa4e58edfe8a458d3c0ff4667cd26e7f36", null ],
      [ "kUSB_HostGetDeviceHSHubPort", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fa010a8702505050530181c96187b4cce7", null ],
      [ "kUSB_HostGetDeviceLevel", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fa6da6364f3fb7f87373e1790ec45b5a08", null ],
      [ "kUSB_HostGetHostHandle", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fa651d3d87ee0be3e653ee3be3e5232f49", null ],
      [ "kUSB_HostGetDeviceControlPipe", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fa7d90c6e25d679f6005a8d0453044c370", null ],
      [ "kUSB_HostGetDevicePID", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fab774a2927d60911170e60cb0ffdefa79", null ],
      [ "kUSB_HostGetDeviceVID", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2fac39438fb037fe20d34c44ac35ab6a005", null ],
      [ "kUSB_HostGetHubThinkTime", "group__usb__host__drv.html#ggaa5e143f1705e6e0ed33487c660da3a2faf2d94590708cfb30a7afe47145096da8", null ]
    ] ],
    [ "usb_host_device_status_t", "group__usb__host__drv.html#gabe55094de8be7a9bdb11dea924771794", [
      [ "kStatus_DEV_Notinit", "group__usb__host__drv.html#ggabe55094de8be7a9bdb11dea924771794a3788ee4c53da2b4c783275faa4f52e97", null ],
      [ "kStatus_DEV_Initial", "group__usb__host__drv.html#ggabe55094de8be7a9bdb11dea924771794afe126900f97c727aeb8017d704b23da4", null ],
      [ "kStatus_DEV_GetDes8", "group__usb__host__drv.html#ggabe55094de8be7a9bdb11dea924771794a69a825aa5b76010c54743d15e8c3da22", null ],
      [ "kStatus_DEV_SetAddress", "group__usb__host__drv.html#ggabe55094de8be7a9bdb11dea924771794abc73090645e16beea52a92aea56150f0", null ],
      [ "kStatus_DEV_GetDes", "group__usb__host__drv.html#ggabe55094de8be7a9bdb11dea924771794ac7922495ca0d6f1c379bc6ac7eab6853", null ],
      [ "kStatus_DEV_GetCfg9", "group__usb__host__drv.html#ggabe55094de8be7a9bdb11dea924771794a4dd672e1e26fc68d755de499ddcd4a3f", null ],
      [ "kStatus_DEV_GetCfg", "group__usb__host__drv.html#ggabe55094de8be7a9bdb11dea924771794aaaf5ff515c25c54af76efa43d79a2217", null ],
      [ "kStatus_DEV_SetCfg", "group__usb__host__drv.html#ggabe55094de8be7a9bdb11dea924771794af44bbe23bc29edbf0d97eff1f41c61fd", null ],
      [ "kStatus_DEV_EnumDone", "group__usb__host__drv.html#ggabe55094de8be7a9bdb11dea924771794aa50f8c3b8135843b5ac068b03710cdfc", null ],
      [ "kStatus_DEV_AppUsed", "group__usb__host__drv.html#ggabe55094de8be7a9bdb11dea924771794af5b163251f56277ad556b0c178baea06", null ]
    ] ],
    [ "usb_host_interface_state_t", "group__usb__host__drv.html#ga0c43828fdf7d33710e311ec1f30477e7", [
      [ "kStatus_interface_Attached", "group__usb__host__drv.html#gga0c43828fdf7d33710e311ec1f30477e7a58c0e3f33726ee3e294a2034e0de3227", null ],
      [ "kStatus_interface_Opened", "group__usb__host__drv.html#gga0c43828fdf7d33710e311ec1f30477e7ad7dd2f94f88ab425ef95e4465f1b08a8", null ],
      [ "kStatus_interface_Detached", "group__usb__host__drv.html#gga0c43828fdf7d33710e311ec1f30477e7aba50bfa08bb9f1dbf8750302046d50c2", null ]
    ] ],
    [ "USB_HostInit", "group__usb__host__drv.html#ga26aa7c859710bad485ae95e0141177b7", null ],
    [ "USB_HostDeinit", "group__usb__host__drv.html#gac9366cd27d9859428b1e57f9617a35a0", null ],
    [ "USB_HostHelperGetPeripheralInformation", "group__usb__host__drv.html#ga05dc958708527048b361c1cadbd793fc", null ],
    [ "USB_HostHelperParseAlternateSetting", "group__usb__host__drv.html#gaf6490b30d769170a7fc55ae79c9db965", null ],
    [ "USB_HostRemoveDevice", "group__usb__host__drv.html#ga503eb285edc33168ede38e271571399e", null ],
    [ "USB_HostKhciTaskFunction", "group__usb__host__drv.html#ga16ecb6f92fa74e0dea8973c65910062e", null ],
    [ "USB_HostEhciTaskFunction", "group__usb__host__drv.html#ga60e26d9c13c598adc0b2881ceffc5101", null ],
    [ "USB_HostKhciIsrFunction", "group__usb__host__drv.html#ga0d37575edcbe4ce8fa9df792da98eaaf", null ],
    [ "USB_HostEhciIsrFunction", "group__usb__host__drv.html#gab40a4c673629f965cbc76a2ecd8e7726", null ],
    [ "USB_HostOpenPipe", "group__usb__host__drv.html#ga3bccd218140f3c7b146b79b84e0647c5", null ],
    [ "USB_HostClosePipe", "group__usb__host__drv.html#ga1cca76a622382ae9a8ef73db13643064", null ],
    [ "USB_HostSend", "group__usb__host__drv.html#gac75e8813fed5fbc33d6e56801323c84f", null ],
    [ "USB_HostSendSetup", "group__usb__host__drv.html#gae395349df82f9e8fe18d4399348a00d6", null ],
    [ "USB_HostRecv", "group__usb__host__drv.html#ga934b705199bd39984efe3c71d235dcc8", null ],
    [ "USB_HostCancelTransfer", "group__usb__host__drv.html#ga6de8c374d9b41eb77a2666bdd2557bad", null ],
    [ "USB_HostMallocTransfer", "group__usb__host__drv.html#gafc1f5a4b8ee651418cb1e09d9ff2a92d", null ],
    [ "USB_HostFreeTransfer", "group__usb__host__drv.html#gaa74c51699203009d23f2b75aa6055223", null ],
    [ "USB_HostRequestControl", "group__usb__host__drv.html#ga107d1df6f17b2fce741369da1171536f", null ],
    [ "USB_HostOpenDeviceInterface", "group__usb__host__drv.html#ga9f7f1bf84122a5d12347348a91337af8", null ],
    [ "USB_HostCloseDeviceInterface", "group__usb__host__drv.html#ga2c4bd7b6c05d2150f788a1b79fc3d994", null ],
    [ "USB_HostGetVersion", "group__usb__host__drv.html#gad021de531af53e106c4bbc770871a7ec", null ],
    [ "USB_HostAttachDevice", "group__usb__host__drv.html#gaa540b7cc8a65ea46fc5acf43a8f16ddc", null ],
    [ "USB_HostDetachDevice", "group__usb__host__drv.html#ga2ca365fa7d0a5fe85da965e1322c2f4b", null ],
    [ "USB_HostDetachDeviceInternal", "group__usb__host__drv.html#gabd758a1fcb87b98e5762de9b62df9a98", null ],
    [ "USB_HostGetDeviceAttachState", "group__usb__host__drv.html#gac93b0f9c305f5f0e0ebedf703af49369", null ],
    [ "USB_HostValidateDevice", "group__usb__host__drv.html#gac6c685b9569ac60f88599b66265bdb29", null ]
];