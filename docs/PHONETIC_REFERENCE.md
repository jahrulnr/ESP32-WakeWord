# Phonetic Conversion Quick Reference

## Vowel Sounds

| Sound | Phonetic | Examples |
|-------|----------|----------|
| A (cat) | c | Apple → cPcL, Cat → KcT |
| A (make) | e | Make → MeK, Take → TeK |
| E (bed) | e | Red → ReD, Get → GeT |
| E (see) | i | See → Si, Be → Bi |
| I (bit) | g | In → gN, It → gT |
| I (bite) | i | Light → LiT, Right → RiT |
| O (hot) | n | Not → NnT, Got → GnT |
| O (go) | b | Go → Gb, No → Nb |
| U (but) | c | Up → kP, Cut → KcT |
| U (use) | k | Use → YkS, Music → MYkSgK |

## Consonant Sounds

| Letter | Phonetic | Examples |
|--------|----------|----------|
| B | B | Book → BkK, Big → BgG |
| C (hard) | K | Cat → KcT, Come → KcM |
| C (soft) | S | Nice → NiS, City → SgTi |
| D | D | Dog → DnG, Day → De |
| F | F | Fish → FgS, Off → eF |
| G (hard) | G | Go → Gb, Big → BgG |
| G (soft) | j | Page → Pej, Large → LnRj |
| H | H | House → HdS, Help → HeLP |
| J | j | Jump → jcMP, Just → jcST |
| K | K | Key → Ki, Make → MeK |
| L | L | Light → LiT, Love → LcV |
| M | M | Music → MYkSgK, Home → HbM |
| N | N | No → Nb, Turn → TkN |
| P | P | Put → PkT, Stop → STnP |
| Q | KW | Quick → KWgK, Question → KWeSpcN |
| R | R | Red → ReD, Start → STnRT |
| S | S | Stop → STnP, Yes → YeS |
| T | T | Turn → TkN, Water → WnTcR |
| V | V | Very → VeRi, Have → HcV |
| W | W | With → WgT, Water → WnTcR |
| X | KS | Box → BnKS, Next → NeKST |
| Y | Y | Yes → YeS, You → Yk |
| Z | S | Zero → SiRb, Size → SiS |

## Common Word Endings

| Ending | Phonetic | Examples |
|--------|----------|----------|
| -ING | gN | Starting → STnRTgN, Ring → RgN |
| -ED | eD / D / T | Started → STnRTeD, Turned → TkND |
| -ER | cR | Water → WnTcR, Better → BeTcR |
| -LY | Li | Really → RiLi, Only → bNLi |
| -TION | SYN | Question → KWeSpcN, Action → cKSYN |

## Common Contractions

| Word | Phonetic | Full Form |
|------|----------|-----------|
| Don't | DbNT | Do Not |
| Can't | KcNT | Cannot |
| Won't | WbNT | Will Not |
| I'll | iL | I Will |
| You're | YkR | You Are |
| It's | gTS | It Is |

## Frequently Used Commands

### Lighting
```cpp
{0, "Turn on the light", "TkN nN jc LiT"},
{0, "Lights on", "LiTS nN"},
{1, "Turn off the light", "TkN eF jc LiT"},
{1, "Lights off", "LiTS eF"},
{1, "Go dark", "Gb DnRK"},
```

### Electronics
```cpp
{2, "Start fan", "STnRT FaN"},
{3, "Stop fan", "STnP FaN"},
{4, "Turn on TV", "TkN nN TiVi"},
{5, "Turn off TV", "TkN eF TiVi"},
```

### Music/Audio
```cpp
{6, "Play music", "PLe MYkSgK"},
{7, "Stop music", "STnP MYkSgK"},
{8, "Volume up", "VnLYkM kP"},
{9, "Volume down", "VnLYkM DdN"},
```

### Smart Home
```cpp
{10, "Open garage", "bPcN GcRnj"},
{11, "Close garage", "KLbS GcRnj"},
{12, "Lock door", "LnK DbR"},
{13, "Unlock door", "kNLnK DbR"},
```

## Tips for Better Recognition

1. **Use Simple Words**: "Start" instead of "Initialize"
2. **Avoid Similar Sounds**: Don't use "Turn on" and "Turn off" if they sound too similar
3. **Test Pronunciation**: Speak naturally, not robotically
4. **Add Alternatives**: Multiple phonetic representations for the same command
5. **Context Matters**: "Light" vs "Right" - add distinguishing words

## Testing Your Phonetics

1. Build and upload your code
2. Open serial monitor
3. Speak your commands clearly
4. Check if they're recognized correctly
5. Adjust phonetics if needed and repeat

Remember: It may take several iterations to get the phonetics right for your specific voice and accent!
