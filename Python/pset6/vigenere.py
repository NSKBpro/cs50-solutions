#!/usr/bin/python3

import sys

def getCipherText(text, key):
    ciphertext = []
    k = 0
    n = len(key)
    for i in range(len(text)):
        p = text[i]
        if p.isalpha():
            ciphertext.append(chr((ord(p) + ord(
            (key[k % n].upper(), key[k % n].lower())[int(p.islower())]
            ) - 2*ord('Aa'[int(p.islower())])) % 26 +
            ord('Aa'[int(p.islower())])))
            k += 1
        else:
            ciphertext.append(p)
    return ''.join(ciphertext)

def main():
    if len(sys.argv) != 2:
        print("Usage: ./vigenere.py cipher")
        return 1
    elif len(sys.argv) == 2:
        if not sys.argv[1].isalpha():
            return 2
    tmp = input("plaintext:")
    cipherText = getCipherText(tmp,sys.argv[1])
    print(cipherText)
    return 0
    

if __name__ == "__main__":
    main()
    