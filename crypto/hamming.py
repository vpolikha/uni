def hamming_set_correct(in_code):
    out_code = ""
    pos_x = 0
    num_x = 0
    used = 0

    parity_count = 0
    while 2 ** parity_count < len(in_code) + parity_count + 1:
        parity_count += 1

    target_len = len(in_code) + parity_count

    for i in range(target_len):
        if i == pos_x:
            out_code += "2"
            num_x += 1
            pos_x = 2 ** num_x - 1
        else:
            out_code += in_code[used]
            used += 1

    return out_code


def xor_str(string):
    if len(string) == 0:
        return -1
    if len(string) == 1:
        return string[0]
    res = int(string[0] != string[1])
    for i in range(2, len(string)):
        res = (res + int(string[i])) % 2
    return res

def get_relevant(string, i, size):
    res = ""
    work_string = string[i:]
    for i in range(0, len(work_string)):
        if (i // size) % 2 == 0:
            res += work_string[i]
    return res

def hamming_in(in_code):
    out_code = hamming_set_correct(in_code)
    x_num = 0
    for i in range(0, len(out_code)):
        if out_code[i] != "2":
            continue
        relevant = get_relevant(out_code, i, pow(2,x_num))[1:]
        out_code = out_code[:i] + str(xor_str(relevant)) + out_code[i+1:]
        x_num += 1
    return out_code

def hamming_out(out_code):
    res = ""
    work_string = out_code[:]
    num_x = 0
    for i in range(0, len(out_code)):
        if i == 2 ** num_x - 1:
            num_x += 1
        else:
            res += work_string[i]
    return res
    
print(hamming_in("0000"))
print(hamming_in("0001"))
print(hamming_in("0010"))
print(hamming_in("0011"))
print(hamming_in("0100"))
print(hamming_in("0101"))
print(hamming_in("0110"))
print(hamming_in("0111"))
print(hamming_in("1000"))
print(hamming_in("1001"))
print(hamming_in("1010"))
print(hamming_in("1011"))
print(hamming_in("1100"))
print(hamming_in("1101"))
print(hamming_in("1110"))
print(hamming_in("1111"))
print(hamming_in("11011"))
print(hamming_out((hamming_in("0000"))))
print(hamming_out((hamming_in("0001"))))
print(hamming_out((hamming_in("0010"))))
print(hamming_out((hamming_in("0011"))))
print(hamming_out((hamming_in("0100"))))
print(hamming_out((hamming_in("0101"))))
print(hamming_out((hamming_in("0110"))))
print(hamming_out((hamming_in("0111"))))
print(hamming_out((hamming_in("1000"))))
print(hamming_out((hamming_in("1001"))))
print(hamming_out((hamming_in("1010"))))
print(hamming_out((hamming_in("1011"))))
print(hamming_out((hamming_in("1100"))))
print(hamming_out((hamming_in("1101"))))
print(hamming_out((hamming_in("1110"))))
print(hamming_out((hamming_in("1111"))))
print(hamming_out((hamming_in("11011"))))