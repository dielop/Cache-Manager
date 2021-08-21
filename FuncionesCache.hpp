#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>
#include <utility>
#include <string>
#include <cstring>

using namespace std;

//variables globales
int i = 0;
int c = 0;

/**************************************************************/

//Clase cache
template <class T>
class CacheManager{
    int capacity;
    map<string, pair<T, int>>cache_data; //map donde almaceno la cache
    void write_file(string, T); //Funcion que escribe en el archivo

    public:
          CacheManager(int);//constructor
          ~CacheManager();//destructor

          T get(string, T); //Funcion que va a buscar los objetos y retornar
          void set(string, T); //Funcion que inserta claves y objetos
          template<class Q> friend ostream& operator<<(ostream&, CacheManager<Q>);
          int reemplazo(string, T);    //Funcion de reemplazos en el archivo      
         
          
};

//Sobrecargo para mostrar la cache
template <class Q>
ostream& operator<<(ostream& os, CacheManager<Q> c){
    os << "Nivel de uso\tKey\t\tObjeto\n";

    for(auto it =  c.cache_data.begin(); it != c.cache_data.end(); it++){
        os <<  (*it).second.second  << "\t\t" << (*it).first<< "\t"  << (*it).second.first << "\t" << endl;
    }
    return os;
}

//Funcion que guarda la capacidad del cache
template <class T>
CacheManager<T>::CacheManager(int cap){
    capacity = cap;
}

template <class T>
 CacheManager<T>:: ~CacheManager(){

}

//Funcion de reemplazos en el archivo
template <class T>
int CacheManager<T>::reemplazo(string key, T obj){
    char clave;
    
    int claveInt = -1;
    int ki = stoi(key);
    char k[1];  //para escribir
    strcpy(k, key.c_str()); //para escribir

    fstream Archivo("OBJETOS.txt", ios::app | ios::in | ios::binary);
    if(!Archivo){
        cerr << "No se pudo abrir el archivo" << endl;
                 exit(1);
    }
    while(!Archivo.eof()){ //Recorre el archivo leyendo las claves y comparando. Si existe, escribe el objeto que reemplaza
        Archivo.read(reinterpret_cast<char *>(&clave), sizeof(T));    // Leo las claves         
        claveInt = (clave-48);   //convierto la clave de char a entero, le resto el codigo ascii  
        if(ki == claveInt){   //Si la clave que lei es igual a la que ingrese a la funcion, reemplazo.

            Archivo.seekp(ki * sizeof(T));  //Posicion el apuntador
            Archivo.write(k, sizeof(T));  //Escribo key
            Archivo.write(reinterpret_cast<const char *>(&obj), sizeof(T)); //Escribo objeto
            c++;

            return 0;
        }
    }
    return 1;
}

//Funcion que escribe en el archivo
template <class T>
void CacheManager<T>::write_file(string key, T obj){
     
     char k[1];  //para escribir
     strcpy(k, key.c_str()); //para escribir
     int claveInt = -1;
     
       
     
     ofstream Archivo("OBJETOS.txt", ios::app | ios::binary);                    //Creo el archivo OBJETOS
         if(!Archivo){
                 cerr << "No se pudo abrir el archivo" << endl;
                 exit(1);
             }       
        
        //Cuento las veces que ingrese, si c > 0, ingreso a la funcion replace. 
        //Replace lee y actualiza el objeto en el archivo y retorna.
        // Si c = 0, ingrese por primera vez, solo escribo en el archivo.

          if(c>0){
             int replace = reemplazo(key, obj); //Ingreso a la funcion de reemplazo y retorno 0 o 1.
                if(replace == 0){  //Si replace = 0, actualizo el archivo y salio de la funcion.
                     return;
                }else{  //Si no, la clave no existia en el archivo y solo escribe.
                     Archivo.write(k, sizeof(T));  //Escribo key
                     Archivo.write(reinterpret_cast<const char *>(&obj), sizeof(T));  //Escribo objeto
                     c++; //Aumento el contador de veces que ingrese.
                }                 
              
          }else{     
                Archivo.write(k, sizeof(T));  //Escribo key
                Archivo.write(reinterpret_cast<const char *>(&obj), sizeof(T)); //Escribo objeto
                c++;  //Aumento el contador de veces que ingrese.           
          }
             
}

//Funcion que inserta claves y objetos
template<class T>
void CacheManager<T>::set(string key, T obj){
int lru; //Declaro la variable lru
i++; //Aumento el contador
char n[1]; //Array de char donde voy a ir ingresando las key
strcpy(n, key.c_str());


    //inserto en el archivo   
    write_file(key, obj);    

    //si la cache no esta llena
    if(cache_data.size() != capacity){   
    for(auto t = cache_data.begin(); t != cache_data.end(); t++){ //recorro, si la clave ya se encuentra, actualizo        
            if(t->first == key){            
               t->second.first = obj;
               t->second.second = i;
               return;                   
           }
       }
       cache_data.insert(make_pair(key, make_pair(obj,i))); //si la cache no esta llena y la clave no se encuentra, inserto la nueva

    }else{       
     //Si la cache esta llena
        for(auto t = cache_data.begin(); t != cache_data.end(); t++){ //recorro la cache, si la clave ya se encuentra, actualizo        
            if(t->first == key){            
               t->second.first = obj;
               t->second.second = i;
               return;                   
           }
       }

        auto k = cache_data.begin(); //Defino k con el principio de la cache
        lru = k->second.second;      //Inicializo la variable lru

             for(auto j = cache_data.begin(); j != cache_data.end(); j++){ //obtengo el LRU mas chico
                if(lru >= j->second.second){
                    lru = j->second.second;
                }                    
            }
            for(auto m = cache_data.begin(); m != cache_data.end(); m++){ //borro la clave con el lru e inserto la nueva
                if(m->second.second == lru){
                    cache_data.erase(m);                    
                    cache_data.insert(make_pair(key, make_pair(obj,i)));
                    return;                    
                }
            }        
    }     
                             
}

//Funcion que va a buscar los objetos y retornar
template<class T>
T CacheManager<T>::get(string key, T obj){    
    
    int k = stoi(key);  //Convierto la clave a int
    int claveInt = -1;   
    char clave;    
      
     //busco la key en la cache    
     for(auto g = cache_data.begin(); g != cache_data.end(); g++){
         if(g->first == key){ //Si coindice
             i++;             // Incremento el contador
             g->second.second = i; //Actualizo
             return g->second.first; // Retorno el objeto
         }
     }

     ifstream ArchivoObj("OBJETOS.txt", ios::in | ios::binary); // Abro el archivo para leer
     if(!ArchivoObj){
         cerr << "No se pudo abrir el archivo" << endl;
         exit(1);
     }else{
         while(!ArchivoObj.eof()){ //Recorro el archivo hasta el final
            if(k == claveInt){ //Busco la key, si coincide con la leida, retorno el objeto.
            ArchivoObj.seekg(k * sizeof(T));

            auto k = cache_data.begin(); //Defino k con el principio de la cache
            int lru = k->second.second;  //Inicializo la variable lru

             for(auto j = cache_data.begin(); j != cache_data.end(); j++){ //obtengo el LRU mas chico
                if(lru >= j->second.second){
                    lru = j->second.second;
                }                    
            }

            for(auto m = cache_data.begin(); m != cache_data.end(); m++){ //borro la clave con el lru e inserto la nueva
                if(m->second.second == lru){
                    cache_data.erase(m);
                    i++; //Aumento el contador                   
                    cache_data.insert(make_pair(key, make_pair(obj,i)));
                }
            }
                return obj; //Retorno el objeto
            }
         
          ArchivoObj.read(reinterpret_cast<char *>(&clave), sizeof(T)); // leo la clave
          claveInt = (clave-48); //convierto la clave de char a entero, le resto el codigo ascii
          ArchivoObj.read(reinterpret_cast<char *>(&obj), sizeof(T)); // leo el objeto                         
         }
        }          

    cout << "\nLa clave no existe en la cache y tampoco en el archivo" << endl;
    
}