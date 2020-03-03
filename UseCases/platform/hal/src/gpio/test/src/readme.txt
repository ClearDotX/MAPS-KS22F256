Test Scope:

This unit test covers most of gpio hal interface and verify its operation.

board setting:
					
board			defined			status	input		output		board led	led pin

frdmk22f120m		FRDM_K22F120M		ok	ptd6:J2(8)	ptd7:J2(10)	kGpioLED1	A2
frdmk22f120mk02		FRDM_K22F120M		ok	ptd6:J2(8)	ptd7:J2(10)	kGpioLED1	A2
frdmk22f120mk0264	FRDM_K22F120M		ok	ptd6:J2(8)	ptd7:J2(10)	kGpioLED1	A2
frdmk64f120m		FRDM_K64F120M		ok	ptc16:J1(2)	ptc17:J1(4)	kGpioLED1	E26
frdmk82f		FRDM_K82F		ok	ptc11:J1(10)	ptc12:J1(6)	kGpioLED1	C8
frdmkl02z			FRDM_KL02Z			ok	pta12:J8(6)	pta11:J8(3)	kGpioLED1	B6
frdmkl03z48m		FRDM_KL03Z48M		ok	pta6:J2(5)	pta6:J2(6)	kGpioLED1	B10
frdmkl25z			FRDM_KL25Z			ok	ptd7:J2(19)	ptd6:J2(17)	kGpioLED1	B10
frdmkl27z			FRDM_KL27Z			ok	ptd7:J2(20)	ptd6:J2(18)	kGpioLED1	B18
frdmkl43z			FRDM_KL43Z			ok	pta1:J1(2)	ptb18:J1(1)	kGpioLED1	D5
frdmkw40z			FRDM_KW40Z			ok	ptc2:J25(10)	ptc3:J25(12)	kGpioLED1	C0
twrkl43z48m			TWR_KL43Z48M		ok	pta2:J15(8)	ptb1:J15(7)	kGpioLED1	A12
frdmkl46z48m		FRDM_KL46Z48M		ok	ptd6:J2(8)	ptd7:J2(10)	kGpioLED1	D5
twr60d100m		TWR_K60D100M		ok	ptc12:B77	ptc13:B78	kGpioLED1	A11
twrk22f120m		TWR_K22F120M		ok	ptc12:B77	ptc13:B78	kGpioLED1	D4
twrk22f120m128r		TWR_K22F120M128R	ok	ptc12:B77	ptc13:B78	kGpioLED1	D4
twrk22f120m256r		TWR_K22F120M256R	ok	ptc12:B77	ptc13:B78	kGpioLED1	D4
twrk22f120mk02		TWR_K22F120M		ok	ptc12:B77	ptc13:B78	kGpioLED1	D4
twrk24f120m		TWR_K24F120M		ok	ptd2:B45	ptd3:B44	kGpioLED1	D4
twrk64f120m		TWR_K64F120M		ok	ptc12:B77	ptc13:B78	kGpioLED1	E6
twrkv10z32		TWR_KV10Z32		ok	pta1:A34	pta2:A33	kGpioLED1	C1
twrkv11z75m		TWR_KV11Z75M		ok	pte17:J18(6)	pte16:J18(5)	kGpioLED1	D7
twrkv31f120m		TWR_KV31F120M		ok	ptc12:B59	ptc13:B57	kGpioLED1	E1
twrkv31f120m128r	TWR_KV31F120M128R	ok	ptc12:B59	ptc13:B57	kGpioLED1	E1
twrkv31f120m256r	TWR_KV31F120M256R	ok	ptc12:B59	ptc13:B57	kGpioLED1	E1
twrkv31f120mkv30	TWR_KV30F100M		ok	ptc12:B59	ptc13:B57	kGpioLED1	E1
twrk80f150m	TWR_K80F150M		ok	ptc12:B77	ptc13:B78	kGpioLED1	D11
						
Note:
     
1.  ptdn:Jm(x) means Port n, at J2 head x slot. Ptdn:[B|A]x means Port n, at elvetor A(or B) side slot x
   

Test Setup:

1. UART PORT: board setting.
2. Pin connection: accord above informaion.

Output:

GPIO HAL Test: Start...
GPIO HAL test success!
