var group__lpuart =
[
    [ "lpuart_config_t", "group__lpuart.html#structlpuart__config__t", [
      [ "baudRate_Bps", "group__lpuart.html#a09d359de28dc114424b2d702df014d1c", null ],
      [ "parityMode", "group__lpuart.html#ae7ce86796f025a059c973c540d5a94ad", null ],
      [ "enableTx", "group__lpuart.html#a0d677467cd14ee90f544d688f3dc9b9a", null ],
      [ "enableRx", "group__lpuart.html#a48d3f37a9364c0093083ef843d5d062e", null ]
    ] ],
    [ "lpuart_transfer_t", "group__lpuart.html#structlpuart__transfer__t", [
      [ "data", "group__lpuart.html#a7c49cf389dea8ad6f674bff6cedd8e37", null ],
      [ "dataSize", "group__lpuart.html#ad269a833a8e8e4cdbc0d485d59e256d8", null ]
    ] ],
    [ "lpuart_handle_t", "group__lpuart.html#struct__lpuart__handle", [
      [ "txData", "group__lpuart.html#a85dfd2cc5aa30f259ce7338a48c832a8", null ],
      [ "txDataSize", "group__lpuart.html#a2114edec74578fe798a62cf852ab0194", null ],
      [ "rxData", "group__lpuart.html#a6d3fceca5b3bee7f183116ee7b3a3b93", null ],
      [ "rxDataSize", "group__lpuart.html#a6b9d688ed6ecbb3b71d6b266bec66edb", null ],
      [ "rxRingBuffer", "group__lpuart.html#a3eca0396fcb49d3652a40c7cf49024c8", null ],
      [ "rxRingBufferSize", "group__lpuart.html#a12cff540159d2fb592e4856957d820cd", null ],
      [ "rxRingBufferHead", "group__lpuart.html#aaffed28a2a686bb90e3238a3d8e597b3", null ],
      [ "rxRingBufferTail", "group__lpuart.html#acecaaa5df8327c5b33815cb7483abd75", null ],
      [ "callback", "group__lpuart.html#a881c1d7231d1ee0bcb3eb5f1447f75a4", null ],
      [ "userData", "group__lpuart.html#ad9f3989cffe26d3ca63d381da36024be", null ],
      [ "txState", "group__lpuart.html#a86fe3c8f354afbe67d28a71068206b26", null ],
      [ "rxState", "group__lpuart.html#a088a1e9a009d852b90fb8390fa7cbbcd", null ]
    ] ],
    [ "lpuart_dma_handle_t", "group__lpuart.html#struct__lpuart__dma__handle", [
      [ "callback", "group__lpuart.html#a09232c7434d5e4ac726bc2506806069c", null ],
      [ "userData", "group__lpuart.html#a78faeb01b6deb36c4bbbaf3a03a99af7", null ],
      [ "txDmaHandle", "group__lpuart.html#aa6b9d2686f4812180ce77dfe6c2a6117", null ],
      [ "rxDmaHandle", "group__lpuart.html#aee2f2563ecb247c5757291d9f8979475", null ],
      [ "txState", "group__lpuart.html#afa8891571f2db1e20c1d594f6cfd3972", null ],
      [ "rxState", "group__lpuart.html#a73c466f909c820e0da0cd106ed64742e", null ]
    ] ],
    [ "lpuart_edma_handle_t", "group__lpuart.html#struct__lpuart__edma__handle", [
      [ "callback", "group__lpuart.html#a90af06bca6fa1c835176bb2f7baae37c", null ],
      [ "userData", "group__lpuart.html#ac699f5a5123647eefb2aebb1b1a35c4a", null ],
      [ "txEdmaHandle", "group__lpuart.html#a1748fd406d158b129b40430218be3372", null ],
      [ "rxEdmaHandle", "group__lpuart.html#aa365980412bd8d507718e3bdb6991983", null ],
      [ "txState", "group__lpuart.html#a17fd779099d5f3707693ac7c1212dc8a", null ],
      [ "rxState", "group__lpuart.html#a2a62eb17faa4b16b47cb7ca6992c9980", null ]
    ] ],
    [ "FSL_LPUART_DRIVER_VERSION", "group__lpuart.html#ga0870fb824ece32739bd35f819f8c408e", null ],
    [ "FSL_LPUART_UCOSII_DRIVER_VERSION", "group__lpuart.html#ga6b3475637da6b7f0bfc59677a336bf54", null ],
    [ "FSL_LPUART_UCOSIII_DRIVER_VERSION", "group__lpuart.html#gab60da9b84b2db90e5cf03927660669e1", null ],
    [ "lpuart_transfer_callback_t", "group__lpuart.html#ga7ab1637091d166aa8b69517350fb05c8", null ],
    [ "lpuart_dma_transfer_callback_t", "group__lpuart.html#ga01b0dcde73780ef8241f9d6d8fbfd32a", null ],
    [ "lpuart_edma_transfer_callback_t", "group__lpuart.html#gaed2bf1ac041ea4526ccf5ab0eba4da73", null ],
    [ "_lpuart_status", "group__lpuart.html#ga91d929761e975dda91c0cc811d253ce5", [
      [ "kStatus_LPUART_TxBusy", "group__lpuart.html#gga91d929761e975dda91c0cc811d253ce5abddb317db7dfffa95856604bfac2a17f", null ],
      [ "kStatus_LPUART_RxBusy", "group__lpuart.html#gga91d929761e975dda91c0cc811d253ce5a98dfeab0a23c81f6b7f2c18acff8cc30", null ],
      [ "kStatus_LPUART_TxIdle", "group__lpuart.html#gga91d929761e975dda91c0cc811d253ce5a079c6d0957d121ce7cd95cf97487dfbb", null ],
      [ "kStatus_LPUART_RxIdle", "group__lpuart.html#gga91d929761e975dda91c0cc811d253ce5a02b8fe84bbfd8c52856b5d48865f7088", null ],
      [ "kStatus_LPUART_TxWatermarkTooLarge", "group__lpuart.html#gga91d929761e975dda91c0cc811d253ce5abbee969cb0f7b4c49b44b10c4eb583cc", null ],
      [ "kStatus_LPUART_RxWatermarkTooLarge", "group__lpuart.html#gga91d929761e975dda91c0cc811d253ce5aca81b4d6e2e792c6d841b95ec25daad9", null ],
      [ "kStatus_LPUART_FlagCannotClearManually", "group__lpuart.html#gga91d929761e975dda91c0cc811d253ce5a07d5a5092ded158fe53ea4df940f8bdb", null ],
      [ "kStatus_LPUART_Error", "group__lpuart.html#gga91d929761e975dda91c0cc811d253ce5af069a1997c3d25588d834ccb33dd35f4", null ],
      [ "kStatus_LPUART_RxRingBufferOverrun", "group__lpuart.html#gga91d929761e975dda91c0cc811d253ce5a45a7ed684f4c31aec8999b3da68d3b5e", null ],
      [ "kStatus_LPUART_RxHardwareOverrun", "group__lpuart.html#gga91d929761e975dda91c0cc811d253ce5af8ada3eacff87751ec865b4fc584bac1", null ]
    ] ],
    [ "lpuart_parity_mode_t", "group__lpuart.html#ga6fcd73abf900b8a17dab1f2b1f3f53c7", [
      [ "kLPUART_ParityDisabled", "group__lpuart.html#gga6fcd73abf900b8a17dab1f2b1f3f53c7a0e5bdb8fc4e1010930d05e1490a75b1a", null ],
      [ "kLPUART_ParityEven", "group__lpuart.html#gga6fcd73abf900b8a17dab1f2b1f3f53c7ac92adb3fdf11240ca5dee6c6543b6ff7", null ],
      [ "kLPUART_ParityOdd", "group__lpuart.html#gga6fcd73abf900b8a17dab1f2b1f3f53c7adab63856e98e0715f4ab289ac0da3575", null ]
    ] ],
    [ "lpuart_stop_bit_count_t", "group__lpuart.html#ga7540d87bc3fa8a605d8da53fe08219f0", [
      [ "kLPUART_OneStopBit", "group__lpuart.html#gga7540d87bc3fa8a605d8da53fe08219f0a4304f0ee73a5fabfadfc05ca1fbd3901", null ],
      [ "kLPUART_TwoStopBit", "group__lpuart.html#gga7540d87bc3fa8a605d8da53fe08219f0a6e049e0faaf89ed2dfe493ad8f6c93d4", null ]
    ] ],
    [ "_lpuart_interrupt_enable", "group__lpuart.html#ga199a157d391291a9d003bf23954f9603", [
      [ "kLPUART_RxActiveEdgeInterruptEnable", "group__lpuart.html#gga199a157d391291a9d003bf23954f9603a43750aac20f72535350c1ab4a1862a3a", null ],
      [ "kLPUART_TxDataRegEmptyInterruptEnable", "group__lpuart.html#gga199a157d391291a9d003bf23954f9603a0e8bf389f65e5f86a3063dc55b8aae1c", null ],
      [ "kLPUART_TransmissionCompleteInterruptEnable", "group__lpuart.html#gga199a157d391291a9d003bf23954f9603a0b7f783393a61cce88eab28307e9fe14", null ],
      [ "kLPUART_RxDataRegFullInterruptEnable", "group__lpuart.html#gga199a157d391291a9d003bf23954f9603a5b4a0893c98bf79d0dad88aeb7714d22", null ],
      [ "kLPUART_IdleLineInterruptEnable", "group__lpuart.html#gga199a157d391291a9d003bf23954f9603af3f83c56032b14e50a682857dd7c152d", null ],
      [ "kLPUART_RxOverrunInterruptEnable", "group__lpuart.html#gga199a157d391291a9d003bf23954f9603a8a29c714eaa6acab06a87314f4f29636", null ],
      [ "kLPUART_NoiseErrorInterruptEnable", "group__lpuart.html#gga199a157d391291a9d003bf23954f9603ad1a35bc7e89f170a5b82cb4801b73b5e", null ],
      [ "kLPUART_FramingErrorInterruptEnable", "group__lpuart.html#gga199a157d391291a9d003bf23954f9603a1b43ca06e0af6b0fcf4b55612363a64d", null ],
      [ "kLPUART_ParityErrorInterruptEnable", "group__lpuart.html#gga199a157d391291a9d003bf23954f9603a13c12b816605deaabcbc3e6a6db82466", null ]
    ] ],
    [ "_lpuart_flags", "group__lpuart.html#ga24bb7ca3f894fe5ff55b9f38bec16c89", [
      [ "kLPUART_TxDataRegEmptyFlag", "group__lpuart.html#gga24bb7ca3f894fe5ff55b9f38bec16c89aa765f8ddac00348328f64ad4222d057a", null ],
      [ "kLPUART_TransmissionCompleteFlag", "group__lpuart.html#gga24bb7ca3f894fe5ff55b9f38bec16c89ae0539c4cf39d4d1e7839a4896fad2a85", null ],
      [ "kLPUART_RxDataRegFullFlag", "group__lpuart.html#gga24bb7ca3f894fe5ff55b9f38bec16c89a9bc679bcc20b81768f84b04e499c5a2f", null ],
      [ "kLPUART_IdleLineFlag", "group__lpuart.html#gga24bb7ca3f894fe5ff55b9f38bec16c89acf1a40aca38878bc395f9f0b10d8225e", null ],
      [ "kLPUART_RxOverrunFlag", "group__lpuart.html#gga24bb7ca3f894fe5ff55b9f38bec16c89a994f5f484509133c632215a8b701f8d1", null ],
      [ "kLPUART_NoiseErrorFlag", "group__lpuart.html#gga24bb7ca3f894fe5ff55b9f38bec16c89a2198232cb9d239f937ab221bbf03259f", null ],
      [ "kLPUART_FramingErrorFlag", "group__lpuart.html#gga24bb7ca3f894fe5ff55b9f38bec16c89ad1d40f89cb6aab9c12113586fda7f510", null ],
      [ "kLPUART_ParityErrorFlag", "group__lpuart.html#gga24bb7ca3f894fe5ff55b9f38bec16c89a2f8d4c03df61bc4678b2ccf1c69eedeb", null ],
      [ "kLPUART_RxActiveEdgeFlag", "group__lpuart.html#gga24bb7ca3f894fe5ff55b9f38bec16c89ae89f181d8dbbf5254d6fe843e4cac6d6", null ],
      [ "kLPUART_RxActiveFlag", "group__lpuart.html#gga24bb7ca3f894fe5ff55b9f38bec16c89a1f14711a74db1cc141b5c2627ee4c799", null ]
    ] ],
    [ "LPUART_Init", "group__lpuart.html#gae0bf80e73707feee830060ff4fbfe292", null ],
    [ "LPUART_Deinit", "group__lpuart.html#ga303ff3b77767bafa449d96f1e8c921e0", null ],
    [ "LPUART_GetDefaultConfig", "group__lpuart.html#ga95dfe3c3886692f1fe32023d59708440", null ],
    [ "LPUART_SetBaudRate", "group__lpuart.html#ga4e374f897de11217ce02eeb138c03d12", null ],
    [ "LPUART_GetStatusFlags", "group__lpuart.html#gab83db963591b116f90daf3e24118cb09", null ],
    [ "LPUART_ClearStatusFlags", "group__lpuart.html#gac6bcdb29d31a2addb8d80d98855ba147", null ],
    [ "LPUART_EnableInterrupts", "group__lpuart.html#gab5998ebb0dc457f0864c84c16f83d745", null ],
    [ "LPUART_DisableInterrupts", "group__lpuart.html#ga2540406e4a338199acddaa828829cad8", null ],
    [ "LPUART_GetEnabledInterrupts", "group__lpuart.html#ga172b524bae8bacc6dc95d9daf82c97b5", null ],
    [ "LPUART_EnableTx", "group__lpuart.html#ga49961d4e9043ff9bd10e477add9258d7", null ],
    [ "LPUART_EnableRx", "group__lpuart.html#gaadb8a7199526555e50e65e017da4eae2", null ],
    [ "LPUART_WriteBlocking", "group__lpuart.html#gac5377aeebf4327f4ef9de295d8695cd9", null ],
    [ "LPUART_WriteByte", "group__lpuart.html#ga9a9389e1c7bcce6c23e00eb77006a717", null ],
    [ "LPUART_ReadBlocking", "group__lpuart.html#gaf213abd4457c20519b54ceedb5a0e09c", null ],
    [ "LPUART_ReadByte", "group__lpuart.html#ga02013105f0aacc7062eaae59d7401d29", null ],
    [ "LPUART_CreateHandle", "group__lpuart.html#ga76eacacec4ce5d1d2e2f56d982b3c69b", null ],
    [ "LPUART_SendNonBlocking", "group__lpuart.html#ga165d18fc484fa65138ebeaac8c9e7345", null ],
    [ "LPUART_StartRingBuffer", "group__lpuart.html#ga7731764ce8fe62b3079ee5f014443cb4", null ],
    [ "LPUART_StopRingBuffer", "group__lpuart.html#ga17a5373e645e81248a8c62044483cb9c", null ],
    [ "LPUART_AbortSend", "group__lpuart.html#ga2b132f6d96e7012255134969c03415ba", null ],
    [ "LPUART_GetSendRemainingBytes", "group__lpuart.html#ga7b46a5d295a05d3190418fda0bc35465", null ],
    [ "LPUART_ReceiveNonBlocking", "group__lpuart.html#ga6bd6a187e31e9179bf9151eb83b96269", null ],
    [ "LPUART_AbortReceive", "group__lpuart.html#ga557353168b8cbdcf9f8556a04ff3d75c", null ],
    [ "LPUART_GetReceiveRemainingBytes", "group__lpuart.html#ga79e2aa87521eee2cf688c7e244b1cd34", null ],
    [ "LPUART_HandleIRQ", "group__lpuart.html#gaf6aeac12759bf75c7d6b7fe7492e4b7e", null ],
    [ "LPUART_HandleErrorIRQ", "group__lpuart.html#ga301eb5fd702468a042bd4a9e04cca470", null ],
    [ "LPUART_CreateHandleDMA", "group__lpuart.html#ga4d20299b9a3f3ad62507dbf947ea0553", null ],
    [ "LPUART_SendDMA", "group__lpuart.html#ga35ef3ea3a009de615461bd4bd025fce8", null ],
    [ "LPUART_ReceiveDMA", "group__lpuart.html#ga8d285fdd9e37179658f8b2dd882023cb", null ],
    [ "LPUART_AbortSendDMA", "group__lpuart.html#gaca0c72106eb78aaf11767ee6cc7921a7", null ],
    [ "LPUART_AbortReceiveDMA", "group__lpuart.html#ga40ede31a99c6dfb310a1d2fe236bb58d", null ],
    [ "LPUART_GetSendRemainingBytesDMA", "group__lpuart.html#ga1d3e7960c41ff5124da277e505b8424e", null ],
    [ "LPUART_GetReceiveRemainingBytesDMA", "group__lpuart.html#ga5957bff9ac8969211be5db9008047e9b", null ],
    [ "LPUART_CreateHandleEDMA", "group__lpuart.html#ga3631beea5a5c5c8f7a863d95706e65b6", null ],
    [ "LPUART_SendEDMA", "group__lpuart.html#ga24c9dff9f142243ed5ffb3112a356386", null ],
    [ "LPUART_ReceiveEDMA", "group__lpuart.html#ga57b356ccc56276a557b3ddf53f6a7c0f", null ],
    [ "LPUART_AbortSendEDMA", "group__lpuart.html#ga3f942508654ea082c55f67037d8af4a8", null ],
    [ "LPUART_AbortReceiveEDMA", "group__lpuart.html#gadf5376a76cde2b0e83d00ae492c9f0bb", null ],
    [ "LPUART_GetSendRemainingBytesEDMA", "group__lpuart.html#ga19c16c5b938548e7543001c562bb9ec1", null ],
    [ "LPUART_GetReceiveRemainingBytesEDMA", "group__lpuart.html#gaca3a7d6ad54b1a3e41b6803b6a0cdc24", null ],
    [ "LPUART_RTOS_Init", "group__lpuart.html#ga045c5024f89b436fdf55c47340359f86", null ],
    [ "LPUART_RTOS_DeInit", "group__lpuart.html#gaa3ae28bddee769a39b8939173ac2fff7", null ],
    [ "LPUART_RTOS_Send", "group__lpuart.html#ga8fa93379ae9896d5f93de86cfeff79d7", null ],
    [ "LPUART_RTOS_Receive", "group__lpuart.html#ga0561f87122863c6e9b20f8991a73a0fb", null ]
];