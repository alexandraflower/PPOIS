#include <iostream>
#include <stdexcept>
#include "Vector3D.hpp"

using namespace std;

int main(){
    cout<<"VECTOR\n";
    cout<<"Creating zero vector\n";
    Vector3D v0;
    cout<<v0;
    cout<<"\nCreating your own vectors\n";
    Vector3D v1(0,0,0,1,1,1);
    Vector3D v2;
    cout<<"Enter 6 coordinates separated by a space: ";
    cin>>v2;
    cout<<"v0: "<<v0<<", v1: "<<v1<<", v2: "<<v2<<"\n";

    cout<<"\nGet coordinates: \n";
    cout<<"v0: ("<<v0.getX1()<<", "<<v0.getY1()<<", "<<v0.getZ1()<<") ("<<
    v0.getX2()<<", "<<v0.getY2()<<", "<<v0.getZ2()<<")\n";
    cout<<"v1: ("<<v1.getX1()<<", "<<v1.getY1()<<", "<<v1.getZ1()<<") ("<<
    v1.getX2()<<", "<<v1.getY2()<<", "<<v1.getZ2()<<")\n";
    cout<<"v2: ("<<v2.getX1()<<", "<<v2.getY1()<<", "<<v2.getZ1()<<") ("<<
    v2.getX2()<<", "<<v2.getY2()<<", "<<v2.getZ2()<<")\n";
    
    cout<<"\nGet vector projections\n";
    cout<<"v0: dx="<<v0.dx()<<", dy="<<v0.dy()<<", dz="<<v0.dz()<<"\n";
    cout<<"v1: dx="<<v1.dx()<<", dy="<<v1.dy()<<", dz="<<v1.dz()<<"\n";
    cout<<"v2: dx="<<v2.dx()<<", dy="<<v2.dy()<<", dz="<<v2.dz()<<"\n";

    cout<<"\nGet vector length\n";
    cout<<"|v0|="<<v0.length()<<"\n";
    cout<<"|v1|="<<v1.length()<<"\n";
    cout<<"|v2|="<<v2.length()<<"\n";

    cout<<"\nCosine:\n";
    try{
        cout<<"cos(v0, v1)="<<(v0^v1)<<"\n";
    } catch (const invalid_argument& e){
        cout<<"cos(v0, v1) error: "<<e.what()<<"\n";
    }
    cout<<"cos(v1, v2)="<<(v1^v2)<<"\n";

    cout<<"\nAddition: \nv0+v1="<<(v0+v1)<<"\nv2+=v1";
    v2+=v1;
    cout<<", v2="<<v2<<"\n";

    cout<<"\nSubtraction: \nv2-v1="<<(v2-v1)<<"\nv1-=v2";
    v1-=v2;
    cout<<", v1="<<v1<<"\n";

    cout<<"\nScalar product: \nv1*v2="<<(v1*v2)<<"\n";

    cout<<"\nProduct of vector and number: \n3*v0="<<(3*v0)<<"\n2.5*v1="<<(2.5*v1)<<"\n"<<"v2*4="<<(v2*4)<<"\n";
    
    cout<<"\nComponent division: \nv2/=v1, ";
    v2/=v1;
    cout<<"v2="<<v2<<"\n";
    try{
        cout<<"v1/v0="<<(v1/v0)<<"\n";
    } catch (const invalid_argument& e){
        cout<<"v1/v0 error: "<<e.what()<<"\n";
    }

    cout<<"\nComparison\n";
    cout<<boolalpha;
    cout<<"v1==v2 : "<<(v1==v2)<<"\n";
    cout<<"v0!=v1 : "<<(v0!=v1)<<"\n";
    cout<<"v1==v2 : "<<(v1==v2)<<"\n";
    cout<<"v0<v1 : "<<(v0<v1)<<"\n";
    cout<<"v2<=v1 : "<<(v2<=v1)<<"\n";
    cout<<"v1>v2 : "<<(v1>v2)<<"\n";
    cout<<"v2>=v2 : "<<(v2>=v2)<<"\n";

    cout<<"ALL DONE\n"; 
    return 0;
}