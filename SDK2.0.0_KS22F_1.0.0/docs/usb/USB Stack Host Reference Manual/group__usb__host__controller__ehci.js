var group__usb__host__controller__ehci =
[
    [ "usb_host_ehci_pipe_t", "group__usb__host__controller__ehci.html#structusb__host__ehci__pipe__t", [
      [ "pipeCommon", "group__usb__host__controller__ehci.html#a95ffd75e49e9b9f377923f109ea14348", null ],
      [ "ehciQh", "group__usb__host__controller__ehci.html#a4566d6403b2699222ec08e3397d17fbd", null ],
      [ "uframeInterval", "group__usb__host__controller__ehci.html#a6e3be150e018f1f6e47ac9a6e70efd0d", null ],
      [ "startFrame", "group__usb__host__controller__ehci.html#a1b99e3c6dca5fedfcf06bf928cbf38f9", null ],
      [ "dataTime", "group__usb__host__controller__ehci.html#a864c1fc318b111e2db9f9ee982ac8095", null ],
      [ "startSplitTime", "group__usb__host__controller__ehci.html#a8392d05a104a4f410f0c721030a0a4cb", null ],
      [ "completeSplitTime", "group__usb__host__controller__ehci.html#a8a5d567ee4e9734ce81ab179ea99e7e6", null ],
      [ "startUframe", "group__usb__host__controller__ehci.html#abfb8f3b21997ad6e9bd51d5d653aa286", null ],
      [ "uframeSmask", "group__usb__host__controller__ehci.html#a347db9138d5807d6817a02ea3ae124ac", null ],
      [ "uframeCmask", "group__usb__host__controller__ehci.html#a4ef87a67c97adf3dc5d93631ead55781", null ]
    ] ],
    [ "usb_host_ehci_qh_t", "group__usb__host__controller__ehci.html#structusb__host__ehci__qh__t", [
      [ "horizontalLinkPointer", "group__usb__host__controller__ehci.html#ac7c2b2e64a381c968889d80d1bc5926a", null ],
      [ "staticEndpointStates", "group__usb__host__controller__ehci.html#ae4bc8921dd0642e62577838a5beb91ef", null ],
      [ "currentQtdPointer", "group__usb__host__controller__ehci.html#adb36cf3d85709db76ec8106b3c29026f", null ],
      [ "nextQtdPointer", "group__usb__host__controller__ehci.html#a888fa467e5ef57c7dac83accb58c51dd", null ],
      [ "alternateNextQtdPointer", "group__usb__host__controller__ehci.html#aea17b63bbd4fc166f6382878040838d5", null ],
      [ "transferOverlayResults", "group__usb__host__controller__ehci.html#adb174eeab38831f31e8208cce8826033", null ],
      [ "ehciPipePointer", "group__usb__host__controller__ehci.html#a59df3ae64c57af903ed7db92fccc0d33", null ],
      [ "ehciTransferHead", "group__usb__host__controller__ehci.html#a2d8b912af16da0bc5ae540ccfb6eec0b", null ],
      [ "ehciTransferTail", "group__usb__host__controller__ehci.html#acd3d417a961ee7337f7b0f58b801a1e7", null ],
      [ "timeOutValue", "group__usb__host__controller__ehci.html#a104d56a20f5cd97f1fabe04a4cf6a4b4", null ],
      [ "timeOutLabel", "group__usb__host__controller__ehci.html#a9d89b0d4c5bbedd3c974a4f749cf98d5", null ]
    ] ],
    [ "usb_host_ehci_qtd_t", "group__usb__host__controller__ehci.html#structusb__host__ehci__qtd__t", [
      [ "nextQtdPointer", "group__usb__host__controller__ehci.html#aeb3178d6f5bdd8e998a2f3d63c11baec", null ],
      [ "alternateNextQtdPointer", "group__usb__host__controller__ehci.html#a93865e3e0bb7affd8bd404bdb2b64c29", null ],
      [ "transferResults", "group__usb__host__controller__ehci.html#a034574d0471375d08d25dbe127e78e9a", null ],
      [ "bufferPointers", "group__usb__host__controller__ehci.html#adeea732fed610a8281dc4726143f1034", null ]
    ] ],
    [ "usb_host_ehci_itd_t", "group__usb__host__controller__ehci.html#structusb__host__ehci__itd__t", [
      [ "nextLinkPointer", "group__usb__host__controller__ehci.html#ab902e99aa996c20241b358631fb19c5c", null ],
      [ "transactions", "group__usb__host__controller__ehci.html#aaa6e8a363d14f44809642e8ef45f6626", null ],
      [ "bufferPointers", "group__usb__host__controller__ehci.html#a8272c6a7dbe9263901ff74f7923e2d60", null ],
      [ "nextItdPointer", "group__usb__host__controller__ehci.html#ace7b55860cefa33c0ee118b53c72835a", null ],
      [ "frameEntryIndex", "group__usb__host__controller__ehci.html#a185adb2f744e76ce58468500ea6296be", null ],
      [ "reserved", "group__usb__host__controller__ehci.html#a08cd52ca66295bf14fc507a3f3acc6fe", null ]
    ] ],
    [ "usb_host_ehci_sitd_t", "group__usb__host__controller__ehci.html#structusb__host__ehci__sitd__t", [
      [ "nextLinkPointer", "group__usb__host__controller__ehci.html#afae235d617c979e81a5e6e5adea37526", null ],
      [ "endpointStates", "group__usb__host__controller__ehci.html#acb1261d5262a7f99d937dcb7fb5c6528", null ],
      [ "transferResults", "group__usb__host__controller__ehci.html#a1cf50136abb1a17edb5024d653144d1d", null ],
      [ "backPointer", "group__usb__host__controller__ehci.html#ad3cdeb39a9f31677084aa094e6fc6d3b", null ],
      [ "frameEntryIndex", "group__usb__host__controller__ehci.html#ab896155ffb8db7f3c03d2ee596fbebd1", null ],
      [ "nextSitdIndex", "group__usb__host__controller__ehci.html#a451655d9b6f1bc6c21fa503328eb15df", null ],
      [ "reserved", "group__usb__host__controller__ehci.html#adcd7dde95a9e99834b2b01fb4d94ae3c", null ]
    ] ],
    [ "usb_host_ehci_iso_t", "group__usb__host__controller__ehci.html#structusb__host__ehci__iso__t", [
      [ "next", "group__usb__host__controller__ehci.html#afb7f6f1163e2a51da289dab689990bfa", null ],
      [ "ehciPipePointer", "group__usb__host__controller__ehci.html#aa94ae75b86dc441829bfd05c635d2b3a", null ],
      [ "ehciTransferHead", "group__usb__host__controller__ehci.html#ae2dbfb9ce167d3fbc334ead7c3792c5f", null ],
      [ "ehciTransferTail", "group__usb__host__controller__ehci.html#ab024877671d9ef26529334cd5a6d863c", null ],
      [ "lastLinkFrame", "group__usb__host__controller__ehci.html#a0040cb7be3d252e22c3167ca114b96c0", null ]
    ] ],
    [ "usb_host_ehci_instance_t", "group__usb__host__controller__ehci.html#structusb__host__ehci__instance__t", [
      [ "hostHandle", "group__usb__host__controller__ehci.html#a8fbbf6b493a2a20b7a0553f0fd8642ff", null ],
      [ "ehciUnitBase", "group__usb__host__controller__ehci.html#a83e49a4d4a7a5b6fa10654019ac22108", null ],
      [ "ehciQhList", "group__usb__host__controller__ehci.html#a1fe9c94e82628e14fb3e79f61219ed61", null ],
      [ "ehciQtdHead", "group__usb__host__controller__ehci.html#a299de7033270d99a853c87c536d00780", null ],
      [ "ehciQtdTail", "group__usb__host__controller__ehci.html#a2715c30dd7b3b8f2e065acb43990a547", null ],
      [ "ehciItdList", "group__usb__host__controller__ehci.html#a3e71c5cb54fbeb70d9d44920542ee21e", null ],
      [ "ehciSitdIndexBase", "group__usb__host__controller__ehci.html#ab1da6f132cd3c93742a40e542e4c6f19", null ],
      [ "ehciSitdList", "group__usb__host__controller__ehci.html#a72dae4ac68d51d36b8e55e58aeeb72f4", null ],
      [ "ehciIsoList", "group__usb__host__controller__ehci.html#a060a488cc5e2e5ef7f0ea0daad271e9c", null ],
      [ "ehciIpBase", "group__usb__host__controller__ehci.html#aec5a99697513e079cfd4666d38e90b8e", null ],
      [ "shedFirstQh", "group__usb__host__controller__ehci.html#a31e70232ce125b707ceeef9233e1fa2e", null ],
      [ "ehciPipeIndexBase", "group__usb__host__controller__ehci.html#a82493508fd31853391e4b1864b5f5d78", null ],
      [ "ehciPipeList", "group__usb__host__controller__ehci.html#a9b8f4b56ae78a10343f3726e0c46a33c", null ],
      [ "ehciRunningPipeList", "group__usb__host__controller__ehci.html#ab193ed11976cc39951cb7311f97946ad", null ],
      [ "ehciMutex", "group__usb__host__controller__ehci.html#a3e3504e1986d8b1c1b6aa3238766c297", null ],
      [ "taskEventHandle", "group__usb__host__controller__ehci.html#ac030f284623723527772f38c0b8c486e", null ],
      [ "controllerId", "group__usb__host__controller__ehci.html#a48707cb4a2faee87170d3e128746fa0b", null ],
      [ "deviceAttached", "group__usb__host__controller__ehci.html#ac213bd2a68e4a33cf07d6088a5d2d930", null ],
      [ "firstDeviceSpeed", "group__usb__host__controller__ehci.html#af382f0c107fadf4f17f7659f48db105f", null ],
      [ "ehciItdNumber", "group__usb__host__controller__ehci.html#ac7767577b8e5836aa737e826085880cf", null ],
      [ "ehciSitdNumber", "group__usb__host__controller__ehci.html#abfaff23916e7980701a666b2a3a23d89", null ],
      [ "ehciQtdNumber", "group__usb__host__controller__ehci.html#adf417ccbfa6a0928d8c1cdfe227db516", null ]
    ] ],
    [ "USB_HOST_EHCI_ISO_NUMBER", "group__usb__host__controller__ehci.html#ga2922f45f92d7a1fdcac522bbdaefb8c4", null ],
    [ "USB_HOST_EHCI_PORT_CONNECT_DEBOUNCE_DELAY", "group__usb__host__controller__ehci.html#gadcb2005d7c962e905dbffa47d9730254", null ],
    [ "USB_HOST_EHCI_PORT_RESET_DELAY", "group__usb__host__controller__ehci.html#ga050bcb4d2dbcba1cbf74451e2b68d1bf", null ],
    [ "USB_HOST_EHCI_ISO_BOUNCE_FRAME_NUMBER", "group__usb__host__controller__ehci.html#gabb16981a5f046180ac458a4555d51aa0", null ],
    [ "USB_HOST_EHCI_ISO_BOUNCE_UFRAME_NUMBER", "group__usb__host__controller__ehci.html#ga91876d07e49f2ba3adc6115b5aabe7e2", null ],
    [ "USB_HOST_EHCI_CONTROL_BULK_TIME_OUT_VALUE", "group__usb__host__controller__ehci.html#ga60ae1fe6a2774611c317423a9f523b16", null ],
    [ "host_ehci_device_state_t", "group__usb__host__controller__ehci.html#gad8e2935a72b978cb1af7d8d7f654e9f0", [
      [ "kEHCIDevicePhyAttached", "group__usb__host__controller__ehci.html#ggad8e2935a72b978cb1af7d8d7f654e9f0aaef9d53c1d5944910be4bdbae10060b4", null ],
      [ "kEHCIDeviceAttached", "group__usb__host__controller__ehci.html#ggad8e2935a72b978cb1af7d8d7f654e9f0ac8efc7448b74241138b83227c0155051", null ],
      [ "kEHCIDeviceDetached", "group__usb__host__controller__ehci.html#ggad8e2935a72b978cb1af7d8d7f654e9f0ad7a8be4d7cdce1395a85a844b651b3db", null ]
    ] ],
    [ "USB_HostEhciCreate", "group__usb__host__controller__ehci.html#ga2fc5dc841996ce3794fed277446d5364", null ],
    [ "USB_HostEhciDestory", "group__usb__host__controller__ehci.html#ga41bb5d1519eb5737dccb2c488c3671a9", null ],
    [ "USB_HostEhciOpenPipe", "group__usb__host__controller__ehci.html#gacd8fa8458ce9cfb3e65d1e215dfc86d5", null ],
    [ "USB_HostEhciClosePipe", "group__usb__host__controller__ehci.html#gaf4fa5ad319c01e8fb3676038c1b8bc49", null ],
    [ "USB_HostEhciWritePipe", "group__usb__host__controller__ehci.html#ga7a80165cc92170fd963e57abf6f4c154", null ],
    [ "USB_HostEhciReadpipe", "group__usb__host__controller__ehci.html#ga697c60b5138f5643b90b74fb897c89ae", null ],
    [ "USB_HostEhciIoctl", "group__usb__host__controller__ehci.html#ga28e50b13d29c9503381ac9c2288c1947", null ]
];