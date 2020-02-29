var group__flexio__uart =
[
    [ "FLEXIO_UART_Type", "group__flexio__uart.html#structFLEXIO__UART__Type", [
      [ "flexioBase", "group__flexio__uart.html#ad80f1e208b704cc3719933e2448826c4", null ],
      [ "TxPinIndex", "group__flexio__uart.html#ab6df247340eafe9076c6ac3a20f0d81c", null ],
      [ "RxPinIndex", "group__flexio__uart.html#a21879d73921f1cd42601b0a80d029a6a", null ],
      [ "shifterIndex", "group__flexio__uart.html#a36f31f0641373f903d2371aafe42ee67", null ],
      [ "timerIndex", "group__flexio__uart.html#a69c6696d06f94527af4280dc2921022b", null ]
    ] ],
    [ "flexio_uart_config_t", "group__flexio__uart.html#structflexio__uart__config__t", [
      [ "enableUart", "group__flexio__uart.html#aa2daf1e1468130631b21f73e9dcc2e95", null ],
      [ "enableInDoze", "group__flexio__uart.html#ad2752bcddb3b5e9d987eb9d74f3e034b", null ],
      [ "enableInDebug", "group__flexio__uart.html#ac5bf2792d85a76b3652dee457e580218", null ],
      [ "enableFastAccess", "group__flexio__uart.html#a51bdfc0b1d8493123f1be71343c9f070", null ],
      [ "baudRate_Bps", "group__flexio__uart.html#a082e38cf0edf524602d1d7a578949fd6", null ],
      [ "bitCountPerChar", "group__flexio__uart.html#a31835537405f9f15cd37a10b17211990", null ]
    ] ],
    [ "flexio_uart_transfer_t", "group__flexio__uart.html#structflexio__uart__transfer__t", [
      [ "data", "group__flexio__uart.html#a38416ca93058d1875ae13843c6a63827", null ],
      [ "dataSize", "group__flexio__uart.html#a4d962516ae6cc6a2358c4175c154b5b8", null ]
    ] ],
    [ "flexio_uart_handle_t", "group__flexio__uart.html#struct__flexio__uart__handle", [
      [ "txData", "group__flexio__uart.html#ae3d8eb7651736123f7fcfe2cf9c2fc30", null ],
      [ "txDataSize", "group__flexio__uart.html#ad464d30fa2304bbd122747ce92d88268", null ],
      [ "rxData", "group__flexio__uart.html#aed331dee4e91b4554acc26b625cbe63f", null ],
      [ "rxDataSize", "group__flexio__uart.html#a0aa6001272b9d5637fa272208489837b", null ],
      [ "rxRingBuffer", "group__flexio__uart.html#af29ffdc8a03366ed88354e23466a3f83", null ],
      [ "rxRingBufferSize", "group__flexio__uart.html#ac1e6450bbc80511733aaa4b1839c7479", null ],
      [ "rxRingBufferHead", "group__flexio__uart.html#ac4be17b9c7516934956489b6c65f35f1", null ],
      [ "rxRingBufferTail", "group__flexio__uart.html#a1b27a83bb89be05c44cf32184ad9bfb2", null ],
      [ "callback", "group__flexio__uart.html#ae19e57fc92931bc751bc4c142c6335a6", null ],
      [ "userData", "group__flexio__uart.html#ae547f7e39c3448956c7496801a7a853a", null ],
      [ "txState", "group__flexio__uart.html#a8cb8e5214de0990c540ae29202773a72", null ],
      [ "rxState", "group__flexio__uart.html#a7f4b0bf16597c2799d5f97eaa14168c7", null ]
    ] ],
    [ "flexio_uart_dma_handle_t", "group__flexio__uart.html#struct__flexio__uart__dma__handle", [
      [ "callback", "group__flexio__uart.html#a0f0b116f068276903c002ced0e84773a", null ],
      [ "userData", "group__flexio__uart.html#af58e46e9663796941a07d5f86e66c461", null ],
      [ "txDmaHandle", "group__flexio__uart.html#a171898553a60021c426cc44610c75279", null ],
      [ "rxDmaHandle", "group__flexio__uart.html#a960cd180236c2cb8a86d24e5f70b560e", null ],
      [ "txState", "group__flexio__uart.html#a7df6f9bbfc7888c2a5ca7bfa77b12dc1", null ],
      [ "rxState", "group__flexio__uart.html#a55d2af2a9d05c33a9962f38a3266ebd5", null ]
    ] ],
    [ "flexio_uart_edma_handle_t", "group__flexio__uart.html#struct__flexio__uart__edma__handle", [
      [ "callback", "group__flexio__uart.html#a51c14c68c9c8fe8880ce0266d0fb517f", null ],
      [ "userData", "group__flexio__uart.html#a776db0493a197eba59938811591e6610", null ],
      [ "txEdmaHandle", "group__flexio__uart.html#afc8e317c4730e3225433e1328fa73d3d", null ],
      [ "rxEdmaHandle", "group__flexio__uart.html#abf875faf83b61232522aa9766657c5dc", null ],
      [ "txState", "group__flexio__uart.html#a565d9e6fef4177d522cb29a55c9fc13d", null ],
      [ "rxState", "group__flexio__uart.html#a41a55d8ad49ffbf71884686e64b62677", null ]
    ] ],
    [ "FSL_FLEXIO_UART_DRIVER_VERSION", "group__flexio__uart.html#gaa668f10e7e0820c4d504f124ef506b59", null ],
    [ "flexio_uart_transfer_callback_t", "group__flexio__uart.html#ga47ba0f819db72b1dc4c043205ba7cb18", null ],
    [ "flexio_uart_dma_transfer_callback_t", "group__flexio__uart.html#gab60d2fbba744ced187ab9b45c02b2ca9", null ],
    [ "flexio_uart_edma_transfer_callback_t", "group__flexio__uart.html#gab5c1a2db5ce578862b1c444884f72355", null ],
    [ "_flexio_uart_status", "group__flexio__uart.html#ga2b22d3ab9c2d22eea927ee858017f1df", [
      [ "kStatus_FLEXIO_UART_TxBusy", "group__flexio__uart.html#gga2b22d3ab9c2d22eea927ee858017f1dfa741e184e1ca8ef870d70da2be537a6ab", null ],
      [ "kStatus_FLEXIO_UART_RxBusy", "group__flexio__uart.html#gga2b22d3ab9c2d22eea927ee858017f1dfa8dcfa64158a113e9ed598ac23c9df021", null ],
      [ "kStatus_FLEXIO_UART_TxIdle", "group__flexio__uart.html#gga2b22d3ab9c2d22eea927ee858017f1dfa828da21609507716299bfc8640c57ec3", null ],
      [ "kStatus_FLEXIO_UART_RxIdle", "group__flexio__uart.html#gga2b22d3ab9c2d22eea927ee858017f1dfa451ebbdc82bffc6c0205c7c9bd539046", null ],
      [ "kStatus_FLEXIO_UART_ERROR", "group__flexio__uart.html#gga2b22d3ab9c2d22eea927ee858017f1dfa3b7dca7a2a8364d24c686bbfc22aa2bc", null ],
      [ "kStatus_FLEXIO_UART_RxRingBufferOverrun", "group__flexio__uart.html#gga2b22d3ab9c2d22eea927ee858017f1dfa565cdecf3ec56df0f889e2e49d31b34f", null ],
      [ "kStatus_FLEXIO_UART_RxHardwareOverrun", "group__flexio__uart.html#gga2b22d3ab9c2d22eea927ee858017f1dfab892c50df6c586138fe1b37fde07e4ca", null ]
    ] ],
    [ "flexio_uart_bit_count_per_char_t", "group__flexio__uart.html#ga3298baaedfdeb4df8ad44ce716abca7e", [
      [ "kFLEXIO_UART_7BitsPerChar", "group__flexio__uart.html#gga3298baaedfdeb4df8ad44ce716abca7ead9818673f2a8abacdb73e6f323fa4c42", null ],
      [ "kFLEXIO_UART_8BitsPerChar", "group__flexio__uart.html#gga3298baaedfdeb4df8ad44ce716abca7eaa2563f1b9bbacb425bd892eedda0a161", null ],
      [ "kFLEXIO_UART_9BitsPerChar", "group__flexio__uart.html#gga3298baaedfdeb4df8ad44ce716abca7ea8f9d374fa26cfa27dff950fca3e404a5", null ]
    ] ],
    [ "_flexio_uart_interrupt_enable", "group__flexio__uart.html#gaacbc60527c13c7135ab8cdeb379e72fb", [
      [ "kFLEXIO_UART_TxDataRegEmptyInterruptEnable", "group__flexio__uart.html#ggaacbc60527c13c7135ab8cdeb379e72fba41d4cc3f3cf42008e68fe819a9a3da14", null ],
      [ "kFLEXIO_UART_RxDataRegFullInterruptEnable", "group__flexio__uart.html#ggaacbc60527c13c7135ab8cdeb379e72fba8228cbdfa6b84d76cd2e658ae4df09c0", null ]
    ] ],
    [ "_flexio_uart_status_flags", "group__flexio__uart.html#gab80096f9e757f4083565dd183ce8c491", [
      [ "kFLEXIO_UART_TxDataRegEmptyFlag", "group__flexio__uart.html#ggab80096f9e757f4083565dd183ce8c491a8fd298132aa5ee69800dfba7a224a252", null ],
      [ "kFLEXIO_UART_RxDataRegFullFlag", "group__flexio__uart.html#ggab80096f9e757f4083565dd183ce8c491a3bc49d5b480f3a37acfcf82234c6f48b", null ],
      [ "kFLEXIO_UART_RxOverRunFlag", "group__flexio__uart.html#ggab80096f9e757f4083565dd183ce8c491a139fb91d122113f9fde834253d930e26", null ]
    ] ],
    [ "FLEXIO_UART_Init", "group__flexio__uart.html#ga9041220b518f9576e3e20e6ff7d8d148", null ],
    [ "FLEXIO_UART_Deinit", "group__flexio__uart.html#ga641ed5c6b8588064d82e5690e1994724", null ],
    [ "FLEXIO_UART_GetDefaultConfig", "group__flexio__uart.html#gab705f673f07da7b1b385be7edfd2385e", null ],
    [ "FLEXIO_UART_GetStatusFlags", "group__flexio__uart.html#gaefea3702f5a53a82246aaba191605716", null ],
    [ "FLEXIO_UART_ClearStatusFlags", "group__flexio__uart.html#ga22b879f2d9e08524999a98c7a743c3b0", null ],
    [ "FLEXIO_UART_EnableInterrupts", "group__flexio__uart.html#ga7f48f0a283e6458db7ff72c433e8a325", null ],
    [ "FLEXIO_UART_DisableInterrupts", "group__flexio__uart.html#gaf4e18aa7772ae63d2eef7030cfa67175", null ],
    [ "FLEXIO_UART_GetTxDataRegisterAddress", "group__flexio__uart.html#ga06b2b751b69190006ddbd4783abd5135", null ],
    [ "FLEXIO_UART_GetRxDataRegisterAddress", "group__flexio__uart.html#gaa2b438423a09911bdc122109f2d11ced", null ],
    [ "FLEXIO_UART_EnableTxDMA", "group__flexio__uart.html#gaa23cdab45bc3ed89eb9ac2998f7f339b", null ],
    [ "FLEXIO_UART_EnableRxDMA", "group__flexio__uart.html#ga0d005d0a785cdc80bdc77a0232533706", null ],
    [ "FLEXIO_UART_Enable", "group__flexio__uart.html#ga23a9bf703cf1c371fd362759ad1c66f9", null ],
    [ "FLEXIO_UART_WriteByte", "group__flexio__uart.html#gaca10cded3e2fe4fb41af031874385775", null ],
    [ "FLEXIO_UART_ReadByte", "group__flexio__uart.html#ga19a7990926d6e3ed6e5799ce9c060c7d", null ],
    [ "FLEXIO_UART_WriteBlocking", "group__flexio__uart.html#ga45a07e6f7d7b74ee3d5e461ce54cde32", null ],
    [ "FLEXIO_UART_ReadBlocking", "group__flexio__uart.html#gae918075d3d85386b3a7a81fa0b77415a", null ],
    [ "FLEXIO_UART_CreateHandle", "group__flexio__uart.html#ga9c7e3c481fb36c12ca8d5332c7f23199", null ],
    [ "FLEXIO_UART_StartRingBuffer", "group__flexio__uart.html#gaee5f8e2328c678188e75626426f889ee", null ],
    [ "FLEXIO_UART_StopRingBuffer", "group__flexio__uart.html#ga964906c8c9943735c24b5997486f690c", null ],
    [ "FLEXIO_UART_SendNonBlocking", "group__flexio__uart.html#gada6d833070a2e6cf1f98dccce4a48c2b", null ],
    [ "FLEXIO_UART_AbortSend", "group__flexio__uart.html#ga22bba3bfc02e844d4ff7a5084ef3dec4", null ],
    [ "FLEXIO_UART_GetSendRemainingBytes", "group__flexio__uart.html#ga5b96f687429fd6ff54519fa5d265e5bf", null ],
    [ "FLEXIO_UART_ReceiveNonBlocking", "group__flexio__uart.html#ga330841daac434e03c19c8805dab02df4", null ],
    [ "FLEXIO_UART_AbortReceive", "group__flexio__uart.html#ga17d0158fb414ba171e62b95df6364a9a", null ],
    [ "FLEXIO_UART_GetReceiveRemainingBytes", "group__flexio__uart.html#gad9e4071450547d08c76fb359006e64bd", null ],
    [ "FLEXIO_UART_HandleIRQ", "group__flexio__uart.html#gaac0af0ab12d5063646016d33e791704c", null ],
    [ "FLEXIO_UART_CreateHandleDMA", "group__flexio__uart.html#gab4bfbfb107ebedf7225fbd5123d0f090", null ],
    [ "FLEXIO_UART_SendDMA", "group__flexio__uart.html#ga7e0075c574d34c0e1a19acd5a149b0f7", null ],
    [ "FLEXIO_UART_ReceiveDMA", "group__flexio__uart.html#ga0c01ef46f18063812c961835ca0b4456", null ],
    [ "FLEXIO_UART_AbortSendDMA", "group__flexio__uart.html#ga9a83c0c53bfad40de0957d5125f140d7", null ],
    [ "FLEXIO_UART_AbortReceiveDMA", "group__flexio__uart.html#ga7c816717961c60dcdc8cb1a3b5a09c5e", null ],
    [ "FLEXIO_UART_GetSendRemainingBytesDMA", "group__flexio__uart.html#gac576839a96564e961b2e60273d2d6d97", null ],
    [ "FLEXIO_UART_GetReceiveRemainingBytesDMA", "group__flexio__uart.html#gab851410c9f2df85b57180ffa1bc4aceb", null ],
    [ "FLEXIO_UART_CreateHandleEDMA", "group__flexio__uart.html#ga53139db1d1b65b3a6f885f7f77c9a841", null ],
    [ "FLEXIO_UART_SendEDMA", "group__flexio__uart.html#ga1ad96642297cde26467b9457c6e42351", null ],
    [ "FLEXIO_UART_ReceiveEDMA", "group__flexio__uart.html#gac4ba5e720a7ffad241bfa06cdc2f440c", null ],
    [ "FLEXIO_UART_AbortSendEDMA", "group__flexio__uart.html#gaa16fa6aaed8d9ad1234b474e094ccf6c", null ],
    [ "FLEXIO_UART_AbortReceiveEDMA", "group__flexio__uart.html#ga1b59781274b9b96a41a6a2c2b65de741", null ],
    [ "FLEXIO_UART_GetSendRemainingBytesEDMA", "group__flexio__uart.html#gaf9c21baf04a029edf06526226abc4ad5", null ],
    [ "FLEXIO_UART_GetReceiveRemainingBytesEDMA", "group__flexio__uart.html#ga54e11f23d0d1e8b7a719c8a2c6c4e850", null ]
];