import json
import string
import random

KEY_AMT = 20
JSON_FILE = "keys_json.py"

class OneTimeNote:
    def gen_key(self, length: int) -> str:
        chars = string.ascii_letters + string.digits + "!@#$%^&*()?"
        return ''.join(random.choice(chars) for _ in range(length))

    def create_key_json(self, name: str, length: int, amt: int) -> None:
        keys = {}
        for i in range(0, amt):
            keys[i] = self.gen_key(length)
        with open(name, "w") as file:
            json.dump(keys, file)

    def xor_ascii(self, l1: str, l2: str) -> str:
        return chr(ord(l1) ^ ord(l2))

    def encrypt(self, name: str, text: str) -> str:
        res = ""
        with open(name, 'r') as file:
            keys = json.load(file)
        key_amt = len(keys.keys())
        key_id = random.choice(list(keys.keys()))
        key = keys[key_id]
        if len(key) != len(text):
            return res
        for i in range(0, len(key)):
            res += self.xor_ascii(key[i], text[i])
        del keys[key_id]
        with open(name, "w") as file:
            json.dump(keys, file)
        return res

def main() -> None:
    one_timer = OneTimeNote()

    in_text = str(input("Enter input text:\n"))
    in_text_len = len(in_text)

    one_timer.create_key_json(JSON_FILE, in_text_len, KEY_AMT)
    print(one_timer.encrypt(JSON_FILE, in_text))
    print(one_timer.encrypt(JSON_FILE, in_text))
    print(one_timer.encrypt(JSON_FILE, in_text))

main()
