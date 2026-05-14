# CLAUDE.md — Python Lab Project: Courses Aggregator

> Αυτό το αρχείο φορτώνεται αυτόματα από το Claude Code σε κάθε συνομιλία σε αυτόν τον φάκελο.
> Σκοπός: να δίνει πλήρες context χωρίς να ξαναδιαβαστεί ο κώδικας από την αρχή.

---

## 1. Επισκόπηση

**Φοιτητής:** Kandilas Giorgos, ΑΜ 1115510
**Μάθημα:** Αρχές Γλωσσών Προγραμματισμού & Μεταφραστών — CEID, Πανεπιστήμιο Πατρών
**Deadline:** 31 Μαΐου 2026 (εξεταστική Ιουνίου)
**Ομάδα:** 1 άτομο (solo project)

### Τι κάνει η εφαρμογή

Μια desktop εφαρμογή Tkinter που συλλέγει εκπαιδευτικά μαθήματα (courses) από 6 online πηγές — 3 μέσω API και 3 μέσω Web Scraping. Τα δεδομένα αποθηκεύονται σε τοπικό CSV (`courses_1115510.csv`), εμφανίζονται σε πίνακα με φίλτρα, οπτικοποιούνται με 3 matplotlib γραφήματα, και τροφοδοτούν μια μηχανή σύστασης που επιστρέφει top-3 προτάσεις βάσει σύνθετης βαθμολογίας. Υπάρχει επίσης auto-refresh scheduler που ανανεώνει τα δεδομένα στο background.

### Φιλοσοφία κώδικα — ΣΗΜΑΝΤΙΚΟ για το Claude

**Ο κώδικας πρέπει να είναι απλός και λειτουργικός — όχι state-of-the-art.** Είμαστε προπτυχιακοί φοιτητές που υλοποιούμε εργαστηριακή άσκηση, όχι senior developers που χτίζουν production σύστημα.

Αυτό σημαίνει πρακτικά: **πριν προτείνεις οποιαδήποτε βελτίωση, ρώτα "αξίζει τον κόπο για τον βαθμό;"**. Αν η απάντηση είναι "όχι", μην αλλάξεις τίποτα. Ο κώδικας πρέπει να εξηγείται σε προφορική παρουσίαση ~15 λεπτών. Βλέπε Ενότητα 8 για λίστα με πράγματα που σκόπιμα ΔΕΝ υλοποιήθηκαν.

---

## 2. Χάρτης Αρχείων

| Αρχείο | Σκοπός | Κύριο API |
|---|---|---|
| `main.py` | Tkinter GUI orchestrator | `class App(tk.Tk)` |
| `collectors.py` | 6 πηγές δεδομένων + fallback λίστες | `collect_all()`, `fetch_coursera()`, `fetch_itunes()`, `fetch_github()`, `scrape_w3schools()`, `scrape_mit_ocw()`, `scrape_tutorialspoint()` |
| `normalizer.py` | Mapping ανομοιογενών κατηγοριών/δυσκολιών | `normalize_category(raw)`, `normalize_difficulty(raw)` |
| `storage.py` | CSV load/save/export με append + dedupe | `load_csv()`, `save_courses(new_courses)`, `export_csv(filepath)` |
| `recommender.py` | Composite-score top-3 σύσταση | `recommend(df, category, difficulty, language, max_cost)` |
| `charts.py` | 3 matplotlib γραφήματα | `bar_top_duration(df)`, `pie_difficulty(df)`, `line_cost_duration(df)` |
| `scheduler.py` | Background auto-refresh daemon thread | `class AutoRefresher` → `.start()` / `.stop()` |
| `config.py` | Κεντρικές σταθερές + console logger | `STUDENT_NAME`, `AM`, `CSV_FILE`, `W_DURATION`, `W_COST`, `log(source, status, detail)` |

### Εξαρτήσεις μεταξύ αρχείων

```
main.py
 ├── collectors.py  (collect_all)
 │    └── normalizer.py  (normalize_*)
 │    └── config.py  (log)
 ├── storage.py  (load_csv, save_courses, export_csv)
 │    └── config.py  (CSV_FILE, CSV_COLUMNS, log)
 ├── recommender.py  (recommend)
 │    └── config.py  (W_DURATION, W_COST)
 ├── charts.py  (bar_top_duration, pie_difficulty, line_cost_duration)
 ├── scheduler.py  (AutoRefresher)
 │    └── config.py  (SCHEDULER_INTERVAL_HOURS, log)
 └── config.py  (STUDENT_NAME, AM, CATEGORIES, DIFFICULTIES, LANGUAGES)
```

---

## 3. Λογική κάθε module — το "γιατί"

### `config.py`

Κεντρικές σταθερές σε ένα αρχείο ώστε να αλλάζουμε βάρη/όνομα/AM σε ένα μόνο σημείο.

**Βάρη recommender (`W_DURATION=0.6`, `W_COST=0.4`):** Μεγαλύτερη βαρύτητα στη διάρκεια γιατί μαθήματα με περισσότερο υλικό = πιο πλούσιο περιεχόμενο. Το κόστος παίζει δευτερεύοντα ρόλο — δίνει προτεραιότητα στα φθηνά/δωρεάν όταν οι διάρκειες είναι παρόμοιες.

**`log(source, status, detail)`:** Παράγει γραμμές `[Source_Name] Status: Success — N mathimata` που απαιτεί ρητά το spec. Χρησιμοποιείται από κάθε collector και storage.

### `collectors.py`

**Γιατί κάθε πηγή έχει `_*_FALLBACK` λίστα:** Σε demo/παρουσίαση το δίκτυο μπορεί να αποτύχει (rate limiting, πεσμένο site). Το fallback εξασφαλίζει ότι η εφαρμογή πάντα επιστρέφει δεδομένα και δεν κρεμάει το GUI. Τα fallback δεδομένα είναι πραγματικά (γνωστά courses), όχι random placeholders.

**Γιατί `_course()` helper (`collectors.py:14`):** Δημιουργεί ενιαία δομή dict για κάθε course και καλεί αυτόματα `normalize_category` / `normalize_difficulty`, ώστε κάθε fetcher να μην επαναλαμβάνει αυτή τη λογική.

**Proxies (παραδοχές που πρέπει να εξηγηθούν στην προφορική):**
- GitHub stars → duration: `min(round(stars/500), 200)` — δεν υπάρχει πεδίο "duration" στα GitHub repos. Τα stars αντιπροσωπεύουν δημοτικότητα/μέγεθος project. Cap στα 200h για αποφυγή ακραίων τιμών.
- iTunes `trackTimeMillis` → hours: `ms / 3_600_000` — χρησιμοποιούμε `entity=podcast` γιατί το iTunes δεν έχει dedicated courses API. Τα educational podcasts αντιμετωπίζονται ως courses.
- Coursera: `category` και `difficulty` hardcoded σε `"programming"` / `"intermediate"` (`collectors.py:61`) — το Coursera API v1 επιστρέφει αυτά τα πεδία αλλά η parsing τους απαιτεί επιπλέον fields που δεν φέρνει το basic endpoint. Είναι γνωστή παραδοχή.

### `normalizer.py`

**Στρατηγική 2-stage:**
1. Exact match: `CATEGORY_MAP[key]` — γρήγορο lookup για γνωστές ακριβείς τιμές.
2. Substring fallback: `if k in key` — αντιμετωπίζει τιμές τύπου `"Introduction to Machine Learning"` που δεν υπάρχουν ακριβώς στον map αλλά περιέχουν γνωστό keyword.
3. Default: `"Other"` / `"Beginner"` — ποτέ δεν αποτυγχάνει, πάντα επιστρέφει έγκυρη τιμή.

### `storage.py`

**Γιατί dedupe σε `(title, provider)` (`storage.py:33`):** Ο συνδυασμός τίτλου + παρόχου αναγνωρίζει μοναδικά ένα course. Μόνο τίτλος δεν αρκεί (π.χ. "Python Tutorial" υπάρχει από W3Schools ΚΑΙ TutorialsPoint). Χρησιμοποιούμε `keep="last"` ώστε η πιο πρόσφατη fetch να αντικαταστήσει παλιότερη εκδοχή.

**Γιατί `infer_objects(copy=False)`:** Αποφεύγει FutureWarning του pandas κατά το concat — ανώδυνη κλήση.

### `recommender.py`

**Min-max normalization (`recommender.py:37-41`):** Βάζει duration και cost στο `[0, 1]` ώστε να είναι συγκρίσιμα παρά τις διαφορετικές μονάδες (ώρες vs ευρώ). Αν `max == min` (όλες οι τιμές ίδιες), επιστρέφει `0.5` — ουδέτερη βαθμολογία.

**Γιατί `(1 - norm_cost)`:** Το cost πρέπει να έχει αντίστροφη επίδραση — χαμηλότερο κόστος = υψηλότερο score. Χωρίς αντιστροφή, τα ακριβά θα παίρνουν ψηλή βαθμολογία.

**Median fill για ελλιπές duration (`recommender.py:29-30`):** Αντί να αγνοούμε courses χωρίς διάρκεια ή να βάζουμε 0 (που τα τιμωρεί), χρησιμοποιούμε το median των υπόλοιπων — "μέτρια" εκτίμηση που δεν τα αδικεί.

**`nlargest(3, "score")`:** Pandas built-in, επιστρέφει ακριβώς 3 (ή λιγότερα αν δεν υπάρχουν αρκετά αποτελέσματα).

### `charts.py`

**Bar chart:** `nlargest(5, "duration_hours")` — top 5 κατά φθίνουσα διάρκεια. Τίτλοι κόβονται στους 30 χαρακτήρες για να χωράνε στον άξονα.

**Line chart:** `nlargest(5).sort_values("duration_hours")` — το sort είναι απαραίτητο ώστε η γραμμή να σχεδιάζεται από αριστερά-προς-τα-δεξιά (αύξουσα duration). Χωρίς sort, η γραμμή θα ήταν "τσαλακωμένη".

**Pie chart:** `value_counts()` — αυτόματη καταμέτρηση ανά κατηγορία δυσκολίας.

### `scheduler.py`

**Daemon thread:** `daemon=True` στο `threading.Thread` — ο thread τερματίζεται αυτόματα όταν κλείσει το κύριο παράθυρο. Χωρίς `daemon=True` η Python δεν θα έκλεινε το process.

**Γιατί `time.sleep` αντί `schedule` library:** Απλούστερο, χωρίς εξωτερική εξάρτηση. Ο interval από config (`SCHEDULER_INTERVAL_HOURS=6`) μετατρέπεται εσωτερικά σε δευτερόλεπτα (`* 3600`).

### `main.py`

**Background thread για collection (`main.py:153-158`):** Η συλλογή δεδομένων (network requests) παίρνει 5–15 δευτερόλεπτα. Χωρίς threading, το GUI "παγώνει". Το `self.after(0, callback)` εξασφαλίζει ότι το UI update γίνεται στο main thread (απαίτηση του Tkinter).

**GUI title (`main.py:17`):** `f"Courses App — {STUDENT_NAME} ({AM})"` → `"Courses App — Kandilas Giorgos (1115510)"`. Απαίτηση spec.

---

## 4. Σχήμα CSV

**Αρχείο:** `courses_1115510.csv` (ίδιος φάκελος με τον κώδικα)

| Στήλη | Τύπος | Περιγραφή |
|---|---|---|
| `title` | str | Τίτλος μαθήματος |
| `provider` | str | Πάροχος / Πανεπιστήμιο / Platform |
| `category` | str | Κανονικοποιημένη κατηγορία (πάντα μία από: Programming, Data Science, Web Development, Mathematics, Business, Languages, Other) |
| `difficulty` | str | Κανονικοποιημένη δυσκολία (Beginner / Intermediate / Advanced) |
| `cost` | float | Κόστος σε ευρώ (0.0 = δωρεάν) |
| `duration_hours` | float | Εκτιμώμενη διάρκεια σε ώρες |
| `language` | str | Γλώσσα διδασκαλίας (κυρίως "English") |
| `source` | str | Πηγή: `Coursera-API`, `iTunes-API`, `GitHub-API`, `W3Schools-Scraper`, `OpenLearn-Scraper`, `TutorialsPoint-Scraper` |
| `fetched_at` | str | ISO timestamp της στιγμής ανάκτησης |

**Παράδειγμα γραμμής:**
```
Python for Everybody,University of Michigan,Programming,Beginner,0.0,36.0,English,Coursera-API,2026-04-27T14:23:11.123456
```

---

## 5. Πώς τρέχει η εφαρμογή

### Εγκατάσταση

```bash
cd D:\Work\Github\CEID\Compilers\Python
pip install -r requirements.txt
```

Εξαρτήσεις: `requests`, `beautifulsoup4`, `pandas`, `matplotlib`. Το `tkinter` είναι stdlib (έρχεται με Python). Python 3.9+ συνιστάται.

### Εκτέλεση

```bash
python main.py
```

### Workflow χρήστη

1. **"Syllogi Dedomenon"** — ξεκινάει background thread που τρέχει `collect_all()`. Console εκτυπώνει `[Source] Status: ...` για κάθε πηγή. Μετά: auto-refresh πίνακα.
2. **Φίλτρα** — Katigoria / Dyskolia dropdowns + Max Kostos text entry → "Filtrare". "Reset" επαναφέρει.
3. **Bar / Pie / Line Chart** — ανοίγει matplotlib παράθυρο. Δεν απαιτεί φιλτράρισμα πρώτα — λειτουργεί πάνω στο πλήρες dataset.
4. **Mhxani Systaseon** — επιλογή κριτηρίων (cat/diff/lang/max cost) → "Bres Top 3". Εμφανίζει 3 γραμμές με score.
5. **"Ekssagogi CSV"** — ανοίγει file dialog για αποθήκευση σε νέο path.
6. **Auto-refresh checkbox** — ενεργοποιεί AutoRefresher που ανανεώνει κάθε 6 ώρες.

### Αναμενόμενο console output

```
[Coursera-API] Status: Success — 20 mathimata
[iTunes-API] Status: Success — 15 mathimata
[GitHub-API] Status: Success — 15 mathimata
[W3Schools-Scraper] Status: Success — 12 mathimata
[OpenLearn-Scraper] Status: Success — 12 mathimata
[TutorialsPoint-Scraper] Status: Success — 12 mathimata
[Storage] Status: Success — 74 nea, 74 synola sto courses_1115510.csv
```

---

## 6. Παραδοχές & Απαντήσεις για Προφορική

Αυτά πρέπει να εξηγηθούν στην παρουσίαση / written report (ενότητα "Σχόλια - Παραδοχές" του PDF):

| Παραδοχή | Αιτιολόγηση |
|---|---|
| iTunes χρησιμοποιεί `entity=podcast` | Δεν υπάρχει courses API στο iTunes. Τα educational podcasts έχουν τίτλο, πάροχο, και duration — αρκούν ως "courses" για το σκοπό της άσκησης. |
| Αντικατάσταση MIT OCW με OpenLearn | Το ocw.mit.edu είναι React SPA — αδύνατο scraping με BeautifulSoup. Αντικαταστάθηκε με το OpenLearn (The Open University) που είναι Drupal site (server-side rendered). |
| Coursera category/difficulty hardcoded | Το Coursera API v1 (χωρίς auth) δεν επιστρέφει αυτά τα πεδία στο basic endpoint. Αποφασίσαμε να hardcode αντί να κάνουμε extra API calls που μπορεί να αποτύχουν. |
| GitHub stars → duration heuristic | Δεν υπάρχει πεδίο "course duration" σε GitHub repo. Χρησιμοποιούμε stars ÷ 500 ως proxy για "μέγεθος/πλούτο" του υλικού. Cap στα 200h αποτρέπει ακραίες τιμές. |
| iTunes ms → ώρες duration | `trackTimeMillis / 3_600_000`. Ακριβής μετατροπή milliseconds → ώρες. |
| Language πάντα "English" στα fallbacks | Οι πηγές που χρησιμοποιούμε παράγουν κυρίως αγγλόφωνο περιεχόμενο. Το language dropdown υποστηρίζει και ελληνικά/γαλλικά αλλά στο CSV το περιεχόμενο είναι κυρίως English. |

---

## 7. Spec → Code Mapping (Αξιολόγηση Επάρκειας)

**Συμπέρασμα: ο κώδικας καλύπτει 100% των ζητούμενων.**

| Κριτήριο spec | Βάρος | Status | Πού |
|---|---|---|---|
| 3 API + 3 Scraping, ≥10 courses | 30% | ✅ | `collectors.py` — Coursera/iTunes/GitHub + W3Schools/OpenLearn/TutorialsPoint |
| 7 required fields | μέρος 30% | ✅ | `collectors.py:14` `_course()` |
| CSV append (no overwrite) + pandas | μέρος 30% | ✅ | `storage.py:21-34` |
| Normalization category + difficulty | μέρος 30% | ✅ | `normalizer.py` CATEGORY_MAP + DIFFICULTY_MAP |
| Tkinter GUI με collect/table/filters/charts/export | 20% | ✅ | `main.py:35-145` |
| Bar chart top-5 duration | μέρος 20% | ✅ | `charts.py:5-19` |
| Pie chart difficulty distribution | μέρος 20% | ✅ | `charts.py:22-29` |
| Line chart cost vs duration top-5 | μέρος 20% | ✅ | `charts.py:33-51` |
| Recommender GUI (cat/diff/lang/maxcost) | μέρος 20% | ✅ | `main.py:116-145` |
| Composite score algorithm | μέρος 20% | ✅ | `recommender.py:45-48` |
| Δικαιολόγηση βαρών | μέρος 20% | ✅ | `config.py:15-17` |
| Έλεγχος κενού CSV | μέρος 20% | ✅ | `main.py:229`, `recommender.py:7` |
| Διαχείριση ελλιπών στοιχείων | μέρος 20% | ✅ | `recommender.py:29-30` median fill |
| Δομή κώδικα & design-decision comments | 10% | ✅ | Όλα τα σχόλια εξηγούν "γιατί" |
| Scheduler extension | +5–10% | ✅ | `scheduler.py` |
| GUI title = name + AM | υποχρ. | ✅ | `main.py:17` |
| CSV filename περιέχει AM | υποχρ. | ✅ | `config.py:3` → `courses_1115510.csv` |
| Console: `[Source] Status: ...` | υποχρ. | ✅ | `config.py:24-28`, χρήση σε κάθε collector |

---

## 8. Τι ΔΕΝ είναι στόχος (οδηγίες για το Claude)

Αυτά **σκόπιμα ΔΕΝ υλοποιήθηκαν** και δεν πρέπει να προταθούν:

- Type hints (π.χ. `def recommend(df: pd.DataFrame) -> list[dict]`)
- Dataclasses ή Pydantic models για courses
- Unit tests / pytest
- `async`/`await` για concurrent API calls
- Python `logging` module (το απλό `print()` αρκεί και το spec το ζητάει ρητά)
- Dependency injection ή factory patterns
- ORM / SQLite αντί για CSV
- Ξεχωριστά config files (.env, YAML)
- Error retry με exponential backoff
- Caching layer

**Γιατί;** Ο κώδικας πρέπει να εξηγείται σε προφορική παρουσίαση ~15 λεπτών από φοιτητή. Κάθε extra αφαίρεση αυξάνει την πολυπλοκότητα χωρίς να αυξάνει τον βαθμό.

Αν προτείνεις αλλαγή, πρώτα ρώτα: **"αυτό βελτιώνει βαθμό ή απλώς κάνει τον κώδικα πιο 'επαγγελματικό';"** Αν δεύτερο, μην το αλλάξεις.

---

## 9. Παραδοτέα Checklist

Από το PDF spec, ο φοιτητής παραδίδει:

- [ ] **PDF report** που περιέχει:
  - Εξώφυλλο (όνομα, ΑΜ, έτος)
  - Βήμα-βήμα περιγραφή υλοποίησης (πηγές δεδομένων, mapping logic, composite score algorithm)
  - Περιγραφή βιβλιοθηκών
  - Τμηματικός κώδικας με σχόλια
  - Screenshots εφαρμογής
  - Τα 3 γραφήματα (με τίτλο, υπόμνημα, περιγραφή)
  - Παραδοχές που έγιναν
  - Προβλήματα που αντιμετωπίστηκαν + λύσεις
- [ ] **Τελικός κώδικας** (8 `.py` αρχεία + `requirements.txt`)
- [ ] **`courses_1115510.csv`** με αποτελέσματα
- [ ] **Zip αρχείο** με όνομα `1115510.zip` (μόνος φοιτητής → ένα ΑΜ)
- [ ] **Υποβολή στο eclass** με μήνυμα που αναφέρει: ονοματεπώνυμο, έτος, ΑΜ, email

**Deadline:** 31 Μαΐου 2026
