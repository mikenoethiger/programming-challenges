Links:

* [Divisor function](https://en.wikipedia.org/wiki/Divisor_function)
* [Divisors Calculator](https://www.hackmath.net/en/calculator/divisors?n=1426539184&submit=Calculate)
* [Number of Divisors](https://primes.utm.edu/glossary/page.php?sort=tau)
* [Carmichael numbers](https://primes.utm.edu/glossary/page.php?sort=CarmichaelNumber)


# Zahlentheorie

Relatively Prime

	Two integers are relatively prime (or coprime) if there is no integer greater than one that divides them both (that is, their greatest common divisor is one). For example, 12 and 13 are relatively prime, but 12 and 14 are not.

Modulo Sätze

	a mod n = (a + i*n) mod n
	(a+b) mod n = (a mod n)+(b mod n) mod n
	(a*b) mod n = (a mod n)*(b mod n) mod n
	a^b mod n = (a mod n)^y mod n

Kongruenz Sätze

	a ≡ b(mod n) <=> m|(a-b) <=> m|a und m|b
	a ≡ b(mod n) and c ≡ d(mod n) <=> a + c ≡ b + d(mod n)
	a ≡ b(mod n) => a*d ≡ b*d(mod n)

Definition Teilbarkeit

	a|b := b=q*a (q∈ℤ)

Teilbarkeits Sätze

	a|0 (jede Zahl teilt 0)
	a|b => a|-b
	(3|6, 3|-6, -3|6, -3|-6)
	a|b und a|c => a|b+c 
	               => a|b-c
	               => a|s*b + t*c
	  weil b=q*a, c=r*a => b+c = q*a + r*a = a*(q+r)
	a|b => a|bc
	a|b => |a| = |b| oder |a| < |b|/2
	a|b => a <= b

Definition ggT

	ggT(a,b) = T(a) ∩ T(b)
	T(n) := Menge aller Teiler von n

ggT Sätze

	ggT(a,a) = a           weil T(a) ∩ T(a) = T(a) 
	ggT(a,1) = 1           weil T(a) ∩ T(1) = {1}
	ggT(a,0) = a           weil T(a) ∩ ℕ = T(a)
	ggT(a,b) = ggT(b,a)    weil Schnittmengen Kommutativ
	ggT(a,b) = ggT(a-b, b) weil ggt(a,b)|a ∧ ggT(a,b)|b => ggT(a,b)|a-b (siehe Beweis Teilbarkeitssätze)

Primzahlsatz

	pi(x) ≈ x/ln(x)

Bedeuted die Anzahl Primzahlen kleiner oder gleich x ist ungefähr `x/ln(x)`.

Hauptsatz der elementaren Zahlentheorie

	n =  p1^e1 * p2^e2 * ... * pk^ek

`p1`, ..., `pk` sind Primzahlen, `e1`, ..., `ek` sind Natürliche Zahlen. Das heisst jede natürliche Zahl hat eine eindeutige Zerlegung in Primfaktoren.

## Primzahlen Finden

Gegeben n, finde heraus ob n Primzahl ist.

**Naiver Ansatz:** Zähle alle Zahlen von 2 ... n-1 die n teilen.

**Optimierter Ansatz 1:** Zähle alle Zahlen von 2 ... sqrt(n) die n teilen. Warum? Wenn p > sqrt(n) eine Zahl ist die n teilt, dann muss die Zahl mit der sie multipliziert n ergibt (kozahl) kleiner als sqrt(n) sein. Wenn man also die Zahlen bis sqrt(n) doppelt zählt (ausser Zahl und Kozahl sind diesselben), die n teilen, hat man bereits alle Kozahlen die über sqrt(n) liegen mitgezählt.

**Optimierter Ansatz 2:** 

## Schnelle Exponentiation

`a^x` in` O(log x)` Multiplikationen berechnen. 

**Idee:** `a` in jeder Iteration mit sich selber multiplizieren. So entsteht die Folge:

	a^1
	a^2
	a^4
	a^8
	...

`a^x` kann nun berechnet werden, indem bestimmte Zahlen aus dieser Folge miteinander multipliziert werden. Es gilt nämlich:

	a^(x+y) = a^x * a^y

Jede Dezimalzahl lässt sich als Summe von 2er Potenzen darstellen (Binärdarstellung). Beispiel: 

	27 = 2^0 + 2^1 + 2^3 + 2^4 = 1 + 2 + 8 + 16

`a^27` könnte man demenstprechend wiefolgt schreiben:

	a^(1+2+8+16) = a^1 * a^2 * a^8 * a^16

Das sind alles Glieder aus der obig beschriebenen Folge. Wenn man also die richtigen Glieder aus der Folge miteinander multipliziert erhält man `a^x`. Mit einem Beispiel lässt sich wohl besser als mit Worten zeigen, wie diese Glieder selektiert werden:

	27 in binär = 1011
	1  a^1 
	1  a^2 
	0  a^4 (ignorieren)
	1  a^8

Wenn wir also in jeder Iteration a mit sich selber multiplizieren entsteht automatisch die `a^1`, `a^2`, `a^4`, ... , `a^(2^n)` Folge, und von denen multiplizieren wir alle die miteinander, wo in der Binärdarstellung vom Exponent eine 1 steht. Das ist insofern praktisch, als dass die Zahlen in einem Programm bereits als Binärzahlen vorligen. Das kann man sich zunutze machen, die meisten Programmiersprachen bieten ein logisches UND `&` an,  welches zwei Zahlen wiefolgt verrechnet:

	 1011 Zahl 1
	&0001 Zahl 2
	=0001 Resultat

Zugleich gibt es den Bitshift-Rechts Operator, der einfach alle Bits nach rechts schiebt:

	1011 >> 1 = 0101

Diese zwei Mechanismen kombiniert man um herauszufinden, ob das aktuelle `a^(2^n)` Glied multipliziert werden muss:

	1. Iteration: 1011 & 1 = 1  multiplizieren
	2. Iteration: 0101 & 1 = 1  multiplizieren
	3. Iteration: 0010 & 1 = 0  nichts tun
	4. Iteration: 0001 & 1 = 1  multiplizieren
	5. Iteration: 0000 & 1 = 0 (fertig wenn exponent 0 ist)

Folgend ein Beispiel Programm für die schnelle Exponentiation:

	long res = 1;
	while (x > 0) {
		if (x & 1) {
			res *= a;
		}
		x >>= 1;
		a *= a;
	}
Beim naiven Ansatz von `a^x` würden `x` Multiplikationen ausgeführt; `a * a * a ...` und das `x` mal. Bei der schnellen Exponentiation verdoppelt sich der Exponent in jedem Schritt (statt sich nur um 1 zu erhöhen), deshalb ist man bereits nach `log2(x)` Schritten bei `x` angelangt; `1, 2, 4, 8, log2(x)-4 weitere Schritte, x`. Dazu kommen die Multiplikationen mit dem Resultat, nämlich für jede 1 in der Binärdarstellung vom Exponent eine Multiplikation, also maximale  `log2(x)` zusätzliche Multiplikationen (alles Einsen). Das macht Total `2*log2(x)` Multiplikationen, damit ist die Berechnungskomplexität in `O(log x)`.