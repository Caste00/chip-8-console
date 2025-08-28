import json

def load_instruction_set(filename):
    """ carica il set di istruzioni da un file json
    è più comodo se in futuro volessi aggiungere o modificare le funzioni """

    with open(filename, "r", encoding="utf-8") as f:
        data = json.load(f)
    return data

def parse_arg(arg):
    """ la funzione prende un argomento e ritorna il valore
    per esempio gli passo VF e ritorno 0xF oppure %1010 ritorna il valore 0b1010 """

    arg = arg.strip()
    if arg.startswith("V"):
        return int(arg[1:], 16)     # registro in hex
    if arg.startswith("0x"):
        return int(arg, 16)
    if arg.startswith("%"):
        return int(arg[1:], 2)
    return int(arg)

def assemble_line(line, instruction_set):
    """ converte una riga assembly in opcode """
    parts = line.strip().split()
    if not parts:
        return None
    mnemonic = parts[0].upper()
    if mnemonic not in instruction_set:
        raise ValueError(f"Istruzione sconosciuta {mnemonic}")
    meta = instruction_set[mnemonic]

    args = []
    if meta["args"] > 0:
        args_str = " ".join(parts[1:])
        args = [parse_arg(i) for i in args_str.split(",")]

    # costruisco l'opcode
    opcode = meta["code"]
    match meta["type"]:
        case "no_arg":
            return opcode
        case "nnn": 
            return opcode | args[0]
        case "x":
            return opcode | (args[0] << 8)
        case "xnn":
            return opcode | (args[0] << 8) | args[1]
        case "xy":
            return opcode | (args[0] << 8) | (args[1] << 4)
        case "xyn": 
            return opcode | (args[0] << 8) | (args[1] << 4) | args[2]
        case _:
            raise ValueError(f"Istruzione non ricostruita {mnemonic}")


def assemble_file(namefile, path_instruction_set):
    instruction_set = load_instruction_set(path_instruction_set)
    
    with open(namefile, "r") as f:
        opcodes = []
        for raw_line in f:
            line = raw_line.split(";")[0].strip()
            if not line:
                continue
            opcode = assemble_line(line, instruction_set)
            if opcode is not None:
                opcodes.append(opcode)
        return opcodes


def save_bin(namefile, opcodes):
    with open(namefile + ".ch8", "wb") as f:
        for op in opcodes:
            f.write(op.to_bytes(2, byteorder="big"))


if __name__ == "__main__":
    ops = assemble_file("../programma.txt", "instruction_set.json")
    print("Operazione terminata correttamente")
    save_bin("../prova", ops)