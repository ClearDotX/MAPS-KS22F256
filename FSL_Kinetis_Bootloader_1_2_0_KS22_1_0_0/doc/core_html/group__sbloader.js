var group__sbloader =
[
    [ "SB File Format", "group__sb__file__format.html", "group__sb__file__format" ],
    [ "boot_cmd_t", "group__sbloader.html#structboot__cmd__t", [
      [ "address", "group__sbloader.html#a21a8ca585296632d7918dcfc8bcf200c", null ],
      [ "checksum", "group__sbloader.html#a36d5708186849ded9d7e05c860e47567", null ],
      [ "count", "group__sbloader.html#ae8e7896cc6c13246d68a8f57779ff2b0", null ],
      [ "data", "group__sbloader.html#aef923cff9098c76196a5148b48d85642", null ],
      [ "flags", "group__sbloader.html#af1cbb4c406b08d56d433f09fb5e77670", null ],
      [ "tag", "group__sbloader.html#a57bc16ad24072823c7c7a3c1943607b5", null ]
    ] ],
    [ "boot_hdr1_t", "group__sbloader.html#structboot__hdr1__t", [
      [ "fileChunks", "group__sbloader.html#a6bfc76ee4ae5134969453e06db2fdfc4", null ],
      [ "fileFlags", "group__sbloader.html#a6713be472bfce61ef2ce5193c7e8acbf", null ],
      [ "hash", "group__sbloader.html#a0eb863cb9bfc4195f56726f523f7bebe", null ],
      [ "major", "group__sbloader.html#a522f4c9e9eac37eb1b88b19bbcb32986", null ],
      [ "minor", "group__sbloader.html#a671fc6fb34ff1aedee94c78250cb3fb0", null ],
      [ "signature", "group__sbloader.html#a1e56021db51e65b583acac1b0d46e43d", null ]
    ] ],
    [ "boot_hdr2_t", "group__sbloader.html#structboot__hdr2__t", [
      [ "bootOffset", "group__sbloader.html#a4a88fde399c5b24c1660cce55eaf0368", null ],
      [ "bootSectID", "group__sbloader.html#a7c3a00bbc35d6464a70ef5d01febabe7", null ],
      [ "hdrChunks", "group__sbloader.html#a3cba19357570d5cff65fcce853029ef6", null ],
      [ "keyCount", "group__sbloader.html#a91faaa16494065ecfaca577b2f668960", null ],
      [ "keyOffset", "group__sbloader.html#abd077477741ed83bfb4531a4f89edbe0", null ],
      [ "sectCount", "group__sbloader.html#a2480a41a297d14c742e6bff170b7d3a8", null ]
    ] ],
    [ "ldr_Context_t", "group__sbloader.html#struct__ldr___context", [
      [ "Action", "group__sbloader.html#af390bd37fc4c7f980b997b4c9aa0ca4b", null ],
      [ "bootCmd", "group__sbloader.html#a81667303489b7259a752bbf093a0cfa3", null ],
      [ "bootSectChunks", "group__sbloader.html#a332985f84084f312a984b936fb9d38c2", null ],
      [ "crc32", "group__sbloader.html#a1752021991f7ac9ed0a12db173b9e387", null ],
      [ "dek", "group__sbloader.html#a33cb485d048a3e835f42adcd6bbb4003", null ],
      [ "fileChunks", "group__sbloader.html#af935616cca93321ccc4551e9f5e9a188", null ],
      [ "fileFlags", "group__sbloader.html#a23d24fcc7dc9c1664e37e72ddca80be6", null ],
      [ "initVector", "group__sbloader.html#a0e1579b32146d9b92a1bede0c554664f", null ],
      [ "keyCount", "group__sbloader.html#af444e6a1d2c32b5cb48825b573bef8a7", null ],
      [ "objectID", "group__sbloader.html#a4f760d91d684790f14c08e9590456dcb", null ],
      [ "receivedChunks", "group__sbloader.html#af3c81eaaba8593a7405bddc5aebfad6a", null ],
      [ "scratchPad", "group__sbloader.html#a1eafb9ce96c6cc56241bd9677dbb514a", null ],
      [ "sectChunks", "group__sbloader.html#a2e9ab74c6fe797a4f9183d7481e4f575", null ],
      [ "skipCount", "group__sbloader.html#abf4c2d3ae563a9cdda82eaada402d6bf", null ],
      [ "skipToEnd", "group__sbloader.html#a41460f6b2e077888c9fa190b5c3294d5", null ],
      [ "src", "group__sbloader.html#ad5c496a50e3b6e972acbd1215d061ea2", null ]
    ] ],
    [ "ldr_buf_t", "group__sbloader.html#struct__ldr__buf", [
      [ "data", "group__sbloader.html#ac589811126c7382ac3d26e6cb522791f", null ],
      [ "fillPosition", "group__sbloader.html#ac4eababbfc10f90d75bd7c570d3d29bf", null ]
    ] ],
    [ "pCallFnc_t", "group__sbloader.html#gae7e313e395b2df0802c1aae65ce7abef", null ],
    [ "pJumpFnc_t", "group__sbloader.html#ga5d25aba041950269412e47557d907ee6", null ],
    [ "pLdrFnc_t", "group__sbloader.html#gab11ac618387b67ded6844e33aa5199fa", null ],
    [ "_sbloader_status", "group__sbloader.html#ga776140b8326b3fbe0d466c71e7f424e4", null ],
    [ "ldr_DoCallCmd", "group__sbloader.html#ga08fefb64c1909e91cc855695d4c29d88", null ],
    [ "ldr_DoCommand", "group__sbloader.html#gaf82590b20d223f900afb0526629af78c", null ],
    [ "ldr_DoEraseCmd", "group__sbloader.html#gabd574bf6ff4543b4e4bb642de4edd0ad", null ],
    [ "ldr_DoFillCmd", "group__sbloader.html#ga391e8156461e75bf9c458a8c239aa668", null ],
    [ "ldr_DoGetDek", "group__sbloader.html#ga0b6aac7b3b933de983dd7b20a0a71dfa", null ],
    [ "ldr_DoHeader", "group__sbloader.html#ga08fb4f865473d3c330ba4091416ce7fb", null ],
    [ "ldr_DoHeader1", "group__sbloader.html#gab008ca0a26303b427e73f72cc7350841", null ],
    [ "ldr_DoHeader2", "group__sbloader.html#ga10fe3a0f3adc3839cf3c5b46a243e8c1", null ],
    [ "ldr_DoHeaderMac", "group__sbloader.html#ga0049471d3fe8b9d8caccb3417836bc85", null ],
    [ "ldr_DoInit", "group__sbloader.html#gad4ba3d76f1d6c2be59aa4822161e7080", null ],
    [ "ldr_DoJumpCmd", "group__sbloader.html#ga3a3306081691ef431d63ce7ad1ade981", null ],
    [ "ldr_DoKeyTest", "group__sbloader.html#ga359cb64ded6184ec3237924e1d489f9c", null ],
    [ "ldr_DoLoadBytes", "group__sbloader.html#ga0496db4b513d904caa7cff9bfd3a3b1a", null ],
    [ "ldr_DoLoadChunks", "group__sbloader.html#gad336bbae43e8ca08f9300397b316d0bb", null ],
    [ "ldr_DoLoadCmd", "group__sbloader.html#ga179d2dbc48908c8880f0d1c38fdf0118", null ],
    [ "ldr_DoTagCmd", "group__sbloader.html#ga16375b0cc8350f605d7cf5f2c2302b99", null ],
    [ "ldr_GoToNextSection", "group__sbloader.html#ga6c85e96bf9b72b3f2e34385dbfe8b97f", null ],
    [ "sbloader_finalize_jump", "group__sbloader.html#ga38c6ca3aa025abfbff1244875f2c97cc", null ],
    [ "sbloader_handle_chunk", "group__sbloader.html#ga4c7887c97e9f07681ad1be30dab94987", null ],
    [ "sbloader_init", "group__sbloader.html#ga196130e1b193207247c017a437943ea5", null ],
    [ "sbloader_pump", "group__sbloader.html#ga3235bfe8a0d7df4fb1b4c398b9f45108", null ],
    [ "s_aesKey", "group__sbloader.html#ga44f04e86cd75f107e97ea3f948fc65c7", null ],
    [ "s_loaderBuf", "group__sbloader.html#ga8bd4c85b5ac97233a4f6d141a8b3b838", null ],
    [ "s_loaderContext", "group__sbloader.html#ga6900924e02ee40bd281f9655a672a5cc", null ]
];