#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "conio2.h"

using namespace std;

#define SIZE_PIONKI 24
#define SMALL_TAB 2

struct daneRozgrywka {
    int pionkiTab[SIZE_PIONKI];
    int kostkaTab[SMALL_TAB];
    int barTab[SMALL_TAB];
    int bicieTab[SMALL_TAB];
    int zdjeteTab[SMALL_TAB];
    int punktyTab[SMALL_TAB];
    int dublet;
};

void gameplay(daneRozgrywka* daneRoz, int g);
void poczatek_rozgrywki(daneRozgrywka* daneRoz);
void opcje_ruchu_gr1(daneRozgrywka* daneRoz);
void opcje_ruchu_gr2(daneRozgrywka* daneRoz);
void pszypisaneWartosciStruct(daneRozgrywka* daneRoz);
void rys_zdejmownie(daneRozgrywka* daneRoz);
void rys_pionki_gora(daneRozgrywka* daneRoz);
void rys_pionki_dol(daneRozgrywka* daneRoz);
void menu_glowne();
void menu_operacja(daneRozgrywka* daneRoz);

int war_bez(int a) {
    if (a >= 0) {
        return a;
    }
    else {
        return a * (-1);
    }
}

void czysz_ekranu() {
    clrscr();
    gotoxy(1, 1);
}

void rzutKostkami(int* k) {

    int liczba;
    for (int i = 0; i < 2; i++) {
        liczba = (rand() % 6) + 1;
        k[i] = liczba;
    }
}

void fDublet(daneRozgrywka* daneRoz, int player) {

    cout << "DUBLET" << endl;
    cout << "Kostka:1 " << (*daneRoz).kostkaTab[0] << endl;
    cout << "Kostka:2 " << (*daneRoz).kostkaTab[1] << "\n\n";

    if (player) {
        opcje_ruchu_gr2(daneRoz);
    }
    else {
        opcje_ruchu_gr1(daneRoz);
    }

}

void operacje_po_przesunieciu(daneRozgrywka* daneRoz, int curr, int next, int nr_k) {

    if ((*daneRoz).dublet <= 0) {

        (*daneRoz).kostkaTab[nr_k] = 0;

    }
    else {
        (*daneRoz).dublet--;

        if (((*daneRoz).dublet) == 0) {
            (*daneRoz).kostkaTab[0] = 0;
            (*daneRoz).kostkaTab[1] = 0;
            (*daneRoz).dublet == 0;
        }
    }

    if ((*daneRoz).kostkaTab[0] == 0 && (*daneRoz).kostkaTab[1] == 0) {
        gameplay(daneRoz, next);
    }
    else {
        gameplay(daneRoz, curr);
    }

}

int wybierzRuch(int j) {
    char r = 0;
    cin >> r;

    r -= 49;

    if (r > j || r < 0) {
        return wybierzRuch(j);
    }
    else {
        return r;
    }
}

//funkjca przesuwajaca pionek gracza
void przesun_pionek(daneRozgrywka* daneRoz, int prze_kos[][2], int curr_player, int next_player, int  nr_k, int j) {
    int r;
    int poz_pocz, poz_kon;


    cout << "\nWybierz ruch: \n";
    r = wybierzRuch(j);

    poz_pocz = prze_kos[r][0];
    poz_kon = prze_kos[r][1];

    if (curr_player) {
        (*daneRoz).pionkiTab[poz_pocz]--;
        (*daneRoz).pionkiTab[poz_kon]++;
    }
    else {
        (*daneRoz).pionkiTab[poz_pocz]++;
        (*daneRoz).pionkiTab[poz_kon]--;
    }
    operacje_po_przesunieciu(daneRoz, curr_player, next_player, nr_k);
}

void ruchBicie(daneRozgrywka* daneRoz, int g, int* k) {
    int poz_pocz = (*daneRoz).bicieTab[0], poz_kon = (*daneRoz).bicieTab[1];

    if (g) {
        (*daneRoz).pionkiTab[poz_pocz]--;
        (*daneRoz).pionkiTab[poz_kon] += 2;
        (*daneRoz).barTab[0]++;
        if (poz_kon - poz_pocz == (*daneRoz).kostkaTab[1]) {
            *k = 1;
        }
    }
    else {
        (*daneRoz).pionkiTab[poz_pocz]++;
        (*daneRoz).pionkiTab[poz_kon] -= 2;
        (*daneRoz).barTab[1]++;
        if (poz_pocz - poz_kon == (*daneRoz).kostkaTab[1]) {
            *k = 1;
        }
    }
}

void fBicie(daneRozgrywka* daneRoz, int curr) {

    int k = 0, next = 0;
    (next == curr) ? next = 1 : next = 0;
    ruchBicie(daneRoz, curr, &k);

    cout << "Kostka 1: " << (*daneRoz).kostkaTab[0] << "\nKostka 2: " << (*daneRoz).kostkaTab[1] << "\n\n";
    cout << "Przymusowe bicie pionka z pozycji: " << (*daneRoz).bicieTab[0] + 1 << ", na pozycje: " << (*daneRoz).bicieTab[1] + 1 << endl;

    cout << "Wcisnij jaki kolwiek klawisz by wykonac bicie." << endl;
    getch();
    operacje_po_przesunieciu(daneRoz, curr, next, k);
}

void mniejszaKostka(daneRozgrywka* daneRoz, int* kos1, int* kos2) {
    if ((*daneRoz).kostkaTab[0] <= (*daneRoz).kostkaTab[1]) {
        *kos1 = (*daneRoz).kostkaTab[0];
        *kos2 = (*daneRoz).kostkaTab[1];
    }
    else {
        *kos2 = (*daneRoz).kostkaTab[0];
        *kos1 = (*daneRoz).kostkaTab[1];
    }
}

int czy_moze_bic_gr1(daneRozgrywka* daneRoz) {

    int i, kos1, kos2;

    mniejszaKostka(daneRoz, &kos1, &kos2);

    for (i = 23; i >= 0; i--) {
        if ((*daneRoz).pionkiTab[i] < 0) {
            if ((i - kos1 >= 0 && (*daneRoz).pionkiTab[i - kos1] == 1)) {
                (*daneRoz).bicieTab[0] = i;
                (*daneRoz).bicieTab[1] = i - kos1;
                return 1;
            }
            if ((i - kos2 >= 0 && (*daneRoz).pionkiTab[i - kos2] == 1)) {
                (*daneRoz).bicieTab[0] = i;
                (*daneRoz).bicieTab[1] = i - kos2;
                return 1;
            }
        }
    }

    return 0;
}

int czy_moze_bic_gr2(daneRozgrywka* daneRoz) {

    int i, kos1, kos2;

    mniejszaKostka(daneRoz, &kos1, &kos2);

    for (i = 0; i <= 23; i++) {
        if ((*daneRoz).pionkiTab[i] > 0) {
            if ((i + kos1 <= 23 && (*daneRoz).pionkiTab[i + kos1] == -1)) {
                (*daneRoz).bicieTab[0] = i;
                (*daneRoz).bicieTab[1] = i + kos1;
                return 1;
            }
            if ((i + kos2 <= 23 && (*daneRoz).pionkiTab[i + kos2] == -1)) {
                (*daneRoz).bicieTab[0] = i;
                (*daneRoz).bicieTab[1] = i + kos2;
                return 1;
            }
        }
    }

    return 0;
}

void brakRuchu(daneRozgrywka* daneRoz, int curr, int next, int nr_k) {
    if ((*daneRoz).dublet > 0) {
        (*daneRoz).dublet = 1;
    }
    cout << "Kostka:1 " << (*daneRoz).kostkaTab[0] << endl;
    cout << "Kostka:2 " << (*daneRoz).kostkaTab[1] << "\n\n";
    cout << "Brak mozliwosci ruchu kostka\n";
    getch();
    operacje_po_przesunieciu(daneRoz, curr, next, nr_k);
}

void mozliwoscRuchu(daneRozgrywka* daneRoz, int prze_kos[][2], int curr, int next, int nr_k, int j) {
    if (j == 0) {
        brakRuchu(daneRoz, curr, next, nr_k);
    }
    else {
        przesun_pionek(daneRoz, prze_kos, curr, next, nr_k, j);
    }
}

int czy_pionki_w_bazie_gr1(daneRozgrywka* daneRoz) {
    for (int i = 6; i < 24; i++) {
        if ((*daneRoz).pionkiTab[i] < 0) {
            return 0;
        }
    }
    return 1;
}

int czy_pionki_w_bazie_gr2(daneRozgrywka* daneRoz) {
    for (int i = 0; i < 18; i++) {
        if ((*daneRoz).pionkiTab[i] > 0) {
            return 0;
        }
    }
    return 1;
}

void nowaRozgrywka(daneRozgrywka* daneRoz, int g, int p) {

    textcolor(14);
    rys_zdejmownie(daneRoz);
    gotoxy(92, 7);
    cout << "Gracz" << g + 1 << " otrzymal: " << p << " point";
    textcolor(15);
    getch();
    czysz_ekranu();
    pszypisaneWartosciStruct(daneRoz);
    poczatek_rozgrywki(daneRoz);
}

void punktacja(daneRozgrywka* daneRoz) {
    int p = 1;
    if ((*daneRoz).zdjeteTab[0] == 15) {

        if ((*daneRoz).zdjeteTab[1] == 0) {
            p = 2;
        }
        if ((*daneRoz).barTab[1] > 0) {
            p = 3;
        }
        (*daneRoz).punktyTab[0] += p;
        nowaRozgrywka(daneRoz, 0, p);
    }
    else if ((*daneRoz).zdjeteTab[1] == 15) {
        if ((*daneRoz).zdjeteTab[0] == 0) {
            p = 2;
        }
        if ((*daneRoz).barTab[0] > 0) {
            p = 3;
        }
        (*daneRoz).punktyTab[1] += p;
        nowaRozgrywka(daneRoz, 1, p);
    }
}

int czy_mozna_wychodzic_gr1(daneRozgrywka* daneRoz, int* k) {
    int kos1 = (*daneRoz).kostkaTab[0], kos2 = (*daneRoz).kostkaTab[1];

    for (int i = 5; i >= 0; i--) {
        if ((*daneRoz).pionkiTab[i] < 0) {
            if (kos1 - i >= 1) {
                (*daneRoz).pionkiTab[i]++;
                (*daneRoz).zdjeteTab[0]++;
                punktacja(daneRoz);
                *k = 0;
                cout << "Pozbycie sie pionka z pozyji: " << i + 1;
                getch();
                return 1;
            }
            else if (kos2 - i >= 1) {
                (*daneRoz).pionkiTab[i]++;
                (*daneRoz).zdjeteTab[0]++;
                punktacja(daneRoz);
                *k = 1;
                cout << "Pozbycie sie pionka z pozyji: " << i + 1;
                getch();
                return 1;
            }
        }
    }
    return 0;
}

int czy_mozna_wychodzic_gr2(daneRozgrywka* daneRoz, int* k) {
    int kos1 = (*daneRoz).kostkaTab[0], kos2 = (*daneRoz).kostkaTab[1];

    for (int i = 18; i <= 23; i++) {
        if ((*daneRoz).pionkiTab[i] > 0) {
            if (kos1 + i >= 24) {
                (*daneRoz).pionkiTab[i]--;
                (*daneRoz).zdjeteTab[1]++;
                punktacja(daneRoz);
                *k = 0;
                cout << "Pozbycie sie pionka z pozyji: " << i + 1;
                getch();
                return 1;
            }
            else if (kos2 + i >= 24) {
                (*daneRoz).pionkiTab[i]--;
                (*daneRoz).zdjeteTab[1]++;
                punktacja(daneRoz);
                *k = 1;
                cout << "Pozbycie sie pionka z pozyji: " << i + 1;
                getch();
                return 1;
            }
        }
    }
    return 0;
}

//wybor kostki do ruchu
int wyborKostki() {
    char wyb_kos;
    cin >> wyb_kos;

    if (wyb_kos == '1') {
        return 0;
    }
    else if (wyb_kos == '2') {
        return 1;
    }
    else {
        return wyborKostki();
    }
}

void jakaKostka(daneRozgrywka* daneRoz, int* kos, int* nr_k) {
    if ((*daneRoz).kostkaTab[0] != 0 && (*daneRoz).kostkaTab[1] != 0) {
        cout << "Wybierz kostke, ktora chcesz ruszyc: \n";
        *nr_k = wyborKostki();
        if (*nr_k) {
            cout << "Kostka2:\n";
        }
        else {
            cout << "Kostka1:\n";
        }
    }
    else if ((*daneRoz).kostkaTab[0] != 0) {
        cout << "Kostka1: \n";
        *nr_k = 0;
    }
    else {
        cout << "Kostka2: \n";
        *nr_k = 1;
    }

    *kos = (*daneRoz).kostkaTab[*nr_k];

}

void wychodzeniePlansza_gr1(daneRozgrywka* daneRoz) {
    int k;
    if (czy_pionki_w_bazie_gr1(daneRoz) && czy_mozna_wychodzic_gr1(daneRoz, &k)) {
        cout << "Kostka1: " << (*daneRoz).kostkaTab[0] << ", Kostka2: " << (*daneRoz).kostkaTab[1] << endl;
        operacje_po_przesunieciu(daneRoz, 0, 1, k);
    }
}

void wychodzeniePlansza_gr2(daneRozgrywka* daneRoz) {
    int k;
    if (czy_pionki_w_bazie_gr2(daneRoz) && czy_mozna_wychodzic_gr2(daneRoz, &k)) {
        cout << "Kostka1: " << (*daneRoz).kostkaTab[0] << ", Kostka2: " << (*daneRoz).kostkaTab[1] << endl;
        operacje_po_przesunieciu(daneRoz, 1, 0, k);
    }
}

//wypisanie mozliwych ruchów dla X
void opcje_ruchu_gr1(daneRozgrywka* daneRoz) {

    int j = 0, prze_kos[15][2], kos = 0, nr_k;

    wychodzeniePlansza_gr1(daneRoz);

    if ((*daneRoz).dublet > 0) {
        nr_k = 0;
        kos = (*daneRoz).kostkaTab[0];
    }
    else {
        jakaKostka(daneRoz, &kos, &nr_k);
    }

    for (int i = 23; i >= 0; i--) {
        if ((*daneRoz).pionkiTab[i] < 0) {
            if (i - kos >= 0 && (*daneRoz).pionkiTab[i - kos] <= 0) {
                cout << "Ruch (" << j + 1 << "): z pola " << i + 1 << " na pole " << i - kos + 1 << endl;
                prze_kos[j][0] = i;
                prze_kos[j][1] = i - kos;
                j++;
            }
        }
    }

    mozliwoscRuchu(daneRoz, prze_kos, 0, 1, nr_k, j);
}

//wypisanie mozliwych ruchów dla O
void opcje_ruchu_gr2(daneRozgrywka* daneRoz) {

    int j = 0, prze_kos[15][2], kos = 0, nr_k;

    wychodzeniePlansza_gr2(daneRoz);

    if ((*daneRoz).dublet > 0) {
        nr_k = 0;
        kos = (*daneRoz).kostkaTab[0];
    }
    else {
        jakaKostka(daneRoz, &kos, &nr_k);
    }

    for (int i = 0; i <= 23; i++) {
        if ((*daneRoz).pionkiTab[i] > 0) {
            if (i + kos <= 23 && (*daneRoz).pionkiTab[i + kos] >= 0) {
                cout << "Ruch (" << j + 1 << "): z pola " << i + 1 << " na pole " << i + kos + 1 << endl;
                prze_kos[j][0] = i;
                prze_kos[j][1] = i + kos;
                j++;
            }
        }
    }

    mozliwoscRuchu(daneRoz, prze_kos, 1, 0, nr_k, j);
}

int pierwszenstwo_bicia(daneRozgrywka* daneRoz, int player) {
    int bicie = 0;

    if (player) {
        bicie = czy_moze_bic_gr2(daneRoz);
    }
    else {
        bicie = czy_moze_bic_gr1(daneRoz);
    }
    return bicie;
}

void czy_wyjscie_z_bandy_z_biciem(daneRozgrywka* daneRoz, int curr_player, int  poz, int bicie) {
    if (bicie) {
        if (curr_player) {
            (*daneRoz).pionkiTab[poz] += 2;
            (*daneRoz).barTab[0]++;
        }
        else {
            (*daneRoz).pionkiTab[poz] -= 2;
            (*daneRoz).barTab[1]++;
        }
        cout << "Wymuszone wyjscie z bandy z biciem" << endl;
    }
    else {
        if (curr_player) {
            (*daneRoz).pionkiTab[poz]++;
        }
        else {
            (*daneRoz).pionkiTab[poz]--;
        }
        cout << "Wymuszone wyjscie z bandy" << endl;
    }
}

void wyjscie_z_bandy(daneRozgrywka* daneRoz, int curr_player, int next_player, int  poz, int nr_k, int bicie) {

    (*daneRoz).barTab[curr_player]--;

    cout << "Kostka 1: " << (*daneRoz).kostkaTab[0] << endl;
    cout << "Kostka 2: " << (*daneRoz).kostkaTab[1] << "\n\n";

    czy_wyjscie_z_bandy_z_biciem(daneRoz, curr_player, poz, bicie);

    getch();
    operacje_po_przesunieciu(daneRoz, curr_player, next_player, nr_k);
}

void wyjsciebandaGracz2(daneRozgrywka* daneRoz, int g, int g2, int kos1, int kos2, int k) {
    if ((*daneRoz).pionkiTab[kos1 - 1] >= 0) {
        wyjscie_z_bandy(daneRoz, g, g2, kos1 - 1, k, 0);
    }
    else if ((*daneRoz).pionkiTab[kos1 - 1] == -1) {
        wyjscie_z_bandy(daneRoz, g, g2, kos1 - 1, k, 1);
    }
    else if ((*daneRoz).pionkiTab[kos2 - 1] >= 0) {
        wyjscie_z_bandy(daneRoz, g, g2, kos2 - 1, k, 0);
    }
    else if ((*daneRoz).pionkiTab[kos2 - 1] == -1) {
        wyjscie_z_bandy(daneRoz, g, g2, kos2 - 1, k, 1);
    }
    else {
        brakRuchu(daneRoz, g, g2, k);
    }
}

void wyjsciebandaGracz1(daneRozgrywka* daneRoz, int g, int g2, int kos1, int kos2, int k) {
    if ((*daneRoz).pionkiTab[24 - kos1] <= 0) {
        wyjscie_z_bandy(daneRoz, g, g2, 24 - kos1, k, 0);
    }
    else if ((*daneRoz).pionkiTab[24 - kos1] == 1) {
        wyjscie_z_bandy(daneRoz, g, g2, 24 - kos1, k, 1);
    }
    else if ((*daneRoz).pionkiTab[24 - kos2] <= 0) {
        wyjscie_z_bandy(daneRoz, g, g2, 24 - kos2, k, 0);
    }
    else if ((*daneRoz).pionkiTab[24 - kos2] == 1) {
        wyjscie_z_bandy(daneRoz, g, g2, 24 - kos2, k, 1);
    }
    else {
        brakRuchu(daneRoz, g, g2, k);
    }
}

void pierwszenstwo_banda(daneRozgrywka* daneRoz, int g) {
    int kos1 = (*daneRoz).kostkaTab[0], kos2, k = kos1, g2 = 0;// k- numer kostki1
    if (g == g2) {
        g2 = 1;
    }
    mniejszaKostka(daneRoz, &kos1, &kos2);

    (k == kos1) ? k = 0 : k = 1;

    if ((*daneRoz).kostkaTab[0] == 0) {
        k = 1;
        kos1 = (*daneRoz).kostkaTab[1];
    }

    if ((*daneRoz).kostkaTab[1] == 0) {
        k = 0;
        kos1 = (*daneRoz).kostkaTab[0];
    }

    if (g) {
        wyjsciebandaGracz2(daneRoz, g, g2, kos1, kos2, k);
    }
    else {
        wyjsciebandaGracz1(daneRoz, g, g2, kos1, kos2, k);
    }

}

void zwykly_ruch(daneRozgrywka* daneRoz, int player) {

    int kos[2];
    kos[0] = (*daneRoz).kostkaTab[0];
    kos[1] = (*daneRoz).kostkaTab[1];

    if (player) {

        opcje_ruchu_gr2(daneRoz);
    }
    else {

        opcje_ruchu_gr1(daneRoz);
    }
}



void wypisz_kostki(daneRozgrywka* daneRoz, int player) {

    cout << "Kostka 1: " << (*daneRoz).kostkaTab[0] << endl;
    cout << "Kostka 2: " << (*daneRoz).kostkaTab[1] << "\n\n";

    zwykly_ruch(daneRoz, player);
}

void rys_numery() {
    int k = 13, m = 12;
    for (int i = 6; i <= 74; i += 5) {
        gotoxy(i, 1);
        cout << k;
        gotoxy(i, 21);
        cout << m;
        if (i == 31) {
            i += 13;
        }
        k++;
        m--;
    }
}

void rys_bar(daneRozgrywka* daneRoz) {
    gotoxy(39, 4);
    cout << "X:" << (*daneRoz).barTab[0];
    gotoxy(40, 6);
    cout << "B";
    gotoxy(40, 7);
    cout << "A";
    gotoxy(40, 8);
    cout << "R";
    gotoxy(39, 15);
    cout << "O:" << (*daneRoz).barTab[1];
    gotoxy(40, 17);
    cout << "B";
    gotoxy(40, 18);
    cout << "A";
    gotoxy(40, 19);
    cout << "R";
}

void rys_slupki() {
    for (int i = 3; i <= 19; i++) {
        if (i == 9) {
            i += 5;
        }
        for (int j = 6; j <= 74; j += 5) {
            gotoxy(j, i);
            cout << "|";
            if (j == 36) {
                j += 3;
            }
        }
    }
}

void rys_ramka() {
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 79; j++) {
            if (j == 0 || j == 78) {
                cout << "|";
            }
            else if (i == 1 || i == 10 || i == 19) {
                cout << "-";
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

void rys_zdejmownie(daneRozgrywka* daneRoz) {
    gotoxy(81, 6);
    cout << "O:" << (*daneRoz).zdjeteTab[1] << "/15";
    gotoxy(81, 15);
    cout << "X:" << (*daneRoz).zdjeteTab[0] << "/15";
}

void rys_punkty(daneRozgrywka* daneRoz) {
    gotoxy(95, 2);
    cout << "Punkty O: = " << (*daneRoz).punktyTab[1];
    gotoxy(95, 4);
    cout << "Punkty X: = " << (*daneRoz).punktyTab[0];
}

void rys_pionki_dol(daneRozgrywka* daneRoz) {
    int k = 0, m = 0;
    for (int i = 19; i >= 15; i--) {
        for (int j = 74; j >= 6; j -= 5) {
            if (j == 44) {
                j -= 13;
            }
            gotoxy(j, i);
            if ((*daneRoz).pionkiTab[k] > 0 && (*daneRoz).pionkiTab[k] > m) {
                cout << "O";
            }
            else if ((*daneRoz).pionkiTab[k]<0 && war_bez((*daneRoz).pionkiTab[k])>m) {
                cout << "X";
            }
            k++;
        }
        m++;
        k = 0;
    }
}

void rys_pionki_gora(daneRozgrywka* daneRoz) {
    int k = 12, m = 0;
    for (int i = 3; i <= 7; i++) {
        for (int j = 6; j <= 74; j += 5) {
            gotoxy(j, i);
            if (j == 31) {
                j += 13;
            }
            if ((*daneRoz).pionkiTab[k] > 0 && (*daneRoz).pionkiTab[k] > m) {
                cout << "O";
            }
            else if ((*daneRoz).pionkiTab[k]<0 && war_bez((*daneRoz).pionkiTab[k])>m) {
                cout << "X";
            }
            k++;
        }
        m++;
        k = 12;
    }
}

//Funkcja tworzaca plansze do gry
void plansza(daneRozgrywka* daneRoz) {

    textcolor(9);
    rys_ramka();
    rys_slupki();
    textcolor(14);
    rys_pionki_gora(daneRoz);
    rys_pionki_dol(daneRoz);
    rys_punkty(daneRoz);
    rys_numery();
    rys_zdejmownie(daneRoz);
    rys_bar(daneRoz);
    textcolor(15);
    gotoxy(1, 23);
}

void zapis_ostatni_ruch(daneRozgrywka* daneRoz, int g) {

    FILE* plik_zapis = fopen("rozgrywka.txt", "w");
    fprintf(plik_zapis, "\n");
    for (int i = 0; i < SIZE_PIONKI; i++) {
        fprintf(plik_zapis, " %d", (*daneRoz).pionkiTab[i]);
    }

    for (int i = 0; i < 2; i++) {

        fprintf(plik_zapis, " %d", (*daneRoz).barTab[i]);
        fprintf(plik_zapis, " %d", (*daneRoz).kostkaTab[i]);
        fprintf(plik_zapis, " %d", (*daneRoz).zdjeteTab[i]);
        fprintf(plik_zapis, " %d", (*daneRoz).punktyTab[i]);

    }

    fprintf(plik_zapis, " %d", g);

    fclose(plik_zapis);

}

void zapis_calej_rozgrywki(daneRozgrywka* daneRoz, int g) {

    FILE* plik_zapis = fopen("cala_rozgrywka.txt", "a");
    fprintf(plik_zapis, "\n");
    for (int i = 0; i < SIZE_PIONKI; i++) {
        fprintf(plik_zapis, " %d", (*daneRoz).pionkiTab[i]);
    }

    for (int i = 0; i < 2; i++) {

        fprintf(plik_zapis, " %d", (*daneRoz).barTab[i]);
        fprintf(plik_zapis, " %d", (*daneRoz).kostkaTab[i]);
        fprintf(plik_zapis, " %d", (*daneRoz).zdjeteTab[i]);
        fprintf(plik_zapis, " %d", (*daneRoz).punktyTab[i]);

    }

    fprintf(plik_zapis, " %d\n", g);

    fclose(plik_zapis);

}

void jezeliDublet(daneRozgrywka* daneRoz, int g) {

    if ((*daneRoz).dublet == 0) {
        (*daneRoz).dublet = 4;
    }
    if ((*daneRoz).barTab[g] > 0) {
        pierwszenstwo_banda(daneRoz, g);
    }
    else if (pierwszenstwo_bicia(daneRoz, g)) {
        fBicie(daneRoz, g);
    }
    else {
        fDublet(daneRoz, g);
    }
}

void startRuszania(daneRozgrywka* daneRoz, int g) {

    if ((*daneRoz).kostkaTab[0] == (*daneRoz).kostkaTab[1]) {
        jezeliDublet(daneRoz, g);
    }
    else {
        if ((*daneRoz).barTab[g] > 0) {
            pierwszenstwo_banda(daneRoz, g);
        }
        else if (pierwszenstwo_bicia(daneRoz, g)) {
            fBicie(daneRoz, g);
        }
        else {
            if (g) {
                wypisz_kostki(daneRoz, 1);
            }
            else {
                wypisz_kostki(daneRoz, 0);
            }
        }
    }
}

void gameplay(daneRozgrywka* daneRoz, int g) {
    czysz_ekranu();
    plansza(daneRoz);
    cout << "Ruch: gracz" << g + 1 << "\n\n";

    if ((*daneRoz).kostkaTab[0] <= 0 && (*daneRoz).kostkaTab[1] <= 0) {
        rzutKostkami((*daneRoz).kostkaTab);
    }

    zapis_ostatni_ruch(daneRoz, g);
    zapis_calej_rozgrywki(daneRoz, g);

    startRuszania(daneRoz, g);

}

void wczytanie_rozgrywki(daneRozgrywka* daneRoz) {

    int gracz_wczy = 0;

    FILE* plik_odczyt = fopen("rozgrywka.txt", "r");

    for (int i = 0; i < SIZE_PIONKI; i++) {
        fscanf(plik_odczyt, "%d", &(*daneRoz).pionkiTab[i]);
    }

    for (int i = 0; i < 2; i++) {
        fscanf(plik_odczyt, "%d", &(*daneRoz).barTab[i]);
        fscanf(plik_odczyt, "%d", &(*daneRoz).kostkaTab[i]);
        fscanf(plik_odczyt, "%d", &(*daneRoz).zdjeteTab[i]);
        fscanf(plik_odczyt, "%d", &(*daneRoz).punktyTab[i]);
    }
    fscanf(plik_odczyt, "%d", &gracz_wczy);

    fclose(plik_odczyt);

    gameplay(daneRoz, gracz_wczy);
}

void kto_zaczyna(daneRozgrywka* daneRoz) {
    int kostka[2];
    rzutKostkami(kostka);
    cout << "gracz1: " << kostka[0] << endl;
    cout << "gracz2: " << kostka[1] << endl;
    if (kostka[0] > kostka[1]) {
        cout << "Zaczyna: gracz1" << endl;
        cout << "Wcisnij jakikolwiek przycik, by kontynuowac..." << endl;
        getch();
        gameplay(daneRoz, 0);
    }
    else if (kostka[0] < kostka[1]) {
        cout << "Zaczyna: gracz2" << endl;
        cout << "Wcisnij jakikolwiek przycik, by kontynuowac..." << endl;
        getch();
        gameplay(daneRoz, 1);
    }
    else {
        cout << "jeszcze raz" << endl << endl;
        poczatek_rozgrywki(daneRoz);
    }
}

void poczatek_rozgrywki(daneRozgrywka* daneRoz) {
    char klawisz;
    cout << "Rzuc kostka by rozpoczac gre-> R(r)" << endl;
    cin >> klawisz;
    if (klawisz == 'R' || klawisz == 'r') {
        kto_zaczyna(daneRoz);
    }
    else {
        poczatek_rozgrywki(daneRoz);
    }
}

int closeApp() {
    return 0;
}

void menu_operacja(daneRozgrywka* daneRoz) {

    char klawisz;
    cin >> klawisz;

    switch (klawisz) {
    case 'N': case 'n':
        czysz_ekranu();
        fclose(fopen("cala_rozgrywka.txt", "w"));
        poczatek_rozgrywki(daneRoz);
        break;
    case 'W': case 'w':
        czysz_ekranu();
        wczytanie_rozgrywki(daneRoz);
        break;
    case 'E': case 'e':
        closeApp();
        break;
    default:
        menu_operacja(daneRoz);
    }
}

void pszypisaneWartosciStruct(daneRozgrywka* daneRoz) {
    int tab[SIZE_PIONKI] = { 2, 0, 0, 0 , 0, -5, 0, -3, 0, 0, 0, 5, -5, 0, 0, 0, 3, 0, 5, 0, 0, 0, 0, -2 };
    for (int i = 0; i < SIZE_PIONKI; i++) {
        (*daneRoz).pionkiTab[i] = tab[i];
    }
    (*daneRoz).kostkaTab[0] = 0;
    (*daneRoz).kostkaTab[1] = 0;

    (*daneRoz).barTab[0] = 0;
    (*daneRoz).barTab[1] = 0;

    (*daneRoz).bicieTab[0] = 0;
    (*daneRoz).bicieTab[1] = 0;

    (*daneRoz).zdjeteTab[0] = 0;
    (*daneRoz).zdjeteTab[1] = 0;

    (*daneRoz).dublet = 0;
}

void menu_glowne() {
    cout << "-------MENU-------" << endl;
    cout << "| Nowa gra-> N(n)" << endl;
    cout << "| Wczytaj gre-> W(w)" << endl;
    cout << "| Wyjdz z gry-> E(e)" << endl;

}

int main() {
    srand(time(NULL));
    _setcursortype(_NOCURSOR);
    settitle("Backgammon");

    daneRozgrywka daneRoz;
    pszypisaneWartosciStruct(&daneRoz);

    daneRoz.punktyTab[0] = 0;
    daneRoz.punktyTab[1] = 0;

    menu_glowne();
    menu_operacja(&daneRoz);

    return 0;
}