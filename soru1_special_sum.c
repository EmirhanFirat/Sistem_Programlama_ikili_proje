#include <stdio.h>
#include <stdlib.h>

/*
    special_sum fonksiyonu:
    Dinamik olarak ayrılmış tek boyutlu matris üzerinde
    ana diyagonal ve yan diyagonal elemanların toplamını hesaplar.

  
    Burada pointer aritmetiği matris yerine kullandık

        *(mat + i * cols + j)

    Bu ifade, matrisin i. satır ve j. sütunundaki elemana ulaşır.
*/
int special_sum(int *mat, int rows, int cols)
{
    int sum = 0;

    /*
        Matris üzerinde satır satır ve sütunlara pointer aritmetiği ile ulaşıyoruz
    */
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            /*
                Ana diyagonal şartı:
                    i == j

                Yan diyagonal şartı:
                    i + j == cols - 1

                || kullandığımız için merkez eleman iki kez toplanmaz.
                Çünkü eleman iki şartı da sağlasa bile if bloğu yalnızca
                bir kere çalışır.
            */
            if (i == j || i + j == cols - 1)
            {
                sum += *(mat + i * cols + j);
            }
        }
    }

    return sum;
}

int main()
{
    int n;
    int *mat;

    printf("Matris boyutunu giriniz (N): ");
    scanf("%d", &n);

    /*
        Kullanıcıdan alınan matris boyutu kontrol edilir.
        N değeri 0 veya negatif olamaz.
    */
    if (n <= 0)
    {
        printf("Hata: Matris boyutu pozitif bir değer olmalıdır.\n");
        return 1;
    }

    /*
        N x N matris için tek boyutlu bellek bloğu ayrılır.

        Toplam eleman sayısı:
            n * n

        Her eleman int türünde olduğu için:
            n * n * sizeof(int)
    */
    mat = (int *)malloc(n * n * sizeof(int));

    /*
        malloc başarısız olursa NULL döndürür.
        Bu yüzden bellek ayrılıp ayrılmadığını kontrol ediyoruz.
    */
    if (mat == NULL)
    {
        printf("Hata: Bellek tahsis edilemedi.\n");
        return 1;
    }

    printf("%d x %d matris elemanlarini giriniz:\n", n, n);

    /*
        Matris elemanları kullanıcıdan alınır.

    */
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("Eleman [%d][%d]: ", i, j);
            scanf("%d", (mat + i * n + j));
        }
    }

    /*
        Matris kare olduğu için rows = n, cols = n gönderdik.
    */
    int result = special_sum(mat, n, n);

    printf("Ana diyagonal ve yan diyagonal toplami: %d\n", result);

    
    free(mat);

    return 0;
}
