#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <vector>

// Funkce pro otevření souboru
string otevri_soubor(const string& jmeno_souboru)
{
	// Implementace funkce pro otevření souboru
	ifstream vstupni_soubor(jmeno_souboru, ios::binary);
	if (!vstupni_soubor)
	{
		cout << "Nemohu otevrit soubor: " << jmeno_souboru << endl;
		return "";
	}
	string obsah, radek;
	/* delalo mi to chybu u XOR sifry z nejakeho duvodu mi to vypisovalo y na konci desifrace
	while (getline(vstupni_soubor, radek))
		obsah += radek + "\n";
	}
	*/
	obsah = string((istreambuf_iterator<char>(vstupni_soubor)), istreambuf_iterator<char>());
	return obsah;

}

// Funkce pro Caesarovu šifru
string caesar_sifra(const string& text, int posun, bool sifrovat)
{
	// Implementace Caesarovy šifry
	// sifrovat = true pro šifrování, sifrovat = false pro dešifrování
	string vysledek = "";
	vysledek = text;

	for (char& znak : vysledek)
	{
		if (isalpha(znak))
		{
			char zaklad = islower(znak) ? 'a' : 'A';
			znak = zaklad + (znak - zaklad + (sifrovat ? posun : 26 - posun)) % 26;
		}
	}
	return vysledek;
}

// Funkce pro Vigenerovu šifru
string vigener_sifra(const string& text, const string& klic, bool sifrovat)
{
	// Implementace Vigenerovy šifry
	// sifrovat = true pro šifrování, sifrovat = false pro dešifrování
	string vysledek = text;
	size_t klicIndex = 0;
	size_t klicDelka = klic.length();

	for (char& znak : vysledek)
	{
		if (isalpha(znak))
		{
			char zaklad = islower(znak) ? 'a' : 'A';
			char k = islower(klic[klicIndex]) ? (klic[klicIndex] - 'a') : (klic[klicIndex] - 'A');
			znak = zaklad + (znak - zaklad + (sifrovat ? k : 26 - k)) % 26;
			klicIndex = (klicIndex + 1) % klicDelka;
		}
	}
	return vysledek;
}

// Funkce pro XOR šifru
string xor_sifra(const string& text, const string& klic, bool sifrovat)
{
	// Implementace XOR šifry
	// sifrovat = true pro šifrování, sifrovat = false pro dešifrování
	string vysledek = text;

	for (int i = 0; i < text.length(); ++i)
	{
		vysledek[i] = text[i] ^ klic[i % klic.length()];
	}
	return vysledek;
}

vector<vector<char>> generatePlayfairKeySquare(const string& klic)  // vytvoreni klice
{
	vector<vector<char>> vytvoreniKlice(5, vector<char>(5));
	string upravaKlice;
	bool existuje[26] = { false };
	// kontrola zda se znak v klici neopakuje
	for (char c : klic)
	{
		if (!existuje[c - 'a'] && c != 'j') {
			upravaKlice += c;
			existuje[c - 'a'] = true;
		}
	}

	// kontrola zda se znaky neopakuji
	for (char c = 'a'; c <= 'z'; ++c)
	{
		if (!existuje[c - 'a'] && c != 'j')
		{
			upravaKlice += c;
			existuje[c - 'a'] = true;
		}
	}
	// vytvoreni matice 5x5
	int k = 0;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			vytvoreniKlice[i][j] = upravaKlice[k++];
		}
	}

	return vytvoreniKlice;
}

string playfairCipher(const string& text, const vector<vector<char>>& vytvoreniKlice, bool sifrovat)
{
    string sifra;
    string upravaTextu = text;
    if (sifrovat)
    {
        // Ensure the text length is even by adding 'x' if necessary
        if (upravaTextu.length() % 2 != 0)
        {
            upravaTextu += 'x';
        }
        // Encrypting
        for (size_t i = 0; i < upravaTextu.length(); i += 2)
        {
            if (upravaTextu[i] == upravaTextu[i + 1])
            {
                upravaTextu.insert(i + 1, 1, 'x');
            }
            char a = upravaTextu[i];
            char b = upravaTextu[i + 1];
            int row1 = 0, col1 = 0, row2 = 0, col2 = 0;
            for (int row = 0; row < 5; ++row)
            {
                for (int col = 0; col < 5; ++col)
                {
                    if (vytvoreniKlice[row][col] == a)
                    {
                        row1 = row;
                        col1 = col;
                    }
                    if (vytvoreniKlice[row][col] == b)
                    {
                        row2 = row;
                        col2 = col;
                    }
                }
            }
            if (row1 == row2)
            {
                sifra += vytvoreniKlice[row1][(col1 + 1) % 5];
                sifra += vytvoreniKlice[row2][(col2 + 1) % 5];
            }
            else if (col1 == col2)
            {
                sifra += vytvoreniKlice[(row1 + 1) % 5][col1];
                sifra += vytvoreniKlice[(row2 + 1) % 5][col2];
            }
            else
            {
                sifra += vytvoreniKlice[row1][col2];
                sifra += vytvoreniKlice[row2][col1];
            }
        }
    }
    else
    {
        // Decrypting
        for (size_t i = 0; i < upravaTextu.length(); i += 2)
        {
            char a = upravaTextu[i];
            char b = upravaTextu[i + 1];
            int row1 = 0, col1 = 0, row2 = 0, col2 = 0;
            for (int row = 0; row < 5; ++row)
            {
                for (int col = 0; col < 5; ++col)
                {
                    if (vytvoreniKlice[row][col] == a)
                    {
                        row1 = row;
                        col1 = col;
                    }
                    if (vytvoreniKlice[row][col] == b)
                    {
                        row2 = row;
                        col2 = col;
                    }
                }
            }
            if (row1 == row2)
            {
                sifra += vytvoreniKlice[row1][(col1 + 4) % 5];
                sifra += vytvoreniKlice[row2][(col2 + 4) % 5];
            }
            else if (col1 == col2)
            {
                sifra += vytvoreniKlice[(row1 + 4) % 5][col1];
                sifra += vytvoreniKlice[(row2 + 4) % 5][col2];
            }
            else
            {
                sifra += vytvoreniKlice[row1][col2];
                sifra += vytvoreniKlice[row2][col1];
            }
        }
    }
    return sifra;
}

void uloz_do_souboru(const string& jmeno_souboru, const string& obsah)
{
	// Implementace funkce pro uložení řetězce do souboru
	ofstream vystupni_soubor(jmeno_souboru, ios::binary);
	if (!vystupni_soubor)
	{
		cout << "Nemohu otevrit soubor: " << jmeno_souboru << endl;
	}
	vystupni_soubor << obsah;
}


int main()
{
    // Načtení vstupního souboru
    string vstupni_text = otevri_soubor("vstup.txt");

    //kontrola zda je soubor prazdny nebo zda existuje
    if (vstupni_text.empty())
    {
        cout << "Soubor je prazdny nebo neexistuje." << endl;
    }
    else
    {
        cout << "Nacteny text: " << vstupni_text << endl;
        cout << "" << endl;
    }

    // Šifrování textu pomocí Caesarovy šifry
    string sifrovany_text_caesar = caesar_sifra(vstupni_text, 3, true);
    cout << "Ceaser sifra: " << sifrovany_text_caesar << endl;

    // Šifrování textu pomocí Vigenerovy šifry
    string sifrovany_text_vigener = vigener_sifra(vstupni_text, "tajny_klic", true);
    cout << "Vigener sifra: " << sifrovany_text_vigener << endl;

    // Šifrování textu pomocí XOR šifry
    string sifrovany_text_xor = xor_sifra(vstupni_text, "heslo", true);
    cout << "Xor sifra: " << sifrovany_text_xor << endl;

    // Šifrování textu pomocí Playfairovy šifry
    vector<vector<char>> playfairKlic = generatePlayfairKeySquare("klic");
    string sifrovany_text_playfair = playfairCipher(vstupni_text, playfairKlic, true);
    cout << "Playfair sifra: " << sifrovany_text_playfair << endl;

    // Uložení šifrovaných textů do souborů
    uloz_do_souboru("sifrovany_caesar.txt", sifrovany_text_caesar);
    uloz_do_souboru("sifrovany_vigener.txt", sifrovany_text_vigener);
    uloz_do_souboru("sifrovany_xor.txt", sifrovany_text_xor);
    uloz_do_souboru("sifrovany_playfair.txt", sifrovany_text_playfair);

    // Dešifrování textů
    cout << "\nDesifrovany text pomoci Caesarovy sifry: " << caesar_sifra(otevri_soubor("sifrovany_caesar.txt"), 3, false) << endl;
    cout << "Desifrovany text pomoci Vigenerovy sifry: " << vigener_sifra(otevri_soubor("sifrovany_vigener.txt"), "tajny_klic", false) << endl;
    cout << "Desifrovany text pomoci XOR sifry: " << xor_sifra(otevri_soubor("sifrovany_xor.txt"), "heslo", false) << endl;
    

    return 0;
    
}
