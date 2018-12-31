#include "resource/Resource.hpp"

#include <stdexcept>

namespace bsn {
    namespace resource {

        using namespace std;
        
        Resource::Resource(const std::string _name, const double _capacity, const double _currentLevel, const double _unit) :
            name(_name),
            capacity(_capacity),
            currentLevel(_currentLevel),
            unit(_unit) {
            if (_capacity <= 0) {
                throw std::invalid_argument ("Capacity should not be negative or null");
            }

            if (_currentLevel < 0 || _currentLevel > _capacity ) {
                throw std::invalid_argument ("Current level should not be negative or null nor bigger than the capacity");
            }

            if (_unit < 0  || _unit > _capacity) {
                throw std::invalid_argument ("The resolution should not be negative or null nor bigger than the capacity");
            }
        }

        Resource::Resource() :
            name(),
            capacity(),
            currentLevel(),
            unit() {}
        
        Resource::Resource(const Resource &obj) : 
            name(obj.getName()),
            capacity(obj.getCapacity()),
            currentLevel(obj.getCurrentLevel()),
            unit(obj.getUnit()) {}

        Resource& Resource::operator=(const Resource &obj) {
            name = obj.getName();  
            capacity = obj.getCapacity(); 
            currentLevel = obj.getCurrentLevel();
            unit = obj.getUnit();        
            return (*this);
        }

        Resource::~Resource(){};
        
        void Resource::consume(const int32_t mult) {
            currentLevel-= unit*mult;

            if(currentLevel < 0) currentLevel = 0;
        }

        void Resource::generate(const int32_t mult) {
            currentLevel += unit*mult;
            
            if(currentLevel >= capacity) currentLevel = capacity;
        }

        void Resource::setName (const string &_name) {
            name = _name;
        }

        string Resource::getName() const {
            return name;
        }

        void Resource::setCapacity (const double &_cap) {
            capacity = _cap;
        }

        double Resource::getCapacity() const{
            return capacity;
        }

        void Resource::setCurrentLevel(const double &_cLevel) {
            currentLevel = _cLevel;
        }

        double Resource::getCurrentLevel() const{
            return currentLevel;
        }

        void Resource::setUnit(const double &_unit){
            unit = _unit;
        }

        double Resource::getUnit() const{
            return unit; 
        }

        const string Resource::toString() const {
            stringstream sstr;

            sstr << "Resources: " << name  << " " << capacity << " " << currentLevel << " " << unit << endl;

            return sstr.str();
        }

    }
}