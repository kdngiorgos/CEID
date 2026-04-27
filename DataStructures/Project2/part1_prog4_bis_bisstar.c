/*
 * Project 2 - Πρόγραμμα 4
 * BIS (Binary Interpolation Search) και BIS* (βελτιωμένη παραλλαγή).
 *
 * BIS:  O(log n) μέσος, O(sqrt(n)) χειρότερος χρόνος.
 * BIS*: Χρησιμοποιεί άλματα μεγέθους 2^i * sqrt(n) για
 *       εγγυημένο O(log n) χειρότερο χρόνο.
 *
 * Αρχείο δεδομένων: covid_trade.csv (στον ίδιο φάκελο)
 * Αναζητάμε βάσει Cumulative (ταξινομημένος πίνακας).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_RECORDS 120000
#define MAX_DATE    16
#define MAX_LINE   512

typedef struct {
    char date[MAX_DATE];
    long cumulative;
} Record;

/* ---- Ανάγνωση CSV ---- */

int readCSV(const char *filename, Record *records) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Σφάλμα: δεν βρέθηκε το αρχείο %s\n", filename);
        return 0;
    }
    char line[MAX_LINE];
    int count = 0;
    fgets(line, MAX_LINE, fp);
    while (fgets(line, MAX_LINE, fp) && count < MAX_RECORDS) {
        char direction[32], weekday[16], country[64];
        char commodity[64], transport[32], measure[32];
        int  year;
        long value, cumulative;
        char date[MAX_DATE];
        if (sscanf(line, "%31[^,],%d,%15[^,],%15[^,],%63[^,],%63[^,],%31[^,],%31[^,],%ld,%ld",
                   direction, &year, date, weekday, country, commodity,
                   transport, measure, &value, &cumulative) == 10) {
            strncpy(records[count].date, date, MAX_DATE - 1);
            records[count].cumulative = cumulative;
            count++;
        }
    }
    fclose(fp);
    return count;
}

int cmpCumulative(const void *a, const void *b) {
    long diff = ((Record*)a)->cumulative - ((Record*)b)->cumulative;
    if (diff < 0) return -1;
    if (diff > 0) return  1;
    return 0;
}

/* ---- BIS (Binary Interpolation Search) ----
 * Ξεκινά με άλμα μεγέθους sqrt(n).
 * Βρίσκει το σωστό block με interpolation, μετά binary search μέσα.
 */
int bis(Record *arr, int n, long target) {
    if (n == 0) return -1;
    int sqrtN = (int)sqrt((double)n);
    if (sqrtN < 1) sqrtN = 1;

    int lo = 0, hi = n - 1;
    long arrLo = arr[lo].cumulative;
    long arrHi = arr[hi].cumulative;

    if (target < arrLo || target > arrHi) return -1;

    /* Interpolation jump */
    while (lo < hi) {
        if (arr[hi].cumulative == arr[lo].cumulative) break;
        int pos = lo + (int)(((double)(target - arr[lo].cumulative) /
                               (arr[hi].cumulative - arr[lo].cumulative)) * (hi - lo));
        if (pos < lo) pos = lo;
        if (pos > hi) pos = hi;

        if (arr[pos].cumulative == target) {
            /* Βρίσκουμε πρώτη εμφάνιση */
            while (pos > 0 && arr[pos-1].cumulative == target) pos--;
            return pos;
        } else if (arr[pos].cumulative < target) {
            lo = pos + 1;
        } else {
            hi = pos - 1;
        }
    }
    if (arr[lo].cumulative == target) return lo;
    return -1;
}

/* ---- BIS* (εγγυημένο O(log n) χειρότερος χρόνος) ----
 * Χρησιμοποιεί άλματα μεγέθους 2^i * sqrt(n).
 * Βρίσκει πρώτα το block [L, R] που περιέχει το στοιχείο,
 * μετά εφαρμόζει binary search μέσα στο block.
 */
int bisStar(Record *arr, int n, long target) {
    if (n == 0) return -1;
    int sqrtN = (int)sqrt((double)n);
    if (sqrtN < 1) sqrtN = 1;

    if (target < arr[0].cumulative || target > arr[n-1].cumulative) return -1;

    /* Αρχικό interpolation για να βρούμε κατά προσέγγιση τη θέση */
    int pos = 0;
    if (arr[n-1].cumulative != arr[0].cumulative) {
        pos = (int)(((double)(target - arr[0].cumulative) /
                     (arr[n-1].cumulative - arr[0].cumulative)) * (n - 1));
    }
    if (pos < 0) pos = 0;
    if (pos >= n) pos = n - 1;

    /* Άλματα 2^i * sqrt(n) */
    int lo, hi;
    if (arr[pos].cumulative <= target) {
        lo = pos;
        int jump = sqrtN;
        int i = 0;
        while (lo + jump < n && arr[lo + jump].cumulative < target) {
            lo = lo + jump;
            i++;
            jump = (int)pow(2.0, i) * sqrtN;
        }
        hi = (lo + jump < n) ? lo + jump : n - 1;
    } else {
        hi = pos;
        int jump = sqrtN;
        int i = 0;
        while (hi - jump >= 0 && arr[hi - jump].cumulative > target) {
            hi = hi - jump;
            i++;
            jump = (int)pow(2.0, i) * sqrtN;
        }
        lo = (hi - jump >= 0) ? hi - jump : 0;
    }

    /* Binary search στο block [lo, hi] */
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid].cumulative == target) {
            while (mid > 0 && arr[mid-1].cumulative == target) mid--;
            return mid;
        } else if (arr[mid].cumulative < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

/* ---- Main ---- */

int main() {
    Record *data = (Record*)malloc(MAX_RECORDS * sizeof(Record));
    int n;

    printf("Ανάγνωση δεδομένων από covid_trade.csv...\n");
    n = readCSV("covid_trade.csv", data);
    if (n == 0) { free(data); return 1; }
    printf("Φορτώθηκαν %d εγγραφές.\n", n);

    /* Ταξινόμηση κατά Cumulative */
    qsort(data, n, sizeof(Record), cmpCumulative);
    printf("Δεδομένα ταξινομημένα κατά Cumulative.\n\n");

    /* Δοκιμαστικά κλειδιά */
    long testKeys[] = {
        data[0].cumulative,               /* ελάχιστο */
        data[n/4].cumulative,             /* 1ο τεταρτημόριο */
        data[n/2].cumulative,             /* διάμεσος */
        data[3*n/4].cumulative,           /* 3ο τεταρτημόριο */
        data[n-1].cumulative              /* μέγιστο */
    };
    const char *keyNames[] = {"Ελάχιστο", "Q1", "Διάμεσος", "Q3", "Μέγιστο"};
    int numTests = 5;
    int i;
    clock_t start, end;
    double tBIS, tBISStar;

    printf("%-12s  %-20s  %-15s  %-15s  %s\n",
           "Ζητούμενο", "Cumulative", "BIS(ms)", "BIS*(ms)", "Βρέθηκε");
    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < numTests; i++) {
        int reps = 1000, idx;

        start = clock();
        for (int r = 0; r < reps; r++)
            idx = bis(data, n, testKeys[i]);
        end = clock();
        tBIS = (double)(end - start) / CLOCKS_PER_SEC * 1000.0 / reps;

        start = clock();
        for (int r = 0; r < reps; r++)
            idx = bisStar(data, n, testKeys[i]);
        end = clock();
        tBISStar = (double)(end - start) / CLOCKS_PER_SEC * 1000.0 / reps;

        idx = bis(data, n, testKeys[i]);
        printf("%-12s  %-20ld  %-15.6f  %-15.6f  %s\n",
               keyNames[i], testKeys[i], tBIS, tBISStar,
               idx >= 0 ? data[idx].date : "ΟΧΙ");
    }

    free(data);
    return 0;
}
