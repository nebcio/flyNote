# flyNote
Spis treści:
+ [Opis](#opis)
+ [Aktualnie](#aktualnie)
+ [Plan](#plan)
+ [Do realizacji](#do-realizacji)
+ [Wygląd](#wyglad)

## Opis
Aplikacja do tworzenia notatek na pulpicie realizowana przy pomocy Qt 5.12. 

## Aktualnie
Tworzenie nowych notatek jest możliwe, tak jak zapisywanie i usuwanie. Na bieżąco zapisywana jest też konfiguracja. Dodany przycisk do zmiany stylu (do dopracowania). Możliwa zmiana rozmiaru okien. Aktualnie brak znalezionych błędów. Do optymalizacji. 

## Plan
+ ~~~09.11 mechanizm otwierania podokna~~
+ ~~16.11 zamykanie okna (kooperacja z oknem głównym)~~
+ ~~23.11 zapisywanie notatek i ich usuwanie~~
+ ~~30.11 skalowanie okien~~
+ ~~07.12 style (czcionka i czerwony)~~
+ ~~14.12 wstępne połączenie z kalendarzem (pobieranie daty i porównywanie)~~
+ 30.12 wysłanie powiadomienia
+ 11.01 prototyp
+ do 25.02 drobne poprawy

## Do realizacji:
* ~~regulacja czcionki~~
  * ~~obsługa skrótów (Ctrl + / Ctrl -) ~~
* otwieranie notatek
  * zapobieganie otwierania tej samej notatki kilka razy
  * ~~odznacznie listy po otworzeniu ~~
* ~~dwa style kolorystyczne~~
  * ~~umożliwienie zmiany i interfejsu umożliwiającego zmianę ~~
* przypomnienia
  * obsługa kalendarza
  * wybranie dżwięku
* ~~tworzenie nowej notatki~~
  * ~~podpięcie pod główne okno zamykania, usuwania~~
* ~~zamknięcie notatki
  * ~~zapytanie o zapis, autozapis~~
  * zamykanie bez zapisu za pomocą skrótu
* ~~zapisanie nowej notatki~~
  * ~~aktualizacja listy plików w głównym oknie~~
  * ~~edycja flynotes.json~~
* ~~usuwanie notatek~~
  * ~~grupowo i pojedyńczo ~~
  * ~~edycja flynotes.json~~
  * ~~aktualizacja listy plików w głównym oknie ~~
* pomoc (segment interfejsu, gdzie o najechaniu wyświetla się panej ze wskazówkami)
  * i mniejsze podpowiedzi
*  ~~wyświetlanie na wierzchu ~~
*  ~~przezroczystość ~~
*  ~~zarządzanie podoknami przez główne okno ~~
  *  ~~nieprzechowywanie zamkniętych notatek ~~
* 

## Wygląd
Strona główna (pasek tytułu [minimalizacja, zamykanie aplikacji], lista, pasek przycisków [otwieranie, zamykanie otwrtych, usuwanie, nowa i kontrolka pomocy]:

![image](https://user-images.githubusercontent.com/73241633/138764379-4dd270b9-101b-4dc3-9feb-d4246548fe3a.png)
