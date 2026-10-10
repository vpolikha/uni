# ========================================================
# VIGENERE CIPHER
# ========================================================

class VigenereCipher:

    def __init__(self) -> None:
        self.__etalon_abc = "abcdefghijklmnopqrstuvwxyz"


    # ========================================================
    # ENCRYPTION
    # ========================================================

    def encrypt(self, key: str, text: str) -> str:
        key = key.lower()
        text = text.lower()

        if not key or not key.isascii() or not key.isalpha():
            raise ValueError("Key must contain English letters only")

        res = ""
        key_index = 0

        for char in text:
            text_index = self.__etalon_abc.find(char)

            if text_index == -1:
                res += char
                continue

            current_key_char = key[key_index % len(key)]
            current_key_index = self.__etalon_abc.find(
                current_key_char
            )

            new_index = (
                text_index + current_key_index
            ) % len(self.__etalon_abc)

            res += self.__etalon_abc[new_index]
            key_index += 1

        return res


    # ========================================================
    # DECRYPTION
    # ========================================================

    def decrypt(self, key: str, text: str) -> str:
        key = key.lower()
        text = text.lower()

        if not key or not key.isascii() or not key.isalpha():
            raise ValueError("Key must contain English letters only")

        res = ""
        key_index = 0

        for char in text:
            text_index = self.__etalon_abc.find(char)

            if text_index == -1:
                res += char
                continue

            current_key_char = key[key_index % len(key)]
            current_key_index = self.__etalon_abc.find(
                current_key_char
            )

            new_index = (
                text_index - current_key_index
            ) % len(self.__etalon_abc)

            res += self.__etalon_abc[new_index]
            key_index += 1

        return res


# ========================================================
# PROGRAM
# ========================================================

vigenere = VigenereCipher()

text = "Hello, World!"
key = "KEY"

enc_text = vigenere.encrypt(key, text)
dec_text = vigenere.decrypt(key, enc_text)

print("Original text:", text)
print("Key:", key)
print("Encrypted text:", enc_text)
print("Decrypted text:", dec_text)
