#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Persona
{
private:
    string nombre;

public:
    Persona(string nombre)
    {
        this->nombre = nombre;
    }
    string getNombre()
    {
        return nombre;
    }
};

class Oferta
{
private:
    int monto;
    Persona *ofertante;

public:
    Oferta(int monto, Persona *ofertante)
    {
        this->monto = monto;
        this->ofertante = ofertante;
    }
    ~Oferta()
    {
        delete ofertante;
    }
    int getMonto()
    {
        return monto;
    }
    Persona *getOfertante()
    {
        return ofertante;
    }
};

class Lote
{
private:
    int numero;
    string nombre;
    Oferta *oferta;

public:
    Lote(int numero, string nombre)
    {
        this->numero = numero;
        this->nombre = nombre;
        this->oferta = nullptr;
    }
    ~Lote()
    {
        delete oferta;
    }
    int getNumero()
    {
        return numero;
    }
    string getNombre()
    {
        return nombre;
    }
    Oferta *getOferta()
    {
        return oferta;
    }
    void setOferta(Oferta *oferta)
    {
        this->oferta = oferta;
    }
};

class Subasta
{
private:
    vector<Lote *> lotes;

public:
    Subasta()
    {
        ifstream archivo("Lotes.txt");
        if (archivo.is_open())
        {
            int numero;
            string nombre;
            while (archivo >> numero)
            {
                archivo.ignore();
                getline(archivo, nombre);
                lotes.push_back(new Lote(numero, nombre));
            }
            archivo.close();
        }
        else
        {
            cout << "No se pudo abrir el archivo." << endl;
        }
    }

    ~Subasta()
    {
        for (Lote *lote : lotes)
        {
            delete lote;
        }
    }

    void ofertar(int numero, int monto, string nombre)
    {
        for (Lote *lote : lotes)
        {
            if (lote->getNumero() == numero)
            {
                if (lote->getOferta() == nullptr || lote->getOferta()->getMonto() < monto)
                {
                    delete lote->getOferta();
                    Persona *persona = new Persona(nombre);
                    Oferta *oferta = new Oferta(monto, persona);
                    lote->setOferta(oferta);
                }
                break;
            }
        }
    }

    void cerrar()
    {
        ofstream archivo("LotesSinOfertas.txt");
        ofstream archivo2("LotesComprados.txt");
        if (!archivo.is_open() || !archivo2.is_open())
        {
            cout << "No se pudo abrir uno de los archivos." << endl;
            return;
        }
        for (Lote *lote : lotes)
        {
            if (lote->getOferta() != nullptr)
            {
                archivo2 << lote->getNumero() << " " << lote->getNombre() << " " << lote->getOferta()->getOfertante()->getNombre() << " " << lote->getOferta()->getMonto() << endl;
            }
            else
            {
                archivo << lote->getNumero() << " " << lote->getNombre() << endl;
            }
        }
        archivo.close();
        archivo2.close();
    }
};

int main()
{
    Subasta subasta;

    subasta.ofertar(2, 2300, "Luka");
    subasta.ofertar(2, 2500, "Santiago");
    subasta.ofertar(2, 2400, "Maxi");
    subasta.ofertar(3, 2000, "Lucas");

    subasta.cerrar();

    return 0;
}