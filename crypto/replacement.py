# State Engineering University of Armenia
# command line
ALPHABET = "abcdefghijklmnopqrstuvwxyz"

def process_key(in_str):
    in_str = in_str.lower()
    res = ""
    alpha = "abcdefghijklmnopqrstuvwxyz"
    for char in in_str:
        index = alpha.find(char)
        if index == -1:
            continue
        res += alpha[index]
        alpha = alpha[:index] + alpha[index+1:]
    for char in alpha:
        res += char
    return res

def encrypt(init_key, text):
    key = process_key(init_key)
    res = ""
    for char in text:
        index = ALPHABET.find(char)
        if index == -1:
            res += char
        else:
            res += key[index]
    return res

def decrypt(key, text):
    res = ""
    for char in text:
        index = key.find(char)
        if index == -1:
            res += char
        else:
            res += ALPHABET[index]
    return res


enc_key = str(input("Enter encryption key:"))
enc_text = str(input("Enter encryption text:")).lower()
print(process_key(enc_key))
print(encrypt(enc_key, enc_text))
dec_key = str(input("Enter decryption key:"))
dec_text = str(input("Enter decryption text:")).lower()
print(decrypt(dec_key, dec_text))