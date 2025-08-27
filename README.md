# CHIP‑8

[Pagina Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)

---

## 📦 MEMORY
- **4096 byte** di memoria indirizzabile (`0x000–0xFFF`)
- Ogni indirizzo = 1 byte
- **0x000–0x1FF (0–511):** riservati per interprete/variabili/fontset (80 byte per sprite cifre 0–F)
- **0x200 (512):** punto di inizio standard delle ROM CHIP‑8
- **0xEA0–0xEFF:** variabili di sistema/stack nei sistemi originali (non obbligatorio negli emulatori moderni)
- **0xF00–0xFFF:** framebuffer video nei sistemi originali (64×32 bit = 256 byte)
- Negli emulatori moderni il framebuffer è spesso in RAM separata

---

## 🗄️ REGISTRI
- **V0 … VF**: 16 registri generali da 8‑bit
  - `VF` usato come flag (carry/borrow, collisione sprite)
- **I**: registro indice (originariamente 12‑bit, oggi 16‑bit)
- **PC**: Program Counter, 16‑bit
- **SP**: Stack Pointer

---

## 📚 STACK
- Contiene indirizzi di ritorno delle subroutine
- Minimo 12 livelli (spesso 16 × 2 byte = 32 byte)
- Implementato come array di `uint16_t`

---

## ⏱️ TIMERS
- **Delay Timer (DT)**: decrementa a 60 Hz fino a 0
- **Sound Timer (ST)**: decrementa a 60 Hz fino a 0, se >0 emette beep
- Timer a 8‑bit, aggiornati indipendentemente dal ciclo CPU

---

## 🎮 INPUT
- 16 tasti esadecimali (0x0–0xF)
- Layout tipico:

    1 2 3 C  
    4 5 6 D  
    7 8 9 E  
    A 0 B F  

- 8/4/6/2 usati spesso come direzionali
- Opcode legati:
- `Ex9E`: salta se tasto in `Vx` premuto
- `ExA1`: salta se tasto in `Vx` non premuto
- `Fx0A`: attende pressione di un tasto e lo memorizza in `Vx`

---

## 🖥️ DISPLAY
- Risoluzione: **64 × 32** pixel (monocromatico)
- Ogni pixel = 1 bit
- Sprite: larghezza 8 pixel, altezza 1–15 righe
- Disegno in XOR → pixel accesi possono spegnersi (collisione)
- VF = 1 se almeno un pixel acceso è stato spento durante il draw
- **Super‑CHIP‑8**: supporta 128×64, sprite 16×16, scroll

---

## 🔊 SUONO
- Beep continuo finché `ST > 0`
- Frequenza e forma d’onda dipendono dall’hardware emulato

---

## 🆕 ESTENSIONI
- **Super‑CHIP‑8**:
- Schermo 128×64
- Sprite 16×16
- Scroll orizzontale/verticale
- **XO‑CHIP**: estensioni audio e grafiche

---

## 🛠️ EXTRA IMPLEMENTATIVI
- **Display fisico suggerito**: OLED 128×64 (SSD1306/SH1106)  
Mappatura: 1 pixel CHIP‑8 → blocco 2×2 pixel reali
- **MCU possibili**: STM32, ESP8266, AVR
- **SD card**: per caricare ROM senza collegare al PC
- **Clock CPU emulata**: tipico 500–1000 istruzioni/sec, sincronizzato a 60 Hz

---

## 📜 OPCODE CHIP‑8

| Opcode | Parametri | Descrizione | Note |
|--------|-----------|-------------|------|
| `0NNN` | NNN = addr | Chiamata a programma RCA 1802 a NNN | Obsoleto, ignorato negli emulatori moderni |
| `00E0` | — | Cancella lo schermo | — |
| `00EE` | — | Ritorna da subroutine | Pop da stack in PC |
| `1NNN` | NNN = addr | Salta a NNN | PC = NNN |
| `2NNN` | NNN = addr | Chiama subroutine a NNN | Push PC, PC = NNN |
| `3XNN` | X = reg, NN = byte | Salta prossima istruzione se `Vx == NN` | — |
| `4XNN` | X, NN | Salta prossima istruzione se `Vx != NN` | — |
| `5XY0` | X, Y | Salta prossima istruzione se `Vx == Vy` | Solo se nibble basso = 0 |
| `6XNN` | X, NN | `Vx = NN` | — |
| `7XNN` | X, NN | `Vx += NN` | No flag carry |
| `8XY0` | X, Y | `Vx = Vy` | — |
| `8XY1` | X, Y | `Vx = Vx OR Vy` | VF non modificato |
| `8XY2` | X, Y | `Vx = Vx AND Vy` | VF non modificato |
| `8XY3` | X, Y | `Vx = Vx XOR Vy` | VF non modificato |
| `8XY4` | X, Y | `Vx += Vy` | VF = carry |
| `8XY5` | X, Y | `Vx -= Vy` | VF = NOT borrow |
| `8XY6` | X | Shift destro `Vx >>= 1` | VF = bit0 originale |
| `8XY7` | X, Y | `Vx = Vy - Vx` | VF = NOT borrow |
| `8XYE` | X | Shift sinistro `Vx <<= 1` | VF = bit7 originale |
| `9XY0` | X, Y | Salta se `Vx != Vy` | Solo se nibble basso = 0 |
| `ANNN` | NNN | `I = NNN` | — |
| `BNNN` | NNN | Salta a `V0 + NNN` | — |
| `CXNN` | X, NN | `Vx = rand() & NN` | rand = 8‑bit casuale |
| `DXYN` | X, Y, N | Disegna sprite 8×N a `(Vx, Vy)` da memoria[I] | VF = collisione |
| `EX9E` | X | Salta se tasto `Vx` premuto | — |
| `EXA1` | X | Salta se tasto `Vx` NON premuto | — |
| `FX07` | X | `Vx = Delay Timer` | — |
| `FX0A` | X | Attende pressione tasto → `Vx` | Bloccante |
| `FX15` | X | `Delay Timer = Vx` | — |
| `FX18` | X | `Sound Timer = Vx` | — |
| `FX1E` | X | `I += Vx` | VF non modificato |
| `FX29` | X | `I = addr sprite cifra Vx` | Font 4×5 |
| `FX33` | X | BCD di `Vx` in memoria[I..I+2] | Centinaia, decine, unità |
| `FX55` | X | Salva V0..Vx in memoria[I..I+X] | I++ dopo ogni store (varia) |
| `FX65` | X | Carica V0..Vx da memoria[I..I+X] | I++ dopo ogni load (varia) |

---

## 📌 Note sulle varianti
- Alcuni opcode (`8XY6`, `8XYE`) in certe implementazioni usano `Vy` come sorgente invece di `Vx`.
- Le istruzioni `FX55` e `FX65` possono o meno incrementare `I` a seconda dell’interprete.
- Super‑CHIP‑8 aggiunge opcode per scroll e modalità grafiche estese.