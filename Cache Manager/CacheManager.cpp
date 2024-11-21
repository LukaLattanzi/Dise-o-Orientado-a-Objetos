#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <string>

using namespace std;

template <class T>
class CacheManager
{
private:
    int capacity;
    int current_mru;
    map<string, pair<T, int>> cache_data; // <key, <value, MRU>>
    string disk_file;
    bool write_file(string, T);
    bool read_file(string, T &);

public:
    CacheManager(int);
    ~CacheManager();
    void insert(string, T);
    T get(string);
    void show_cache();
};

template <class T>
CacheManager<T>::CacheManager(int capacity)
{
    this->capacity = capacity;
    this->current_mru = 0;
    this->disk_file = "disk.txt";
}

template <class T>
CacheManager<T>::~CacheManager() {}

template <class T>
bool CacheManager<T>::write_file(string key, T obj)
{
    ofstream file(disk_file, ios::app);
    if (!file)
    {
        return false;
    }
    file << key << " " << obj << endl;
    file.close();
    return true;
}

template <class T>
bool CacheManager<T>::read_file(string key, T &obj)
{
    ifstream file(disk_file);
    if (!file)
    {
        return false;
    }
    string file_key;
    while (file >> file_key)
    {
        if (file_key == key)
        {
            file >> obj;
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

template <class T>
void CacheManager<T>::insert(string key, T obj)
{
    if (cache_data.find(key) == cache_data.end() && cache_data.size() >= capacity)
    {
        auto i = cache_data.begin();
        int min_mru = i->second.second;
        string min_key = i->first;
        for (; i != cache_data.end(); ++i)
        {
            if (i->second.second < min_mru)
            {
                min_mru = i->second.second;
                min_key = i->first;
            }
        }
        cache_data.erase(min_key);
    }
    current_mru++;
    cache_data[key] = make_pair(obj, current_mru);
    write_file(key, obj);
}

template <class T>
T CacheManager<T>::get(string key)
{
    if (cache_data.find(key) != cache_data.end())
    {
        current_mru++;
        cache_data[key].second = current_mru;
        cout << "Found in cache: " << key << endl;
        return cache_data[key].first;
    }
    T obj;
    if (read_file(key, obj))
    {
        cout << "Found in disk file: " << key << endl;
        return obj;
    }
    cout << "Key not found in cache or disk: " << key << endl;
    return T();
}

template <class T>
void CacheManager<T>::show_cache()
{
    cout << "Cache:" << endl;
    for (auto i = cache_data.begin(); i != cache_data.end(); ++i)
    {
        cout << i->first << ": " << i->second.first << " (MRU: " << i->second.second << ")" << endl;
    }
}

class Student
{
private:
    int id;
    int value;
    string data;

public:
    static const string class_name;
    Student() : id(0), value(0), data("") {}
    Student(int _id, int _value, string _data) : id(_id), value(_value), data(_data) {}
    void print() { cout << id << " " << value << " " << data << endl; }
    friend ostream &operator<<(ostream &os, const Student &student)
    {
        os << student.id << " " << student.value << " " << student.data;
        return os;
    }
    friend istream &operator>>(istream &is, Student &student)
    {
        is >> student.id >> student.value >> student.data;
        return is;
    }
};

const string Student::class_name = "StudentClass";

int main()
{
    CacheManager<Student> my_cache(3); // Crea una caché con capacidad para 3 elementos

    // Inserta elementos en la caché
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;
    my_cache.insert("0", Student(0, 22, "Student1"));
    my_cache.insert("1", Student(1, 23, "Student2"));
    my_cache.insert("2", Student(2, 25, "Student3"));
    my_cache.show_cache();
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;

    // Inserta un elemento adicional en la caché (debería eliminar el elemento con MRU mínimo)
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;
    my_cache.insert("3", Student(3, 29, "Student4"));
    my_cache.show_cache();
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;

    // Obtiene un elemento de la caché
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;
    Student return_obj = my_cache.get("1");
    return_obj.print();
    my_cache.show_cache();
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;

    // Obtiene un elemento que no se encuentra en la caché pero sí en el archivo del disco
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;
    return_obj = my_cache.get("0");
    return_obj.print();
    my_cache.show_cache();
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;

    // Obtiene un elemento que no se encuentra en la caché ni en el archivo del disco (debería devolver un objeto vacío)
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;
    return_obj = my_cache.get("4");
    return_obj.print();
    my_cache.show_cache();
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;

    // Inserta un par de elementos adicionales en la caché
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;
    my_cache.insert("4", Student(4, 31, "Student6"));
    my_cache.insert("5", Student(5, 32, "Student7"));
    my_cache.show_cache();
    cout << "- - - - - - - - - - - - - - - - - - - - -" << endl;

    return 0;
}