#include <pcre2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int match(const string& filename, const vector<pcre2_code*>& database) {
    FILE* fp = fopen(filename.c_str(), "r");
    if (!fp) {
        cerr << "Cannot open file: " << filename << endl;
        return 1;
    }

    // Bufor wczytujący linie. Jeśli plik ma długie linie, zwiększ rozmiar.
    vector<char> buffer(8192);

    for (pcre2_code* re : database) {
        rewind(fp); // zaczynamy od początku pliku dla każdego regexu

        while (fgets(buffer.data(), buffer.size(), fp)) {
            size_t len = strlen(buffer.data());

            // Usuń znak końca linii '\n' jeśli istnieje
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
                len--;
            }

            // Tworzymy strukturę dla wyników dopasowania
            pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, NULL);

            int rc = pcre2_match(
                re,                        // skompilowany regex
                (PCRE2_SPTR)buffer.data(), // dane wejściowe
                len,                       // długość danych
                0,                         // offset
                0,                         // flagi
                match_data,                // struktura wynikowa
                NULL                       // kontekst
            );

            if (rc >= 0) {
                PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);
                for (int i = 0; i < rc; i++) {
                    size_t start = ovector[2*i];
                    size_t end   = ovector[2*i+1];
                    if (end > start) {
                        cout << "Match: " << string(buffer.data() + start, end - start) << endl;
                    }
                }
            }

            pcre2_match_data_free(match_data);
        }
    }

    fclose(fp);
    return 0;
}
