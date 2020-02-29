var group__flexio__spi =
[
    [ "FLEXIO_SPI_Type", "group__flexio__spi.html#structFLEXIO__SPI__Type", [
      [ "flexioBase", "group__flexio__spi.html#a6f884e3d820db0fa2c2974d9e8feaca1", null ],
      [ "SDOPinIndex", "group__flexio__spi.html#a5df20f69be0906774c5b3d9c06821868", null ],
      [ "SDIPinIndex", "group__flexio__spi.html#a176d632a86bd48ee20db504ddb5ace5d", null ],
      [ "SCKPinIndex", "group__flexio__spi.html#a5bd00a1e1450d172c1c651036af261bc", null ],
      [ "CSnPinIndex", "group__flexio__spi.html#af3462a62b5058e897b7d8387daf30317", null ],
      [ "shifterIndex", "group__flexio__spi.html#ad2ab114592f4a5982720447f8cd9e90d", null ],
      [ "timerIndex", "group__flexio__spi.html#a952204114daf59aeeb9437af1abc6eef", null ]
    ] ],
    [ "flexio_spi_master_config_t", "group__flexio__spi.html#structflexio__spi__master__config__t", [
      [ "enableMaster", "group__flexio__spi.html#abfa7b62cfa5ecc303b203c9206e8511b", null ],
      [ "enableInDoze", "group__flexio__spi.html#ace1bcfad14f01d682d72126fe8977379", null ],
      [ "enableInDebug", "group__flexio__spi.html#ae25034f9e6018e4fb4688ed8867c6b18", null ],
      [ "enableFastAccess", "group__flexio__spi.html#aca4f2130311c4dea49126cfba678669c", null ],
      [ "baudRate_Bps", "group__flexio__spi.html#a6f95fac4a0afc21630b3837787bbb353", null ],
      [ "phase", "group__flexio__spi.html#a76cac3e67183266b66cf25a29c2669bf", null ],
      [ "dataMode", "group__flexio__spi.html#a1ed3e04044edd43a3ab1658b83cc7a70", null ]
    ] ],
    [ "flexio_spi_slave_config_t", "group__flexio__spi.html#structflexio__spi__slave__config__t", [
      [ "enableSlave", "group__flexio__spi.html#a5fcf34758ddd907b1860679305858de1", null ],
      [ "enableInDoze", "group__flexio__spi.html#ab13e01169ca99c72157d7e927e9c7d6f", null ],
      [ "enableInDebug", "group__flexio__spi.html#af4180104efb170139a0e4ab731e38361", null ],
      [ "enableFastAccess", "group__flexio__spi.html#a431d5f78a4038ee716d38905334e558e", null ],
      [ "phase", "group__flexio__spi.html#a5ba29aaf41caf774f7077f1e8fe64404", null ],
      [ "dataMode", "group__flexio__spi.html#a5b7c853f42ed1b2e53e07034daf3d26f", null ]
    ] ],
    [ "flexio_spi_transfer_t", "group__flexio__spi.html#structflexio__spi__transfer__t", [
      [ "txData", "group__flexio__spi.html#adce5c034b8842ec6a9d3c3d2f12f2b72", null ],
      [ "rxData", "group__flexio__spi.html#a5a1a761e9cc6c90947ad55b701324258", null ],
      [ "dataSize", "group__flexio__spi.html#a5e76062efe44914a5bbb2f4fc3ca3aac", null ],
      [ "flags", "group__flexio__spi.html#a890b496be606905d29d43abedf7e753b", null ]
    ] ],
    [ "flexio_spi_master_handle_t", "group__flexio__spi.html#struct__flexio__spi__master__handle", [
      [ "txData", "group__flexio__spi.html#a9a8bad99d5d80699be0d248092ac8d80", null ],
      [ "rxData", "group__flexio__spi.html#a6f04ebbd2ad671dd6657be16c26bd1e6", null ],
      [ "txRemainingBytes", "group__flexio__spi.html#aeecd6ae001c1bbdfab21eb288f0ca8a9", null ],
      [ "rxRemainingBytes", "group__flexio__spi.html#aee7be6e3dd2109f9d5374a1b2fb45ade", null ],
      [ "state", "group__flexio__spi.html#a25ee5785454882c9d0856f3a0fb8e23a", null ],
      [ "bytePerFrame", "group__flexio__spi.html#ab0c81d09878eaff0b35189d7b7d4441a", null ],
      [ "direction", "group__flexio__spi.html#a8ab5ab2070ea63f0c52d1c6da2786459", null ],
      [ "callback", "group__flexio__spi.html#ac4118ced7c959f43005a9b4cdc48786d", null ],
      [ "userData", "group__flexio__spi.html#a7d3fb8c3a3c3b23a1908c0bb70230166", null ]
    ] ],
    [ "flexio_spi_master_dma_handle_t", "group__flexio__spi.html#struct__flexio__spi__master__dma__handle", [
      [ "txInProgress", "group__flexio__spi.html#a31a72fb1786d3cf14b55e9484c16e6ed", null ],
      [ "rxInProgress", "group__flexio__spi.html#ab7f0a830489dd5b317449fb076600b76", null ],
      [ "txHandle", "group__flexio__spi.html#a3f0b9ac3cdea1d88f0311b4bdf31d130", null ],
      [ "rxHandle", "group__flexio__spi.html#aa4de2d400db28e529bb943e9b4af12a4", null ],
      [ "callback", "group__flexio__spi.html#a6fd4da90bc09374ffdf6c50cdbada787", null ],
      [ "userData", "group__flexio__spi.html#a6ff237e2cebdf3347a6c6915276f0588", null ]
    ] ],
    [ "flexio_spi_master_edma_handle_t", "group__flexio__spi.html#struct__flexio__spi__master__edma__handle", [
      [ "txInProgress", "group__flexio__spi.html#af497e9228ec009ef999a3d1cb77d175d", null ],
      [ "rxInProgress", "group__flexio__spi.html#aa8bfa7c132f8415458603e2f55525597", null ],
      [ "txHandle", "group__flexio__spi.html#a539d1f291e21f3de7a9b573fec434e3c", null ],
      [ "rxHandle", "group__flexio__spi.html#a60662f5dc343b61782c0a4284ec6cab0", null ],
      [ "callback", "group__flexio__spi.html#a5a692a33dc01fe79b4ae30b19510f834", null ],
      [ "userData", "group__flexio__spi.html#ac23b20ba23a813e03d5c52586c06daaa", null ]
    ] ],
    [ "FSL_FLEXIO_SPI_DRIVER_VERSION", "group__flexio__spi.html#gadd4392d8b86be4236270866fa214dcbe", null ],
    [ "FLEXIO_SPI_DUMMYDATA", "group__flexio__spi.html#gad411a43e379e70215b462831b24c5c5b", null ],
    [ "flexio_spi_slave_handle_t", "group__flexio__spi.html#ga1950678efd37cb3665e303e094cec428", null ],
    [ "flexio_spi_master_transfer_callback_t", "group__flexio__spi.html#ga9c8b509b90588dea1921de0bdb3cfec8", null ],
    [ "flexio_spi_slave_transfer_callback_t", "group__flexio__spi.html#gad2accadc980b51b0d65e41d122abe261", null ],
    [ "flexio_spi_slave_dma_handle_t", "group__flexio__spi.html#ga914a417275e7dbb5a731a82b083fe4b4", null ],
    [ "flexio_spi_master_dma_transfer_callback_t", "group__flexio__spi.html#ga0cf79490a9b206d8ce88a885fa01b76c", null ],
    [ "flexio_spi_slave_dma_transfer_callback_t", "group__flexio__spi.html#gad8c94c7e3d725f8198aa840f7b47c420", null ],
    [ "flexio_spi_slave_edma_handle_t", "group__flexio__spi.html#gaab3d72fce6c5312795c6a24252d42089", null ],
    [ "flexio_spi_master_edma_transfer_callback_t", "group__flexio__spi.html#ga7f1f81bab39b9c570d45316f74fa597c", null ],
    [ "flexio_spi_slave_edma_transfer_callback_t", "group__flexio__spi.html#gaa0c3259164f82ccaaec19cc4231535eb", null ],
    [ "_flexio_spi_status", "group__flexio__spi.html#gac9f5b10ab474a8ef035d48bb8b3990db", [
      [ "kStatus_FLEXIO_SPI_Busy", "group__flexio__spi.html#ggac9f5b10ab474a8ef035d48bb8b3990dba9fd2c82e84cdc65b4c2d7c3d335bc4e1", null ],
      [ "kStatus_FLEXIO_SPI_Idle", "group__flexio__spi.html#ggac9f5b10ab474a8ef035d48bb8b3990dba69ccc78c05f6f6ed17f4d48e20323ec8", null ],
      [ "kStatus_FLEXIO_SPI_Error", "group__flexio__spi.html#ggac9f5b10ab474a8ef035d48bb8b3990dba4ae19baeae73d05d2dcf34dc4c54c6c4", null ]
    ] ],
    [ "flexio_spi_clock_phase_t", "group__flexio__spi.html#gac059394d6eabe169d3a1ee8a8b6f7edd", [
      [ "kFLEXIO_SPI_ClockPhaseFirstEdge", "group__flexio__spi.html#ggac059394d6eabe169d3a1ee8a8b6f7eddad07a920661cea5317982da4405bfd9e1", null ],
      [ "kFLEXIO_SPI_ClockPhaseSecondEdge", "group__flexio__spi.html#ggac059394d6eabe169d3a1ee8a8b6f7edda9d85a76b5ce670c1d47e5f95f4a46e6c", null ]
    ] ],
    [ "flexio_spi_shift_direction_t", "group__flexio__spi.html#ga25594db149526d67b23d92724487e87f", [
      [ "kFLEXIO_SPI_MsbFirst", "group__flexio__spi.html#gga25594db149526d67b23d92724487e87fad6c96f26bda5bc22af19be1e7906f3f2", null ],
      [ "kFLEXIO_SPI_LsbFirst", "group__flexio__spi.html#gga25594db149526d67b23d92724487e87faa2a981323f199d9359c61d39cb059795", null ]
    ] ],
    [ "flexio_spi_data_bitcount_mode_t", "group__flexio__spi.html#gae4e518a5b077982fca7b6bf1025c7aaf", [
      [ "kFLEXIO_SPI_8BitMode", "group__flexio__spi.html#ggae4e518a5b077982fca7b6bf1025c7aafaef0c633539e44547b47ad7b754234752", null ],
      [ "kFLEXIO_SPI_16BitMode", "group__flexio__spi.html#ggae4e518a5b077982fca7b6bf1025c7aafa343c7a9db539019f0c99fb82b4adab35", null ]
    ] ],
    [ "_flexio_spi_interrupt_enable", "group__flexio__spi.html#ga239e9a0ff79e0d36ecf6ae7c68dfd357", [
      [ "kFLEXIO_SPI_TxEmptyInterruptEnable", "group__flexio__spi.html#gga239e9a0ff79e0d36ecf6ae7c68dfd357a5243ceb746eebfdeef2ba2d791e32ff9", null ],
      [ "kFLEXIO_SPI_RxFullInterruptEnable", "group__flexio__spi.html#gga239e9a0ff79e0d36ecf6ae7c68dfd357a3431c6c1875a70da30cba8b4185ad183", null ]
    ] ],
    [ "_flexio_spi_status_flags", "group__flexio__spi.html#ga5dec706a17cf2b92ef701f684503d4b2", [
      [ "kFLEXIO_SPI_TxBufferEmptyFlag", "group__flexio__spi.html#gga5dec706a17cf2b92ef701f684503d4b2a80c0db2a058d82d018b2e8bc76f1aa6b", null ],
      [ "kFLEXIO_SPI_RxBufferFullFlag", "group__flexio__spi.html#gga5dec706a17cf2b92ef701f684503d4b2aa679f6244353e98ace08ec5d0281851b", null ]
    ] ],
    [ "_flexio_spi_dma_enable", "group__flexio__spi.html#ga713aa7c7a59e84b9fdafb2923de177ef", [
      [ "kFLEXIO_SPI_TxDmaEnable", "group__flexio__spi.html#gga713aa7c7a59e84b9fdafb2923de177efa4cdf95c3f7a1b9ae7931166a8abe1542", null ],
      [ "kFLEXIO_SPI_RxDmaEnable", "group__flexio__spi.html#gga713aa7c7a59e84b9fdafb2923de177efa002de6d16630e5d11d94b871da9a8fbd", null ],
      [ "kFLEXIO_SPI_DmaAllEnable", "group__flexio__spi.html#gga713aa7c7a59e84b9fdafb2923de177efa6b4ccfcf63573b0a3b281c32c4913f49", null ]
    ] ],
    [ "_flexio_spi_transfer_flags", "group__flexio__spi.html#gabf4ebfbc0d68b969e1b7dd838c45512d", [
      [ "kFLEXIO_SPI_8bitMsb", "group__flexio__spi.html#ggabf4ebfbc0d68b969e1b7dd838c45512da7979c2a48d6662af1777827c1afa5537", null ],
      [ "kFLEXIO_SPI_8bitLsb", "group__flexio__spi.html#ggabf4ebfbc0d68b969e1b7dd838c45512da1d31a5996c45efebdc7aeb1a894647f8", null ],
      [ "kFLEXIO_SPI_16bitMsb", "group__flexio__spi.html#ggabf4ebfbc0d68b969e1b7dd838c45512dabeb4baa55114987385ab398ceda142f9", null ],
      [ "kFLEXIO_SPI_16bitLsb", "group__flexio__spi.html#ggabf4ebfbc0d68b969e1b7dd838c45512dad6774d8673e5aa247be136e38c8fad91", null ]
    ] ],
    [ "FLEXIO_SPI_MasterInit", "group__flexio__spi.html#gacc68d67c46d897e6e9b2268c0fe999d2", null ],
    [ "FLEXIO_SPI_MasterDeInit", "group__flexio__spi.html#ga516b3f407f183903340eb3122ef72390", null ],
    [ "FLEXIO_SPI_MasterGetDefaultConfig", "group__flexio__spi.html#ga2d99f8a94f95262fc821b42e3812ee7e", null ],
    [ "FLEXIO_SPI_SlaveInit", "group__flexio__spi.html#gae14521e2d88c63c34a51a31c726bac48", null ],
    [ "FLEXIO_SPI_SlaveDeInit", "group__flexio__spi.html#gabb967386e16d71f492c3ce398c238e42", null ],
    [ "FLEXIO_SPI_SlaveGetDefaultConfig", "group__flexio__spi.html#gaa6979876d2c0b4f65deabd6f5ce16d41", null ],
    [ "FLEXIO_SPI_GetStatusFlags", "group__flexio__spi.html#ga1b709c34a5861a755ef2365eca4aef6d", null ],
    [ "FLEXIO_SPI_ClearStatusFlags", "group__flexio__spi.html#ga53311c201694351c61d5d56f2924d73e", null ],
    [ "FLEXIO_SPI_EnableInterrupts", "group__flexio__spi.html#ga877ffb51ccf9f5a2ecd5bd5a23f208c3", null ],
    [ "FLEXIO_SPI_DisableInterrupts", "group__flexio__spi.html#gac765a8199e5d7c7546d2b31c06563638", null ],
    [ "FLEXIO_SPI_EnableDMA", "group__flexio__spi.html#ga584eae5056ceeee0aea9ba9d5e2302ff", null ],
    [ "FLEXIO_SPI_GetTxDataRegisterAddress", "group__flexio__spi.html#gac2629d5c8821fe5a2a3496b51dbc99f5", null ],
    [ "FLEXIO_SPI_GetRxDataRegisterAddress", "group__flexio__spi.html#ga061dcced42575767baf53f6b6e5da609", null ],
    [ "FLEXIO_SPI_Enable", "group__flexio__spi.html#ga68656f6d77e46b59e311e1a97f3b5878", null ],
    [ "FLEXIO_SPI_MasterSetBaudRate", "group__flexio__spi.html#gaa9287322d22895ac0352db5fe0dd909e", null ],
    [ "FLEXIO_SPI_WriteData", "group__flexio__spi.html#ga1eb1063ea3e7b0af749f899d86e85c42", null ],
    [ "FLEXIO_SPI_ReadData", "group__flexio__spi.html#gaa36e294b6b74373a5466a3c69ecdbf18", null ],
    [ "FLEXIO_SPI_WriteBlocking", "group__flexio__spi.html#gadeb6573f99e8ee11b9917683efbf8dd3", null ],
    [ "FLEXIO_SPI_ReadBlocking", "group__flexio__spi.html#ga46eb22a1bcbdc6e7baff4b8a3abe70d6", null ],
    [ "FLEXIO_SPI_MasterCreateHandle", "group__flexio__spi.html#gaac0161e52ec2406215e1b8a59b905395", null ],
    [ "FLEXIO_SPI_MasterTransferNonBlocking", "group__flexio__spi.html#gabcfcbee985cb49bdbd02ab1698d2c041", null ],
    [ "FLEXIO_SPI_MasterAbortTransfer", "group__flexio__spi.html#ga560588b2dff072fa5fe9aa403a3691e3", null ],
    [ "FLEXIO_SPI_MasterGetTransferRemainingBytes", "group__flexio__spi.html#ga647e53ccc313782dc7d5f19a1f3d9423", null ],
    [ "FLEXIO_SPI_MasterHandleIRQ", "group__flexio__spi.html#ga213573e37e143291f6ae47f7aeb7d2d4", null ],
    [ "FLEXIO_SPI_SlaveCreateHandle", "group__flexio__spi.html#ga8bb10bda6d3d0f75cc39356c4b64cf38", null ],
    [ "FLEXIO_SPI_SlaveTransferNonBlocking", "group__flexio__spi.html#gac87e12f90c2449a92d9c2aaef2910a4e", null ],
    [ "FLEXIO_SPI_SlaveAbortTransfer", "group__flexio__spi.html#ga12ec575db9ceb3cd50f72fbb6faf8e4e", null ],
    [ "FLEXIO_SPI_SlaveGetTransferRemainingBytes", "group__flexio__spi.html#gac14b9bc54925b0b7952bc9414ac98732", null ],
    [ "FLEXIO_SPI_SlaveHandleIRQ", "group__flexio__spi.html#ga4c8e7be7b00596c58975490d2f8805d4", null ],
    [ "FLEXIO_SPI_MasterCreateHandleDMA", "group__flexio__spi.html#gabae049f3144e0129788518e04e506aca", null ],
    [ "FLEXIO_SPI_MasterTransferDMA", "group__flexio__spi.html#gac31d7725e8e1871c7dff13fc291d1951", null ],
    [ "FLEXIO_SPI_MasterAbortTransferDMA", "group__flexio__spi.html#ga83d7c225d3c85c54173fe4ed3b4093eb", null ],
    [ "FLEXIO_SPI_MasterGetTransferRemainingBytesDMA", "group__flexio__spi.html#gab8043c0bb8819b174394cff9f26013b5", null ],
    [ "FLEXIO_SPI_SlaveCreateHandleDMA", "group__flexio__spi.html#gaf50058150483b9aede64c66178c58e87", null ],
    [ "FLEXIO_SPI_SlaveTransferDMA", "group__flexio__spi.html#ga1504ded98981c890ec01722a01f691cb", null ],
    [ "FLEXIO_SPI_SlaveAbortTransferDMA", "group__flexio__spi.html#ga541a0c1e77c75b0d28eb5a0cb76f9579", null ],
    [ "FLEXIO_SPI_SlaveGetTransferRemainingBytesDMA", "group__flexio__spi.html#ga9fdf2351411fac520bf2f7451d6f8c8c", null ],
    [ "FLEXIO_SPI_MasterCreateHandleEDMA", "group__flexio__spi.html#ga2a0d893c7f17050d5b9cd701e785b835", null ],
    [ "FLEXIO_SPI_MasterTransferEDMA", "group__flexio__spi.html#ga2ea179f209731d1696e999e4dca9d4cf", null ],
    [ "FLEXIO_SPI_MasterAbortTransferEDMA", "group__flexio__spi.html#ga0a77654d467159a56b4a929312c22557", null ],
    [ "FLEXIO_SPI_MasterGetTransferRemainingBytesEDMA", "group__flexio__spi.html#gabe8f7967d74632873e445798de61efce", null ],
    [ "FLEXIO_SPI_SlaveCreateHandleEDMA", "group__flexio__spi.html#ga048ab1732685d4d17d23a2f201284368", null ],
    [ "FLEXIO_SPI_SlaveTransferEDMA", "group__flexio__spi.html#ga4ae856116465255de9660df5e5d287a8", null ],
    [ "FLEXIO_SPI_SlaveAbortTransferEDMA", "group__flexio__spi.html#ga78df743d4d6e557f15847e8caaa9e988", null ],
    [ "FLEXIO_SPI_SlaveGetTransferRemainingBytesEDMA", "group__flexio__spi.html#gad92677ec6275da5fc2766b6e0e02a95f", null ]
];