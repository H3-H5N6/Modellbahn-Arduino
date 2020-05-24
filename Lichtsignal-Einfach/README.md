# Lichtsignal Einfach

Die Beispielschaltung wurde mit einen TT Licht-Ausfahrsignal 4913 von Viessmann getestet. Funktionieren sollten auch die folgenden Lichtsignale:
* 4013 H0 Licht-Ausfahrsignal
* 4913 TT Licht-Ausfahrsignal
* 4413 N Licht-Ausfahrsignal

Wichtig ist, dass das Lichtsignal eine gemeinsame Anode hat.

Da das Lichtsignal hier direkt an den Arduino angeschlossen wird, muss das Lichtsignal ebenfalls mit 5V betrieben werden. Aus meiner Sicht leuchten die LEDs bei 5V mit den originalen Widerständen ausreichend hell. 

Die am Signal angebrachte Diode kann, muss aber nicht, entfallen.

Das Progamm selbst soll nur als Beispiel dienen und zeigen, wie eine einfache Ansteuerung eines Lichtsignals erfolgen kann.
