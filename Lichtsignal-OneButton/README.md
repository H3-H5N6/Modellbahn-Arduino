# Lichtsignal Einfach

Die Beispielschaltung wurde mit einen TT Licht-Ausfahrsignal 4913 von Viessmann getestet. Funktionieren sollten auch die folgenden Lichtsignale:
* 4013 H0 Licht-Ausfahrsignal
* 4913 TT Licht-Ausfahrsignal
* 4413 N Licht-Ausfahrsignal

Wichtig ist, dass das Lichtsignal eine gemeinsame Anode hat.

Da das Lichtsignal hier direkt an den Arduino angeschlossen wird, muss das Lichtsignal ebenfalls mit 5V betrieben werden. Aus meiner Sicht leuchten die LEDs bei 5V mit den originalen Widerständen ausreichend hell. 

Die am Signal angebrachte Diode kann, muss aber nicht, entfallen.

Das Programm selbst soll nur als Beispiel dienen und zeigen, wie eine einfache Ansteuerung eines Lichtsignals erfolgen kann.

Des weiteren wurde als Arduino ein Arduino Nano eingesetzt. 

## Was ist neu?

Zur Ermittelung, welche Taste gedrückt worden ist, wird nun die Bibliothek OneButton verwendet. Dies vereinfacht den Programmablauf. Das gleichzeitige drücken von Tasten wird ignoriert.

```CPP
// Eingangspin für Button festlegen
OneButton hp0_button(BUTTON_HP0_PIN, true, true);

// ...

void setup() {
// Tastendruck mir der gewünschten Funktion verknüpfen
	hp0_button.attachClick(hp0);
}

void loop() {
	hp0_button.tick();
}
```

## Hinweis

* Damit das Programm mit der Arduino-IDE verwendet werden kann, muss die Datei  Lichtsignal-OneButton.cpp zu Lichtsignal-OneButton.ino umbenannt werden!
* Zusätzlich muss noch die Bibliothek OneButton installiert werden. Getestet wurde dieses Programm mit der Version 1.5.0
