# GRUP ÜYELERİ:
# G221210383 Metin Aydın
# G221210071 Bilal Avcı
# G221210089 Furkan Ay
# G221210047 Furkan Kesicioğlu
# G221210045 Mehmet Bosdancı
# Derleyici ve bayraklar
CC = gcc
CFLAGS = -I ./include/ -Wall

# Klasörler
SRC = ./src
LIB = ./lib
BIN = ./bin
INCLUDE = ./include
DOC = ./doc

# Kaynak dosyaları (.c dosyaları)
SRC_FILES = $(wildcard $(SRC)/*.c)

# Objekt dosyaları (.o dosyaları) -> lib/ altında
OBJ_FILES = $(SRC_FILES:$(SRC)/%.c=$(LIB)/%.o)

# Çıktı dosyası
EXEC = $(BIN)/shell

# Varsayılan hedef
all: $(EXEC) calistirma

# Derleme hedefi: Tüm .c dosyalarından .o dosyaları oluştur
$(LIB)/%.o: $(SRC)/%.c
	@mkdir -p $(LIB)  # lib dizini yoksa oluştur
	$(CC) $(CFLAGS) -o $@ -c $<

# Bağlantı hedefi: Tüm .o dosyalarını birleştirerek çalıştırılabilir dosya oluştur
$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ_FILES)

# Çalıştırma hedefi: Derleme tamamlandıktan sonra shell'i çalıştır
calistirma: $(EXEC)
	$(EXEC)

# Temizlik hedefi
clean:
	rm -rf $(LIB) $(EXEC)

# Yardımcı hedefler
.PHONY: all clean calistirma
