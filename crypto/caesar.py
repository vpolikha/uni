ALPHABET = "abcdefghijklmnopqrstuvwxyz"

def encrypt(key, text):
    text = text.lower()
    res= ""
    for char in text:
        index = ALPHABET.find(char)
        if index == -1:
            res += char
        else:
            new_index = (index + key) % 26
            new_char = ALPHABET[new_index]
            res += new_char
    return res

def decrypt(key, text):
    text = text.lower()
    res= ""
    for char in text:
        index = ALPHABET.find(char)
        if index == -1:
            res += char
        else:
            new_index = (index - key) % 26
            if new_index < 0:
                new_index = 26 + new_index
            new_char = ALPHABET[new_index]
            res += new_char
    return res

enc_key = int(input("Enter encryption key:"))
enc_text = str(input("Enter encryption text:")).lower()
print(encrypt(enc_key, enc_text))
dec_key = int(input("Enter decryption key:"))
dec_text = str(input("Enter decryption text:")).lower()
print(decrypt(dec_key, dec_text))
