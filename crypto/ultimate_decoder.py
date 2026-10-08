import enchant
import time


MONOGRAMS = [
    "e", "t", "a", "o", "i", "n", "s", "h", "r", "d",
    "l", "c", "u", "m", "w", "f", "g", "y", "p", "b",
    "v", "k", "j", "x", "q", "z",
]


BIGRAMS = [
    "th", "he", "in", "en", "nt", "re", "er", "an",
    "ti", "es", "on", "at", "se", "nd", "or", "ar",
    "al", "te", "co", "de", "to", "ra", "et", "ed",
    "it", "sa", "em", "ro",
]


TRIGRAMS = [
    "the", "and", "tha", "ent", "ing", "ion", "tio",
    "for", "nde", "has", "nce", "edt", "tis", "oft",
    "sth", "men",
]


class UltimateDecrypter:

    # ========================================================
    # INITIALIZATION
    # ========================================================

    def __init__(self) -> None:
        self.__etalon_abc = "abcdefghijklmnopqrstuvwxyz"
        self.__dictionary = enchant.Dict("en_US")


    # ========================================================
    # CAESAR CIPHER
    # ========================================================

    def caesar_decrypt(self, key: int, text: str) -> str:
        text = text.lower()
        res = ""

        for char in text:
            index = self.__etalon_abc.find(char)

            if index == -1:
                res += char
            else:
                new_index = (index - key) % 26
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


    # ========================================================
    # N-GRAM ANALYSIS
    # ========================================================

    def get_gramms(
        self,
        words: list[str],
        n: int
    ) -> list[str]:

        res: list[str] = []

        for word in words:
            w_len = len(word)

            if w_len < n:
                continue

            for i in range(0, w_len - n + 1):
                res.append(word[i:i + n])

        if len(res) == 0:
            return [""]

        return res

    def index_grams(
        self,
        grams: list[str]
    ) -> dict[str, int]:

        res: dict[str, int] = {}

        for gram in grams:
            if gram not in res:
                res[gram] = 1
            else:
                res[gram] += 1

        return res

    def get_gram_index(
        self,
        text: str,
        max_n: int
    ) -> dict[int, dict[str, int]]:

        text = text.lower()
        words = text.split()

        res: dict[int, dict[str, int]] = {}

        for n in range(1, max_n + 1):
            grams = self.get_gramms(words, n)
            res[n] = self.index_grams(grams)

        return res

    def get_top_grams(
        self,
        gram_index: dict[int, dict[str, int]],
        n: int
    ) -> list[str]:

        return [
            gram
            for gram, count in sorted(
                gram_index[n].items(),
                key=lambda item: item[1],
                reverse=True
            )
        ]


    # ========================================================
    # SUBSTITUTION MAPPING
    # ========================================================

    def apply_mapping(
        self,
        text: str,
        mapping: dict[str, str]
    ) -> str:

        text = text.lower()
        res = ""

        for char in text:
            if char in mapping:
                res += mapping[char]
            elif char.isalpha():
                res += "?"
            else:
                res += char

        return res

    def add_mapping(
        self,
        mapping: dict[str, str],
        cipher_char: str,
        plain_char: str
    ) -> bool:

        if cipher_char in mapping:
            return mapping[cipher_char] == plain_char

        if plain_char in mapping.values():
            return False

        mapping[cipher_char] = plain_char

        return True

    def add_gram_mapping(
        self,
        mapping: dict[str, str],
        cipher_gram: str,
        plain_gram: str
    ) -> bool:

        if len(cipher_gram) != len(plain_gram):
            return False

        candidate = mapping.copy()

        for cipher_char, plain_char in zip(
            cipher_gram,
            plain_gram
        ):
            if not self.add_mapping(
                candidate,
                cipher_char,
                plain_char
            ):
                return False

        mapping.clear()
        mapping.update(candidate)

        return True

    def is_mapping_valid(
            self,
            text: str,
            mapping: dict[str, str]
        ) -> bool:
    
            decrypted = self.apply_mapping(
                text,
                mapping
            )
    
            words = decrypted.split()
    
            for word in words:
                if "?" not in word:
                    # print("checking:", word)
                    if not self.__dictionary.check(word):
                        return False
    
            return True

    # ========================================================
    # SUBSTITUTION BACKTRACKING
    # ========================================================

    def solve(
        self,
        cipher_grams: list[str],
        english_grams: list[str],
        index: int,
        mapping: dict[str, str],
        level: str,
        text: str,
    ) -> dict[str, str] | None:

        indent = "    " * index

        if index >= len(cipher_grams):
            # print(
            #     f"{indent}SUCCESS [{level}]"
            # )
            # print(
            #     f"{indent}Mapping: {mapping}"
            # )
            return mapping

        cipher_gram = cipher_grams[index]

        # print()
        # print(
        #     f"{indent}[{level} {index + 1}/{len(cipher_grams)}]"
        # )
        # print(
        #     f"{indent}Cipher gram: {cipher_gram}"
        # )

        for english_gram in english_grams:

            candidate_mapping = mapping.copy()

            # print(
            #     f"{indent}TRY: "
            #     f"{cipher_gram} -> {english_gram}"
            # )

            if not self.add_gram_mapping(
                candidate_mapping,
                cipher_gram,
                english_gram
            ):
                # print(
                #     f"{indent}  REJECT: conflict"
                # )
                continue
            if not self.is_mapping_valid(
                text,
                candidate_mapping
            ):
                # print(
                #     f"{indent}  REJECT: invalid word"
                # )
                continue
            # print(
            #     f"{indent}  ACCEPT"
            # )
            # print(
            #     f"{indent}  Mapping: "
            #     f"{candidate_mapping}"
            # )

            result = self.solve(
                cipher_grams,
                english_grams,
                index + 1,
                candidate_mapping,
                level,
                text
            )

            if result is not None:
                return result

            # print(
            #     f"{indent}BACKTRACK: "
            #     f"{cipher_gram} -> {english_gram}"
            # )

        # print()
        # print(
        #     f"{indent}NO SOLUTION "
        #     f"[{level} {index + 1}]"
        # )

        return None


    # ========================================================
    # MAIN SUBSTITUTION CIPHER ANALYSIS
    # ========================================================

    def find_replacement(self, text: str) -> str:
        gram_index = self.get_gram_index(text, 3)

        cipher_trigrams = self.get_top_grams(
            gram_index,
            3
        )

        cipher_bigrams = self.get_top_grams(
            gram_index,
            2
        )

        cipher_monograms = self.get_top_grams(
            gram_index,
            1
        )

        print()
        print("========================================================")
        print("SUBSTITUTION SEARCH")
        print("========================================================")

        print()
        print("Cipher trigrams:")
        print(cipher_trigrams)

        print()
        print("Cipher bigrams:")
        print(cipher_bigrams)

        print()
        print("Cipher monograms:")
        print(cipher_monograms)

        mapping: dict[str, str] = {}

        print()
        print("========================================================")
        print("TRIGRAM LEVEL")
        print("========================================================")

        mapping = self.solve(
            cipher_trigrams,
            TRIGRAMS,
            0,
            mapping,
            "TRIGRAM",
            text,
        )

        if mapping is None:
            print()
            print("TRIGRAM SEARCH FAILED")
            # return ""
            mapping = {}

        print()
        print("========================================================")
        print("BIGRAM LEVEL")
        print("========================================================")

        mapping = self.solve(
            cipher_bigrams,
            BIGRAMS,
            0,
            mapping,
            "BIGRAM",
            text,
        )

        if mapping is None:
            print()
            print("BIGRAM SEARCH FAILED")
            # return ""
            mapping = {}

        print()
        print("========================================================")
        print("MONOGRAM LEVEL")
        print("========================================================")

        mapping = self.solve(
            cipher_monograms,
            MONOGRAMS,
            0,
            mapping,
            "MONOGRAM",
            text,
        )

        if mapping is None:
            print()
            print("MONOGRAM SEARCH FAILED")
            # return ""
            mapping = {}

        print()
        print("========================================================")
        print("FINAL MAPPING")
        print("========================================================")

        print(mapping)

        return self.apply_mapping(
            text,
            mapping
        )


# ============================================================
# PROGRAM START
# ============================================================

decrypter = UltimateDecrypter()

enc_text = "Gdaysbpcnkp tnkvbjqksy dl rjmbnkr"
enc_text_1 = "rjmbnkr"

caesar_solution = decrypter.find_caesar(enc_text)

if caesar_solution != "":
    print(
        f"Caesar solution for {enc_text}:\n"
        + caesar_solution
    )

start = time.perf_counter()

replacement_solution = decrypter.find_replacement(
    enc_text
)

end = time.perf_counter()
print(f"Time: {end - start:.6f} seconds")

print()
print(
    f"Replacement solution for {enc_text}:"
)

print(
    replacement_solution
)