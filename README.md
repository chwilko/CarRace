Gra napisana na zaliczenie przemiotu *Wstęp do programowania w języku* C.

Program napisany w języku C.
wykożystuje biblioteki:
gtk/gtk.h
math.h
string.h
stdlib.h
cairo.h

## Zawartość 
# car.c

Plik ze strukturą Car, pomocnicznymi strukturami RGB (kolor) oraz instruction (inforamcje co ma zrobić samochodzik)
i obsługującymi ją funkcjami.

Car *car_make(double x, double y, instruction *instr, RGB *color, char *name);
''' 
    Tworzy samochodzik o położeniu (x, y) sterowaqny instr w kolorze color i nazwie name.
'''
RGB *color_make(int R, int G, int B);
'''
    Zapamiętuje wartości R, G, B koloru. 
'''

instruction *instruction_make();
'''
    Tworzy zestaw instrukci do samochodzika.
'''

int tick(Car *car, int a[], double fric, int checkpoint, int n_checkpoints);
'''
    Funkcja odpowiednio poruszająca samochodzikiem.
    car -> samochodzik
    a[] -> tablica przycisków sterujących tym samochodzikiem
    fric -> współczynnik tarcia na jakim jest dany samochodzik
    checkpoint -> 0 jeżeli nie mija chechpointu, w przeciwnym wypadku nr checkpointu (1:n)
    n_checkpoints -> liczba checkpointów na tej mapie
'''

# game.c
Plik z funkcjami odpowiedzialnymi za grę
Najważniejsza funkcja:

void play_game(int argc, char *argv[]);
'''
    Wszystko jest póżniej sczytywane z przygotowanych wcześniej plikow.
'''

# main.c
Plik tworzący okienko menu i ustawiający parametry gry do plików

# instruction.c
Plik z okienkiem instrukcji.

# setup
colors.txt
    Dostępne kolory samochodzików
controls.txt
    Kolejne wartości charów odpowiedzialnych za poruszanie się samochodzików
current_game_info.txt
    Parametry bierzacej gry, nazwa mapy, liczba graczy i nazwy oraz kolory samochodzików.
Game_Icon.png
    Ikonka gry na pasku wybierania
instruction.txt
    Instrukcja wypisywana w okienku instrukcji
maps
    Pilik z mapami opisanymi wedłkóg wzorca, patrz maps
tmp_maps.m
    Plik pomagający tworzyć mapy.


## WYŚCIGI SAMOCHODOWE

Gra polega na wyścigach samochodowych.
Jest to gra przeznaczona dla od jednego do czterech graczy.

# STEROWANIE:
Steruje się za pomocą czterech przycisków.
Gracze pierwszy, drugi, trzeci i czwarty mają odpowiednio 
strzałki lub zestaw wsad, tgfh czy ikjl.
Sterowanie jest względem gracza, nie samochodu.
Po wciśnięciu przeciwstawnych przycisków
(góra - dół, prawo - lewo) samochód zwalnia.
Po zjechaniu z tasy samochód znacząco zwalnia.

# USTAWIENIA:
W menu jest możliwość dostosowania wyścigu.
Można wybrać mapę, liczbę graczy i liczbę okrążeń.
Dodadkowo, można nadać nazwy graczom
i wybrać kolory samochodów.

# CEL GRY:
Celem gry jest jak najszybsze wykonanie
określonej liczby okrążeń.
Okrążenie jest zaliczone po minięciu określonej
liczby checkpointów.
Pierwszy checkpoint w sktronę którego należy jechać
jest w kolorze niebieskim.
Checkpoint minięty kiedy nie
jest się na trasie może być nie zaliczony,
decyduje o tym bezstronny sędzia -- czyli ja.
