#!/bin/sh


BASE_HEX=${1:-0x04330000}
printf "DMAC_IDREG                              "; devmem $(( $BASE_HEX + 0x0)) 64
printf "DMAC_COMPVERREG                         "; devmem $(( $BASE_HEX + 0x008)) 32
printf "DMAC_CFGREG                             "; devmem $(( $BASE_HEX + 0x010)) 32
printf "DMAC_CHENREG                            "; devmem $(( $BASE_HEX + 0x018)) 64
printf "DMAC_INTSTATUSREG                       "; devmem $(( $BASE_HEX + 0x030)) 32
printf "DMAC_COMMONREG_INTSTATUS_ENABLEREG      "; devmem $(( $BASE_HEX + 0x040)) 32
printf "DMAC_COMMONREG_INTSIGNAL_ENABLEREG      "; devmem $(( $BASE_HEX + 0x048)) 32
printf "DMAC_COMMONREG_INTSTATUSREG             "; devmem $(( $BASE_HEX + 0x050)) 32


printf "CH0_SAR                                "; devmem $(( $BASE_HEX + 0x100)) 64 
printf "CH0_DAR                                "; devmem $(( $BASE_HEX + 0x108)) 64
printf "CH0_BLOCK_TS                           "; devmem $(( $BASE_HEX + 0x110)) 32
printf "CH0_CTL                                "; devmem $(( $BASE_HEX + 0x118)) 64 
printf "CH0_CFG                                "; devmem $(( $BASE_HEX + 0x120)) 64 
printf "CH0_LLP                                "; devmem $(( $BASE_HEX + 0x128)) 64 
printf "CH0_STATUSREG                          "; devmem $(( $BASE_HEX + 0x130)) 64 
printf "CH0_SWHSSRCREG                         "; devmem $(( $BASE_HEX + 0x138)) 32 
printf "CH0_SWHSDSTREG                         "; devmem $(( $BASE_HEX + 0x140)) 32 
printf "CH0_AXI_IDREG                          "; devmem $(( $BASE_HEX + 0x150)) 32 
printf "CH0_AXI_QOSREG                         "; devmem $(( $BASE_HEX + 0x158)) 32 
printf "CH0_SSTAT                              "; devmem $(( $BASE_HEX + 0x160)) 32 
printf "CH0_DSTAT                              "; devmem $(( $BASE_HEX + 0x168)) 32 
printf "CH0_SSTATAR                            "; devmem $(( $BASE_HEX + 0x170)) 64 
printf "CH0_DSTATAR                            "; devmem $(( $BASE_HEX + 0x178)) 64 
printf "CH0_INTSTATUS_ENABLEREG                "; devmem $(( $BASE_HEX + 0x180)) 32 
printf "CH0_INTSTATUS                          "; devmem $(( $BASE_HEX + 0x188)) 32 
printf "CH0_INTSIGNAL_ENABLEREG                "; devmem $(( $BASE_HEX + 0x190)) 32 

printf "CH1_SAR                                "; devmem $(( $BASE_HEX + 0x100 +0x100)) 64 
printf "CH1_DAR                                "; devmem $(( $BASE_HEX + 0x100 +0x108)) 64
printf "CH1_BLOCK_TS                           "; devmem $(( $BASE_HEX + 0x100 +0x110)) 32
printf "CH1_CTL                                "; devmem $(( $BASE_HEX + 0x100 +0x118)) 64 
printf "CH1_CFG                                "; devmem $(( $BASE_HEX + 0x100 +0x120)) 64 
printf "CH1_LLP                                "; devmem $(( $BASE_HEX + 0x100 +0x128)) 64 
printf "CH1_STATUSREG                          "; devmem $(( $BASE_HEX + 0x100 +0x130)) 64 
printf "CH1_SWHSSRCREG                         "; devmem $(( $BASE_HEX + 0x100 +0x138)) 32 
printf "CH1_SWHSDSTREG                         "; devmem $(( $BASE_HEX + 0x100 +0x140)) 32 
printf "CH1_AXI_IDREG                          "; devmem $(( $BASE_HEX + 0x100 +0x150)) 32 
printf "CH1_AXI_QOSREG                         "; devmem $(( $BASE_HEX + 0x100 +0x158)) 32 
printf "CH1_SSTAT                              "; devmem $(( $BASE_HEX + 0x100 +0x160)) 32 
printf "CH1_DSTAT                              "; devmem $(( $BASE_HEX + 0x100 +0x168)) 32 
printf "CH1_SSTATAR                            "; devmem $(( $BASE_HEX + 0x100 +0x170)) 64 
printf "CH1_DSTATAR                            "; devmem $(( $BASE_HEX + 0x100 +0x178)) 64 
printf "CH1_INTSTATUS_ENABLEREG                "; devmem $(( $BASE_HEX + 0x100 +0x180)) 32 
printf "CH1_INTSTATUS                          "; devmem $(( $BASE_HEX + 0x100 +0x188)) 32 
printf "CH1_INTSIGNAL_ENABLEREG                "; devmem $(( $BASE_HEX + 0x100 +0x190)) 32 