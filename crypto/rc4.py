class RC4:
    def __init__(self, key: list[int]) -> None:
        self.SIZE = 256
        self.S = [i for i in range(0, self.SIZE)]
        self.i = 0
        self.j = 0
        if not self.__set_key(key):
            return

    def __set_key(self, key: list[int]) -> bool:
        if len(key) < 1 or len(key) > 256 or any(i < 0 or i > 255 for i in key):
            return False

        self.key_len = len(key)
        self.key = [i for i in key]

        j = 0
        for i in range(0, self.SIZE):
            j = (j + self.S[i] + key[i % self.key_len]) % self.SIZE
            s = self.S[i]
            self.S[i] = self.S[j]
            self.S[j] = s

        return True

    def encrypt(self, data: bytes) -> bytes:
        res = bytearray()

        for k in range(0, len(data)):
            self.i = (self.i + 1) % self.SIZE
            self.j = (self.j + self.S[self.i]) % self.SIZE

            c = self.S[self.i]
            self.S[self.i] = self.S[self.j]
            self.S[self.j] = c

            t = (self.S[self.i] + self.S[self.j]) % self.SIZE

            res.append(data[k] ^ self.S[self.i])

        return res

key = [1, 100, 101, 11, 101, 127]
data = b"asdfghjkl;'"
A = RC4(key)
B = RC4(key)
print(str(B.encrypt(A.encrypt(data))))
