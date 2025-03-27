//типа с-совместимая штука для работы с алгоритмами из плюсов
typedef void* CDoubleTreeHandle;
CDoubleTreeHandle createDoubleTree(int** graph, int n);
void callAlgPrima(CDoubleTreeHandle handle);
void callFleri(CDoubleTreeHandle handle); 
int* getCycle(CDoubleTreeHandle handle, int* out_size);
int getSum(CDoubleTreeHandle handle);
void freeDoubleTree(CDoubleTreeHandle handle);
