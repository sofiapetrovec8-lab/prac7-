#include <iostream>
using namespace std;

const int MAX_N = 10000;

struct Stats {
    long long comparisons;
    long long writes;
    long long merges;
    int maxDepth;
};

void resetStats(Stats& s) {
    s.comparisons = 0;
    s.writes = 0;
    s.merges = 0;
    s.maxDepth = 0;
}

void copyArray(const int source[], int target[], int n) {
    for (int i = 0; i < n; i++) target[i] = source[i];
}

void printArray(const int a[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << a[i];
        if (i < n - 1) cout << ", ";
    }
    cout << "]";
}

bool isSorted(const int a[], int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) return false;
    }
    return true;
}

long long sumArray(const int a[], int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) sum += a[i];
    return sum;
}

long long sumSquares(const int a[], int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) sum += 1LL * a[i] * a[i];
    return sum;
}

bool sameContent(const int a[], const int b[], int n) {
    return sumArray(a, n) == sumArray(b, n) &&
        sumSquares(a, n) == sumSquares(b, n);
}

// Злиття двох сусідніх упорядкованих частин масиву
void mergeParts(int a[], int temp[], int left, int mid, int right,
    Stats& s, bool trace, int depth) {
    int i = left;
    int j = mid + 1;
    int k = left;

    s.merges++;

    if (trace) {
        cout << "Depth " << depth << ", merge ["
            << left << ".." << mid << "] and ["
            << mid + 1 << ".." << right << "]\n";
        cout << "Left part: ";
        printArray(a + left, mid - left + 1);
        cout << "\nRight part: ";
        printArray(a + mid + 1, right - mid);
        cout << "\n";
    }

    // Вибір меншого елемента; за рівності першим береться лівий
    while (i <= mid && j <= right) {
        s.comparisons++;
        if (a[i] <= a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
        s.writes++;
    }

    // Копіювання залишку лівої частини
    while (i <= mid) {
        temp[k++] = a[i++];
        s.writes++;
    }

    // Копіювання залишку правої частини
    while (j <= right) {
        temp[k++] = a[j++];
        s.writes++;
    }

    // Перенесення об'єднаної ділянки назад в основний масив
    for (int p = left; p <= right; p++) {
        a[p] = temp[p];
        s.writes++;
    }

    if (trace) {
        cout << "Merged result: ";
        printArray(a + left, right - left + 1);
        cout << "\n";
    }
}

// Рекурсивний поділ і сортування злиттям
void mergeSortRec(int a[], int temp[], int left, int right,
    Stats& s, int depth, bool trace) {
    if (depth > s.maxDepth) s.maxDepth = depth;
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    if (trace) {
        cout << "Depth " << depth << ", split ["
            << left << ".." << right << "] at " << mid << "\n";
    }

    mergeSortRec(a, temp, left, mid, s, depth + 1, trace);
    mergeSortRec(a, temp, mid + 1, right, s, depth + 1, trace);
    mergeParts(a, temp, left, mid, right, s, trace, depth);
}

// Запуск MergeSort з повторним використанням допоміжного масиву
void mergeSort(int a[], int n, Stats& s, bool trace = false) {
    resetStats(s);
    int temp[MAX_N];
    if (n > 0) mergeSortRec(a, temp, 0, n - 1, s, 1, trace);
}

// Сортування вставками
void insertionSort(int a[], int n, Stats& s) {
    resetStats(s);
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0) {
            s.comparisons++;
            if (a[j] > key) {
                a[j + 1] = a[j];
                s.writes++;
                j--;
            }
            else break;
        }
        a[j + 1] = key;
        s.writes++;
    }
}

// Сортування вибором
void selectionSort(int a[], int n, Stats& s) {
    resetStats(s);
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            s.comparisons++;
            if (a[j] < a[minIndex]) minIndex = j;
        }
        if (minIndex != i) {
            int temp = a[i]; a[i] = a[minIndex]; a[minIndex] = temp;
            s.writes += 3;
        }
    }
}

// Бульбашкове сортування
void bubbleSort(int a[], int n, Stats& s) {
    resetStats(s);
    for (int i = 0; i < n - 1; i++) {
        bool changed = false;
        for (int j = 0; j < n - 1 - i; j++) {
            s.comparisons++;
            if (a[j] > a[j + 1]) {
                int temp = a[j]; a[j] = a[j + 1]; a[j + 1] = temp;
                s.writes += 3;
                changed = true;
            }
        }
        if (!changed) break;
    }
}

// Сортування Шелла з послідовністю проміжків n/2
void shellSort(int a[], int n, Stats& s) {
    resetStats(s);
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = a[i];
            int j = i;
            while (j >= gap) {
                s.comparisons++;
                if (a[j - gap] > temp) {
                    a[j] = a[j - gap];
                    s.writes++;
                    j -= gap;
                }
                else break;
            }
            a[j] = temp;
            s.writes++;
        }
    }
}

// Відновлення властивості максимальної купи
void heapify(int a[], int n, int i, Stats& s) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n) {
        s.comparisons++;
        if (a[left] > a[largest]) largest = left;
    }
    if (right < n) {
        s.comparisons++;
        if (a[right] > a[largest]) largest = right;
    }
    if (largest != i) {
        int temp = a[i]; a[i] = a[largest]; a[largest] = temp;
        s.writes += 3;
        heapify(a, n, largest, s);
    }
}

// Сортування купою
void heapSort(int a[], int n, Stats& s) {
    resetStats(s);
    for (int i = n / 2 - 1; i >= 0; i--) heapify(a, n, i, s);
    for (int end = n - 1; end > 0; end--) {
        int temp = a[0]; a[0] = a[end]; a[end] = temp;
        s.writes += 3;
        heapify(a, end, 0, s);
    }
}

// Розбиття ділянки для QuickSort
int partitionArray(int a[], int low, int high, Stats& s) {
    // Вибір середнього елемента як опорного, щоб уникнути крайніх поділів на впорядкованих даних
    int middle = low + (high - low) / 2;
    if (middle != high) {
        int swapTemp = a[middle]; a[middle] = a[high]; a[high] = swapTemp;
        s.writes += 3;
    }
    int pivot = a[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        s.comparisons++;
        if (a[j] <= pivot) {
            i++;
            if (i != j) {
                int temp = a[i]; a[i] = a[j]; a[j] = temp;
                s.writes += 3;
            }
        }
    }
    if (i + 1 != high) {
        int temp = a[i + 1]; a[i + 1] = a[high]; a[high] = temp;
        s.writes += 3;
    }
    return i + 1;
}

// Рекурсивне сортування QuickSort
void quickSortRec(int a[], int low, int high, Stats& s, int depth) {
    if (depth > s.maxDepth) s.maxDepth = depth;
    if (low < high) {
        int p = partitionArray(a, low, high, s);
        quickSortRec(a, low, p - 1, s, depth + 1);
        quickSortRec(a, p + 1, high, s, depth + 1);
    }
}

// Запуск QuickSort
void quickSort(int a[], int n, Stats& s) {
    resetStats(s);
    if (n > 0) quickSortRec(a, 0, n - 1, s, 1);
}

// Перевірка окремого злиття двох упорядкованих частин
bool testMerge(const int left[], int nl, const int right[], int nr,
    const int expected[], int testNo) {
    int a[MAX_N], temp[MAX_N];
    int n = nl + nr;
    for (int i = 0; i < nl; i++) a[i] = left[i];
    for (int i = 0; i < nr; i++) a[nl + i] = right[i];

    Stats s;
    resetStats(s);
    mergeParts(a, temp, 0, nl - 1, n - 1, s, false, 1);

    bool ok = isSorted(a, n);
    for (int i = 0; i < n && ok; i++) {
        if (a[i] != expected[i]) ok = false;
    }

    // Порожні частини перевіряються окремо, бо в них немає меж для mergeParts
    if (n == 0) ok = true;

    cout << "Merge test " << testNo << ": "
        << (ok ? "OK" : "ERROR") << "\n";
    return ok;
}

// Перевірка повного сортування MergeSort
bool testMergeSort(const int input[], int n, const int expected[], int testNo) {
    int a[MAX_N], original[MAX_N];
    copyArray(input, a, n);
    copyArray(input, original, n);

    Stats s;
    mergeSort(a, n, s);

    bool ok = isSorted(a, n) && sameContent(original, a, n);
    for (int i = 0; i < n && ok; i++) {
        if (a[i] != expected[i]) ok = false;
    }

    cout << "MergeSort test " << testNo << ": "
        << (ok ? "OK" : "ERROR") << "\n";
    return ok;
}

// Формування наборів для порівняльного експерименту
void makeData(int a[], int n, int type) {
    for (int i = 0; i < n; i++) {
        if (type == 0) a[i] = i;
        else if (type == 1) a[i] = n - i;
        else if (type == 2) a[i] = (i % 10 == 0) ? n - i : i;
        else a[i] = (i * 37 + 11) % (n + 17);
    }
}

// Запуск алгоритму на копії даних і виведення статистики
void runAndPrint(const char* name, const int source[], int n, int method) {
    int a[MAX_N];
    copyArray(source, a, n);
    Stats s;
    if (method == 0) insertionSort(a, n, s);
    else if (method == 1) selectionSort(a, n, s);
    else if (method == 2) bubbleSort(a, n, s);
    else if (method == 3) shellSort(a, n, s);
    else if (method == 4) quickSort(a, n, s);
    else if (method == 5) heapSort(a, n, s);
    else mergeSort(a, n, s);

    cout << name << "; " << s.comparisons << "; " << s.writes
        << "; " << s.maxDepth << "; "
        << (isSorted(a, n) && sameContent(source, a, n) ? "OK" : "ERROR")
        << "\n";
}

// Порівняння алгоритмів на заданих розмірах і типах масивів
void experiment() {
    const int sizes[] = { 100, 1000, 10000 };
    const char* types[] = { "ascending", "descending", "nearly sorted", "mixed" };

    cout << "\nALGORITHM COMPARISON\n";
    cout << "n; data type; algorithm; comparisons; writes/swaps; max depth; check\n";

    for (int si = 0; si < 3; si++) {
        int n = sizes[si];
        for (int type = 0; type < 4; type++) {
            int data[MAX_N];
            makeData(data, n, type);
            cout << "\nDATA: " << n << "; " << types[type] << "\n";

            // Квадратичні алгоритми запускаються лише на малих наборах
            if (n <= 1000) {
                runAndPrint("Insertion", data, n, 0);
                runAndPrint("Selection", data, n, 1);
                runAndPrint("Bubble", data, n, 2);
            }
            else {
                cout << "Insertion; skipped at n=10000\n";
                cout << "Selection; skipped at n=10000\n";
                cout << "Bubble; skipped at n=10000\n";
            }

            runAndPrint("Shell", data, n, 3);
            runAndPrint("QuickSort", data, n, 4);
            runAndPrint("HeapSort", data, n, 5);
            runAndPrint("MergeSort", data, n, 6);
        }
    }
}

int main() {
    cout << "PRACTICAL WORK 7. MERGESORT\n\n";
    cout << "VARIANT 18\n";

    const int variant[] = {
        37, 13, 24, 40, 9, 30, 17, 34,
        11, 21, 38, 8, 28, 15, 23, 32
    };
    const int n = 16;
    int a[MAX_N], original[MAX_N];
    copyArray(variant, a, n);
    copyArray(variant, original, n);

    cout << "Initial array: ";
    printArray(a, n);
    cout << "\n\nMERGESORT TRACE\n";
    Stats mergeStats;
    mergeSort(a, n, mergeStats, true);

    cout << "\nSorted array: ";
    printArray(a, n);
    cout << "\nComparisons: " << mergeStats.comparisons
        << "\nWrites: " << mergeStats.writes
        << "\nMerges: " << mergeStats.merges
        << "\nMaximum recursion depth: " << mergeStats.maxDepth
        << "\nOrder and content check: "
        << (isSorted(a, n) && sameContent(original, a, n) ? "OK" : "ERROR")
        << "\n\n";

    // Окремі тести злиття; частини мають бути впорядковані
    const int l1[] = { 1, 3 };
    const int r1[] = { 2, 4 };
    const int e1[] = { 1, 2, 3, 4 };

    const int l2[] = { 1, 2, 3 };
    const int r2[] = { 4 };
    const int e2[] = { 1, 2, 3, 4 };

    const int l3[] = { 5, 7 };
    const int r3[] = { 1, 2 };
    const int e3[] = { 1, 2, 5, 7 };

    const int l4[] = { 1, 2, 2 };
    const int r4[] = { 2, 3 };
    const int e4[] = { 1, 2, 2, 2, 3 };

    const int l5[] = { -5, -1 };
    const int r5[] = { -3, 0 };
    const int e5[] = { -5, -3, -1, 0 };

    const int* l6 = nullptr;
    const int r6[] = { 1, 2, 3, 4 };
    const int e6[] = { 1, 2, 3, 4 };

    int mergePassed = 0;
    mergePassed += testMerge(l1, 2, r1, 2, e1, 1);
    mergePassed += testMerge(l2, 3, r2, 1, e2, 2);
    mergePassed += testMerge(l3, 2, r3, 2, e3, 3);
    mergePassed += testMerge(l4, 3, r4, 2, e4, 4);
    mergePassed += testMerge(l5, 2, r5, 2, e5, 5);
    mergePassed += testMerge(l6, 0, r6, 4, e6, 6);
    cout << "Successful merge tests: " << mergePassed << "/6\n\n";

    // Тести повного сортування MergeSort
    const int* t1 = nullptr;
    const int* e0 = nullptr;
    const int t2[] = { 7 };
    const int e2s[] = { 7 };
    const int t3[] = { 1, 2 };
    const int e3s[] = { 1, 2 };
    const int t4[] = { 2, 1 };
    const int e4s[] = { 1, 2 };
    const int t5[] = { 1, 2, 3, 4, 5 };
    const int e5s[] = { 1, 2, 3, 4, 5 };
    const int t6[] = { 5, 4, 3, 2, 1 };
    const int e6s[] = { 1, 2, 3, 4, 5 };
    const int t7[] = { 4, 1, 5, 2, 3 };
    const int e7s[] = { 1, 2, 3, 4, 5 };
    const int t8[] = { 3, 1, 3, 2, 1 };
    const int e8s[] = { 1, 1, 2, 3, 3 };
    const int t9[] = { 0, -5, 8, -2, 4 };
    const int e9s[] = { -5, -2, 0, 4, 8 };
    const int t10[] = { 6, 6, 6, 6 };
    const int e10s[] = { 6, 6, 6, 6 };
    const int t11[] = { 9, -1, 9, 0, -1, 5 };
    const int e11s[] = { -1, -1, 0, 5, 9, 9 };
    const int t12[] = { 8, 3, 7, 2, 6, 1, 5 };
    const int e12s[] = { 1, 2, 3, 5, 6, 7, 8 };
    const int t13[] = { 12, 4, 7, 4, 0, -3, 8, 2, 1, 9, 5, 6, 3, 11, 10 };
    const int e13s[] = { -3, 0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    const int t14[] = { 2147483647, 0, -2147483647, 4 };
    const int e14s[] = { -2147483647, 0, 4, 2147483647 };

    int mergeSortPassed = 0;
    mergeSortPassed += testMergeSort(t1, 0, e0, 1);
    mergeSortPassed += testMergeSort(t2, 1, e2s, 2);
    mergeSortPassed += testMergeSort(t3, 2, e3s, 3);
    mergeSortPassed += testMergeSort(t4, 2, e4s, 4);
    mergeSortPassed += testMergeSort(t5, 5, e5s, 5);
    mergeSortPassed += testMergeSort(t6, 5, e6s, 6);
    mergeSortPassed += testMergeSort(t7, 5, e7s, 7);
    mergeSortPassed += testMergeSort(t8, 5, e8s, 8);
    mergeSortPassed += testMergeSort(t9, 5, e9s, 9);
    mergeSortPassed += testMergeSort(t10, 4, e10s, 10);
    mergeSortPassed += testMergeSort(t11, 6, e11s, 11);
    mergeSortPassed += testMergeSort(t12, 7, e12s, 12);
    mergeSortPassed += testMergeSort(t13, 15, e13s, 13);
    mergeSortPassed += testMergeSort(t14, 4, e14s, 14);
    cout << "Successful MergeSort tests: " << mergeSortPassed << "/14\n\n";

    cout << "COMPARISON ON VARIANT 18\n";
    cout << "Algorithm; comparisons; writes/swaps; max depth; check\n";
    runAndPrint("Insertion", variant, n, 0);
    runAndPrint("Selection", variant, n, 1);
    runAndPrint("Bubble", variant, n, 2);
    runAndPrint("Shell", variant, n, 3);
    runAndPrint("QuickSort", variant, n, 4);
    runAndPrint("HeapSort", variant, n, 5);
    runAndPrint("MergeSort", variant, n, 6);

    experiment();
    return 0;
}
