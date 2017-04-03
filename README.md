twMPI on PSoC
--------------------------------------------------------------------------

-work in progress-

minimalistic transmit- and receive chain for twMPI
based on PSoC 5LP / Cypress / Creator 4.0

Tx: 4x 8 bit DACs, 1 MS/s, 50000 Samples per wave-form
Rx:		12 bit ADC, 2x 1 MS/s ADC -> 2 MS/s 12bit   


Core files:

MPI_4_Channel.cydsn/main.c
MPI_4_Channel.cydsn/TopDesign/TopDesign.cysch
MPI_4_Channel.cydsn/MPI_4_Channel.cydwr


