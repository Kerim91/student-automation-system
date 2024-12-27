#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Öğrenci kayıt otomasyonu - İkili arama ağacı
struct ogrenci {
    int ogrNo;
    char adSoyad[30];
    char telNo[15];
    char ePosta[50];
    char bolum[30];
    struct ogrenci* left;
    struct ogrenci* right;
};

// Yeni düğümü ağaca ekler
struct ogrenci* agacaEkle(struct ogrenci* root, struct ogrenci* eklenecekDugum) {
    if (root == NULL)
        return eklenecekDugum;
    if (eklenecekDugum->ogrNo < root->ogrNo)
        root->left = agacaEkle(root->left, eklenecekDugum);
    else
        root->right = agacaEkle(root->right, eklenecekDugum);
    return root;
}

// Yeni öğrenci ekler
struct ogrenci* ogrenciEkle(struct ogrenci* root) {
    struct ogrenci* eklenecekOgrenci = malloc(sizeof(struct ogrenci));
    if (eklenecekOgrenci == NULL) {
        printf("Bellek ayrılırken hata oluştu!\n");
        return root;
    }

    printf("Eklenecek öğrencinin:\n");
    printf("Öğrenci Numarası: ");
    scanf("%d", &eklenecekOgrenci->ogrNo);
    printf("Adı ve Soyadı: ");
    scanf(" %[^\n]%*c", eklenecekOgrenci->adSoyad);
    printf("Bölümü: ");
    scanf(" %[^\n]%*c", eklenecekOgrenci->bolum);
    printf("Telefon Numarası: ");
    scanf(" %14s", eklenecekOgrenci->telNo);
    printf("E-Posta Adresi: ");
    scanf(" %49s", eklenecekOgrenci->ePosta);

    eklenecekOgrenci->left = NULL;
    eklenecekOgrenci->right = NULL;

    return agacaEkle(root, eklenecekOgrenci);
}

// Ağacı sıralı şekilde yazdırır
void ogrencileriYazdir(struct ogrenci* root) {
    if (root != NULL) {
        ogrencileriYazdir(root->left);
        printf("-------------\n");
        printf("Öğrenci Numarası: %d\n", root->ogrNo);
        printf("Adı: %s\n", root->adSoyad);
        printf("Bölümü: %s\n", root->bolum);
        printf("Telefon: %s\n", root->telNo);
        printf("E-Posta: %s\n", root->ePosta);
        ogrencileriYazdir(root->right);
    }
}

// Öğrenciyi arar
void ogrenciAra(struct ogrenci* root, int gelenNo) {
    if (root == NULL) {
        printf("Aranan öğrenci bulunamadı.\n");
        return;
    }
    if (gelenNo < root->ogrNo)
        ogrenciAra(root->left, gelenNo);
    else if (gelenNo > root->ogrNo)
        ogrenciAra(root->right, gelenNo);
    else {
        printf("Öğrenci bulundu:\n");
        printf("Numara: %d, Ad Soyad: %s, Bölüm: %s\n", root->ogrNo, root->adSoyad, root->bolum);
        printf("Telefon: %s, E-Posta: %s\n", root->telNo, root->ePosta);
    }
}

// Minimum düğümü bulur
struct ogrenci* findMinimum(struct ogrenci* root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

// Öğrenci siler
struct ogrenci* ogrenciSil(struct ogrenci* root, int aranan) {
    if (root == NULL) {
        printf("Silmek istenen öğrenci bulunamadı.\n");
        return NULL;
    }
    if (aranan < root->ogrNo)
        root->left = ogrenciSil(root->left, aranan);
    else if (aranan > root->ogrNo)
        root->right = ogrenciSil(root->right, aranan);
    else {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        if (root->left == NULL || root->right == NULL) {
            struct ogrenci* tekCocuk = (root->left) ? root->left : root->right;
            free(root);
            return tekCocuk;
        }
        struct ogrenci* minSag = findMinimum(root->right);
        root->ogrNo = minSag->ogrNo;
        strcpy(root->adSoyad, minSag->adSoyad);
        strcpy(root->bolum, minSag->bolum);
        strcpy(root->telNo, minSag->telNo);
        strcpy(root->ePosta, minSag->ePosta);
        root->right = ogrenciSil(root->right, minSag->ogrNo);
    }
    return root;
}

// Belleği serbest bırakır
void bellekSerbestBirak(struct ogrenci* root) {
    if (root) {
        bellekSerbestBirak(root->left);
        bellekSerbestBirak(root->right);
        free(root);
    }
}

int main() {
    int secim;
    struct ogrenci* root = NULL;

    while (1) {
        printf("\n1- Öğrencileri Listele\n2- Öğrenci Ara\n3- Öğrenci Ekle\n4- Öğrenci Sil\n0- Çıkış\nSeçim: ");
        scanf("%d", &secim);
        if (secim == 0) {
            bellekSerbestBirak(root);
            printf("Çıkış yapılıyor...\n");
            break;
        }
        switch (secim) {
        case 1:
            ogrencileriYazdir(root);
            break;
        case 2:
            printf("Aranan öğrenci numarasını giriniz: ");
            int no;
            scanf("%d", &no);
            ogrenciAra(root, no);
            break;
        case 3:
            root = ogrenciEkle(root);
            break;
        case 4:
            printf("Silinecek öğrenci numarasını giriniz: ");
            scanf("%d", &no);
            root = ogrenciSil(root, no);
            break;
        default:
            printf("Hatalı giriş. Tekrar deneyin.\n");
        }
    }
    return 0;
}
