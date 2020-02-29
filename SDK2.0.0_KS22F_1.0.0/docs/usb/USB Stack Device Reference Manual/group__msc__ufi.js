var group__msc__ufi =
[
    [ "usb_device_inquiry_command_struct_t", "group__msc__ufi.html#structusb__device__inquiry__command__struct__t", [
      [ "operationCode", "group__msc__ufi.html#ac779db5a21a05a5a7a91d1abe3510c58", null ],
      [ "logicalUnitNumber", "group__msc__ufi.html#a39aa47893288f2a90b8324d5499c7499", null ],
      [ "pageCode", "group__msc__ufi.html#a90f891a71a8e27bae04cac99b9e7ad9e", null ],
      [ "reserved", "group__msc__ufi.html#a22037b1d57f91f3fe86d75f048d7980d", null ],
      [ "allocationLength", "group__msc__ufi.html#a2143a52be634077015af190465a5b518", null ],
      [ "reserved1", "group__msc__ufi.html#a25593c377455a8721c28e35bb6324147", null ]
    ] ],
    [ "usb_device_request_sense_command_struct_t", "group__msc__ufi.html#structusb__device__request__sense__command__struct__t", [
      [ "operationCode", "group__msc__ufi.html#a4697b4a21a04d97eab603a242e590e13", null ],
      [ "logicalUnitNumber", "group__msc__ufi.html#a1f852bb9d8119be72e6d7ba18867f79e", null ],
      [ "reserved", "group__msc__ufi.html#a2936fcaabeae5922605d912765f76869", null ],
      [ "allocationLength", "group__msc__ufi.html#a1965854f6d2a7bfa4ce40b6d248461a1", null ],
      [ "reserved1", "group__msc__ufi.html#ac230aaed780f883893d2005f7019f645", null ]
    ] ],
    [ "usb_device_read_format_capatities_command_struct_t", "group__msc__ufi.html#structusb__device__read__format__capatities__command__struct__t", [
      [ "operationCode", "group__msc__ufi.html#a683968cd1338b9c106acf8f8c17169aa", null ],
      [ "logicalUnitNumber", "group__msc__ufi.html#a557ffb15f21b7a9627385c3d9bff6cc1", null ],
      [ "reserved", "group__msc__ufi.html#a2b2c00db29f5703a3d313538e8231515", null ],
      [ "allocationLength", "group__msc__ufi.html#a58ed8cabcbb462ad56dd76e20566d7d6", null ],
      [ "reserved1", "group__msc__ufi.html#a294b9d8ae300de27ff323f41488c8f79", null ]
    ] ],
    [ "usb_device_read_capatities_command_struct_t", "group__msc__ufi.html#structusb__device__read__capatities__command__struct__t", [
      [ "operationCode", "group__msc__ufi.html#a4a098ef445d2bd78f4cf82df44b913ff", null ],
      [ "logicalUnitNumber", "group__msc__ufi.html#af01c3b7f458e31d765847bd472278734", null ],
      [ "lba", "group__msc__ufi.html#a5c5db6e4405df6b89fb9c77ab26ab817", null ],
      [ "reserved", "group__msc__ufi.html#af6b9fe3f332292709aea8868c14f61c8", null ],
      [ "pmi", "group__msc__ufi.html#af66d75ef9b61df3b900ae4daa0fe130f", null ],
      [ "reserved1", "group__msc__ufi.html#a86d5e033eec460c48e05348d390d738c", null ]
    ] ],
    [ "usb_device_read_write_10_command_struct_t", "group__msc__ufi.html#structusb__device__read__write__10__command__struct__t", [
      [ "operationCode", "group__msc__ufi.html#a52f751b4b51797d3c01d30a0ab829d1e", null ],
      [ "lunDpoFuaReladr", "group__msc__ufi.html#a6d6acf9145baf45cb73ba9280a78db8c", null ],
      [ "lba", "group__msc__ufi.html#aef671c460da029eece61e353189327ef", null ],
      [ "reserved", "group__msc__ufi.html#a7cb3f0c230fa0554819b7f2fc44e3d18", null ],
      [ "transferLengthMsb", "group__msc__ufi.html#afabb8aaf7e54c042ff3b9a13aa705082", null ],
      [ "transferLengthLsb", "group__msc__ufi.html#a081c7ae3db7be0e37012b429f07c1f67", null ],
      [ "reserved1", "group__msc__ufi.html#a836afe5de29940356d8e7aa5242bde87", null ]
    ] ],
    [ "usb_device_inquiry_data_fromat_struct_t", "group__msc__ufi.html#structusb__device__inquiry__data__fromat__struct__t", [
      [ "peripheralDeviceType", "group__msc__ufi.html#aefa2dd7e0f5b0ed6a00c613631d766f0", null ],
      [ "rmb", "group__msc__ufi.html#acecb707c602a89e3a298f25d2d8489d2", null ],
      [ "versions", "group__msc__ufi.html#a59002f3bba2b539b37256d1c9eb94969", null ],
      [ "responseDataFormat", "group__msc__ufi.html#ac3df4c1701903dcf20aa67d3cbaddcc9", null ],
      [ "additionalLength", "group__msc__ufi.html#a271a2f2d9cbb17a6f0ee0855a63e173d", null ],
      [ "reserved", "group__msc__ufi.html#aaeff3464f3207fe6cf45b84f59f690eb", null ],
      [ "vendorInformatin", "group__msc__ufi.html#a048c9695f7d7de8efc021dc948e86809", null ],
      [ "productId", "group__msc__ufi.html#a28853f6ae6bebf71cdc8ab45e938821b", null ],
      [ "productVersionLevel", "group__msc__ufi.html#ac8f8d4b9199b42a701b6f1d88208aed1", null ]
    ] ],
    [ "usb_device_request_sense_data_struct_t", "group__msc__ufi.html#structusb__device__request__sense__data__struct__t", [
      [ "validErrorCode", "group__msc__ufi.html#a039c0ff3c6d2ad5a610d30ba432c4f7c", null ],
      [ "reserved", "group__msc__ufi.html#a6c2f632057df886b8ee8a84c60c673ed", null ],
      [ "senseKey", "group__msc__ufi.html#a5e9cede8e9895e9fbfdf11e578566736", null ],
      [ "information", "group__msc__ufi.html#ab5edbc0f5d33d75dd14a5fccdc15c009", null ],
      [ "additionalSenseLength", "group__msc__ufi.html#a017f4d22d4d52eb263d6659aac6351f1", null ],
      [ "reserved1", "group__msc__ufi.html#ad52f6c8aaaf9d8bf255035cd015dc1b6", null ],
      [ "additionalSenseCode", "group__msc__ufi.html#a239d9e483aa52b942f5e6f6347bd6310", null ],
      [ "additionalSenseQualifer", "group__msc__ufi.html#a76da3621b56258520065245762eb2a52", null ],
      [ "reserved2", "group__msc__ufi.html#ae4b0c35b5e5d2e971ea5289465f0cb55", null ]
    ] ],
    [ "usb_device_read_capacity_struct_t", "group__msc__ufi.html#structusb__device__read__capacity__struct__t", [
      [ "lastLogicalBlockAddress", "group__msc__ufi.html#ae4c55127f7630d3aec496dd939fc18ab", null ],
      [ "blockSize", "group__msc__ufi.html#afb8e968a8941f5dbaf15f992894b6baf", null ]
    ] ],
    [ "usb_device_read_capacity16_data_struct_t", "group__msc__ufi.html#structusb__device__read__capacity16__data__struct__t", [
      [ "lastLogicalBlockAddress0", "group__msc__ufi.html#afac025c1be77eb45f059a1de1e87a696", null ],
      [ "lastLogicalBlockAddress1", "group__msc__ufi.html#a92844a06b480b5e66f9cc1ef557fe6da", null ],
      [ "blockSize", "group__msc__ufi.html#a14eaecebc6ff0361d534ae97c71fc0b7", null ]
    ] ],
    [ "usb_device_capacity_list_header_struct_t", "group__msc__ufi.html#structusb__device__capacity__list__header__struct__t", [
      [ "reserverd", "group__msc__ufi.html#a55804edc4ec0469622dbac029a1adf8d", null ],
      [ "capacityListLength", "group__msc__ufi.html#ae324417747653fb3c74f52ba3a5e87b5", null ]
    ] ],
    [ "usb_device_current_max_capacity_descriptor_struct_t", "group__msc__ufi.html#structusb__device__current__max__capacity__descriptor__struct__t", [
      [ "blockNumber", "group__msc__ufi.html#af195bf3d92bf238fd60a6f145b4caa82", null ],
      [ "descriptorCodeBlockLength", "group__msc__ufi.html#ab534171b2f44d3a0cbebb3c1e29af2d3", null ]
    ] ],
    [ "usb_device_formattable_capacity_descriptor_struct_t", "group__msc__ufi.html#structusb__device__formattable__capacity__descriptor__struct__t", [
      [ "blockNumber", "group__msc__ufi.html#a7048fcedf4dcaf68d510e7e84b306b7f", null ],
      [ "blockLength", "group__msc__ufi.html#abb0e278bb4f5eb98b23bd62a2c143e45", null ]
    ] ],
    [ "usb_device_mode_parameters_header_struct_t", "group__msc__ufi.html#structusb__device__mode__parameters__header__struct__t", [
      [ "modeDataLength", "group__msc__ufi.html#a6714111b25605ea9b745cee300728b92", null ],
      [ "mediumTypeCode", "group__msc__ufi.html#aa740a8a5963cbd18805651f1a96d37ff", null ],
      [ "wpDpfua", "group__msc__ufi.html#a51eda72f82adf8a51a6ddf8183b8ebdb", null ],
      [ "reserved", "group__msc__ufi.html#a250ea10854b3e8f40e4ab7e9beb49c19", null ]
    ] ],
    [ "usb_device_format_capacity_response_data_struct_t", "group__msc__ufi.html#structusb__device__format__capacity__response__data__struct__t", [
      [ "capacityListHead", "group__msc__ufi.html#a03e1746a444af094d4fff17c49f05bec", null ],
      [ "currentMaxCapacityDesccriptor", "group__msc__ufi.html#a5a92012bc37e831bbe8bb33b22c92694", null ],
      [ "formattableCapacityDesccriptor", "group__msc__ufi.html#af5cd257ed91a6112a1cdb73ad9c568dd", null ]
    ] ],
    [ "USB_DEVICE_MSC_UFI_NO_SENSE", "group__msc__ufi.html#ga431aecf8984c93a1efadf1f092acd0a4", null ],
    [ "USB_DEVICE_MSC_UFI_RECOVERED_ERROR", "group__msc__ufi.html#gae24540c5c8a0819e72fd49934d881d49", null ],
    [ "USB_DEVICE_MSC_UFI_NOT_READY", "group__msc__ufi.html#ga03646b2d08939bdef2f7e971bcac8829", null ],
    [ "USB_DEVICE_MSC_UFI_MEDIUM_ERROR", "group__msc__ufi.html#ga5d5a336c24295b635912c5a340c1eefa", null ],
    [ "USB_DEVICE_MSC_UFI_HARDWARE_ERROR", "group__msc__ufi.html#gaf551a51a25aee793e313e280eb7ab722", null ],
    [ "USB_DEVICE_MSC_UFI_ILLEGAL_REQUEST", "group__msc__ufi.html#ga1dd1f81820e5ab16d306d9057cbad5e6", null ],
    [ "USB_DEVICE_MSC_UFI_UNIT_ATTENTION", "group__msc__ufi.html#ga6c2a71b7e71453a393ea3eac07531bc9", null ],
    [ "USB_DEVICE_MSC_UFI_DATA_PROTECT", "group__msc__ufi.html#gaf61a04ab0064618a6dc7951c4fb69a7e", null ],
    [ "USB_DEVICE_MSC_UFI_BLANK_CHECK", "group__msc__ufi.html#ga7caf7d981f3d1a4b8684b381e43142eb", null ],
    [ "USB_DEVICE_MSC_UFI_VENDOR_SPECIFIC_ERROR", "group__msc__ufi.html#ga4c0be4575f16164b51073664fe463e59", null ],
    [ "USB_DEVICE_MSC_UFI_ABORTED_COMMAND", "group__msc__ufi.html#gafae75920902f07bf65efae1b06b61a4b", null ],
    [ "USB_DEVICE_MSC_UFI_VOLUME_OVERFLOW", "group__msc__ufi.html#ga23e1a0802de725c30b57d368303a06a3", null ],
    [ "USB_DEVICE_MSC_UFI_MISCOMPARE", "group__msc__ufi.html#ga017d83556bced9d9b52c07f969b845c3", null ],
    [ "USB_DEVICE_MSC_UFI_INVALID_COMMAND_OPCODE", "group__msc__ufi.html#gac6d73980664d3ac370a52df4a5186900", null ],
    [ "USB_DEVICE_MSC_UFI_WRITE_FAULT", "group__msc__ufi.html#gaca7bf3c15a4d51c7dcef2051a8f374b9", null ],
    [ "USB_DEVICE_MSC_UFI_UNRECOVERED_READ_ERROR", "group__msc__ufi.html#ga90d1e6ac000072a12d44317efccb9720", null ],
    [ "USB_DEVICE_MSC_UFI_UNKNOWN_ERROR", "group__msc__ufi.html#ga1dfa8f84c2de2288a418ee5e01ae86c5", null ],
    [ "USB_DEVICE_MSC_UFI_INVALID_FIELD_IN_COMMAND_PKT", "group__msc__ufi.html#ga4ad5e672f2641c01718b0914eaea5376", null ],
    [ "USB_DEVICE_MSC_UFI_LBA_OUT_OF_RANGE", "group__msc__ufi.html#gad7776388423a53a1ff60d1e1dc942171", null ],
    [ "USB_DEVICE_MSC_UFI_REQ_SENSE_VALID_ERROR_CODE", "group__msc__ufi.html#gaa2eb91ef4e8562c6b79613bcff163214", null ],
    [ "USB_DEVICE_MSC_UFI_REQ_SENSE_ADDITIONAL_SENSE_LEN", "group__msc__ufi.html#ga748ef6aef62954add98f2f34501685ce", null ],
    [ "USB_DEVICE_MSC_UFI_PREVENT_ALLOW_REMOVAL_MASK", "group__msc__ufi.html#ga2919a2143634dc39533f170b53397bf3", null ],
    [ "USB_DEVICE_MSC_UFI_LOAD_EJECT_START_MASK", "group__msc__ufi.html#ga8b281f431d55951fc09973ea45499dc4", null ],
    [ "USB_DEVICE_MSC_UFI_FORMATTED_MEDIA", "group__msc__ufi.html#gaaa642d4a50d9d5395c882a805e6d2f00", null ],
    [ "USB_DEVICE_MSC_UFI_UNFORMATTED_MEDIA", "group__msc__ufi.html#gaad6ade7bd802492291516d51fac5bec5", null ],
    [ "USB_DEVICE_MSC_UFI_NO_CARTRIDGE_IN_DRIVE", "group__msc__ufi.html#ga5873da0b3e87a5ecfea60bf18d43312e", null ],
    [ "USB_DEVICE_MSC_UFI_INQUIRY_ALLOCATION_LENGTH", "group__msc__ufi.html#ga24848236ad6ece4afb01ead7d9dba5cd", null ],
    [ "USB_DEVICE_MSC_UFI_REQ_SENSE_DATA_LENGTH", "group__msc__ufi.html#ga015ceb0527ebc9240d85b7ce0f73047e", null ],
    [ "USB_DEVICE_MSC_UFI_READ_CAPACITY_DATA_LENGTH", "group__msc__ufi.html#ga5ee9f68450e2a8c107aea98138b687e2", null ],
    [ "USB_DEVICE_MSC_UFI_READ_CAPACITY16_DATA_LENGTH", "group__msc__ufi.html#ga164299313e71584c4af4399c7c47f456", null ],
    [ "USB_DEVICE_MSC_UFI_PERIPHERAL_QUALIFIER", "group__msc__ufi.html#gac008e30aca9ab59bc347b04659d9fcbb", null ],
    [ "USB_DEVICE_MSC_UFI_PERIPHERAL_QUALIFIER_SHIFT", "group__msc__ufi.html#ga2709f57bcaf78f6df9ad589d41bb0c02", null ],
    [ "USB_DEVICE_MSC_UFI_VERSIONS", "group__msc__ufi.html#ga8d65b634ff8a8abf8b636e8522e13a53", null ],
    [ "USB_DEVICE_MSC_UFI_PERIPHERAL_DEVICE_TYPE", "group__msc__ufi.html#ga2d678f3b427bcf1331f25a053794f370", null ],
    [ "USB_DEVICE_MSC_UFI_REMOVABLE_MEDIUM_BIT", "group__msc__ufi.html#ga60fb46600b20b3853fc754ccac92d52a", null ],
    [ "USB_DEVICE_MSC_UFI_REMOVABLE_MEDIUM_BIT_SHIFT", "group__msc__ufi.html#gaca29eb8b888afa1f57944c08cb8596b5", null ],
    [ "USB_DEVICE_MSC_UFI_ADDITIONAL_LENGTH", "group__msc__ufi.html#ga369771a234ae385df4f16c4cd57928f2", null ]
];