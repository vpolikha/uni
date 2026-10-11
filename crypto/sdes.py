# ========================================================
# S-DE
# ========================================================

class SDES:

    def __init__(self, key: list[int]) -> None:
        self.__key = key

        self.__P10 = [3, 5, 2, 7, 4, 10, 1, 9, 8, 6]
        self.__P8 = [6, 3, 7, 4, 8, 5, 10, 9]
        self.__IP = [2, 6, 3, 1, 4, 8, 5, 7]
        self.__EP = [4, 1, 2, 3, 2, 3, 4, 1]
        self.__P4 = [2, 4, 3, 1]
        self.__IP_inv = [4, 1, 3, 5, 7, 2, 8, 6]

        self.__S0 = [
            [1, 0, 3, 2],
            [3, 2, 1, 0],
            [0, 2, 1, 3],
            [3, 1, 3, 2]
        ]

        self.__S1 = [
            [0, 1, 2, 3],
            [2, 0, 1, 3],
            [3, 0, 1, 0],
            [2, 1, 0, 3]
        ]

        self.__key1 = []
        self.__key2 = []

        self.__key_generation()


    # ========================================================
    # KEY GENERATION
    # ========================================================

    def __permute(self, bits: list[int], table: list[int]) -> list[int]:
        return [bits[i - 1] for i in table]


    def __shift(self, bits: list[int], n: int) -> list[int]:
        n %= len(bits)
        return bits[n:] + bits[:n]


    def __key_generation(self) -> None:
        key = self.__permute(self.__key, self.__P10)

        left = key[:5]
        right = key[5:]

        left = self.__shift(left, 1)
        right = self.__shift(right, 1)

        self.__key1 = self.__permute(left + right, self.__P8)

        left = self.__shift(left, 2)
        right = self.__shift(right, 2)

        self.__key2 = self.__permute(left + right, self.__P8)


    # ========================================================
    # S-BOX
    # ========================================================

    def __sbox(self, bits: list[int], box: list[list[int]]) -> list[int]:
        row = int(f"{bits[0]}{bits[3]}", 2)
        col = int(f"{bits[1]}{bits[2]}", 2)

        value = box[row][col]

        return [int(bit) for bit in f"{value:02b}"]


    # ========================================================
    # ROUND FUNCTION
    # ========================================================

    def __function(self, bits: list[int], key: list[int]) -> list[int]:
        left = bits[:4]
        right = bits[4:]

        expanded = self.__permute(right, self.__EP)
        mixed = [a ^ b for a, b in zip(expanded, key)]

        s0_result = self.__sbox(mixed[:4], self.__S0)
        s1_result = self.__sbox(mixed[4:], self.__S1)

        substituted = s0_result + s1_result
        permuted = self.__permute(substituted, self.__P4)

        left = [a ^ b for a, b in zip(left, permuted)]

        return left + right


    # ========================================================
    # SWAP
    # ========================================================

    def __swap(self, bits: list[int]) -> list[int]:
        return bits[4:] + bits[:4]


    # ========================================================
    # ENCRYPTION
    # ========================================================

    def encrypt(self, plaintext: list[int]) -> list[int]:
        bits = self.__permute(plaintext, self.__IP)

        bits = self.__function(bits, self.__key1)
        bits = self.__swap(bits)
        bits = self.__function(bits, self.__key2)

        return self.__permute(bits, self.__IP_inv)


    # ========================================================
    # DECRYPTION
    # ========================================================

    def decrypt(self, ciphertext: list[int]) -> list[int]:
        bits = self.__permute(ciphertext, self.__IP)

        bits = self.__function(bits, self.__key2)
        bits = self.__swap(bits)
        bits = self.__function(bits, self.__key1)

        return self.__permute(bits, self.__IP_inv)


    # ========================================================
    # KEY ACCESS
    # ========================================================

    def get_keys(self) -> tuple[list[int], list[int]]:
        return self.__key1.copy(), self.__key2.copy()


# ========================================================
# PROGRAM
# ========================================================

key = [1, 0, 1, 0, 0, 0, 0, 0, 1, 0]
plaintext = [1, 0, 0, 1, 0, 1, 1, 1]

sdes = SDES(key)

key1, key2 = sdes.get_keys()
ciphertext = sdes.encrypt(plaintext)
decrypted = sdes.decrypt(ciphertext)

print("Key-1:", key1)
print("Key-2:", key2)

print("Plaintext: ", plaintext)
print("Ciphertext:", ciphertext)
print("Decrypted: ", decrypted)
