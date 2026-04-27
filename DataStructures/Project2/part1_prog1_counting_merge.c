/*
 * Project 2 - Πρόγραμμα 1
 * Ταξινόμηση ημερομηνιών κατά αύξουσα σειρά Cumulative
 * με Counting Sort και Merge Sort.
 *
 * Αρχείο δεδομένων: covid_trade.csv (στον ίδιο φάκελο)
 * Στήλες CSV: Direction,Year,Date,Weekday,Country,Commodity,
 *             Transport_Mode,Measure,Value,Cumulative
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RECORDS 120000
#define MAX_DATE    16
#define MAX_LINE   512

typedef struct {
    char  date[MAX_DATE];
    long  cumulative;
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
    /* Παράλειψη header */
    fgets(line, MAX_LINE, fp);
    while (fgets(line, MAX_LINE, fp) && count < MAX_RECORDS) {
        /* Direction,Year,Date,Weekday,Country,Commodity,
           Transport_Mode,Measure,Value,Cumulative */
        char direction[32], weekday[16], country[64];
        char commodity[64], transport[32], measure[32];
        int  year;
        long value, cumulative;
        char date[MAX_DATE];

        /* Απλό parsing με sscanf (τα string πεδία δεν έχουν κόμμα) */
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

/* ---- Βοηθητικές ---- */

void copyRecords(Record *dst, Record *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

int isSortedByCumulative(Record *arr, int n) {
    int i;
    for (i = 1; i < n; i++)
        if (arr[i].cumulative < arr[i-1].cumulative) return 0;
    return 1;
}

/* ---- Counting Sort ----
 * Κατάλληλο όταν το εύρος τιμών είναι μικρό σε σχέση με n.
 * Εδώ βρίσκουμε min/max και χρησιμοποιούμε offset.
 * ΠΡΟΣΟΧΗ: αν το range είναι πολύ μεγάλο, χρειάζεται πολλή μνήμη.
 */
void countingSort(Record *arr, int n) {
    if (n <= 0) return;
    long minVal = arr[0].cumulative;
    long maxVal = arr[0].cumulative;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i].cumulative < minVal) minVal = arr[i].cumulative;
        if (arr[i].cumulative > maxVal) maxVal = arr[i].cumulative;
    }
    long range = maxVal - minVal + 1;

    if (range > 20000000L) {
        /* Πολύ μεγάλο εύρος - χρησιμοποιούμε normalized counting sort
         * σε 10000 buckets (bucket sort παραλλαγή) */
        int buckets = 10000;
        int *count  = (int*)calloc(buckets, sizeof(int));
        Record **bucketArr = (Record**)calloc(buckets, sizeof(Record*));
        int *bucketSize    = (int*)calloc(buckets, sizeof(int));
        int *bucketCap     = (int*)calloc(buckets, sizeof(int));
        int b;

        for (b = 0; b < buckets; b++) {
            bucketCap[b] = 16;
            bucketArr[b] = (Record*)malloc(bucketCap[b] * sizeof(Record));
        }
        for (i = 0; i < n; i++) {
            int idx = (int)(((double)(arr[i].cumulative - minVal) / range) * (buckets - 1));
            if (bucketSize[idx] == bucketCap[idx]) {
                bucketCap[idx] *= 2;
                bucketArr[idx] = (Record*)realloc(bucketArr[idx], bucketCap[idx] * sizeof(Record));
            }
            bucketArr[idx][bucketSize[idx]++] = arr[i];
        }
        /* Insertion sort ανά bucket */
        int pos = 0;
        for (b = 0; b < buckets; b++) {
            int j, k;
            for (j = 1; j < bucketSize[b]; j++) {
                Record tmp = bucketArr[b][j];
                k = j - 1;
                while (k >= 0 && bucketArr[b][k].cumulative > tmp.cumulative) {
                    bucketArr[b][k+1] = bucketArr[b][k];
                    k--;
                }
                bucketArr[b][k+1] = tmp;
            }
            for (j = 0; j < bucketSize[b]; j++)
                arr[pos++] = bucketArr[b][j];
            free(bucketArr[b]);
        }
        free(count); free(bucketArr); free(bucketSize); free(bucketCap);
        return;
    }

    int *count = (int*)calloc((size_t)range, sizeof(int));
    Record *output = (Record*)malloc(n * sizeof(Record));

    for (i = 0; i < n; i++)
        count[arr[i].cumulative - minVal]++;
    /* Prefix sum */
    for (i = 1; i < (int)range; i++)
        count[i] += count[i-1];
    /* Σταθερή έξοδος */
    for (i = n - 1; i >= 0; i--) {
        int idx = arr[i].cumulative - minVal;
        output[--count[idx]] = arr[i];
    }
    for (i = 0; i < n; i++) arr[i] = output[i];

    free(count);
    free(output);
}

/* ---- Merge Sort ---- */

void merge(Record *arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Record *L = (Record*)malloc(n1 * sizeof(Record));
    Record *R = (Record*)malloc(n2 * sizeof(Record));
    int i, j, k;
    for (i = 0; i < n1; i++) L[i] = arr[left + i];
    for (j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    i = 0; j = 0; k = left;
    while (i < n1 && j < n2) {
        if (L[i].cumulative <= R[j].cumulative)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}

void mergeSort(Record *arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

/* ---- Main ---- */

int main() {
    Record *data = (Record*)malloc(MAX_RECORDS * sizeof(Record));
    Record *data2;
    int n;
    clock_t start, end;
    double tCounting, tMerge;

    printf("Ανάγνωση δεδομένων από covid_trade.csv...\n");
    n = readCSV("covid_trade.csv", data);
    if (n == 0) { free(data); return 1; }
    printf("Φορτώθηκαν %d εγγραφές.\n\n", n);

    data2 = (Record*)malloc(n * sizeof(Record));
    copyRecords(data2, data, n);

    /* Counting Sort */
    start = clock();
    countingSort(data, n);
    end = clock();
    tCounting = (double)(end - start) / CLOCKS_PER_SEC;

    /* Merge Sort */
    start = clock();
    mergeSort(data2, 0, n - 1);
    end = clock();
    tMerge = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Αποτελέσματα ταξινόμησης (πρώτες 5 εγγραφές):\n");
    printf("%-15s %15s\n", "Ημερομηνία", "Cumulative");
    printf("----------------------------------------\n");
    int i;
    for (i = 0; i < 5 && i < n; i++)
        printf("%-15s %15ld\n", data[i].date, data[i].cumulative);

    printf("\nΈλεγχος ταξινόμησης Counting Sort: %s\n",
           isSortedByCumulative(data, n) ? "OK" : "ΛΑΘΟΣ");
    printf("Έλεγχος ταξινόμησης Merge Sort:    %s\n",
           isSortedByCumulative(data2, n) ? "OK" : "ΛΑΘΟΣ");

    printf("\n--- Χρόνοι εκτέλεσης ---\n");
    printf("Counting Sort: %.4f δευτερόλεπτα\n", tCounting);
    printf("Merge Sort:    %.4f δευτερόλεπτα\n", tMerge);

    free(data);
    free(data2);
    return 0;
}
