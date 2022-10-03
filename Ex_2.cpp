#include <iostream>
#include <fstream>
#include <string>
#include <random>

class Hash_Table
{
private:

	class pair
	{
	public:
		std::string cheie = "";
		std::string sir = "";
		bool este_sters = true;
	};

	pair* vector_de_perechi;
	int dimensiune_tabela;
	int factor_de_incarcare;
	int nr_elemente;
	int nr_elemente_sterse;

	unsigned int fnv_hash(std::string sir)
	{
		unsigned int len = sir.size();
		unsigned int h = 2166136261;
		for (int i = 0; i < len; i++)
		{
			h = (h * 16777619) ^ sir[i];
		}
		return h;
	}

	unsigned int functie_de_repartizare_numere(int numar)
	{
		return 1 + numar % (dimensiune_tabela - 1);
	}

	void rehashing()
	{
		Hash_Table nou(2 * dimensiune_tabela);
		for (int i = 0; i < dimensiune_tabela; i++)
		{
			nou.vector_de_perechi[i] = vector_de_perechi[i];
		}
		vector_de_perechi = new pair[2 * dimensiune_tabela];
		dimensiune_tabela *= 2;
		nr_elemente = 0;
		nr_elemente_sterse = 0;
		for (int i = 0; i < dimensiune_tabela / 2; i++)
		{
			if (nou.vector_de_perechi[i].cheie != "" && nou.vector_de_perechi[i].este_sters == false)
			{
				inserare(nou.vector_de_perechi[i].cheie, nou.vector_de_perechi[i].sir);
			}
		}
	}


public:

	Hash_Table(int dimensiune = 10)
	{
		vector_de_perechi = new pair[dimensiune];
		dimensiune_tabela = dimensiune;
		factor_de_incarcare = 0;
		nr_elemente = 0;
		nr_elemente_sterse = 0;
	}

	~Hash_Table()
	{
		delete[] vector_de_perechi;
	}

	void stergere(int pos)
	{
		vector_de_perechi[pos].este_sters = true;
		nr_elemente_sterse++;
		nr_elemente--;
		double factor_de_stergere = (double)nr_elemente_sterse / dimensiune_tabela;
		if (factor_de_stergere >= 0.3)
		{
			rehashing();
		}
	}

	int cautare(std::string cheie, std::string sir = "")
	{
		pair p;
		p.cheie = cheie;
		int teste = 0, j;
		j = fnv_hash(cheie) % dimensiune_tabela;
		if (vector_de_perechi[j].cheie == cheie && vector_de_perechi[j].este_sters == false)
		{
			if (sir == "")
			{
				return j;
			}
			else if (vector_de_perechi[j].sir == sir)
			{
				return j;
			}
		}
		do
		{
			j = functie_de_repartizare_numere(j) % dimensiune_tabela;
			if (vector_de_perechi[j].cheie == cheie && vector_de_perechi[j].sir == sir && vector_de_perechi[j].este_sters == false)
			{
				if (sir == "")
				{
					return j;
				}
				else if (vector_de_perechi[j].sir == sir)
				{
					return j;
				}
			}
			teste++;

		} while (vector_de_perechi[j].cheie != cheie && teste != dimensiune_tabela);

		return -1;
	}

	void inserare(std::string cheie, std::string sir = "")
	{
		int j;
		pair p;
		j = fnv_hash(cheie) % dimensiune_tabela;
		if (vector_de_perechi[j].cheie == "" && vector_de_perechi[j].este_sters == true)
		{
			p.cheie = cheie;
			p.sir = sir;
			p.este_sters = false;
			vector_de_perechi[j] = p;
			nr_elemente++;
		}
		else if (vector_de_perechi[j].cheie == cheie)
		{
			return;
		}
		else
		{
			do
			{
				j = functie_de_repartizare_numere(j) % dimensiune_tabela;
				if (vector_de_perechi[j].cheie == "" && vector_de_perechi[j].este_sters == true)
				{
					p.cheie = cheie;
					p.sir = sir;
					p.este_sters = false;
					vector_de_perechi[j] = p;
					nr_elemente++;
					break;
				}
				else if (vector_de_perechi[j].cheie == cheie)
				{
					return;
				}
			} while (vector_de_perechi[j].cheie != cheie);
		}

		if (getFactorIncarcare() >= 0.7)
		{
			rehashing();
		}
	}

	void citire_elemente_fisier()
	{
		std::string cheie;
		std::string sir;
		std::ifstream fin;
		fin.open("input.in");
		while (!fin.eof())
		{
			std::getline(fin, cheie, ' ');
			std::getline(fin, sir);
			inserare(cheie, sir);
		}
		fin.close();
	}

	double getFactorIncarcare()
	{
		double val = (double)nr_elemente / dimensiune_tabela;
		return val;
	}

	std::string& operator[](std::string cheie)
	{
		int pos = cautare(cheie);
		if (pos == -1)
		{
			inserare(cheie);
			pos = cautare(cheie);
		}
		return vector_de_perechi[pos].sir;
	}

	void afisare_Hash_table()
	{
		for (int i = 0; i < dimensiune_tabela; i++)
		{
			if (vector_de_perechi[i].cheie != "" && vector_de_perechi[i].este_sters == false)
			{
				std::cout << vector_de_perechi[i].cheie << " " << vector_de_perechi[i].sir << std::endl;
			}
		}
	}
};

void meniu(Hash_Table& h)
{
	int op = 1, pos;
	std::string cheie, element;
	while (op != 0)
	{
		std::cout << "Meniu" << std::endl;
		std::cout << "1. Inserare element in Hash-Table: \n";
		std::cout << "2. Cautare element in Hash-Table: \n";
		std::cout << "3. Stergere element din Hash-Table: \n";
		std::cout << "4. Afiseaza Hash-Table: \n";
		std::cout << std::endl << std::endl;
		std::cout << "0. Exit " << std::endl << std::endl;
		std::cout << "Selecteaza optiune: ";

		std::cin >> op;

		switch (op)
		{
		case 1:
			std::cout << std::endl << "Citeste cheia si elementul: \n";
			std::getchar();
			std::getline(std::cin, cheie, ' ');
			std::getline(std::cin, element);
			h.inserare(cheie, element);
			std::cout << std::endl;
			break;
		case 2:
			std::cout << std::endl << "Citeste ceea ce doresti sa cauti: \n";
			std::getchar();
			std::getline(std::cin, cheie, ' ');
			std::getline(std::cin, element);
			std::cout << std::endl << "Elementul cautat este gasit pe pozitia: " << h.cautare(cheie, element);
			std::cout << std::endl;
			break;
		case 3:
			std::cout << std::endl << "Citeste cheia pe care doresti sa o stergi: \n";
			std::getchar();
			std::getline(std::cin, cheie);
			pos = h.cautare(cheie);
			if (pos != -1)
			{
				h.stergere(pos);
				std::cout << std::endl << "Elementul a fost sters cu succes\n";
			}
			else
			{
				std::cout << "Elementul nu a fost sters cu succes\n";
			}
			std::cout << std::endl;
			break;
		case 4:
			std::cout << std::endl;
			h.afisare_Hash_table();
			std::cout << std::endl;
			break;
			std::cout << std::endl;
		}

	}

}

int main()
{
	Hash_Table h;
	h.citire_elemente_fisier();
	meniu(h);
	return 0;
}