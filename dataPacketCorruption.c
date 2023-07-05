/*  Algorithm:
*	In this code, the isPacketCorrupted function calculates the CRC (Cyclic Redundancy Check).
*	For obtaining the packet data by summing up all the bytes in the data array.
*	It then compares the calculated CRC with the value stored in the crc field of the packet.
*	If the two values don't match, it indicates that the packet is corrupted.
*	we are calling the isPacketCorrupted function each time to check received packet 
*	From that we can understand whether it is corrupted or not.
*/

#include <stdio.h>
#include <stdint.h>
#define MAX_PACKET_DATA_LENGTH (50)

typedef struct data_packet_t {
    uint8_t id;
    uint8_t data_length;
    uint8_t data[MAX_PACKET_DATA_LENGTH];
    uint16_t crc;
} data_packet_t;

uint16_t calculateCRC(const uint8_t *data, uint8_t length) {
    // Initialising CRC value as 0xFFFF
    uint16_t crc = 0xFFFF;  

    for (int i = 0; i < length; i++) {
        // XOR the byte with the CRC
        crc ^= (uint16_t)data[i] << 8;  

        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                // If MSB is 1, shifted left and XOR with polynomial
                crc = (crc << 1) ^ 0x1021;  
            } else {
                // If MSB is 0, It shifted left
                crc = crc << 1;  
            }
        }
    }

    return crc;
}

int isPacketCorrupted(const data_packet_t *packet) {
    // Calculate the CRC(cyclic redundancy code) of the packet data
    uint16_t calculatedCRC = calculateCRC(packet->data, packet->data_length);
    //At the time of transmitting
    printf("packet's CRC :%d\n",packet->crc);
    //At the time of receiving
    printf("Calculated CRC :%d\n",calculatedCRC);
    // Comparing the calculated CRC with the packet's CRC
    if (calculatedCRC == packet->crc) {
        return 0;  // Packet is not corrupted
    } else {
        return 1;  // Packet is corrupted
    }
}

int main() {
    /*      Here we are examine two situations:
                1.Accurate data reception
                2.corrupted data reception       */
    // 1.Accurate data reception
    printf("Accurate data reception:\n");
    data_packet_t Packet1;
    Packet1.id = 1;
    Packet1.data_length = 5;
    Packet1.data[0] = 0x12;
    Packet1.data[1] = 0x34;
    Packet1.data[2] = 0x56;
    Packet1.data[3] = 0x78;
    Packet1.data[4] = 0x9A;
    Packet1.crc = calculateCRC(Packet1.data, Packet1.data_length);
    int isCorrupted = isPacketCorrupted(&Packet1);
    if (isCorrupted) {
        printf("Packet is corrupted.\n");
    } else {
        printf("Packet is not corrupted.\n");
    }
    printf("***************************\n");
    
    // 2.corrupted data reception
    printf("Corrupted data reception:\n");
    data_packet_t Packet2;
    Packet2.id = 2;
    Packet2.data_length = 5;
    Packet2.data[0] = 0x12;
    Packet2.data[1] = 0x34;
    Packet2.data[2] = 0x56;
    Packet2.data[3] = 0x78;
    Packet2.data[4] = 0x9A;
    Packet2.crc = calculateCRC(Packet2.data, Packet2.data_length);
    
    // If we changed the data then Calculated CRC value also changed that indicates packet is corrupted
    Packet2.data[2] = 0xBC;

    isCorrupted = isPacketCorrupted(&Packet2);

    if (isCorrupted) {
        printf("Packet is corrupted.\n");
    } else {
        printf("Packet is not corrupted.\n");
    }
    printf("***************************\n");
    return 0;
}

/* Output

Accurate data reception:
packet's CRC :63648
Calculated CRC :63648
Packet is not corrupted.
***************************
Corrupted data reception:
packet's CRC :63648
Calculated CRC :40784
Packet is corrupted.
***************************

*/