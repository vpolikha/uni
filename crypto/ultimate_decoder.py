import enchant
from freq import BIGRAMS

class UltimateDecrypter:
    def __init__(self) -> None:
        self.__etalon_abc = "abcdefghijklmnopqrstuvwxyz"
        self.__dictionary = enchant.Dict("en_US")

    def caesar_decrypt(self, key, text: str) -> str:
        text = text.lower()
        res= ""
        for char in text:
            index = self.__etalon_abc.find(char)
            if index == -1:
                res += char
            else:
                new_index = (index - key) % 26
                if new_index < 0:
                    new_index = 26 + new_index
                new_char = self.__etalon_abc[new_index]
                res += new_char
        return res

    def find_caesar(self, text: str) -> str:
        text = text.lower()
        for i in range(1, 26):
            decrypted = self.caesar_decrypt(i, text)
            words = decrypted.split()
            normal = True
            for word in words:
                normal = self.__dictionary.check(word)
                if not normal:
                    break
            if normal:
                return decrypted
        return ""

    def get_gramms(self, words: list[str], n: int) -> list[str]:
        res: list[str] = []
        for word in words:
            w_len = len(word)
            if w_len < n:
                continue
            for i in range(0, w_len - n + 1):
                res.append(word[i:i+n])
        if len(res) == 0:
            return [""]
        return res

    def index_grams(self, grams: list[str]) -> dict:
        res: dict = {}
        for gram in grams:
            if res.get(gram) is None:
                res[gram] = 1
                continue
            res[gram] += 1
        return res

    def apply_decr(self, words: list[str], gramm_old: str, gramm_new: str) -> list[str]:
        res = words.copy()
        print(f"old: '{gramm_old}' -> new: '{gramm_new}'")
        for i in range(0, len(gramm_old)):
            cur_old = gramm_old[i]
            cur_new = gramm_new[i]
            for j in range(0, len(res)):
                res[j] = res[j].replace(cur_old, cur_new)
        return res


    def find_replacement(self, text: str) -> str:
        text = text.lower()
        words = text.split()
        grams = self.get_gramms(words, 2)
        index = self.index_grams(grams)
        # for key, val in index.items():
        #     print(f"{key} -> {val}")
        bi_gramm_old = max(index, key=index.get)
        bi_gramm_new = max(BIGRAMS, key=BIGRAMS.get)
        applied_words = self.apply_decr(words, bi_gramm_old, bi_gramm_new)
        print(words)
        print(applied_words)
        return ""

decrypter = UltimateDecrypter()
# enc_text = "HSPPW spwwz"
enc_text = "Gdaysbpcnkp tnkvbjqksy dl rjmbnkr"
print(f"Caesar solution for {enc_text}:\n" + decrypter.find_caesar(enc_text))
decrypter.find_replacement(enc_text)