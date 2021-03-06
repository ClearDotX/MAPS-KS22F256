var group__uart =
[
    [ "uart_config_t", "group__uart.html#structuart__config__t", [
      [ "baudRate_Bps", "group__uart.html#acc123668836f3432c54ee6a2f6f318f1", null ],
      [ "parityMode", "group__uart.html#a1a3abcf4f0f5d0a3893df14f2991aa39", null ],
      [ "enableTx", "group__uart.html#a91d575902455699a312c64b564133a8d", null ],
      [ "enableRx", "group__uart.html#aa045acd340fc86d70a497f49e0a95bdd", null ]
    ] ],
    [ "uart_transfer_t", "group__uart.html#structuart__transfer__t", [
      [ "data", "group__uart.html#af91f05b0f951836e3002f60f0575f369", null ],
      [ "dataSize", "group__uart.html#a21820f8294de0d75d863b72512a7c12f", null ]
    ] ],
    [ "uart_handle_t", "group__uart.html#struct__uart__handle", [
      [ "txData", "group__uart.html#a046372eac11f58a16f519c0ee264ea9e", null ],
      [ "txDataSize", "group__uart.html#a1e67a92b463ad328646834ea0de4d70d", null ],
      [ "rxData", "group__uart.html#a5c87354702502998d3ccca9d9bfd3069", null ],
      [ "rxDataSize", "group__uart.html#a46bd84d885004806f89744ad79edd81e", null ],
      [ "rxRingBuffer", "group__uart.html#afeba8b549facc329937046e734681963", null ],
      [ "rxRingBufferSize", "group__uart.html#a8d6afed4b64233c959a180b66686c14c", null ],
      [ "rxRingBufferHead", "group__uart.html#a85d061606b31401fb4a9d0e22e77056c", null ],
      [ "rxRingBufferTail", "group__uart.html#a9f4482059188bea2603b1c07cf9cc084", null ],
      [ "callback", "group__uart.html#a2ef9706e8203ba72fa1e04c48581fb0f", null ],
      [ "userData", "group__uart.html#a69ce1fdb7a2f60c0ecc94c4d1b2ed6ff", null ],
      [ "txState", "group__uart.html#a9fa8f119b6e0e3a7805fb25edafb1498", null ],
      [ "rxState", "group__uart.html#a5aa95f3d2cb81a8bb01a11ab2cf66234", null ]
    ] ],
    [ "uart_dma_handle_t", "group__uart.html#struct__uart__dma__handle", [
      [ "base", "group__uart.html#ac5bbb81df9c93c195d060b5d7d418c60", null ],
      [ "callback", "group__uart.html#ab692a5606001d4672b51b319443bc90c", null ],
      [ "userData", "group__uart.html#ae170e623aab9c539b8793b2ffaa3b1c7", null ],
      [ "txDmaHandle", "group__uart.html#af5626cb851f69637fb71eb28da9dae98", null ],
      [ "rxDmaHandle", "group__uart.html#a4bd3fe36e857c34fce6556ad7e031c1c", null ],
      [ "txState", "group__uart.html#a739848198af89237f9506611fb787c78", null ],
      [ "rxState", "group__uart.html#a1545b5d899ea275d3043c975983215f7", null ]
    ] ],
    [ "uart_edma_handle_t", "group__uart.html#struct__uart__edma__handle", [
      [ "callback", "group__uart.html#a254f41d5cf70fff13fd2b1bcd902c2b6", null ],
      [ "userData", "group__uart.html#a234d609cec5123bf352a4fa3f806375b", null ],
      [ "txEdmaHandle", "group__uart.html#ad339b242805d2cb1d2aff0b12315598d", null ],
      [ "rxEdmaHandle", "group__uart.html#a74ce23f0dfe586490d83b754a6b3b558", null ],
      [ "txState", "group__uart.html#a0e8aef6bbb90253640829077217256d4", null ],
      [ "rxState", "group__uart.html#ac9828f820ff1898b81662915af78784b", null ]
    ] ],
    [ "FSL_LPUART_FREERTOS_DRIVER_VERSION", "group__uart.html#gae78d379ef6bde2ad08e1fe64f1f09bf5", null ],
    [ "FSL_UART_DRIVER_VERSION", "group__uart.html#ga89f9649bfe350eb7b1e53146f35d3de5", null ],
    [ "FSL_UART_FREERTOS_DRIVER_VERSION", "group__uart.html#ga0023d77491809dff862358266574febb", null ],
    [ "FSL_UART_UCOSII_DRIVER_VERSION", "group__uart.html#gabda18f1086ae2a4f43682151560a74f6", null ],
    [ "FSL_UART_UCOSIII_DRIVER_VERSION", "group__uart.html#gad8b7f772534a48dcf27793c36834a933", null ],
    [ "uart_transfer_callback_t", "group__uart.html#gaf164fa4e12ff8a3e3f3997512001e007", null ],
    [ "uart_dma_transfer_callback_t", "group__uart.html#gad4fe061c9b2450d48570109b2e2cd654", null ],
    [ "uart_edma_transfer_callback_t", "group__uart.html#ga1fbe483dd5c3f3d55eb6d15b1b9582da", null ],
    [ "_uart_status", "group__uart.html#gacef40dc8e8ac174bfe40ebcbc980f84b", [
      [ "kStatus_UART_TxBusy", "group__uart.html#ggacef40dc8e8ac174bfe40ebcbc980f84ba19ddee4fe5963467600028b78fb468e7", null ],
      [ "kStatus_UART_RxBusy", "group__uart.html#ggacef40dc8e8ac174bfe40ebcbc980f84ba68cf23f981c4b85f82291163fbb2a5e6", null ],
      [ "kStatus_UART_TxIdle", "group__uart.html#ggacef40dc8e8ac174bfe40ebcbc980f84baf36c829b1b889517bf775c0d7ce29fa6", null ],
      [ "kStatus_UART_RxIdle", "group__uart.html#ggacef40dc8e8ac174bfe40ebcbc980f84bae0877b14627ed2aa8ddb2bf5b033f407", null ],
      [ "kStatus_UART_TxWatermarkTooLarge", "group__uart.html#ggacef40dc8e8ac174bfe40ebcbc980f84ba765913f003819ed87b861d187bf79ab8", null ],
      [ "kStatus_UART_RxWatermarkTooLarge", "group__uart.html#ggacef40dc8e8ac174bfe40ebcbc980f84ba720cefde05389830f35f6e6df6f6ac67", null ],
      [ "kStatus_UART_FlagCannotClearManually", "group__uart.html#ggacef40dc8e8ac174bfe40ebcbc980f84ba5ee8e23fc9ab20cf366942751b895f54", null ],
      [ "kStatus_UART_Error", "group__uart.html#ggacef40dc8e8ac174bfe40ebcbc980f84ba950972a9e2ebbaa13c92e8f5a51f785d", null ],
      [ "kStatus_UART_RxRingBufferOverrun", "group__uart.html#ggacef40dc8e8ac174bfe40ebcbc980f84ba2e5b644b1e10c426af818bc78657e992", null ],
      [ "kStatus_UART_RxHardwareOverrun", "group__uart.html#ggacef40dc8e8ac174bfe40ebcbc980f84bab6b1fd6c2deec25992449f5b12303a26", null ]
    ] ],
    [ "uart_parity_mode_t", "group__uart.html#ga436e5a7bdb5f24decb5bfb0c87a83ff4", [
      [ "kUART_ParityDisabled", "group__uart.html#gga436e5a7bdb5f24decb5bfb0c87a83ff4a97e2feae6671a6de1126ebc2edf4606b", null ],
      [ "kUART_ParityEven", "group__uart.html#gga436e5a7bdb5f24decb5bfb0c87a83ff4a9e68fe3aba46e045bee5433ed098bff0", null ],
      [ "kUART_ParityOdd", "group__uart.html#gga436e5a7bdb5f24decb5bfb0c87a83ff4ae48c7fabc9babf1be626ebeb2627a54c", null ]
    ] ],
    [ "uart_stop_bit_count_t", "group__uart.html#gaca4f14d23735c6afefb76cbee18e1db6", [
      [ "kUART_OneStopBit", "group__uart.html#ggaca4f14d23735c6afefb76cbee18e1db6a5a8829a108d8a46abb097a36cde051a7", null ],
      [ "kUART_TwoStopBit", "group__uart.html#ggaca4f14d23735c6afefb76cbee18e1db6a9704b3ae3ee851acf324eb357f75ab56", null ]
    ] ],
    [ "_uart_interrupt_enable", "group__uart.html#ga700f3cd8e3800273a1591307cab6311c", [
      [ "kUART_RxActiveEdgeInterruptEnable", "group__uart.html#gga700f3cd8e3800273a1591307cab6311ca71755319c216d0bdf79729c18c24cf64", null ],
      [ "kUART_TxDataRegEmptyInterruptEnable", "group__uart.html#gga700f3cd8e3800273a1591307cab6311ca5a0870233f2ffa8f5ca60d80f1e3ed14", null ],
      [ "kUART_TransmissionCompleteInterruptEnable", "group__uart.html#gga700f3cd8e3800273a1591307cab6311cafd337213dae85b532dfaf9a90e2e5d7b", null ],
      [ "kUART_RxDataRegFullInterruptEnable", "group__uart.html#gga700f3cd8e3800273a1591307cab6311cac8a751a334e172413ab833cb0ccb01c3", null ],
      [ "kUART_IdleLineInterruptEnable", "group__uart.html#gga700f3cd8e3800273a1591307cab6311ca078ea32d8587f92337a3dee495a1ceed", null ],
      [ "kUART_RxOverrunInterruptEnable", "group__uart.html#gga700f3cd8e3800273a1591307cab6311ca3e85b14c1b64dbfef1d0b4b7bc39c19d", null ],
      [ "kUART_NoiseErrorInterruptEnable", "group__uart.html#gga700f3cd8e3800273a1591307cab6311ca53d0fe2bb0cd90198c632f08488af4b3", null ],
      [ "kUART_FramingErrorInterruptEnable", "group__uart.html#gga700f3cd8e3800273a1591307cab6311ca00a387446d0ca7fd843968eca8af22a5", null ],
      [ "kUART_ParityErrorInterruptEnable", "group__uart.html#gga700f3cd8e3800273a1591307cab6311cadc2475fa65313c6f1d13b36f0443b20a", null ]
    ] ],
    [ "_uart_flags", "group__uart.html#ga259a53f363288115306a45d08fc66eb8", [
      [ "kUART_TxDataRegEmptyFlag", "group__uart.html#gga259a53f363288115306a45d08fc66eb8a5209c671b2f2f064dc512655387e9e1a", null ],
      [ "kUART_TransmissionCompleteFlag", "group__uart.html#gga259a53f363288115306a45d08fc66eb8a4468b60b2d78c34b93c9199df9e95d3a", null ],
      [ "kUART_RxDataRegFullFlag", "group__uart.html#gga259a53f363288115306a45d08fc66eb8aa86a34b2ea609eeeb63f98fea498e540", null ],
      [ "kUART_IdleLineFlag", "group__uart.html#gga259a53f363288115306a45d08fc66eb8ab2fc4e1b1164ed3e3fe7e19a39029b2c", null ],
      [ "kUART_RxOverrunFlag", "group__uart.html#gga259a53f363288115306a45d08fc66eb8a93d3c56466b4656dcce08d9323d7afd8", null ],
      [ "kUART_NoiseErrorFlag", "group__uart.html#gga259a53f363288115306a45d08fc66eb8abd56419f102fb01c7bbc2c38f81ab360", null ],
      [ "kUART_FramingErrorFlag", "group__uart.html#gga259a53f363288115306a45d08fc66eb8aade5071fa56646adcda3562051443868", null ],
      [ "kUART_ParityErrorFlag", "group__uart.html#gga259a53f363288115306a45d08fc66eb8a0c92973ab1d527115fa01934c62a053d", null ],
      [ "kUART_RxActiveEdgeFlag", "group__uart.html#gga259a53f363288115306a45d08fc66eb8adcfe89feb164ff5b1720312922006a7a", null ],
      [ "kUART_RxActiveFlag", "group__uart.html#gga259a53f363288115306a45d08fc66eb8a7a4521cd44a3272a94784670d1bf5a0c", null ]
    ] ],
    [ "LPUART_RTOS_Init", "group__uart.html#ga045c5024f89b436fdf55c47340359f86", null ],
    [ "LPUART_RTOS_DeInit", "group__uart.html#gaa3ae28bddee769a39b8939173ac2fff7", null ],
    [ "LPUART_RTOS_Send", "group__uart.html#ga8fa93379ae9896d5f93de86cfeff79d7", null ],
    [ "LPUART_RTOS_Receive", "group__uart.html#ga0561f87122863c6e9b20f8991a73a0fb", null ],
    [ "UART_Init", "group__uart.html#gaa6162686a1aced16ff39c7d54d81fe5b", null ],
    [ "UART_Deinit", "group__uart.html#ga3ff48bc211831be33e6fe5c50eb671a0", null ],
    [ "UART_GetDefaultConfig", "group__uart.html#ga47c7c09e04a0497f4530d553e27d96c5", null ],
    [ "UART_SetBaudRate", "group__uart.html#gae0482730b0d5f6b7f8d12aba1e4044e5", null ],
    [ "UART_GetStatusFlags", "group__uart.html#ga28bf715678c0d3c8c3902e79455ee923", null ],
    [ "UART_ClearStatusFlags", "group__uart.html#gacc4cb85534361f836edca9454a6426c6", null ],
    [ "UART_EnableInterrupts", "group__uart.html#ga49ecd761481a22956e3f46285038719c", null ],
    [ "UART_DisableInterrupts", "group__uart.html#ga3f24b6b9b730046a9a81918f9abe7d99", null ],
    [ "UART_GetEnabledInterrupts", "group__uart.html#ga79bc0b880286ec0dc0543606fc772912", null ],
    [ "UART_EnableTx", "group__uart.html#gaf2ed68f3dd6ee8a272e9afd8ebd84f93", null ],
    [ "UART_EnableRx", "group__uart.html#gaa503e36c8ce82f1042ce186b9863871f", null ],
    [ "UART_WriteBlocking", "group__uart.html#gad67794d80b7ee2d3292b41af6ff1e100", null ],
    [ "UART_WriteByte", "group__uart.html#ga371d2ceda535de3a74beba5a3465fab0", null ],
    [ "UART_ReadBlocking", "group__uart.html#ga23c2382838fe05bd24fc0744b2322153", null ],
    [ "UART_ReadByte", "group__uart.html#ga3728011ac7906f3e03bb677b0f9cf7b4", null ],
    [ "UART_CreateHandle", "group__uart.html#gaffdc577a350c4cba18f20200aaac30cb", null ],
    [ "UART_StartRingBuffer", "group__uart.html#ga2263797ea1e1b0ef07898d6e6fc16049", null ],
    [ "UART_StopRingBuffer", "group__uart.html#gadd168b3e4043dfa92feb242fcfce0a35", null ],
    [ "UART_SendNonBlocking", "group__uart.html#ga63a181530dc62200a8eed10ec5cf5167", null ],
    [ "UART_AbortSend", "group__uart.html#ga3e1869195f3e7bd6c38d7328aa3df131", null ],
    [ "UART_GetSendRemainingBytes", "group__uart.html#gacf4217b91684e5d24ffe384072566f96", null ],
    [ "UART_ReceiveNonBlocking", "group__uart.html#ga5dfacd59d3fb1a18c06c6bf1519cbc9c", null ],
    [ "UART_AbortReceive", "group__uart.html#ga0ec8215440a7d2f80a421d557b636035", null ],
    [ "UART_GetReceiveRemainingBytes", "group__uart.html#gac254a9d2cd142b2f75bb0b8293bc0df8", null ],
    [ "UART_HandleIRQ", "group__uart.html#gaeb53a26a18473713bf2a8236af933e06", null ],
    [ "UART_HandleErrorIRQ", "group__uart.html#ga309aa9807039e6cf5c2f05be0b0e745f", null ],
    [ "UART_CreateHandleDMA", "group__uart.html#ga747142c31ad4d8cb5530b8d421f3d6eb", null ],
    [ "UART_SendDMA", "group__uart.html#gaf337666a7c99bb9466b01091270ee052", null ],
    [ "UART_ReceiveDMA", "group__uart.html#ga2f61b52a55d53c61c70a4cee4ca058b9", null ],
    [ "UART_AbortSendDMA", "group__uart.html#ga87eb37f48c245202c8454075726c81c8", null ],
    [ "UART_AbortReceiveDMA", "group__uart.html#ga5648311ab64cde4224a6c02b419ddcb0", null ],
    [ "UART_GetSendRemainingBytesDMA", "group__uart.html#ga35dd6debdc778445d185b26642935f2c", null ],
    [ "UART_GetReceiveRemainingBytesDMA", "group__uart.html#ga1d5f8c2424553f80ca6da16f13237c51", null ],
    [ "UART_CreateHandleEDMA", "group__uart.html#gab17730e078a242f20df651d26ae1e221", null ],
    [ "UART_SendEDMA", "group__uart.html#ga93b7de7f76c97da666960d1edf426f8e", null ],
    [ "UART_ReceiveEDMA", "group__uart.html#ga45666dbc1751a6a5a764081c64adedcf", null ],
    [ "UART_AbortSendEDMA", "group__uart.html#ga6e9c1919c863eff2a93d60b48e583113", null ],
    [ "UART_AbortReceiveEDMA", "group__uart.html#ga1e019c6821a5fe59fb3d34728cc50c7d", null ],
    [ "UART_GetSendRemainingBytesEDMA", "group__uart.html#gad7fd548057e67e4f1a20b4fdfde1f8f0", null ],
    [ "UART_GetReceiveRemainingBytesEDMA", "group__uart.html#gadda6f998c744c52bcc2cc36270da6469", null ],
    [ "UART_RTOS_Init", "group__uart.html#ga151bb7582b8aaec316e335446e5bec0b", null ],
    [ "UART_RTOS_DeInit", "group__uart.html#ga0b9ef94904993414beb7cc8abe56b5be", null ],
    [ "UART_RTOS_Send", "group__uart.html#gada4115d237b9517f2587a054dbdc442a", null ],
    [ "UART_RTOS_Receive", "group__uart.html#gaa8adcb11232b565985f3f5961815a00c", null ]
];