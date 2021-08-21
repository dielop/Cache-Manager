#include <iostream>
#include "FuncionesCache.hpp"

using namespace std;

//Clase estudiante
class Student{
 public:
 int id;
 int value;
 const char* data;
 
         static const string class_name; // Cada clase tiene un static llamado : class_name
         
         Student(int _key, int _value, const char* _data):id( _key), value( _value), data( _data){}
                

    ~Student(){};
};
//Sobrecargo
ostream& operator <<(ostream &o, const Student s){
          o << "Student:" << s.id << "," << s.value << "," << s.data << endl;
          return o;
}

const string Student::class_name = "StudentClass";

int main(){
    CacheManager<Student> cache(4);


    //Inserto claves y objetos 
    cache.set("0", Student(0, 22, "student1"));   
    cache.set("1", Student(1, 23, "student2"));
    cache.set("2", Student(2, 25, "student3"));
    cache.set("3", Student(3, 29, "student4"));

    cout << cache;    //Muestro elementos

    //Inserto claves y objetos
    cache.set("7", Student(4, 345, "Ez"));
    cache.set("9", Student(1, 5, "Nine"));
    cache.set("5", Student(32, 8, "Tres"));
    cache.set("6", Student(59, 6, "Cuatro"));
    cache.set("8", Student(15, 35, "five"));

    cout << cache; //Muestro elementos

    //Busco un objeto
    cout << "\nBusco objeto: " << endl;
    cout << cache.get("2", Student(0,0,""));
    
    cout << "\n" << endl;

    cout << cache; // Muestro claves y objetos

    cache.set("2", Student(22, 222, "Eze"));   //Inserto clave y objeto

    cout << cache; // Muestro claves y objetos

    //Inserto claves y objetos
    cache.set("9", Student(1, 5, "Nine"));    
    cache.set("9", Student(1, 5, "Nine"));
    cache.set("9", Student(1, 5, "Nine"));

    //Busco objetos
    cout << "\nBusco objeto: " << endl;
    cout << cache.get("5", Student(0,0,""));
    cout << cache.get("2", Student(0,0,""));

    cout << "\n" << endl;

    cout << cache; //Muestro claves y objetos  

    return 0;
}