void delay(int ms) {
    for (int i = 0; i < 6666 * ms; i++) {}
}

int main(void) {
    //Port configuration register low (GPIOx_CRL) (x=A..G)
    //Address offset: 0x00
    int *GPIOx_CRL;
    //GPIOx_CRH
    //Address offset: 0x04 (GPIOx_CRH)
    int *GPIOx_CRH;
    //Port output data register (GPIOx_ODR) (x=A..G)
    //Address offset: 0x0C
    //Bits 31:16 Reserved // Bits 15:0 ODRy
    int *GPIOx_ODR;
    //APB2ENR register for clock selection
    int *apb2enr;
    //delay counter variable
    int i;
    //The address define
    GPIOx_CRL = (int *) (0x40010800); //A
    GPIOx_ODR = (int *) (0x4001080C); //A
    apb2enr = (int *) 0x40021018;
    *apb2enr |= 0x0004;     // Init fast bus A
    *GPIOx_CRL = 0x00300000; // only turn on A5 as output
    char novykov[] = {'-', ' ', '*', ' ', '-', '-', '-', ' ', '*', '*', '*', '-', ' ', '-', '*', '-', '-', ' ', '-',
                      '*', '-', ' ', '-', '-', '-', ' ', '*', '*', '*', '-'};
    int length = sizeof(novykov) / sizeof(char);
    int j = 0;
    for (;; j++) {
        *GPIOx_ODR = 0x00000000;
        char element = novykov[j % length];
        if (element == '*') {
            *GPIOx_ODR = 0x00000020;
            delay(150);
        } else if (element == '-') {
            *GPIOx_ODR = 0x00000020;
            delay(300);
        } else if (element == ' ') {
            *GPIOx_ODR = 0x0000000;
            delay(600);
        }
        if (j % length == length - 1) {
            delay(5000);
        };
    }
}
