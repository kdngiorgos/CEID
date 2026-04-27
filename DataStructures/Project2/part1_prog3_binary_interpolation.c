/*
 * Project 2 - Πρόγραμμα 3
 * Αναζήτηση Value/Cumulative για δοθείσα ημερομηνία.
 * Υλοποιεί Binary Search και Interpolation Search.
 * Το αρχείο ταξινομείται πρώτα κατά Date (αλφαριθμητικά YYYY-MM-DD).
 *
 * Αρχείο δεδομένων: covid_trade.csv (στον ίδιο φάκελο)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RECORDS 120000
#define MAX_DATE    16
#define MAX_LINE   512

typedef struct {
    char date[MAX_DATE];     /* αποθηκεύεται ως DD/MM/YYYY */
    char dateSort[MAX_DATE]; /* YYYYMMDD για σύγκριση */
    long value;
    long cumulative;
} Record;

/* Μετατροπή DD/MM/YYYY σε YYYYMMDD string για λεξικογραφική σύγκριση */
void toSortable(const char *date, char *out) {
    int d, m, y;
    sscanf(date, "%d/%d/%d", &d, &m, &y);
    sprintf(out, "%04d%02d%02d", y, m, d);
}

/* Μετατροπή YYYYMMDD string σε αριθμό (για interpolation) */
long dateToNum(const char *sortableDate) {
    long v = 0;
    int i;
    for (i = 0; sortableDate[i]; i++)
        v = v * 10 + (sortableDate[i] - '0');
    return v;
}

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
            toSortable(date, records[count].dateSort);
            records[count].value = value;
            records[count].cumulative = cumulative;
            count++;
        }
    }
    fclose(fp);
    return count;
}

/* Comparator για qsort */
int cmpDate(const void *a, const void *b) {
    return strcmp(((Record*)a)->dateSort, ((Record*)b)->dateSort);
}

/* ---- Binary Search ---- */
/* Επιστρέφει τον πρώτο δείκτη με dateSort == target, ή -1 */
int binarySearch(Record *arr, int n, const char *target) {
    int lo = 0, hi = n - 1, result = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int cmp = strcmp(arr[mid].dateSort, target);
        if (cmp == 0) { result = mid; hi = mid - 1; } /* βρες πρώτο */
        else if (cmp < 0) lo = mid + 1;
        else hi = mid - 1;
    }
    return result;
}

/* ---- Interpolation Search ---- */
int interpolationSearch(Record *arr, int n, const char *target) {
    int lo = 0, hi = n - 1;
    long targetNum = dateToNum(target);
    while (lo <= hi &&
           targetNum >= dateToNum(arr[lo].dateSort) &&
           targetNum <= dateToNum(arr[hi].dateSort)) {
        long loNum = dateToNum(arr[lo].dateSort);
        long hiNum = dateToNum(arr[hi].dateSort);
        if (loNum == hiNum) {
            if (strcmp(arr[lo].dateSort, target) == 0) return lo;
            return -1;
        }
        int pos = lo + (int)(((double)(targetNum - loNum) / (hiNum - loNum)) * (hi - lo));
        if (pos < lo) pos = lo;
        if (pos > hi) pos = hi;

        int cmp = strcmp(arr[pos].dateSort, target);
        if (cmp == 0) {
            /* βρίσκουμε πρώτη εμφάνιση */
            while (pos > 0 && strcmp(arr[pos-1].dateSort, target) == 0) pos--;
            return pos;
        } else if (cmp < 0) lo = pos + 1;
        else hi = pos - 1;
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

    /* Ταξινόμηση κατά ημερομηνία */
    qsort(data, n, sizeof(Record), cmpDate);
    printf("Δεδομένα ταξινομημένα κατά ημερομηνία.\n\n");

    /* Δοκιμαστικές ημερομηνίες */
    char *testDates[] = {"01/01/2020", "15/03/2021", "01/07/2019", "15/12/2021"};
    int numTests = 4;
    int i, idx;
    clock_t start, end;
    double tBin, tInterp;

    printf("%-15s  %-15s  %-15s  %-12s  %-12s\n",
           "Ημερομηνία", "Value", "Cumulative", "Binary(ms)", "Interp(ms)");
    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < numTests; i++) {
        char sortable[MAX_DATE];
        toSortable(testDates[i], sortable);

        /* Binary Search - χρόνος */
        start = clock();
        int reps;
        for (reps = 0; reps < 1000; reps++)
            idx = binarySearch(data, n, sortable);
        end = clock();
        tBin = (double)(end - start) / CLOCKS_PER_SEC * 1000.0 / 1000.0;

        /* Interpolation Search - χρόνος */
        start = clock();
        for (reps = 0; reps < 1000; reps++)
            idx = interpolationSearch(data, n, sortable);
        end = clock();
        tInterp = (double)(end - start) / CLOCKS_PER_SEC * 1000.0 / 1000.0;

        idx = binarySearch(data, n, sortable);
        if (idx >= 0) {
            printf("%-15s  %-15ld  %-15ld  %-12.4f  %-12.4f\n",
                   testDates[i], data[idx].value, data[idx].cumulative, tBin, tInterp);
        } else {
            printf("%-15s  %-15s  %-15s  %-12.4f  %-12.4f\n",
                   testDates[i], "Δεν βρέθηκε", "-", tBin, tInterp);
        }
    }
    free(data);
    return 0;
}
