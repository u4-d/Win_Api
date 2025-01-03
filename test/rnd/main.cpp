
#include <windows.h>
#include <wincrypt.h>
#include <bitset>  // ���ڷ���Ķ����ƴ�ӡ
#include <iostream>

// ʹ�� CryptGenRandom ���� 0 �� 1 �������
int generateRandomBit() {
    // ����һ����������ɾ��
    HCRYPTPROV hCryptProv;
    if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        std::cerr << "CryptAcquireContext failed." << std::endl;
        return -1;  // ����
    }

    // ����һ������ֽ�
    BYTE randomByte;
    if (!CryptGenRandom(hCryptProv, sizeof(randomByte), &randomByte)) {
        std::cerr << "CryptGenRandom failed." << std::endl;
        CryptReleaseContext(hCryptProv, 0);
        return -1;  // ����
    }

    // �ͷż���������
    CryptReleaseContext(hCryptProv, 0);

    // ������ֽ�ת��Ϊ 0 �� 1
    return randomByte % 2;
}

// ����һ���ֽ����ͣ���͵� 3 λ�� generateRandomBit ����
unsigned char generateByteWithRandomBits() {
    unsigned char result = 0;  // ��ʼ��Ϊ 0

    // ��ȡ��������͵� 3 λ
    for (int i = 0; i < 3; ++i) {
        int bit = generateRandomBit();  // ���� 0 �� 1
        result |= (bit << i);  // �����ɵ� bit �������ֽڵĶ�Ӧλ��
    }

    return result;
}

// ��ӡ�ֽڵĶ����Ʊ�ʾ
void printBinary(unsigned char byte) {
    std::bitset<8> binary(byte);  // ���ֽ�ת��Ϊ 8 λ������
    std::cout << binary << std::endl;
}

int main() {
    // �������� 10 ����� 0 �� 1
    for (int i = 0; i < 10; ++i) {
        // �������ɴ��������� 3 λ���ֽ�
        unsigned char byteWithRandomBits = generateByteWithRandomBits();

        // ��ӡ���
        printBinary(byteWithRandomBits);

    }
    std::cout << std::endl;

    return 0;
}
