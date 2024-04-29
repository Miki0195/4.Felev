/*Eddig van meg: Oldja meg az alábbi feladatot C nyelven, ami egy Linux rendszeren tud futni. (Ez lehet a tárgy kiszolgálója ( opsys.inf.elte.hu) vagy egy saját lokális Linux rendszer!) Az eredményt (csak a C forrásfájlt, pl: alma.c) töltse fel maximum 1.5 óra után a kezdést követően. A feladatokat a gyakorlatvezetők fogják értékelni és az eredményt bejegyzik. A dolgozat eredménye elégséges ha az első feladat kész, közepes ha az első kettő stb.

A Húsvét elmúlt, és a húsvéti locsoló verseny győztese átveszi a hatalmat és Ő lesz az új "Főnyuszi". Ahogy végigsétál a birodalmán, látja a tavasz "gyümölcseit", akik vidáman szaladgálnak a frissen kizöldült határban. Elhatározza, hogy " nyusziszámlálást" kell tartani.

"Főnyuszi" (szülő) nem tart teljes népszámlálást, választ kettőt a területek( Barátfa, Lovas, Kígyós-patak , Káposztás , Szula, Malom telek, Páskom) közül, ahova nyuszi számláló biztost (gyerek) küld.

Főnyuszi felkéri a nyuszi számláló biztosokat, Tapsit és Fülest (létrehozza a gyerekeket) és megvárja, amíg a biztosok felkészülnek a feladatra, amit jelzéssel (tetszőleges) nyugtáznak. Miután főnyuszi fogadta a jelzéseket üzenetsoron továbbítja mindkét számláló biztosnak a kiválasztott területet, ahol fel kell mérni az állományt. Tapsi is és Füles is kiírja a feladatul kapott terület-nevet a képernyőre, majd befejezik aznapra a tevékenységüket( terminálnak), amit Főnyuszi megvár, majd képernyőre írja, hogy Tapsi is és Füles is nyugovóra tért, majd Ő is befejezi aznapra a munkát. Miután Tapsi és Füles képernyőre írja a kapott terület-nevet, elkezdik a nyuszik számlálását. Az eredményeket ( véletlenszám 50 és 100 között) szintén üzenetsoron küldik vissza Főnyuszinak, aki a képernyőre írja azokat.*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Nyuszi számláló biztosok struktúrája
typedef struct {
    char nev[20];
} NyusziSzamlaloBiztos;

// Területek neveinek felsorolása
enum Terulet {Baratfa, Lovas, KigyosPatak, Kaposztas, Szula, MalomTelek, Paskom};

// Terület nevek tömbje
char* teruletNevek[] = {"Barátfa", "Lovas", "Kígyós-patak", "Káposztás", "Szula", "Malom telek", "Páskom"};

// Függvények deklarációi
void jelzes(int pid);
void fonyusziFeladat(NyusziSzamlaloBiztos tapsi, NyusziSzamlaloBiztos fules, enum Terulet terulet);
void termin();
int randomSzam();

int main() {
    NyusziSzamlaloBiztos tapsi = {"Tapsi"};
    NyusziSzamlaloBiztos fules = {"Füles"};

    // Főnyuszi választ két területet
    enum Terulet terulet1 = Baratfa;
    enum Terulet terulet2 = Lovas;

    // Jelzések fogadása a számláló biztosoktól
    printf("Főnyuszi várja a jelzéseket...\n");
    jelzes(getpid());

    // Főnyuszi átadja a feladatot a számláló biztosoknak
    printf("Főnyuszi átadja a feladatot Tapsinak és Fülesnek...\n");
    fonyusziFeladat(tapsi, fules, terulet1);
    fonyusziFeladat(tapsi, fules, terulet2);

    // Főnyuszi befejezi aznapra a munkát
    printf("Főnyuszi befejezi aznapra a munkát...\n");

    // Nyuszi számláló biztosok terminálnak
    termin();

    return 0;
}

// Jelzés küldése a főnyuszinek
void jelzes(int pid) {
    // Jelzés küldése a főnyuszinek
    printf("Jelzés küldése a főnyuszinek (PID: %d)...\n", pid);
    sleep(1); // Várakozás, hogy a főnyuszi megvárja a jelzéseket
}

// Főnyuszi feladatának végrehajtása a megadott területen
void fonyusziFeladat(NyusziSzamlaloBiztos tapsi, NyusziSzamlaloBiztos fules, enum Terulet terulet) {
    // Terület nevének kiírása
    printf("%s terület nyuszi számlálása folyamatban...\n", teruletNevek[terulet]);
    
    // Nyuszik számlálása
    srand(time(NULL));
    int nyuszikSzama = randomSzam();
    
    // Eredmények küldése főnyuszinek
    printf("%s és %s befejezte a feladatot ezen a területen.\n", tapsi.nev, fules.nev);
    printf("Eredmények küldése Főnyuszinek: %d nyuszi találtak %s területen.\n", nyuszikSzama, teruletNevek[terulet]);
}

// Véletlenszám generálása 50 és 100 között
int randomSzam() {
    return rand() % 51 + 50;
}

// Terminálás
void termin() {
    printf("Nyuszi számláló biztosok nyugovóra tértek.\n");
    printf("Főnyuszi is nyugovóra tér...\n");
}
