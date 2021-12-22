# flyNote
+ [Describe](#describe)
+ [For now](#for-now)
+ [Plan](#plan)
+ [Features](#features)
+ [Application preview](#application-preview)

## Describe
A sticky notes application written with Qt 5.12. 

## For now
Creating, saving, and deleting works as designed. In the main window you can change the style, delete by series or open by series selected notes. The config.json file saves information about the size and location of windows as well as style and font size.
Color style: #AF2222 (red) and #146b70 (blue), background: #202021.

## Plan
+ ~~14.12 wstępne połączenie z kalendarzem (pobieranie daty i porównywanie)~~
+ 30.12 wysłanie powiadomienia
+ 11.01 prototyp
+ do 25.02 drobne poprawy

## Features
* Ctrl + / Ctrl - (increase and decrease font size)
* Button with brush (switch style red->blue, blue->red)
* Checkbox 'fly' (Always on top flag)
* Slider (Opacity to the extent of 30 – 255, 30 prevents the window from disappearing)
* Ctrl S (In main window: save all opened notes, in active note: save only this note)
* Autsave with closing note (!!! but not when you closing app) if the path is known
* QDialog ask if you want to save note if the path is unknown
* The configuration can be changed manually (flynotes.json and config.json) for adding some other *.txt file
* 
  * zapobieganie otwierania tej samej notatki kilka razy
  * obsługa kalendarza
  * wybranie dżwięku
  * zamykanie bez zapisu za pomocą skrótu
* pomoc (segment interfejsu, gdzie o najechaniu wyświetla się panej ze wskazówkami)
  * i mniejsze podpowiedzi
* 

## Application preview
![image](https://user-images.githubusercontent.com/73241633/147145525-9ab25890-5d1b-4dd6-b2df-6c62167fe169.png)
