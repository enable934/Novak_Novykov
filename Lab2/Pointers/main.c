void delay();

int main(void) {
    int *GPIO_C13_CRH = (int *) (0x40011004);
    int *GPIO_C13_ODR = (int *) (0x4001100C);

    int *GPIO_A4_CRL = (int *) (0x40010800);
    int *GPIO_A4_ODR = (int *) (0x4001080C);

    int *APB2ENR = (int *) 0x40021018;
    int i;

    *APB2ENR |= 0x00014;

    *GPIO_C13_CRH = 0x00300000;
    *GPIO_A4_CRL = 0x00030000;

    while (1) {
        *GPIO_C13_ODR = 0x00002000;
        *GPIO_A4_ODR = 0x00000010;

        delay();

        *GPIO_A4_ODR = 0x00000000;

        delay();

        *GPIO_C13_ODR = 0x00000000;
        *GPIO_A4_ODR = 0x00000010;

        delay();

        *GPIO_A4_ODR = 0x00000000;

        delay();
    }
}

void delay(void) {
    for (int i = 0; i < 555555; i++) {}
}