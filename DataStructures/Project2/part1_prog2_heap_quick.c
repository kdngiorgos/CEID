/*
 * Project 2 - Πρόγραμμα 2
 * Ταξινόμηση ημερομηνιών κατά αύξουσα σειρά Value
 * με Heap Sort και Quick Sort.
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
    char date[MAX_DATE];
    long value;
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
    fgets(line, MAX_LINE, fp); /* header */
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
            records[count].value = value;
            count++;
        }
    }
    fclose(fp);
    return count;
}

void copyRecords(Record *dst, Record *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

int isSorted(Record *arr, int n) {
    int i;
    for (i = 1; i < n; i++)
        if (arr[i].value < arr[i-1].value) return 0;
    return 1;
}

/* ---- Heap Sort ---- */

void heapify(Record *arr, int n, int i) {
    int largest = i;
    int left    = 2 * i + 1;
    int right   = 2 * i + 2;
    if (left  < n && arr[left].value  > arr[largest].value) largest = left;
    if (right < n && arr[right].value > arr[largest].value) largest = right;
    if (largest != i) {
        Record tmp = arr[i]; arr[i] = arr[largest]; arr[largest] = tmp;
        heapify(arr, n, largest);
    }
}

void heapSort(Record *arr, int n) {
    int i;
    for (i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (i = n - 1; i > 0; i--) {
        Record tmp = arr[0]; arr[0] = arr[i]; arr[i] = tmp;
        heapify(arr, i, 0);
    }
}

/* ---- Quick Sort (Lomuto partition) ---- */

int partition(Record *arr, int low, int high) {
    long pivot = arr[high].value;
    int i = low - 1, j;
    for (j = low; j < high; j++) {
        if (arr[j].value <= pivot) {
            i++;
            Record tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        }
    }
    Record tmp = arr[i+1]; arr[i+1] = arr[high]; arr[high] = tmp;
    return i + 1;
}

void quickSort(Record *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/* ---- Main ---- */

int main() {
    Record *data  = (Record*)malloc(MAX_RECORDS * sizeof(Record));
    Record *data2;
    int n;
    clock_t start, end;
    double tHeap, tQuick;

    printf("Ανάγνωση δεδομένων από covid_trade.csv...\n");
    n = readCSV("covid_trade.csv", data);
    if (n == 0) { free(data); return 1; }
    printf("Φορτώθηκαν %d εγγραφές.\n\n", n);

    data2 = (Record*)malloc(n * sizeof(Record));
    copyRecords(data2, data, n);

    /* Heap Sort */
    start = clock();
    heapSort(data, n);
    end = clock();
    tHeap = (double)(end - start) / CLOCKS_PER_SEC;

    /* Quick Sort */
    start = clock();
    quickSort(data2, 0, n - 1);
    end = clock();
    tQuick = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Αποτελέσματα ταξινόμησης (πρώτες 5 εγγραφές):\n");
    printf("%-15s %15s\n", "Ημερομηνία", "Value");
    printf("----------------------------------------\n");
    int i;
    for (i = 0; i < 5 && i < n; i++)
        printf("%-15s %15ld\n", data[i].date, data[i].value);

    printf("\nΈλεγχος ταξινόμησης Heap Sort:  %s\n",
           isSorted(data, n)  ? "OK" : "ΛΑΘΟΣ");
    printf("Έλεγχος ταξινόμησης Quick Sort: %s\n",
           isSorted(data2, n) ? "OK" : "ΛΑΘΟΣ");

    printf("\n--- Χρόνοι εκτέλεσης ---\n");
    printf("Heap Sort:  %.4f δευτερόλεπτα\n", tHeap);
    printf("Quick Sort: %.4f δευτερόλεπτα\n", tQuick);

    free(data);
    free(data2);
    return 0;
}
