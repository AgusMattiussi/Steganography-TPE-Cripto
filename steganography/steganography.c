#include "steganography.h"

typedef struct {
  uint8_t mask;
  uint8_t ioff;
  uint8_t doff;
} lsb_params_t;

static const lsb_params_t LSB4 = { 
    .mask = 0x0F, 
    .ioff = 4, 
    .doff = 4 
};

static const lsb_params_t LSB2 = {
    .mask = 0x03, 
    .ioff = 6, 
    .doff = 2
};

static void encoder(lsb_params_t params, uint8_t* image, size_t imageSize, size_t offset, uint8_t* shadows, size_t shadowsSize) {
    uint8_t mask = params.mask;
    uint8_t off = params.ioff;

    for (int i = offset, j = 0; i < imageSize && j < shadowsSize; i++) {
        uint8_t shadowBit = (shadows[j] >> off) & mask;

        image[i] &= ~mask;
        image[i] |= shadowBit;

        off -= params.doff;
        if (off < 0) {
            off += 8;
            j++;
        }
    }
}

void lsb2Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize) {
    encoder(LSB2, image, imageSize, offset, shadows, shadowsSize);
}

void lsb4Encode(uint8_t *image, size_t imageSize, size_t offset, uint8_t *shadows, size_t shadowsSize) {
    encoder(LSB4, image, imageSize, offset, shadows, shadowsSize);
}

/*
void testPrinter(char *image, int size){
    for (int i = 0; i < size; i++) {
        printf("%hhX ", image[i]);
    }
}


int main() {
    char image[16] = {0x00};
    char shadows[3] = {0xFF};
    char image2[16] = {0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    char secret[1] = {0b11011010};
    char original[4] = {0b01110111, 0b01110111, 0b01110110, 0b01110100};
    char offuscated[4] = {0b01110111, 0b01110101, 0b01110110, 0b01110110};

    lsb4Encode(image, 16, 0, shadows, 3);
    lsb2Encode(original, 4, 0, secret, 1);

    printf("============== LSB2 ================\n");
    testPrinter(original, 4);
    printf("\n============= Answer ===============\n");    
    testPrinter(offuscated, 4);
    printf("\n====================================\n\n");    

    printf("\n============== LSB4 ================\n");
    testPrinter(image, 16);
    printf("\n============= Answer ===============\n");    
    testPrinter(image2, 16);
    printf("\n====================================\n");  
}
*/