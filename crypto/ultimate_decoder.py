import json
import enchant
from wordfreq import iter_wordlist


# ========================================================
# WORD PATTERN INDEX
# ========================================================

class WordPatternIndex:

    def __init__(self, index_file: str = "index.json") -> None:
        self.__index_file = index_file
        self.__index: dict = {}


    # ========================================================
    # WORD PATTERN
    # ========================================================

    def get_word_pattern(self, word: str) -> str:
        letters = {}
        pattern = []
        next_number = 0

        for char in word:
            if char not in letters:
                letters[char] = next_number
                next_number += 1

            pattern.append(str(letters[char]))

        return "".join(pattern)


    # ========================================================
    # INDEX BUILDING
    # ========================================================

    def build(self) -> None:
        index = {}

        for word in iter_wordlist("en", wordlist="large"):
            word = word.lower()

            if not word.isascii() or not word.isalpha():
                continue

            length = len(word)
            pattern = self.get_word_pattern(word)

            if length not in index:
                index[length] = {}

            if pattern not in index[length]:
                index[length][pattern] = []

            index[length][pattern].append(word)

        self.__index = index


    # ========================================================
    # FILE OPERATIONS
    # ========================================================

    def save(self) -> None:
        with open(self.__index_file, "w", encoding="utf-8") as file:
            json.dump(self.__index, file, ensure_ascii=False, indent=4)


    def load(self) -> None:
        with open(self.__index_file, "r", encoding="utf-8") as file:
            self.__index = json.load(file)


    # ========================================================
    # WORD SEARCH
    # ========================================================

    def get_words(self, word: str) -> list[str]:
        word = word.lower()
        length = str(len(word))
        pattern = self.get_word_pattern(word)

        if length not in self.__index:
            return []

        if pattern not in self.__index[length]:
            return []

        return self.__index[length][pattern]


# ========================================================
# ULTIMATE DECRYPTER
# ========================================================

class UltimateDecrypter:

    def __init__(self, word_index: WordPatternIndex) -> None:
        self.__etalon_abc = "abcdefghijklmnopqrstuvwxyz"
        self.__dictionary = enchant.Dict("en_US")
        self.__word_index = word_index


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
                res += self.__etalon_abc[new_index]

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
    # REPLACEMENT MAPPING
    # ========================================================

    def add_word_mapping(
        self,
        mapping: dict[str, str],
        cipher_word: str,
        plain_word: str
    ) -> dict[str, str] | None:

        candidate_mapping = mapping.copy()

        for cipher_char, plain_char in zip(cipher_word, plain_word):
            if cipher_char in candidate_mapping:
                if candidate_mapping[cipher_char] != plain_char:
                    return None
            else:
                if plain_char in candidate_mapping.values():
                    return None

                candidate_mapping[cipher_char] = plain_char

        return candidate_mapping


    def get_valid_candidates(
        self,
        cipher_word: str,
        mapping: dict[str, str]
    ) -> list[str]:

        candidates = self.__word_index.get_words(cipher_word)
        valid_candidates = []

        for plain_word in candidates:
            candidate_mapping = self.add_word_mapping(
                mapping,
                cipher_word,
                plain_word
            )

            if candidate_mapping is not None:
                valid_candidates.append(plain_word)

        return valid_candidates


    def get_replacement_key(self, mapping: dict[str, str]) -> str:
        return "".join(mapping.get(c, c) for c in self.__etalon_abc)


    # ========================================================
    # REPLACEMENT BACKTRACKING
    # ========================================================

    def solve_replacement(
        self,
        words: list[str],
        mapping: dict[str, str]
    ) -> dict[str, str] | None:

        if len(words) == 0:
            return mapping

        best_word = None
        best_candidates = None

        for word in words:
            candidates = self.get_valid_candidates(word, mapping)

            if len(candidates) == 0:
                return None

            if (
                best_candidates is None
                or len(candidates) < len(best_candidates)
                or (
                    len(candidates) == len(best_candidates)
                    and len(word) > len(best_word)
                )
            ):
                best_word = word
                best_candidates = candidates

        remaining_words = words.copy()
        remaining_words.remove(best_word)

        print()
        print("WORD:", best_word)
        print("CANDIDATES:", len(best_candidates))

        for plain_word in best_candidates:
            candidate_mapping = self.add_word_mapping(
                mapping,
                best_word,
                plain_word
            )

            if candidate_mapping is None:
                continue

            print("TRY:", best_word, "->", plain_word)

            result = self.solve_replacement(
                remaining_words,
                candidate_mapping
            )

            if result is not None:
                return result

        return None


    # ========================================================
    # REPLACEMENT CIPHER
    # ========================================================

    def find_replacement(self, text: str) -> str:
        words = text.lower().split()
        mapping = {}

        result = self.solve_replacement(words, mapping)

        if result is None:
            return ""

        print()
        print("FINAL MAPPING:", result)
        print("REPLACEMENT KEY:", self.get_replacement_key(result))

        decrypted = ""

        for char in text.lower():
            decrypted += result.get(char, char)

        return decrypted


# ========================================================
# PROGRAM
# ========================================================

word_index = WordPatternIndex()

word_index.build()
word_index.save()
word_index.load()

print(word_index.get_words("hello"))
print(word_index.get_words("there"))

decrypter = UltimateDecrypter(word_index)

enc_text = "Gdaysbpcnkp tnkvbjqksy dl rjmbnkr"

caesar_solution = decrypter.find_caesar(enc_text)

if caesar_solution != "":
    print(f"Caesar solution for {enc_text}:\n{caesar_solution}")

replacement_solution = decrypter.find_replacement(enc_text)

print()
print(f"Replacement solution for {enc_text}:")
print(replacement_solution)