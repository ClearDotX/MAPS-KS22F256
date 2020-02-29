var group__usb__host__phdc__drv =
[
    [ "usb_host_phdc_class_function_desciptor_t", "group__usb__host__phdc__drv.html#structusb__host__phdc__class__function__desciptor__t", [
      [ "bLength", "group__usb__host__phdc__drv.html#ad27eb5b36442ef5caf784c0a891faf19", null ],
      [ "bDescriptortype", "group__usb__host__phdc__drv.html#a73d1d0b001a6e9a406860d45394d44bf", null ],
      [ "bPhdcDataCode", "group__usb__host__phdc__drv.html#a60787b43f5c08f1dada2ab966c1c44c1", null ],
      [ "bmCapability", "group__usb__host__phdc__drv.html#a53d0bcd9634bbaa3512dea712a8bde96", null ]
    ] ],
    [ "usb_host_phdc_function_extension_descriptor_t", "group__usb__host__phdc__drv.html#structusb__host__phdc__function__extension__descriptor__t", [
      [ "bLength", "group__usb__host__phdc__drv.html#af69716e4e5e0414797d082f1b3a7ff0c", null ],
      [ "bDescriptortype", "group__usb__host__phdc__drv.html#aa9f3cac478417294568eee37e9e4cf97", null ],
      [ "bReserved", "group__usb__host__phdc__drv.html#a61dc0cf669acc03664313c5daaa0b4f3", null ],
      [ "bNumDevSpecs", "group__usb__host__phdc__drv.html#a41ed25dee9dfc136b18e8a0f3324755d", null ],
      [ "wDevSpecializations", "group__usb__host__phdc__drv.html#a3d352a67827b2766a3b0257237e9f3b5", null ]
    ] ],
    [ "usb_host_phdc_qos_descriptor_t", "group__usb__host__phdc__drv.html#structusb__host__phdc__qos__descriptor__t", [
      [ "bLength", "group__usb__host__phdc__drv.html#ae5a214746600ab796dfea2a88736b5bb", null ],
      [ "bDescriptortype", "group__usb__host__phdc__drv.html#a4332f240e344ebc80ccb09c71efeaac7", null ],
      [ "bQosEncodingVersion", "group__usb__host__phdc__drv.html#a21ca07287260e77ad910a1b83a495c81", null ],
      [ "bmLatencyReliability", "group__usb__host__phdc__drv.html#a51edd75ccc10fd6f4d3e64bccefb7740", null ]
    ] ],
    [ "usb_host_phdc_metadata_descriptor_t", "group__usb__host__phdc__drv.html#structusb__host__phdc__metadata__descriptor__t", [
      [ "bLength", "group__usb__host__phdc__drv.html#a002b225445c7c6eb968fb154e70f9b6d", null ],
      [ "bDescriptortype", "group__usb__host__phdc__drv.html#a2aa4a951b321c5d3582212dee43e8467", null ],
      [ "bOpaqueData", "group__usb__host__phdc__drv.html#afb74d24d83f7da99eb68c0095c4ff182", null ]
    ] ],
    [ "usb_host_phdc_metadata_preamble_t", "group__usb__host__phdc__drv.html#structusb__host__phdc__metadata__preamble__t", [
      [ "aSignature", "group__usb__host__phdc__drv.html#a0e71ec5e9850342313e720945301066d", null ],
      [ "bNumberTransfers", "group__usb__host__phdc__drv.html#a47c93c707ade4bc3a28c8b6c10a43a13", null ],
      [ "bQosEncodingVersion", "group__usb__host__phdc__drv.html#ac487f90abb6a2ff89ffd00930759f45b", null ],
      [ "bmLatencyReliability", "group__usb__host__phdc__drv.html#a0c638d19a8c55fc39199ce43a33d22ee", null ],
      [ "bOpaqueDataSize", "group__usb__host__phdc__drv.html#a4ea0b5ac9c7b0540fce8fbcdf95c7a2e", null ],
      [ "bOpaqueData", "group__usb__host__phdc__drv.html#a9f26b07806ccd6e8f4dfee167bbc3a88", null ]
    ] ],
    [ "usb_host_phdc_instance_t", "group__usb__host__phdc__drv.html#structusb__host__phdc__instance__t", [
      [ "hostHandle", "group__usb__host__phdc__drv.html#ae172c28dcd07dc35bda78d4601187e5f", null ],
      [ "deviceHandle", "group__usb__host__phdc__drv.html#a93c05c2b7bf7749bc51eab4b4ba7529c", null ],
      [ "interfaceHandle", "group__usb__host__phdc__drv.html#aff892568ef9b784ccaa465c6535c79e8", null ],
      [ "controlPipe", "group__usb__host__phdc__drv.html#a5acf320b76da0d74073d1a332881da06", null ],
      [ "interruptPipe", "group__usb__host__phdc__drv.html#a26b09f5231b8f21a6a9250c38e97c2a4", null ],
      [ "bulkInPipe", "group__usb__host__phdc__drv.html#ad6f76f2f2addc5923e2412e5e02cea65", null ],
      [ "bulkOutPipe", "group__usb__host__phdc__drv.html#a5bcbb5bac0a7630c0b02a8ad6869ce20", null ],
      [ "inCallbackFn", "group__usb__host__phdc__drv.html#a554191fce7caf16789dd73a77c250027", null ],
      [ "inCallbackParam", "group__usb__host__phdc__drv.html#af439130cd1bc5926ed15fdf9d3591f3f", null ],
      [ "outCallbackFn", "group__usb__host__phdc__drv.html#a87d7436f5338b90c8f6ba88130223383", null ],
      [ "outCallbackParam", "group__usb__host__phdc__drv.html#aee1319a0a4f004881bb2a0f207a36e52", null ],
      [ "controlCallbackFn", "group__usb__host__phdc__drv.html#a0de72870b8f276b08606628f6e1739d3", null ],
      [ "controlCallbackParam", "group__usb__host__phdc__drv.html#a8caf15f6b8fbef7c6191cc4f0e375b49", null ],
      [ "controlTransfer", "group__usb__host__phdc__drv.html#ae5fc878eb176c83a8467c17ea970ba63", null ],
      [ "interruptInEndpointInformation", "group__usb__host__phdc__drv.html#ac3b4db1e9bb308ad6ae9e93871aa1dd0", null ],
      [ "bulkInEndpointInformation", "group__usb__host__phdc__drv.html#a768b67d7a6d617fc0cf81976427e1d23", null ],
      [ "bulkOutEndpointInformation", "group__usb__host__phdc__drv.html#ad9f96004c41c0813b2c0f9168e405348", null ],
      [ "isMessagePreambleEnabled", "group__usb__host__phdc__drv.html#a409420985c952e31e52871b901c9288b", null ],
      [ "numberTransferBulkOut", "group__usb__host__phdc__drv.html#ad2ff040da07996deaeb01a0e2c9c3176", null ],
      [ "numberTransferBulkIn", "group__usb__host__phdc__drv.html#a07f3a5ba3e58efa2b6a98433b57cd85a", null ]
    ] ],
    [ "USB_HOST_PHDC_CLASS_CODE", "group__usb__host__phdc__drv.html#gaacaaee9ded8346b0aa6a185d026ef616", null ],
    [ "USB_HOST_PHDC_SUBCLASS_CODE", "group__usb__host__phdc__drv.html#gaeeb12ca25083fb5ad28b3d91dd3afedc", null ],
    [ "USB_HOST_PHDC_PROTOCOL", "group__usb__host__phdc__drv.html#gad861a5ab1c67df1e4f4bbf300a5900ac", null ],
    [ "USB_HOST_PHDC_GET_STATUS_REQUEST", "group__usb__host__phdc__drv.html#gad4cbad41d39b38e6c93593c1ded3b8c3", null ],
    [ "USB_HOST_PHDC_SET_FEATURE_REQUEST", "group__usb__host__phdc__drv.html#ga83db849fcf1862ec3a1abe75e69df7df", null ],
    [ "USB_HOST_PHDC_CLEAR_FEATURE_REQUEST", "group__usb__host__phdc__drv.html#ga1ea13a52c31e5911e581f52639575942", null ],
    [ "USB_HOST_PHDC_FEATURE_METADATA", "group__usb__host__phdc__drv.html#ga36c287958ff1ceac5daa6b7759aefe85", null ],
    [ "USB_HOST_PHDC_QOS_ENCODING_VERSION", "group__usb__host__phdc__drv.html#gaef3338badb1d05e6e35c51b3446a01bc", null ],
    [ "USB_HOST_PHDC_MESSAGE_PREAMBLE_SIGNATURE_SIZE", "group__usb__host__phdc__drv.html#ga7a36ba3f33ee6ece52b3b877bb0f1739", null ],
    [ "USB_HOST_PHDC_CLASSFUNCTION_DESCRIPTOR", "group__usb__host__phdc__drv.html#ga203b2c125964fd8abf5cb96b7113a291", null ],
    [ "USB_HOST_PHDC_QOS_DESCRIPTOR", "group__usb__host__phdc__drv.html#gaba4799625a86760ea4eca379b2a101ff", null ],
    [ "USB_HOST_PHDC_11073PHD_FUNCTION_DESCRIPTOR", "group__usb__host__phdc__drv.html#ga46050d9932b85a594cba73392cb2ff23", null ],
    [ "USB_HOST_PHDC_METADATA_DESCRIPTOR", "group__usb__host__phdc__drv.html#ga9613a5432d601983e551a3f0eb21d141", null ],
    [ "USB_HostPhdcInit", "group__usb__host__phdc__drv.html#ga7ada8936850734f21250523b28cfd707", null ],
    [ "USB_HostPhdcSetInterface", "group__usb__host__phdc__drv.html#gab374e187f3fb6321129e690209e29e61", null ],
    [ "USB_HostPhdcDeinit", "group__usb__host__phdc__drv.html#ga19b339ec8ff88b6924ebddcb2cb5e8aa", null ],
    [ "USB_HostPhdcRecv", "group__usb__host__phdc__drv.html#ga3f431438498c63085b32caadca66c0ff", null ],
    [ "USB_HostPhdcSend", "group__usb__host__phdc__drv.html#ga0c71a96289711d1eadc0b7ae546654c2", null ],
    [ "USB_HostPhdcSendControlRequest", "group__usb__host__phdc__drv.html#ga9d43446bc878a5001a865e0e1d222422", null ],
    [ "USB_HostPhdcSetClearFeatureEndpointHalt", "group__usb__host__phdc__drv.html#ga29f1afd158caa886c282f1f3c8414743", null ],
    [ "USB_HostPhdcGetEndpointInformation", "group__usb__host__phdc__drv.html#ga48145297eb29ae25c45ff951b6729fda", null ]
];