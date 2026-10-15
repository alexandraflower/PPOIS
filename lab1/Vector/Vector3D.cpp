#include "Vector3D.hpp"
#include <cmath>
#include <stdexcept>

Vector3D::Vector3D(): x1(0.), y1(0.), z1(0.),
    x2(0.), y2(0.), z2(0.){}

Vector3D::Vector3D(double startX, double startY, double startZ, 
    double endX, double endY, double endZ):x1(startX), y1(startY), z1(startZ),
    x2(endX), y2(endY), z2(endZ){}

double Vector3D::getX1() const{
    return x1;
}

double Vector3D::getY1() const{
    return y1;
}

double Vector3D::getZ1() const{
    return z1;
}

double Vector3D::getX2() const{
    return x2;
}

double Vector3D::getY2() const{
    return y2;
}

double Vector3D::getZ2() const{
    return z2;
}

double Vector3D::dx() const{
    return x2-x1;
}

double Vector3D::dy() const{
    return y2-y1;
}

double Vector3D::dz() const{
    return z2-z1;
}

double Vector3D::length() const{
    return std::sqrt(dx()*dx() + dy()*dy() + dz()*dz());
}

double Vector3D::operator^(const Vector3D& other) const{ 
    double lenProd=this->length()*other.length();
    if(lenProd==0.){
        throw std::invalid_argument("Cannot calculate cosine: one vector is zero.");
    }
    double scalProd=this->dx()*other.dx()+this->dy()*other.dy()+this->dz()*other.dz();
    return scalProd/lenProd;
}

Vector3D& Vector3D::operator+=(const Vector3D& rhs){
    x2+=rhs.dx();
    y2+=rhs.dy();
    z2+=rhs.dz();
    return *this;
}

Vector3D Vector3D::operator+(const Vector3D& rhs) const{
    Vector3D result=*this;
    result+=rhs;
    return result;
}

Vector3D& Vector3D::operator-=(const Vector3D& rhs){
    x2-=rhs.dx();
    y2-=rhs.dy();
    z2-=rhs.dz();
    return *this;
}

Vector3D  Vector3D::operator-(const Vector3D& rhs) const{
    Vector3D result=*this;
    result-=rhs;
    return result;
}

double Vector3D::operator*(const Vector3D& rhs) const{
    return dx()*rhs.dx()+dy()*rhs.dy()+dz()*rhs.dz();
}

Vector3D& Vector3D::operator*=(double scalar) {
    x2=x1+dx()*scalar;
    y2=y1+dy()*scalar;
    z2=z1+dz()*scalar;
    return *this;
}

Vector3D Vector3D::operator*(double scalar) const{
    Vector3D result=*this;
    result*=scalar;
    return result;
}

Vector3D operator*(double scalar, const Vector3D& vector){
    return vector*scalar;
}

Vector3D& Vector3D::operator/=(const Vector3D& rhs){
    if(rhs.dx()==0||rhs.dy()==0||rhs.dz()==0){
        throw std::invalid_argument("Cannot devide: one vector is zero.");
    }
    x2=x1+dx()/rhs.dx();
    y2=y1+dy()/rhs.dy();
    z2=z1+dz()/rhs.dz();
    return *this;
}

Vector3D Vector3D::operator/(const Vector3D& rhs) const{
    Vector3D result=*this;
    result/=rhs;
    return result;
}

bool Vector3D::operator==(const Vector3D& rhs) const{
    return length()==rhs.length();
}

bool Vector3D::operator!=(const Vector3D& rhs) const{
    return !(*this==rhs);
}

bool Vector3D::operator<(const Vector3D& rhs) const{
    return length()<rhs.length();
}

bool Vector3D::operator<=(const Vector3D& rhs) const{
    return length()<=rhs.length();
}

bool Vector3D::operator>(const Vector3D& rhs) const{
    return length()>rhs.length();
}

bool Vector3D::operator>=(const Vector3D& rhs) const{
    return length()>=rhs.length();
}

std::ostream& operator<<(std::ostream& output, const Vector3D& vector){
    output<<"("<<vector.x1<<", "<<vector.y1<<", "<<vector.z1<<") ("<<
    vector.x2<<", "<<vector.y2<<", "<<vector.z2<<")";
    return output; 
}

std::istream& operator>>(std::istream& input, Vector3D& vector){
    input>>vector.x1>>vector.y1>>vector.z1>>vector.x2>>vector.y2>>vector.z2;
    return input;
}
