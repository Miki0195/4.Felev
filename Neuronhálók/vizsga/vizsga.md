# Szoftver mélyneuronhálók vizsga tételek
## Tartalomjegyzék
- [1. Tétel](#1tétel)  
 Mi a gépi tanulás? Milyen feladatok megoldására ideális? Felügyelt és felügyeletlen tanulás feladatai, formális definíciója és folyamatának lépései. Hipotézisfüggvény és költségfüggvény fogalma, példák. Optimalizáció és tárgya, gradiens módszer, update-szabály, tanulási ráta, garanciák a megtalált megoldásra, feature scaling.
- [2. Tétel](#2tétel)   
Regresszió és klasszifikáció feladata, különbségek. Egy- és többváltozós lineáris regresszió: hipotézisfüggvény, költségfüggvény, megoldás gradiens módszerrel, alkalmazásai.
- [3. Tétel](#3tétel)   
Alultanulás és túltanulás jellemzői, felismerésük. Hiperparaméterek, példák. A modell betanításának és kiértékelésének lépései validációs halmazzal. A túltanulás kezelése (mintaelemek/paraméterek száma, early stopping, adataugmentáció, zaj, dropout).
- [7. Tétel](#7tétel)  
Transfer learning és lépései (mérlegelendő szempontok a háló átalakításánál, finomhangolásánál). Súlyok befagyasztása. Mélyhálók és problémáik: a gradiens skálázódása. Az instabil gradiens probléma és elkerülése: batch normalization és reziduális hálók.
- [8. Tétel](#8tétel)  
Felügyeletlen tanulás neuronhálóval. Az autoencoder felépítése. Az autoencoder megszorítás jelentősége és két tanult fajtája. Az autoencoder alkalmazása: tömörítés, zajszűrés, transfer learning.





## 1.Tétel

### Mi a gépi tanulás?
A gépi tanulás egy olyan számítástechnikai tudományterület, amely algoritmusokat és statisztikai modelleket alkalmaz arra, hogy a számítógépek automatikusan tanuljanak és javuljanak tapasztalatokból anélkül, hogy kifejezetten beprogramoznák őket. A cél az, hogy a gép felismerje a mintákat az adatokban, és ezen minták alapján előrejelzéseket vagy döntéseket hozzon.

### Milyen feladatok megoldására ideális?
A gépi tanulás számos különböző feladat megoldására alkalmas, például:
- **Kép- és hangfelismerés:** Az arcok, tárgyak, beszéd azonosítása.
- **Természetes nyelv feldolgozás (NLP):** Szövegértés, fordítás, érzelemfelismerés.
- **Ajánlórendszerek:** Filmek, termékek ajánlása a felhasználók korábbi viselkedése alapján.
- **Orvosi diagnosztika:** Betegségek felismerése és diagnosztizálása orvosi adatok alapján.
- **Pénzügyi előrejelzések:** Részvényárfolyamok, piaci trendek előrejelzése.
- **Játékok és szimulációk:** Stratégiák kidolgozása és optimalizálása.

### Felügyelt és felügyeletlen tanulás
**Felügyelt tanulás (Supervised Learning)**

A felügyelt tanulás során az algoritmus egy előre definiált, címkézett adathalmaz alapján tanul. A bemeneti adatokhoz tartozik egy célérték (label), amit az algoritmusnak meg kell tanulnia előrejelezni.

**Feladatai:**
- **Klasszifikáció (Classification):** Az adatok kategóriákba sorolása (pl. email spam vagy nem spam).
- **Regresszió (Regression):** Folytonos célértékek előrejelzése (pl. házárak, hőmérséklet).

**Felügyeletlen tanulás (Unsupervised Learning)**

A felügyeletlen tanulás során az algoritmusnak nincs címkézett adathalmaz, hanem az adatokban rejlő struktúrákat és mintákat kell felfedeznie.

**Feladatai:**
- **Klaszterezés (Clustering):** Az adatok csoportosítása (pl. ügyfelek szegmensekbe sorolása).
- **Asszociációs szabályok (Association Rules):** Az adatok közötti kapcsolatok felfedezése (pl. vásárlási szokások elemzése).

### Formális definíciója és folyamatának lépései
A gépi tanulási folyamat általában a következő lépésekből áll:
- **Adatgyűjtés:** Az adatok összegyűjtése a problématerületről.
- **Adat-előkészítés:** Az adatok tisztítása, átalakítása és előkészítése.
- **Adatfelosztás:** Az adathalmaz felosztása tanító (training) és teszt (test) adatokra.
- **Modellek kiválasztása:** Megfelelő gépi tanulási algoritmusok kiválasztása.
- **Tanítás (Training):** Az algoritmus tanítása a tanító adatokkal.
- **Értékelés (Evaluation):** A modell teljesítményének értékelése a teszt adatok alapján.
- **Optimalizálás (Optimization):** A modell finomhangolása és optimalizálása.
- **Telepítés (Deployment):** A végleges modell alkalmazása valós adatokon.

### Hipotézisfüggvény és költségfüggvény fogalma, példák  
**Hipotézisfüggvény (Hypothesis Function)**  
A hipotézisfüggvény egy matematikai függvény, amely a bemeneti adatok alapján előrejelzéseket tesz. Például egy lineáris regresszió esetében a hipotézisfüggvény a következőképpen nézhet ki:
**h<sub>θ</sub>(x)=θ<sub>0</sub>+θ<sub>1</sub>x**, ahol **θ<sub>0</sub>** és **θ<sub>1</sub>** a modell paraméterei, **x** pedig a bemeneti változó.

**Költségfüggvény (Cost Function)**  
A költségfüggvény méri, hogy mennyire pontosak az előrejelzések. Célunk a költség minimalizálása a modell paramétereinek optimalizálásával. Egy gyakori költségfüggvény a négyzetes hibaösszeg (mean squared error, MSE):
![Négyzetes hibaösszeg](Negyzeteshibaosszeg.png "Négyzetes hibaösszeg"), ahol **m** a minták száma, **h<sub>θ</sub>(x<sup>i</sup>)** az **i**-edik minta előrejelzése, **y<sup>i</sup>** az **i**-edik minta valós értéke.

### Optimalizáció és tárgya
Az optimalizáció célja a költségfüggvény minimalizálása, azaz olyan paraméterek (θ) megtalálása, amelyek minimalizálják a modell hibáját.

**Gradiens módszer (Gradient Descent)**  
A gradiens módszer egy iteratív optimalizációs algoritmus, amely a költségfüggvény gradiensét használja a paraméterek frissítésére. A frissítés szabálya:

![Gradiens módszer](gradiensmodszer.png "Gradiens módszer")  
ahol α a tanulási ráta, amely meghatározza a lépés nagyságát.

**Tanulási ráta (Learning Rate)**  
A tanulási ráta egy hiperparaméter, amely befolyásolja, hogy a gradiens módszer milyen nagy lépéseket tesz az optimalizálás során. Túl nagy tanulási ráta esetén az algoritmus nem konvergálhat, míg túl kicsi tanulási ráta esetén az algoritmus lassan konvergál.

**Garanciák a megtalált megoldásra**  
A gradiens módszer nem garantálja, hogy a globális minimumot találja meg, különösen nem konvex költségfüggvények esetén, de gyakran jó közelítést ad a gyakorlatban.

### Feature Scaling (Jellemzők skálázása)
A feature scaling egy előfeldolgozási technika, amely a bemeneti változók (jellemzők) értékeit egy közös skálára hozza. Gyakori módszerek:

- **Normálás (Normalization):** Az értékeket 0 és 1 közé skálázza.
- **Standardizálás (Standardization):** Az értékeket 0 átlag és 1 szórás köré skálázza.  

Feature scaling fontos, mert a különböző skálán lévő jellemzők zavarhatják az optimalizációs algoritmusokat, mint például a gradiens módszert.

### Összefoglalás
A gépi tanulás különféle feladatokra alkalmazható, mint például klasszifikáció, regresszió, klaszterezés és asszociációs szabályok megtalálása. A folyamat magába foglalja az adatgyűjtést, előkészítést, modellek tanítását és optimalizálását. A hipotézis- és költségfüggvények meghatározzák a modell előrejelzéseit és a hibáját, amelyeket az optimalizációs technikákkal, mint a gradiens módszer, minimalizálhatunk. A tanulási ráta és a feature scaling kulcsszerepet játszanak az algoritmus hatékonyságában és pontosságában.

## 2.Tétel  
### Regresszió  
A regresszió feladata az, hogy a függő változót (célváltozó, y) egy vagy több független változó (input változók, X) alapján előre jelezze. A cél itt a folytonos értékek előrejelzése, például házárak, hőmérséklet, vagy a részvényárfolyamok. A regresszió leggyakoribb típusa a lineáris regresszió, de számos más típus is létezik, mint például a polinomiális regresszió, a logisztikus regresszió (bár ez gyakran a klasszifikációval azonosítják) és a Ridge regresszió.

### Klasszifikáció
A klasszifikáció célja, hogy az input adatok alapján egy adott kategóriába sorolja a megfigyeléseket. Itt a célváltozó diszkrét értékekkel bír, például az email spam vagy nem spam kategóriába sorolása, vagy a betegek diagnosztizálása egy adott betegségre vagy annak hiányára. A klasszifikáció algoritmusai közé tartozik a logisztikus regresszió, a döntési fák, a támogatott vektor gépek (SVM) és a neurális hálózatok.

**Különbségek**
- **Kimeneti változó típusa:** Regresszió esetén folytonos, klasszifikáció esetén diszkrét.
- **Feladat típusa:** Regresszió előrejelzést készít, klasszifikáció osztályoz.
- **Értékelési metrikák:** Regresszióban gyakori metrikák az MSE, RMSE, MAE, míg klasszifikációban a pontosság, F1-score, ROC-AUC.

### Egy- és többváltozós lineáris regresszió
### Hipotézisfüggvény
**Egyszerű (egyváltozós) lineáris regresszió**  
Az egyszerű lineáris regresszió egyetlen független változót használ a célváltozó előrejelzéséhez. A hipotézisfüggvény a következőképpen néz ki:  
**h<sub>θ</sub>(x)=θ<sub>0</sub>+θ<sub>1</sub>x**  
- **h<sub>θ</sub>(x):** Az előrejelzett érték
- **θ<sub>0:** Intercept - Metszéspont
- **θ<sub>1</sub>:** Slope - Meredekség
- **x:** Bemeneti változó

**Többváltozós (multivariable) lineáris regresszió**  
Több független változó esetén a hipotézisfüggvény a következőképpen bővül:

![Többváltozós lineáris regresszió](tobbvaltozoslinearisregresszio.png "Többváltozós lineáris regresszió") 
- **x<sub>0</sub>, x<sub>1</sub>, ... ,x<sub>n</sub>:** Bemeneti változók
- **θ<sub>0</sub>, θ<sub>1</sub>, ... ,θ<sub>n</sub>:** Modell paraméterei

### Költségfüggvény
A költségfüggvény méri, hogy mennyire pontosak az előrejelzések az összes adatponton. A leggyakoribb költségfüggvény a négyzetes hibaösszeg (Mean Squared Error, MSE): 
![Négyzetes hibaösszeg](Negyzeteshibaosszeg.png "Négyzetes hibaösszeg")
- **m:** Az adatok száma
- **h<sub>θ</sub>(x<sup>i</sup>):** Az **i**-edik adatpont előrejelzése
- **y<sup>i</sup>:** Az **i**-edik adatpont valódi értéke

### Megoldás gradiens módszerrel  
A gradiens módszer (Gradient Descent) egy iteratív optimalizációs algoritmus, amely célja a költségfüggvény minimalizálása. Az algoritmus a következőképpen működik:

- **Kezdeti paraméterek választása:** Véletlenszerűen vagy nullával.
- **Gradiens számítása:** Számítsuk ki a költségfüggvény gradientjét a paraméterek szerint.
- **Paraméterek frissítése:** Frissítsük a paramétereket a tanulási ráta (α) és a gradiens alapján.

Az egyes paraméterek frissítése:
![Gradiens módszer](gradiensmodszer.png "Gradiens módszer") 

### Alkalmazások

A lineáris regresszió széles körben alkalmazott számos területen, például:
- **Gazdasági előrejelzések:** Például részvényárfolyamok vagy GDP előrejelzése.
- **Egészségügy:** Például a vérnyomás előrejelzése a kor, testsúly és egyéb jellemzők alapján.
- **Marketing:** Hirdetési költségek és az eladások közötti kapcsolat vizsgálata.
- **Tudományos kutatások:** Különböző kísérleti adatok analízise.

### Összefoglalás
A regresszió és a klasszifikáció közötti különbségek alapvetőek, mivel előbbi folytonos, míg utóbbi diszkrét értékeket jósol. Az egy- és többváltozós lineáris regresszió esetében a hipotézisfüggvény és a költségfüggvény különbözőek, de a gradiens módszer ugyanúgy alkalmazható a paraméterek optimalizálására. A lineáris regresszió széles körben alkalmazható számos gyakorlati probléma megoldására, ahol az előrejelzés a cél.

# 3.Tétel
## Alultanulás és túltanulás jellemzői, felismerésük
### Alultanulás
**Jellemzői:**  
>A modell nem tanulja meg megfelelően az adatok mögötti mintákat, ezért alacsony teljesítményt nyújt mind a tanuló, mind a teszt adathalmazon.
Egyszerű modellek, kevés paraméterrel (pl. lineáris regresszió egy bonyolult, nem lineáris adathalmazra).
Magas bias (elfogultság): a modell leegyszerűsíti a problémát, nem képes komplexitást kezelni.  

**Felismerése:**  
>A tanulási és tesztelési hiba magas.
A hiba nem csökken a modell tanítása során.
---
### Túltanulás
**Jellemzői:**  
>A modell túl jól megtanulja a tanuló adatok mintáit, beleértve a zajokat és a véletlen ingadozásokat is.
Komplex modellek, sok paraméterrel (pl. mély neurális hálózat kevés adaton).
Alacsony bias, magas variancia: a modell jól teljesít a tanuló adathalmazon, de gyengén a teszt adathalmazon.

**Felismerése:**  
>Alacsony hiba a tanuló adathalmazon, de magas hiba a teszt adathalmazon.
A hiba gyorsan csökken a tanuló adatokon, de növekszik vagy stagnál a teszt adatokon.
---
### Hiperparaméterek
- Olyan paraméterek, amelyeket a modell tanítása előtt állítunk be, és nem tanulnak az adatokból.
- Példák: tanulási ráta (α), regulárizációs paraméter (λ), rejtett rétegek száma és mérete egy neurális hálózatban, epoch-szám, batch-méret.
---
## A modell betanításának és kiértékelésének lépései validációs halmazzal
### 1. Adatok felosztása:
- **Tanuló halmaz (Training Set):** A modell betanítására használt adatok.
- **Validációs halmaz (Validation Set):** A hiperparaméterek finomhangolására használt adatok.  
- **Teszt halmaz (Test Set):** A végleges modell teljesítményének értékelésére használt adatok.  

### 2. Tanítás (Training):
>A modellt betanítjuk a tanuló halmazon különböző hiperparaméter beállításokkal.

### 3. Validáció (Validation):
>A modellt kiértékeljük a validációs halmazon, és a hiperparamétereket finomhangoljuk a legjobb teljesítmény elérése érdekében.

### 4. Tesztelés (Testing):
>A végleges modellt kiértékeljük a teszt halmazon, hogy megkapjuk a teljesítmény objektív mérőszámát.
---
## Túltanulás kezelése
### 1. Mintaelemek számának növelése  
**Cél:** Az adatok mennyiségének növelése, hogy a modell jobban általánosíthatóvá váljon.  

**Működés:** 
- **Több adatgyűjtés:** Minél több adat áll rendelkezésre a modell tanításához, annál jobban képes a modell felismerni az általános mintákat, és kevésbé fogja megtanulni a zajokat és az adatok véletlenszerű fluktuációit.
- **Adatforrások bővítése:** Új adatokat gyűjthetünk különböző forrásokból, például további mérések, kísérletek, vagy külső adatbázisok felhasználásával.    

**Előnyök:** 
- Csökkenti a modell varianciáját, ezáltal jobban általánosíthat.
- Nagyobb adathalmazoknál a modell robusztusabbá válik.  

**Hátrányok:**
- Adatgyűjtés idő- és erőforrás-igényes lehet.
- Az adatok minősége is fontos, nem csak a mennyiségük.

### 2. Paraméterek számának csökkentése

**Cél:** Egyszerűbb modellek használata kevesebb paraméterrel, hogy elkerüljük a túlkomplexitást és a túltanulást.

**Működés:**

- **Modellek egyszerűsítése:** Például kisebb mélységű döntési fák, kevesebb rejtett réteg vagy neuron egy neurális hálózatban.
- **Felesleges paraméterek eltávolítása:** A nem informatív vagy redundáns változók eltávolítása az adathalmazból.

**Előnyök:**
- Csökkenti a modell varianciáját és egyszerűbbé teszi a modellt.
- Javítja a modell interpretálhatóságát.

**Hátrányok:**
- Ha túlzottan leegyszerűsítjük a modellt, alultanuláshoz vezethet.

### 3. Early Stopping

**Cél:** A modell tanításának leállítása, amikor a validációs hiba elkezd növekedni, hogy elkerüljük a túltanulást.

**Működés:**
- **Tanulási folyamat monitorozása:** A tanítás során folyamatosan figyeljük a validációs halmazon mért hibát.
- **Stop feltétel:** A modell tanítását leállítjuk, amikor a validációs hiba növekedni kezd, még ha a tanulási hiba tovább csökken is.

**Előnyök:**
- Megakadályozza a modell túlillesztését a tanuló adatokra.
- Hatékonyan javítja a modell általánosítási képességét.

**Hátrányok:**
- Megfelelő stop kritérium kiválasztása néha nehéz lehet.
- Előfordulhat, hogy a modell nem éri el a potenciálisan legjobb teljesítményt.

### 4. Adatok augmentációja

**Cél:** Az adatok mesterséges növelése különböző transzformációkkal, hogy a modell ne tanulja meg a bemeneti adatok véletlenszerű zajait.

**Működés:**
- **Képek esetén:** Forgatás, eltolás, méretezés, tükrözés, színek módosítása, zaj hozzáadása stb.
- **Szöveges adatok esetén:** Szinonimák cseréje, mondatok átrendezése stb.

**Előnyök:**
- Növeli az adathalmaz változatosságát anélkül, hogy új adatokat kellene gyűjteni.
- Segíti a modell robusztusabbá válását a különböző adatvariációkkal szemben.

**Hátrányok:**
- Az augmentációs technikák nem minden adatfajtára alkalmazhatóak egyformán jól.
- A rosszul megválasztott augmentációk ronthatják a modell teljesítményét.

### 5. Zaj hozzáadása

**Cél:** Kis mértékű zaj hozzáadása a bemeneti adatokhoz, hogy a modell ne tanulja meg a zajokat és ne váljon túlérzékennyé az adatok véletlenszerű változására.

**Működés:**
- **Zaj típusok:** Gaussi zaj, sós és bors zaj, stb.
- **Zaj mértéke:** A zaj mértéke szabályozható, hogy ne torzítsa túl az eredeti adatokat.

**Előnyök:**
- Segíti a modell robusztusabbá válását és javítja az általánosítási képességet.
- Növeli a modell toleranciáját a valós világban előforduló adatvariációkkal szemben.

**Hátrányok:**

- Túl sok zaj hozzáadása ronthatja a modell teljesítményét.
- Nehéz megtalálni az optimális zajszintet.

### 6. Dropout

**Cél:** Neurális hálózatok esetén bizonyos neurális egységek véletlenszerűen kikapcsolása a tanítás során, hogy a hálózat ne váljon túlzottan függővé bizonyos neuronoktól.

**Működés:**

- **Kikapcsolási arány:** Általában a rétegek neuronjainak 20-50%-át véletlenszerűen kikapcsolják a tanítás minden iterációjában.
- **Kikapcsolási mechanizmus:** A kikapcsolt neuronok nem vesznek részt a számításokban az adott iteráció során, de a következő iterációban újra bekapcsolódhatnak.

**Előnyök:**
- Csökkenti a neurális hálózat túlillesztését és növeli a robusztusságát.
- Javítja a modell általánosítási képességét, mivel a hálózat nem válik túlzottan függővé egy-egy részterülettől.

**Hátrányok:**
- Lassíthatja a tanulási folyamatot.
- Néha bonyolult lehet a megfelelő dropout arány kiválasztása.
---
## Összefoglalás  
Az alultanulás és a túltanulás megértése és kezelése kritikus fontosságú a gépi tanulási modellek fejlesztésében. Az alultanulás során a modell nem képes megtanulni az adatokat megfelelően, míg a túltanulás során túl jól tanulja meg a tanuló adatok mintáit, beleértve a zajokat is. A hiperparaméterek megfelelő beállítása, a validációs halmaz használata, és a túltanulás elleni technikák alkalmazása mind segíthetnek abban, hogy a modell jól általánosítható legyen és pontos előrejelzéseket készítsen új adatokon.  

---
# 7.Tétel
## Transfer Learning és lépései
### Transfer Learning 
>A transfer learning (átviteli tanulás) egy gépi tanulási technika, ahol egy előre betanított modellt újrafelhasználunk egy másik, de kapcsolódó feladatra. Ez különösen hasznos akkor, ha az új feladat rendelkezésre álló adatainak mennyisége korlátozott.

### Lépések

**1. Előre Betanított Modell Kiválasztása:**  
- Az első lépés egy olyan modell kiválasztása, amelyet már egy nagy és általános adatbázison (pl. ImageNet) betanítottak.
- Ez a modell már rendelkezik az általános jellemzők kivonásához szükséges képességekkel.

**2. Háló Átalakítása:**  
- Az előre betanított modell adaptálása az új feladatra. Ez magában foglalhatja a végső rétegek módosítását vagy hozzáadását, hogy az új feladat specifikus követelményeinek megfeleljen.
- Például, ha a cél egy új kategória klasszifikálása, a végső softmax réteget új osztályokra kell cserélni.

**3. Súlyok Befagyasztása:**  
- A súlyok befagyasztása azt jelenti, hogy az előre betanított modell bizonyos rétegeinek súlyait rögzítjük (nem engedjük, hogy ezek a súlyok frissüljenek a tanulás során).
- Általában az alsóbb rétegeket, amelyek az alapvető jellemzőket tanulták meg, fagyasztjuk be, míg a felsőbb rétegeket finomhangoljuk az új adatokkal.

**4. Finomhangolás:**  
- A modell finomhangolása az új adatbázison történik. Ez magában foglalja a fagyasztott rétegeken kívüli rétegek újratanítását az új feladat specifikus adataival.
- Fontos a megfelelő tanulási ráta kiválasztása, hogy a finomhangolás során a modell jól illeszkedjen az új adatokra, de ne felejtse el az általános jellemzőket.

**5. Validáció és Tesztelés:**
- A modell teljesítményének értékelése validációs és teszt adatokon.
- Az eredmények alapján további finomhangolás végezhető, ha szükséges.
---
## Mérlegelendő Szempontok a Háló Átalakításánál és Finomhangolásánál

### Feladathoz Illeszkedés
>Az új feladat specifikációihoz igazítjuk a modellt. Például egy képfelismerő modell esetében a bemeneti adatok mérete és a kimeneti osztályok száma fontos szempont.

### Adatmennyiség:
>Az új feladathoz rendelkezésre álló adat mennyisége. Ha kevés adat áll rendelkezésre, érdemes több réteget befagyasztani és kevesebb réteget finomhangolni.

### Tanulási Ráta:
>A tanulási ráta megfelelő beállítása elengedhetetlen. Túl nagy tanulási ráta esetén a modell túl gyorsan alkalmazkodik, ami túltanuláshoz vezethet, míg túl alacsony tanulási ráta esetén a tanulás lassú lehet.

### Regularizáció:
> Regularizációs technikák (pl. dropout, L2 regularizáció) használata, hogy elkerüljük a túltanulást az új adatokkal történő finomhangolás során.
---
## Mélyhálók és Problémáik: A Gradiens Skálázódása
## A Gradiens Problémák

A mély neurális hálózatoknál gyakori probléma a gradiens skálázódása, amely két fő formában jelentkezhet:

### Eltűnő Gradiens Probléma (Vanishing Gradient Problem)
>Amikor a gradiens értékek a hátrafelé irányuló gradiens számítás során exponenciálisan csökkennek.
Ez különösen a sigmoid vagy tanh aktivációs függvények használatakor fordul elő, mivel ezek a függvények a bemenet szélsőséges értékeire hajlamosak kis gradiensértékeket adni.

### Explodáló Gradiens Probléma (Exploding Gradient Problem)
>Amikor a gradiens értékek a hátrafelé irányuló gradiens számítás során exponenciálisan növekednek.
Ez instabil tanulást eredményezhet, ahol a súlyok gyorsan elérhetetlenül nagy értékeket vesznek fel.

## Az Instabil Gradiens Probléma és Elkerülése

### Batch Normalization (Batch Norm)
- A batch normalization egy olyan technika, amely normalizálja a rétegek bemeneteit egy minibatch-nyi adat alapján. Ez segít stabilizálni és gyorsítani a tanulási folyamatot.
- **Működése:** Az egyes rétegek bemeneteit normalizáljuk úgy, hogy azokat egy meghatározott átlagra és szórásra skálázzuk, majd két további paraméter segítségével újrasúlyozzuk és eltoljuk.

### Reziduális Hálók (Residual Networks, ResNets):
- A reziduális hálók olyan hálózati architektúrák, amelyek reziduális kapcsolatokat (skip connections) használnak. Ezek a kapcsolatok lehetővé teszik, hogy az információ megkerülje az egyik vagy több réteget, közvetlenül egy későbbi rétegbe áramolva.
- **Működése:** Ahelyett, hogy a bemenetet közvetlenül a következő rétegbe küldenénk, hozzáadjuk a bemenetet a réteg kimenetéhez, így a kimenet a bemenet és a réteg kimenetének összegét tartalmazza.
- **Előnye:** Segít megőrizni a gradiens áramlását a hálózaton keresztül, csökkentve az eltűnő gradiens problémát és lehetővé téve a nagyon mély hálózatok hatékony tanulását.
---
# 8.Tétel
## Felügyeletlen Tanulás Neurális Hálóval
>A felügyeletlen tanulás olyan gépi tanulási módszer, ahol a modell nem kap előre címkézett adatokat a tanuláshoz. Ehelyett a modellnek magának kell felfedeznie a bemenetek közötti mintákat és struktúrákat. A felügyeletlen tanulás célja, hogy a bemenetek szerkezetét és jellemzőit felismerje anélkül, hogy konkrét kimeneti változókat kapna.
---
## Autoencoder Felépítése
>Az autoencoder egy speciális neurális hálózati architektúra, amelyet gyakran használnak felügyeletlen tanulási feladatokhoz. Az autoencoder célja, hogy a bemenetet egy kisebb dimenziójú reprezentációba tömörítse, majd ebből a reprezentációból újra előállítsa az eredeti bemenetet.

### Autoencoder Részei

**Encoder (Kódoló):**  
>A bemeneti adatot (például képet vagy szöveget) egy kisebb dimenziójú reprezentációba tömöríti.
Általában több teljesen összekötött rétegből áll, amelyek minden rétegben csökkentik a dimenziók számát.
Az encoder utolsó rétege a kódolt reprezentáció, más néven bottleneck.

**Bottleneck:**  
>Ez a legkisebb dimenziójú réteg, amely az adat tömörített reprezentációját tartalmazza.
A cél az, hogy a bottleneck tartalmazza az összes releváns információt az adatból, miközben eltávolítja a redundáns vagy zajos információkat.

**Decoder (Dekódoló):**  
>A kódolt reprezentációt visszaalakítja az eredeti bemeneti adatformába.
A dekódoló rétegei tükrözik az encoder rétegeit, csak fordított sorrendben, így fokozatosan növelik a dimenziók számát az eredeti méretre.
---
**Az autoencoder működését egy egyszerű képfeldolgozási példán keresztül lehet szemléltetni:**  
- **Bemenet:** 28x28 képpontból álló képek (például MNIST adatbázis képei).  
- **Encoder:** Több réteg, amelyek először 784 (28x28), majd 128, végül 64 dimenzióra csökkentik a bemenetet.  
- **Bottleneck:** 32 dimenziós kódolt reprezentáció.  
- **Decoder:** Több réteg, amelyek először 64, majd 128, végül 784 (28x28) dimenzióra növelik a kódolt reprezentációt, visszaállítva az eredeti képet.
---
## Autoencoder Megszorítás Jelentősége
>Az autoencoderek hatékonysága nagyrészt az encoder és a decoder közötti rétegek megszorításainak köszönhető. Ezek a megszorítások segítenek az autoencodernek abban, hogy ne egyszerűen "megtanulja" a bemenetek visszaállítását, hanem valóban jelentős mintákat és jellemzőket ismerjen fel.
---
## Két tanult autoencoder fajta  
? Sparse, Donising ?

---
## Az Autoencoder alkalmazásai
### Tömörítés  
>Az autoencoderek egyik fő alkalmazása az adatok tömörítése, amely a bemeneti adatok kisebb dimenziójú reprezentációjának létrehozásával valósul meg.

**1. Encoder:**  
- A bemeneti adatokat, például képeket, a kódoló rétegek több szintjén keresztül kisebb dimenziójú reprezentációvá alakítjuk.  
- Az encoder rétegek a bemeneti adatok dimenzióját fokozatosan csökkentik. Például egy 28x28 pixeles kép esetén az encoder rétegei 784 (28x28), majd 128, 64 és végül egy 32 dimenziós kódolt reprezentációt hoznak létre.

**2. Bottleneck:**
- Ez a kódolt reprezentáció tartalmazza az adat leglényegesebb jellemzőit. Mivel kisebb dimenziójú, mint az eredeti adat, a kódolt reprezentációval csökkenthetjük a tárolási helyigényt.
- Például egy nagy felbontású kép 32 dimenziós kódolt reprezentációja jelentős tárhelymegtakarítást eredményezhet, miközben megőrzi a kép lényeges információit.

**3. Decoder:**
- A dekódoló rétegek fokozatosan visszaállítják a kódolt reprezentációt az eredeti dimenzióra.
- A dekódoló célja, hogy a bemeneti adat eredeti formáját minél pontosabban visszaállítsa. Például a 32 dimenziós kódolt reprezentációból újra 64, 128 és végül 784 (28x28) dimenziós képpé alakítja.

### Zajszűrés

>A denoising autoencoder célja, hogy megtanulja eltávolítani a zajt a bemeneti adatokból, és visszaállítani az eredeti, zajmentes adatot.

**1. Zajos Bemenet:**
- A bemeneti adatokat zajjal szennyezzük, például képeknél véletlenszerű pontokat adunk hozzá, vagy szövegek esetén véletlenszerű karaktereket helyezünk be.
- Ez a zajos bemenet szolgál az autoencoder bemeneti adatává.

**2. Encoder:**
- Az encoder a zajos bemenetet egy kisebb dimenziójú reprezentációba tömöríti, hasonlóan a tömörítés esetéhez.

**3. Bottleneck:**
- A kódolt reprezentáció itt is a bemeneti adat lényeges információit tartalmazza, de mivel zajos adatokat használunk, a kódolt reprezentáció is tartalmazhat némi zajt.

**4. Decoder:**
- A dekódoló rétegek célja, hogy a zajos kódolt reprezentációból visszaállítsák az eredeti, zajmentes adatot.
- A dekódoló megtanulja, hogyan szűrje ki a zajt a kódolt reprezentációból, így a végső kimenet közel lesz az eredeti, zajmentes bemenethez.

### Transfer Learning

>Az autoencoderek használhatók transfer learningre, ahol az előre betanított modellek jellemzőit más feladatokra alkalmazzuk. Megtanulhatja a bemenetek lényegi jellemzőit, majd ezek a jellemzők átvhietők más feladatokra.

**1. Előre Betanított Autoencoder:**
- Az autoencoder segítségével megtanuljuk a bemeneti adatok lényeges jellemzőit egy nagy adatbázison.
- Például egy autoencoder betanítható a CIFAR-10 képadatbázison, hogy felismerje a képek alapvető mintáit és jellemzőit.

**2. Encoder Kivonása:**
- A betanított autoencoder encoder részét kivonjuk, és új modellbe integráljuk.
- Az encoder rétegek tartalmazzák a bemeneti adatok jellemzőit, amelyeket más feladatokra is felhasználhatunk.

**3. Új Modell:**
- Az új modell, amelybe az encoder rétegeket integráljuk, lehet egy felügyelt tanulási modell, például egy klasszifikációs hálózat.
- Az új modell bemeneti rétege az előre betanított encoder rétegek lesznek, amelyek jellemzők kivonását végzik, majd további rétegeket adhatunk hozzá a konkrét feladat megoldásához.

**4. Új Modell Betanítása:**
- Az új modellt finomhangoljuk a konkrét feladatra, például a CIFAR-10 adatbázison betanított encoder rétegekkel egy virágok képadatbázisán végzett klasszifikációs feladatra.
- A transfer learning segítségével gyorsabban és hatékonyabban érhetünk el jó eredményeket, mivel az encoder rétegek már előzetesen megtanultak általános jellemzőket.
---
## Összefoglalás
Az autoencoderek erőteljes eszközök a felügyeletlen tanulásban, mivel lehetővé teszik az adatok tömörítését, zajszűrését és jellemzők kinyerését. Az encoder és a decoder közötti megszorítások, mint a sparse és denoising autoencoderek, segítenek abban, hogy a hálózat jelentős mintákat és struktúrákat tanuljon meg. Az autoencoderek széles körben alkalmazhatók adatok tömörítésére, zajszűrésére és transfer learningre, javítva a modellek teljesítményét és hatékonyságát.
