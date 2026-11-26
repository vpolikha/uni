import random
import time

class DH:
    def __init__(self, gen: int, mod: int) -> None:
        if gen <= 0 or mod <= 0:
            return

        self.gen = gen
        self.mod = mod
        self.__priv = random.randint(0, 100)
        self.pub = pow(self.gen, self.__priv, self.mod)
        self.__key: None | int = None

    def recieve(self, oth: int) -> bool:
        if oth <= 0:
            return False

        self.__key = pow(oth, self.__priv, self.mod)

    def get_key(self) -> None | int:
        return self.__key

random.seed(time.time())
gen = 100
mod = 17
A = DH(gen, mod)
B = DH(gen, mod)
B.recieve(A.pub)
A.recieve(B.pub)
print("KEY A:", A.get_key())
print("KEY B:", B.get_key())
